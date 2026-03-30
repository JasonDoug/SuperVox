#ifndef SUPERVOX_DSP_PROCESSOR_H
#define SUPERVOX_DSP_PROCESSOR_H

#include <vector>

namespace supervox {

class Processor {
public:
    virtual ~Processor() = default;
    virtual void process(float* buffer, int numSamples) = 0;
};

class GainProcessor : public Processor {
public:
    GainProcessor(float gain = 1.0f) : gain_(gain) {}
    void setGain(float gain) { gain_ = gain; }
    void process(float* buffer, int numSamples) override {
        for (int i = 0; i < numSamples; ++i) {
            buffer[i] *= gain_;
        }
    }
private:
    float gain_;
};

} // namespace supervox

#endif // SUPERVOX_DSP_PROCESSOR_H
