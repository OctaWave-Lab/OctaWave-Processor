#include <complex>
#include <vector>
#include <cmath>
#include <fstream>
#include "CoreAudioProcessor.h"
using namespace std;

#define PI 3.14159265358979323846

#include "CoreAudioProcessor.h"
#include <iostream>

#include "CoreAudioProcessor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <complex>

namespace OctaWave {
    std::string processAudioFileInProcessor(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            return "Error: File not found.";
        }

        char header[44];
        file.read(header, 44);

        int sampleRate = *reinterpret_cast<int*>(header + 24);

        std::vector<int16_t> audioData;
        int16_t sample;
        while (file.read(reinterpret_cast<char*>(&sample), sizeof(sample))) {
            audioData.push_back(sample);
        }
        file.close();

        int dominantFreq = findDominantFrequency(audioData, sampleRate);
        return "Detected Frequency: " + std::to_string(dominantFreq) + " Hz";
    }

    void fft(vector<std::complex<double>> vector1);

    int findDominantFrequency(const std::vector<int16_t>& audioData, int sampleRate) {
        size_t n = audioData.size();
        std::vector<std::complex<double>> complexData(n);

        // Convert integer samples to complex numbers
        for (size_t i = 0; i < n; i++) {
            complexData[i] = std::complex<double>(audioData[i], 0);
        }

        // Apply FFT
        fft(complexData);

        // Find peak frequency
        int peakIndex = 0;
        double maxMagnitude = 0.0;
        for (size_t i = 0; i < n / 2; i++) {
            double magnitude = std::abs(complexData[i]);
            if (magnitude > maxMagnitude) {
                maxMagnitude = magnitude;
                peakIndex = i;
            }
        }

        // Convert index to frequency
        double dominantFrequency = (peakIndex * sampleRate) / static_cast<double>(n);
        return static_cast<int>(dominantFrequency);
    }

    // Fast Fourier Transform
    void fft(vector<std::complex<double>> data) {
        size_t n = data.size();
        if (n <= 1) return;

        // Split data into even and odd indices
        vector<complex<double>> even(n / 2), odd(n / 2);
        for (size_t i = 0; i < n / 2; i++) {
            even[i] = data[i * 2];
            odd[i] = data[i * 2 + 1];
        }

        // Recursive FFT
        fft(even);
        fft(odd);

        // Combine results
        for (size_t i = 0; i < n / 2; i++) {
            complex<double> t = polar(1.0, -2 * PI * i / n) * odd[i];
            data[i] = even[i] + t;
            data[i + n / 2] = even[i] - t;
        }
    }
}

