#include <iostream>
#include <string> 

#include "JobSystem.h"
#include "RenderJob.h"
#include "CompileJob.h"


int main(void ) {
    std::cout << "Creating Job System" << std::endl;

    JobSystem* js = JobSystem::CreateOrGet(); 

    std::cout << "Creating worker threads" << std::endl; 
   // js->CreateWorkerThread("Thread0", 0xFFFFFFFF);
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
        for(int i = 0; i < 1000; i++)
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
    int loop = 0; 
    int running = 1; 

//     // while(running)
//     // {
        
//         std::string command; 
//         std::cout << "Enter stop, destroy, finish, finishjob, status: ";
//         std::cin >> command; 
//         if (loop >= 7)
//         {
//             command = "stop";
//         }

//         std::cout << "INPUT: " << command << std::endl;

//         if(command == "stop")
//         {
//             std::cout << "stopping" << std::endl; 
//             running = 0; 
//         }
//         else if(command == "destroy")
//         {
//             js->FinishCompletedJobs();
//             js->Destory(); 
//             running = 0; 
//         }
//         else if(command == "finish")
//         {
//            js->FinishCompletedJobs(); 
//            std::cout << "Total Jobs Completed: " << js->totalJobs << std::endl; 

//         }
//         else if(command == "finishjob")
//         {


//         }
//         else if (command == "status")
//         {
            JobStatus stat = js->GetJobStatus(0); 
            std::cout << "Job status " << stat << std::endl; 
            std:: cout << "Finishing Job 0" << std::endl; 
            js->FinishJob(0);
            stat = js->GetJobStatus(0);
            std::cout << "STATUS: " << stat << std::endl;
            
//             switch(stat)
//             {
//                 case JOB_STATUS_NEVER_SEEN : 
//                     std::cout << "Never seen" << std::endl;
//                     break; 
//                 case JOB_STATUS_QUEUED :
//                     std::cout << "Queued" << std::endl;
//                     break; 
//                 case JOB_STATUS_RUNNING : 
//                     std::cout << "Running" << std::endl;
//                     break; 
//                 case JOB_STATUS_COMPLETED : 
//                     std::cout << "Completed" << std::endl; 
//                     break; 
//                 case JOB_STATUS_RETIRED : 
//                     std::cout << "Retired" << std::endl; 
//                     break;
//                 case NUM_JOB_STATUSES : 
//                     std::cout << "Num stats??" << std::endl;
//                     break;


//             }

//         }
//         else 
//         {
//             std::cout <<"Invalid command" << std::endl; 
//         }
//         loop++; 
//    // }


    return 0;
}