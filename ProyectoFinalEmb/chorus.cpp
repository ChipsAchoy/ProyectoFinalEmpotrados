#include "chorus.h"
#include <cmath>
#include <omp.h>

Chorus::Chorus(float delay_time, float depth, float frequency, float sample_rate)
    : delay_time(delay_time), depth(depth), frequency(frequency), sample_rate(sample_rate), phase(0.0f) {
    init_buffer();
}

void Chorus::init_buffer() {
    std::size_t delay_length = static_cast<std::size_t>((delay_time + depth) * sample_rate);
    delay_buffer.resize(delay_length, 0.0f);
    delay_index = 0;
}

void Chorus::process(const std::vector<float>& input, std::vector<float>& output) {
    output.resize(input.size());
    //#pragma omp parallel for
    for (std::size_t i = 0; i < input.size(); ++i) {
    	//#pragma omp critical
        output[i] = process_sample(input[i]);
    }
}

float Chorus::process_sample(float input_sample) {
    float modulated_delay = delay_time + depth * std::sin(2.0f * M_PI * frequency * phase / sample_rate);
    std::size_t modulated_index = static_cast<std::size_t>((delay_index + delay_buffer.size() - static_cast<std::size_t>(modulated_delay * sample_rate)) % delay_buffer.size());

    float delayed_sample = delay_buffer[modulated_index];
    delay_buffer[delay_index] = input_sample;

    delay_index = (delay_index + 1) % delay_buffer.size();
    phase = std::fmod(phase + 1.0f, sample_rate / frequency);

    return input_sample + delayed_sample;
}

