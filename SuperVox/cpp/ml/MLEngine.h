#ifndef SUPERVOX_ML_ENGINE_H
#define SUPERVOX_ML_ENGINE_H

#include <vector>
#include <string>

namespace supervox {

class MLEngine {
public:
    virtual ~MLEngine() = default;
    virtual bool loadModel(const std::string& modelPath) = 0;
    virtual void processFrame(const float* input, float* output, int numSamples) = 0;
};

} // namespace supervox

#endif // SUPERVOX_ML_ENGINE_H
