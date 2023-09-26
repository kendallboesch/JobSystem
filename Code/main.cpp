#include <iostream>
#include <string> 

#include "JobSystem.h"
#include "RenderJob.h"
#include "CompileJob.h"


int main(void ) {
    std::cout << "Creating Job System" << std::endl;

    JobSystem* js = JobSystem::CreateOrGet(); 

    std::cout << "Creating worker threads" << std::endl; 
    js->CreateWorkerThread("Thread0", 0xFFFFFFFF);
    js->CreateWorkerThread("Thread1", 0xFFFFFFFF);
    js->CreateWorkerThread("Thread2", 0xFFFFFFFF);
    js->CreateWorkerThread("Thread3", 0xFFFFFFFF);
    js->CreateWorkerThread("Thread4", 0xFFFFFFFF);
    js->CreateWorkerThread("Thread5", 0xFFFFFFFF);
    js->CreateWorkerThread("Thread6", 0xFFFFFFFF);
    js->CreateWorkerThread("Thread7", 0xFFFFFFFF);
    js->CreateWorkerThread("Thread8", 0xFFFFFFFF);
    js->CreateWorkerThread("Thread9", 0xFFFFFFFF);

    std::cout << "Create jobs" << std::endl; 

    std::vector<Job*> jobs; 

    CompileJob* cjb = new CompileJob(0xFFFFFFFF, 1); 
    jobs.push_back(cjb); 


    for(int j = 0; j < 10; j++)
    {
        RenderJob* rjb = new RenderJob(0xFFFFFFFF, 1); 
        for(int i = 0; i < 5; i++)
        {
            rjb->data.push_back(i+j); 
        }
        jobs.push_back(rjb);
    }


    std::cout << "Queueing Jobs" << std::endl; 

    std::vector<Job*>::iterator it = jobs.begin(); 

    for(; it != jobs.end(); ++it)
    {
        js->QueueJob(*it); 
    }

    int running = 1; 

    while(running)
    {
        std::string command; 
        std::cout << "Enter stop, destroy, finish, finishjob, status" << std::endl; 
        std::cin >> command; 

        if(command == "stop")
        {
            std::cout << "stopping" << std::endl; 
            running = 0; 
        }
        else if(command == "destroy")
        {
            js->FinishCompletedJobs();
            js->Destory(); 
            running = 0; 
        }
        else if(command == "finish")
        {
           js->FinishCompletedJobs(); 
           std::cout << "Total Jobs Completed: " << js->totalJobs << std::endl; 

        }
        else if(command == "finishjob")
        {
            std:: cout << "Finishing Job 0" << std::endl; 
            js->FinishJob(0);

        }
        else if (command == "status")
        {
            std::cout << "Job status " << js->GetJobStatus(0) << std::endl; 
        }
        else 
        {
            std::cout <<"Invalid command" << std::endl; 
        }
        
    }


    return 0;
}