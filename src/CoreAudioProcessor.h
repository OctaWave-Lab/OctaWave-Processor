#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

#include <string>

// Function declaration for processing audio
std::string processAudioFileInProcessor(const std::string& filePath);

// Function declaration for finding the dominant frequency
int findDominantFrequency(const std::vector<int16_t>& audioData, int sampleRate);

#endif // AUDIO_PROCESSOR_H