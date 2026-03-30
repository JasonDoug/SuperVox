#ifndef SUPERVOX_DSP_RINGBUFFER_H
#define SUPERVOX_DSP_RINGBUFFER_H

#include <vector>
#include <atomic>
#include <cstring>

namespace supervox {

/**
 * A simple lock-free (SPSC) ring buffer for audio samples
 */
class RingBuffer {
public:
    RingBuffer(int capacity) : capacity_(capacity), buffer_(capacity) {
        head_.store(0);
        tail_.store(0);
    }

    // Write samples into the buffer. Returns the number of samples actually written.
    int write(const float* data, int numSamples) {
        int tail = tail_.load(std::memory_order_relaxed);
        int head = head_.load(std::memory_order_acquire);
        
        int available = capacity_ - (tail - head);
        if (available <= 0) return 0;
        
        int toWrite = std::min(numSamples, available);
        for (int i = 0; i < toWrite; ++i) {
            buffer_[(tail + i) % capacity_] = data[i];
        }
        
        tail_.store(tail + toWrite, std::memory_order_release);
        return toWrite;
    }

    // Read samples from the buffer. Returns the number of samples actually read.
    int read(float* data, int numSamples) {
        int head = head_.load(std::memory_order_relaxed);
        int tail = tail_.load(std::memory_order_acquire);
        
        int available = tail - head;
        if (available <= 0) return 0;
        
        int toRead = std::min(numSamples, available);
        for (int i = 0; i < toRead; ++i) {
            data[i] = buffer_[(head + i) % capacity_];
        }
        
        head_.store(head + toRead, std::memory_order_release);
        return toRead;
    }

    int availableToRead() const {
        return tail_.load(std::memory_order_acquire) - head_.load(std::memory_order_acquire);
    }

    int availableToWrite() const {
        return capacity_ - (tail_.load(std::memory_order_acquire) - head_.load(std::memory_order_acquire));
    }

private:
    int capacity_;
    std::vector<float> buffer_;
    std::atomic<int> head_;
    std::atomic<int> tail_;
};

} // namespace supervox

#endif // SUPERVOX_DSP_RINGBUFFER_H
