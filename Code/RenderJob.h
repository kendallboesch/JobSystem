#include <vector>
#include "Job.h"

class RenderJob : public Job {
public:
    RenderJob(unsigned long jobChannels, int jobType) : Job(jobChannels, jobType) {}; // constructor
    ~RenderJob(){}; // Deconstructoer

    std::vector<int> data;
    void Execute();
    void JobCompleteCallback();
};

