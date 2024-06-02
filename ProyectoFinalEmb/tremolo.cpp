#include "tremolo.h"
#include <cmath>
#include <omp.h>

Tremolo::Tremolo(float frequency, float depth, float sample_rate)
    : frequency(frequency), depth(depth), sample_rate(sample_rate), phase(0.0f) {
    init();
}

void Tremolo::init() {
    phase = 0.0f;
}

void Tremolo::process(const std::vector<float>& input, std::vector<float>& output) {
    output.resize(input.size());
    //#pragma omp parallel for
    for (std::size_t i = 0; i < input.size(); ++i) {
    	//#pragma omp critical
        output[i] = process_sample(input[i]);
    }
}

float Tremolo::process_sample(float input_sample) {
    float modulation = (1.0f - depth) + depth * std::sin(2.0f * M_PI * frequency * phase / sample_rate);
    phase = std::fmod(phase + 1.0f, sample_rate / frequency);
    return input_sample * modulation;
}

