#ifndef SUPERVOX_COREML_ML_ENGINE_H
#define SUPERVOX_COREML_ML_ENGINE_H

#include "MLEngine.h"
#include <memory>
#include <string>

namespace supervox {

class CoreMLEngine : public MLEngine {
public:
    CoreMLEngine();
    ~CoreMLEngine();

    bool loadModel(const std::string& modelPath) override;
    void processFrame(const float* input, float* output) override;
    int getExpectedFrameSize() const override;
    float getProcessingLatencyMs() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace supervox

#endif // SUPERVOX_COREML_ML_ENGINE_H
