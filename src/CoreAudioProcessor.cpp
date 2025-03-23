#include <complex>
#include <vector>
#include <cmath>
#include <fstream>
#include <android/log.h>
#include "CoreAudioProcessor.h"

#define LOG_TAG "OctaWaveNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#define PI 3.14159265358979323846
#define SAMPLE_WINDOW_SIZE 1024 // Improved sample size for FFT performance

namespace OctaWave {

    std::string processAudioFileInProcessor(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            LOGI("Error: File not found.");
            return "Error: File not found.";
        }

        char header[44];
        file.read(header, 44);

        int sampleRate;
        memcpy(&sampleRate, header + 24, sizeof(int));

        // WAV files store sample rate in little-endian
        sampleRate = ((sampleRate & 0xFF) << 24) |
                     ((sampleRate & 0xFF00) << 8) |
                     ((sampleRate & 0xFF0000) >> 8) |
                     ((sampleRate >> 24) & 0xFF);
        LOGI("Sample Rate: %d Hz", sampleRate);

        std::vector<int16_t> audioData;
        int16_t sample;
        while (file.read(reinterpret_cast<char*>(&sample), sizeof(sample))) {
            audioData.push_back(sample);
        }
        file.close();

        LOGI("Loaded %zu samples.", audioData.size());

        // Process only a chunk of samples for efficiency
        if (audioData.size() < SAMPLE_WINDOW_SIZE) {
            LOGI("Insufficient samples for processing.");
            return "Error: Insufficient audio data.";
        }

        std::vector<int16_t> audioDataChunk(audioData.begin(), audioData.begin() + SAMPLE_WINDOW_SIZE);

        int dominantFreq = findDominantFrequency(audioDataChunk, sampleRate);
        LOGI("Detected Frequency: %d Hz", dominantFreq);

        return "Detected Frequency: " + std::to_string(dominantFreq) + " Hz";
    }

    void fft(std::vector<std::complex<double>>& data);

    int findDominantFrequency(const std::vector<int16_t>& audioData, int sampleRate) {
        std::vector<std::complex<double>> complexData(audioData.begin(), audioData.end());

        // Normalize data to reduce noise impact
        double maxAmplitude = *std::max_element(audioData.begin(), audioData.end());
        if (maxAmplitude != 0) {
            for (auto& sample : complexData) {
                sample /= maxAmplitude;
            }
        }
        LOGI("Normalized Data Applied");

        // Resize data to nearest power of 2 for FFT
        size_t dataSize = complexData.size();
        size_t fftSize = 1;
        while (fftSize < dataSize) fftSize *= 2;

        complexData.resize(fftSize, 0);  // Zero-pad for optimal FFT
        LOGI("FFT Data Size: %zu", fftSize);

        // Apply FFT
        fft(complexData);

        // Find peak frequency
        int peakIndex = 0;
        double maxMagnitude = 0.0;
        for (size_t i = 0; i < fftSize / 2; i++) {
            double magnitude = std::abs(complexData[i]);
            if (magnitude > maxMagnitude) {
                maxMagnitude = magnitude;
                peakIndex = i;
            }
        }

        double dominantFrequency = (peakIndex * sampleRate) / static_cast<double>(fftSize);
        LOGI("Dominant Frequency Calculated: %.2f Hz", dominantFrequency);

        return static_cast<int>(dominantFrequency);
    }

    void fft(std::vector<std::complex<double>>& data) {
        size_t n = data.size();
        if (n <= 1) return;

        std::vector<std::complex<double>> even(n / 2), odd(n / 2);
        for (size_t i = 0; i < n / 2; i++) {
            even[i] = data[i * 2];
            odd[i] = data[i * 2 + 1];
        }

        fft(even);
        fft(odd);

        for (size_t i = 0; i < n / 2; i++) {
            std::complex<double> t = std::polar(1.0, -2 * PI * i / n) * odd[i];
            data[i] = even[i] + t;
            data[i + n / 2] = even[i] - t;
        }

        LOGI("FFT Complete for %zu samples.", n);
    }
}