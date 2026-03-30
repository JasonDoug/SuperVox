#include <iostream>
#include <vector>
#include <cassert>
#include "../dsp/Processor.h"

int main() {
    std::cout << "[Test] Starting GainProcessor Test..." << std::endl;

    supervox::GainProcessor processor(2.0f);
    std::vector<float> buffer = {0.1f, 0.2f, 0.5f, 1.0f};
    
    processor.process(buffer.data(), buffer.size());

    assert(buffer[0] == 0.2f);
    assert(buffer[1] == 0.4f);
    assert(buffer[2] == 1.0f);
    assert(buffer[3] == 2.0f);

    std::cout << "[Test] GainProcessor (Gain 2.0x) PASSED." << std::endl;

    processor.setGain(0.5f);
    processor.process(buffer.data(), buffer.size());

    assert(buffer[0] == 0.1f);
    assert(buffer[1] == 0.2f);
    assert(buffer[2] == 0.5f);
    assert(buffer[3] == 1.0f);

    std::cout << "[Test] GainProcessor (Gain 0.5x) PASSED." << std::endl;
    std::cout << "[Test] All C++ Core Unit Tests PASSED!" << std::endl;

    return 0;
}
