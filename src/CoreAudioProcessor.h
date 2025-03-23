#ifndef CORE_AUDIO_PROCESSOR_H
#define CORE_AUDIO_PROCESSOR_H

#include <string>
#include <complex>
#include <vector>

namespace OctaWave {
    std::string processAudioFileInProcessor(const std::string &filePath);

    int findDominantFrequency(const std::vector<int16_t> &audioData, int sampleRate);

    void fft(std::vector<std::complex<double>> &data, size_t depth = 0);
}

#endif