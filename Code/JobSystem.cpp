#include <iostream> 
#include "JobSystem.h" 
#include "JobWorkerThread.h"

JobSystem* JobSystem::s_jobSystem = nullptr; 

typedef void (*JobCallback) (Job* completedJob);    
//new type called JObCallBack --> can use that to make all the code i write with function pointers way simpler; 

JobSystem::JobSystem()
{
    //
    m_jobHistory.reserve(256 * 1024); 
}
JobSystem::~JobSystem()
{
    m_workerThreadsMutex.lock(); 
    int numWorkerThreads = (int)m_workerThreads.size(); 

    //First, tell each worker thread to stop picking up jobs 
    for(int i = 0; i < numWorkerThreads; ++i)
    {
        m_workerThreads[i]->shutDown(); 
    }

    while( !m_workerThreads.empty()){
        delete m_workerThreads.back(); 
        m_workerThreads.pop_back(); 
    }
    m_workerThreadsMutex.unlock(); 
}

JobSystem* JobSystem::CreateOrGet()
{
    if(!s_jobSystem)
    {
        s_jobSystem = new JobSystem(); 
    }
    return s_jobSystem; 
}

void JobSystem::Destory()
{
    if(s_jobSystem)
    {
        delete s_jobSystem; 
        s_jobSystem = nullptr; 
    }
}

void JobSystem::CreateWorkerThread(const char* uniqueName, unsigned long workerJobChannel)
{
    JobWorkerThread* newWorker = new JobWorkerThread(uniqueName, workerJobChannel, this); 
    m_workerThreadsMutex.lock(); 
    m_workerThreads.push_back(newWorker);
    m_workerThreadsMutex.unlock(); 

    m_workerThreads.back()->startUp(); 
}

void JobSystem::DestroyWorkerThread(const char *uniqueName)
{
    
}

void JobSystem::QueueJob(Job* job)
{
    m_jobsQueuedMutex.lock(); 
    m_jobHistoryMutex.lock();
    m_jobHistory.emplace_back(JobHistoryEntry(job->m_jobType, JOB_STATUS_QUEUED)); 
    m_jobHistoryMutex.unlock(); 
    m_jobsQueued.push_back(job); 
    m_jobsQueuedMutex.unlock(); 

    std::cout << "Queued: " << GetJobID(job) << std::endl;
}

JobStatus JobSystem::GetJobStatus(int jobID) const 
{
    m_jobHistoryMutex.lock(); 

    JobStatus jobStatus = JOB_STATUS_NEVER_SEEN;
    if(jobID, (int) m_jobHistory.size())
    {
        jobStatus = m_jobHistory[jobID].m_jobStatus;
    }

    m_jobHistoryMutex.unlock(); 

    return jobStatus; 
}
int JobSystem::GetJobID(Job* job) 
{
    return job->m_jobID;
}

bool JobSystem::isJobComplete(int jobID) const
{
    return JobStatus (jobID) == JOB_STATUS_COMPLETED; 
}

void JobSystem::FinishCompletedJobs()
{
    std::deque<Job*> jobsCompleted; 

    m_jobsCompletedMutex.lock(); 

    jobsCompleted.swap(m_jobsCompleted); 
    m_jobsCompletedMutex.unlock(); 

    for(Job* job : jobsCompleted)
    {
        job->JobCompleteCallback(); 
        m_jobHistoryMutex.lock();
        m_jobHistory[job->m_jobID].m_jobStatus = JOB_STATUS_RETIRED; 
        m_jobHistoryMutex.unlock(); 
        delete job; 
    }
}

void JobSystem::FinishJob(int jobID)
{
    while(!isJobComplete(jobID))
    {
        JobStatus jobStatus = GetJobStatus(jobID);
        if(jobStatus == JOB_STATUS_NEVER_SEEN || JOB_STATUS_RETIRED)
        {
            std::cout << "ERROR: Waiting for Job(#" << jobID <<") - no such job in JobSystem" << std::endl; 
            return;
        }

        m_jobsCompletedMutex.lock(); 
        Job* thisCompletedJob = nullptr; 
        for(auto jcIter = m_jobsCompleted.begin(); jcIter != m_jobsCompleted.end(); ++jcIter)
        {
            Job* someCompletedJob = *jcIter;
            if(someCompletedJob->m_jobID == jobID)
            {
                thisCompletedJob = someCompletedJob;
                m_jobsCompleted.erase(jcIter);
                break;
            }
        }
        m_jobsCompletedMutex.unlock(); 

        if(thisCompletedJob == nullptr)
        {
            std::cout << "ERROR: Job #" << jobID << " was status completed but not found in completed lisy" << std::endl;    
        }

        thisCompletedJob->JobCompleteCallback(); 

        m_jobHistoryMutex.lock(); 
        m_jobHistory[thisCompletedJob->m_jobID].m_jobStatus = JOB_STATUS_RETIRED;
        m_jobHistoryMutex.unlock(); 

        delete thisCompletedJob; 
    }
}

void JobSystem::onJobCompleted(Job* jobJustExecuted )
{
    totalJobs++;
    m_jobsCompletedMutex.lock(); 
    m_jobsRunningMutex.lock(); 

    std::deque<Job*>::iterator runningJobItr = m_jobsRunning.begin(); 
    for(; runningJobItr!= m_jobsRunning.end(); ++runningJobItr)
    {
        if(jobJustExecuted == * runningJobItr)
        {
            m_jobHistoryMutex.lock(); 
            m_jobsRunning.erase(runningJobItr); 
            m_jobsCompleted.push_back(jobJustExecuted); 
            m_jobHistory[jobJustExecuted->m_jobID].m_jobStatus = JOB_STATUS_COMPLETED; 
            m_jobHistoryMutex.unlock(); 

            break; 
        }
    }
    m_jobsRunningMutex.unlock(); 
    m_jobsCompletedMutex.unlock(); 
}

Job* JobSystem::ClaimAJob(unsigned long workerJobChannels)
{
    m_jobsQueuedMutex.lock(); 
    m_jobsRunningMutex.lock(); 

    Job* claimedJob = nullptr; 
    std::deque<Job *>::iterator queuedJobItr = m_jobsQueued.begin(); 

    for(; queuedJobItr != m_jobsQueued.end(); ++queuedJobItr)
    {
        Job* queuedJob = *queuedJobItr; 

        if((queuedJob->m_jobChannels & workerJobChannels) != 0)
        {
            claimedJob = queuedJob; 

            m_jobHistoryMutex.lock(); 

            m_jobsQueued.erase(queuedJobItr); 
            m_jobsRunning.push_back(claimedJob);
            m_jobHistory[claimedJob->m_jobID].m_jobStatus = JOB_STATUS_RUNNING;
            break; 
        }
    }
    m_jobsRunningMutex.unlock(); 
    m_jobsQueuedMutex.unlock(); 

    return claimedJob;
}