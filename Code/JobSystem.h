#pragma once
#include <vector> 
#include <mutex>
#include <deque>
#include <thread>
//constexpr int JOB_TYPE_ANY = -1; 
const int JOB_TYPE_ANY = -1;

//int JOB_TYPE_ANY = -1; 
class JobWorkerThread; 

enum JobStatus{
    JOB_STATUS_NEVER_SEEN,  // job created but not put anywhjre
    JOB_STATUS_QUEUED,  // waiting to be run
    JOB_STATUS_RUNNING,
    JOB_STATUS_COMPLETED,
    JOB_STATUS_RETIRED,     // main thread has processed & done all its clean up & deleted the job 
    NUM_JOB_STATUSES
}; 

struct JobHistoryEntry
{
    JobHistoryEntry(int jobType, JobStatus jobStatus)
        : m_jobType(jobType), 
         m_jobStatus(jobStatus){}

        int m_jobType = -1; 
        JobStatus m_jobStatus = JOB_STATUS_NEVER_SEEN; 
}; 

class Job; 
class JobSystem 
{
    friend class JobWorkerThread; 
    public: 
    JobSystem(); 
    ~JobSystem(); 

    static JobSystem* CreateOrGet(); 
    static void Destory(); 
    int totalJobs = 0; 

    void CreateWorkerThread(const char* unioqueName, unsigned long workerJobChannels = 0xFFFFFFFF); 
    void DestroyWorkerThread(const char* uniqueName); 
    void QueueJob(Job* job); 

    //Status queries 
    JobStatus GetJobStatus(int jobID) const;
    bool isJobComplete(int jobID) const;
    // TO DO: ASK PROF ABOUT THIS CHANGE 
    void FinishCompletedJobs(); 
    void FinishJob(int jobID); 
    int GetJobID(Job* job);
    void GetAllJobInfo();

    private: 
    Job* ClaimAJob(unsigned long workerJobFlags); 
    void onJobCompleted(Job* jobJustExecuted); 
    // void FinishCompletedJobs(); 
    // void FinishJob(int jobID); 

    static JobSystem* s_jobSystem; 

    std::vector<JobWorkerThread*>   m_workerThreads; 
    mutable std::mutex              m_workerThreadsMutex; 
    std::deque< Job*>               m_jobsQueued; 
    std::deque<Job*>                m_jobsRunning;
    std::deque<Job*>                m_jobsCompleted; 
    mutable std::mutex              m_jobsQueuedMutex; 
    mutable std::mutex              m_jobsRunningMutex; 
    mutable std::mutex              m_jobsCompletedMutex; 

    std::vector<JobHistoryEntry>    m_jobHistory; 
    mutable int                     m_jobHistoryLowestActiveIndex = 0; 
    mutable std::mutex              m_jobHistoryMutex; 


};
