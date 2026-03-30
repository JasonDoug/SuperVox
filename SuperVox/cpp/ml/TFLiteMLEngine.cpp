#include "TFLiteMLEngine.h"
#include <iostream>

namespace supervox {

struct TFLiteMLEngine::Impl {
    int frameSize = 512; // Default frame size for many real-time models
};

TFLiteMLEngine::TFLiteMLEngine() : impl_(std::make_unique<Impl>()) {}

TFLiteMLEngine::~TFLiteMLEngine() = default;

bool TFLiteMLEngine::loadModel(const std::string& modelPath) {
    // Mock implementation for now
    std::cout << "[TFLite] Loading model: " << modelPath << std::endl;
    return true; 
}

void TFLiteMLEngine::processFrame(const float* input, float* output) {
    // Mock processing: just copy input to output (pass-through)
    for (int i = 0; i < impl_->frameSize; ++i) {
        output[i] = input[i];
    }
}

int TFLiteMLEngine::getExpectedFrameSize() const {
    return impl_->frameSize;
}

float TFLiteMLEngine::getProcessingLatencyMs() const {
    return (float)impl_->frameSize / 48.0f; // Approx for 48kHz
}

} // namespace supervox
