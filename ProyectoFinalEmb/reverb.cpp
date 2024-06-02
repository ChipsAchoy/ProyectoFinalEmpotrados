#include "reverb.h"
#include <cmath>
#include <algorithm>
#include <omp.h>

Reverb::Reverb(float room_size, float damping, float width, float sample_rate)
    : room_size(room_size), damping(damping), width(width), sample_rate(sample_rate) {
    init_buffers();
}

void Reverb::init_buffers() {
    std::size_t comb_lengths[4] = {1116, 1188, 1277, 1356};
    std::size_t allpass_lengths[2] = {225, 341};

    for (int i = 0; i < 4; ++i) {
        comb_buffers[i].resize(comb_lengths[i], 0.0f);
        comb_indices[i] = 0;
    }

    for (int i = 0; i < 2; ++i) {
        allpass_buffers[i].resize(allpass_lengths[i], 0.0f);
        allpass_indices[i] = 0;
    }
}

void Reverb::process(const std::vector<float>& input, std::vector<float>& output) {
    output.resize(input.size());
    //#pragma omp parallel for
    for (size_t i = 0; i < input.size(); ++i) {
    	//#pragma omp critical
        output[i] = process_sample(input[i]);
    }
}

float Reverb::process_sample(float input_sample) {
    float output_sample = input_sample;

    for (int i = 0; i < 4; ++i) {
        float buffer_out = comb_buffers[i][comb_indices[i]];
        comb_buffers[i][comb_indices[i]] = input_sample + buffer_out * room_size;
        comb_indices[i] = (comb_indices[i] + 1) % comb_buffers[i].size();
        output_sample += buffer_out;
    }

    for (int i = 0; i < 2; ++i) {
        float buffer_out = allpass_buffers[i][allpass_indices[i]];
        allpass_buffers[i][allpass_indices[i]] = output_sample + buffer_out * damping;
        allpass_indices[i] = (allpass_indices[i] + 1) % allpass_buffers[i].size();
        output_sample = buffer_out - output_sample * damping;
    }

    return output_sample * width;
}

