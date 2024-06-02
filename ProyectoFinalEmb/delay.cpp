#include "delay.h"
#include <cmath>
#include <algorithm>
#include <omp.h>

Delay::Delay(float delay_time, float feedback, float mix, float sample_rate)
    : delay_time(delay_time), feedback(feedback), mix(mix), sample_rate(sample_rate), delay_index(0) {
    init_buffer();
}

void Delay::init_buffer() {
    std::size_t delay_length = static_cast<std::size_t>(delay_time * sample_rate);
    delay_buffer.resize(delay_length, 0.0f);
}

void Delay::process(const std::vector<float>& input, std::vector<float>& output) {
    output.resize(input.size());
    //#pragma omp parallel for
    for (std::size_t i = 0; i < input.size(); ++i) {
    	//#pragma omp critical
        output[i] = process_sample(input[i]);
    }
}

float Delay::process_sample(float input_sample) {
    float delayed_sample = delay_buffer[delay_index];
    float output_sample = input_sample * (1.0f - mix) + delayed_sample * mix;

    delay_buffer[delay_index] = input_sample + delayed_sample * feedback;
    delay_index = (delay_index + 1) % delay_buffer.size();

    return output_sample;
}

