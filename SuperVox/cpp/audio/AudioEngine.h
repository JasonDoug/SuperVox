#ifndef SUPERVOX_AUDIOENGINE_H
#define SUPERVOX_AUDIOENGINE_H

#include <vector>
#include <memory>

namespace supervox {

class AudioEngine {
public:
    virtual ~AudioEngine() = default;
    virtual bool start() = 0;
    virtual void stop() = 0;
    virtual void setFocusStrength(float strength) = 0;
    virtual void setLowEQ(float db) = 0;
    virtual void setMidEQ(float db) = 0;
    virtual void setHighEQ(float db) = 0;

    static std::unique_ptr<AudioEngine> create();
};

} // namespace supervox

#endif // SUPERVOX_AUDIOENGINE_H
