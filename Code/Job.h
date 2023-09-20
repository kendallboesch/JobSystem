#pragma once // What is this? --> prevents circular includes
#include <mutex>
#include <map>
#include <deque>
#include <vector>
#include <thread>

class Job{
    friend class JobSystem; //what is friend class
    friend class JobWorkerThread;

public:
    //What are job channels
    // Inside firsy parenthesis, were defineng parameters for constructor with defaulr values
    Job(unsigned long jobChannels = 0xFFFFFFFF, int jobType = -1) : m_jobChannels(jobChannels), m_jobType(jobType){
        static int s_nextJobID = 0;
        m_jobID = s_nextJobID++;
    }

    virtual ~Job() {}
    //Virtual = ca
    //pure virtual = must be inherited & have to overload --> cerating an abstraxct class
    virtual void Execute() = 0;
    // do not have to implement --> if you dont its going to calll this body (aka do nothing)
    virtual void JobCompleteCallback() {}
    int GetUniqueID() const {return m_jobID; }

private:
    int m_jobID = -1;
    int m_jobType = -1;

    unsigned long m_jobChannels = 0xFFFFFFFF;
};


