#import "CoreMLEngine.h"
#import <CoreML/CoreML.h>
#import <iostream>

namespace supervox {

struct CoreMLEngine::Impl {
    int frameSize = 512;
    // MLModel *model;
};

CoreMLEngine::CoreMLEngine() : impl_(std::make_unique<Impl>()) {}

CoreMLEngine::~CoreMLEngine() = default;

bool CoreMLEngine::loadModel(const std::string& modelPath) {
    // Mock implementation for Phase 2 foundation
    std::cout << "[CoreML] Loading model: " << modelPath << std::endl;
    return true;
}

void CoreMLEngine::processFrame(const float* input, float* output) {
    // Mock processing: pass-through for Phase 1 verification
    for (int i = 0; i < impl_->frameSize; ++i) {
        output[i] = input[i];
    }
}

int CoreMLEngine::getExpectedFrameSize() const {
    return impl_->frameSize;
}

float CoreMLEngine::getProcessingLatencyMs() const {
    return (float)impl_->frameSize / 48.0f; // Approx for 48kHz
}

} // namespace supervox
