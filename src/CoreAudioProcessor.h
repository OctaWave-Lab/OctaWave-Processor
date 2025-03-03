#ifndef CORE_AUDIO_PROCESSOR_H
#define CORE_AUDIO_PROCESSOR_H

#include <string>

namespace OctaWave {
    std::string processAudioFileInProcessor(const std::string& filePath);
    int findDominantFrequency(const std::vector<int16_t>& audioData, int sampleRate);
}

#endif