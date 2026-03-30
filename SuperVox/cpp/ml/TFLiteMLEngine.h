#ifndef SUPERVOX_TFLITE_ML_ENGINE_H
#define SUPERVOX_TFLITE_ML_ENGINE_H

#include "MLEngine.h"
#include <memory>
#include <string>

namespace supervox {

class TFLiteMLEngine : public MLEngine {
public:
    TFLiteMLEngine();
    ~TFLiteMLEngine();

    bool loadModel(const std::string& modelPath) override;
    void processFrame(const float* input, float* output, int numSamples) override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace supervox

#endif // SUPERVOX_TFLITE_ML_ENGINE_H
