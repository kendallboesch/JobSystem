#include <vector>
#include "Job.h"

class RenderJob : public Job {
public:
    RenderJob(unsigned long jobChannels = 0xFFFFFFFF, int jobType = -1) : Job(jobChannels, jobType) {}; // constructor
    ~RenderJob(){}; // Deconstructoer

    std::vector<int> data;
    void Execute();
    void JobCompleteCallback();
};

