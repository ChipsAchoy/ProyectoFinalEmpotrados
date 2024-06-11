#include "chorus.h"
#include <cmath>
#include <omp.h>
#include <vector>

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
    std::size_t delay_length = delay_buffer.size();
    float thread_phase = phase;
    std::size_t thread_delay_index = delay_index;

    // Parallelize this loop with OpenMP
    #pragma omp parallel
    {
        std::vector<float> local_delay_buffer(delay_length, 0.0f);
        std::size_t local_delay_index = thread_delay_index;
        float local_phase = thread_phase;

        #pragma omp for
        for (std::size_t i = 0; i < input.size(); ++i) {
            float input_sample = input[i];

            float modulated_delay = delay_time + depth * std::sin(2.0f * M_PI * frequency * local_phase / sample_rate);
            std::size_t modulated_index = static_cast<std::size_t>((local_delay_index + delay_length - static_cast<std::size_t>(modulated_delay * sample_rate)) % delay_length);

            float delayed_sample = local_delay_buffer[modulated_index];
            local_delay_buffer[local_delay_index] = input_sample;

            local_delay_index = (local_delay_index + 1) % delay_length;
            local_phase = std::fmod(local_phase + 1.0f, sample_rate / frequency);

            output[i] = input_sample + delayed_sample;
        }
    }

    // Update the shared state (optional: you might need to decide how to handle this part)
    phase = std::fmod(phase + input.size() / frequency, sample_rate / frequency);
    delay_index = (delay_index + input.size()) % delay_buffer.size();
}

float Chorus::process_sample(float input_sample) {
    // This function is not used in the parallel processing version
    return input_sample;
}
