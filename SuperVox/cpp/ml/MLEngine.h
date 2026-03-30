#ifndef SUPERVOX_ML_ENGINE_H
#define SUPERVOX_ML_ENGINE_H

#include <vector>
#include <string>

namespace supervox {

class MLEngine {
public:
    virtual ~MLEngine() = default;
    
    // Load a model from a file path
    virtual bool loadModel(const std::string& modelPath) = 0;
    
    // Process a single audio frame of size getExpectedFrameSize()
    // input and output buffers MUST be at least getExpectedFrameSize() samples long
    virtual void processFrame(const float* input, float* output) = 0;
    
    // Returns the number of samples the model expects for a single inference call
    virtual int getExpectedFrameSize() const = 0;
    
    // Returns the approximate latency introduced by the model in milliseconds
    virtual float getProcessingLatencyMs() const = 0;
};

} // namespace supervox

#endif // SUPERVOX_ML_ENGINE_H
