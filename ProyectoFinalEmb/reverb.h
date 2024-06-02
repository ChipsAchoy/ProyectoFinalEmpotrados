#ifndef REVERB_H
#define REVERB_H

#include <vector>
#include <cstddef>  // Añadir esta línea para incluir size_t

class Reverb {
public:
    Reverb(float room_size = 0.5f, float damping = 0.5f, float width = 1.0f, float sample_rate = 44100.0f);
    void process(const std::vector<float>& input, std::vector<float>& output);

private:
    float room_size;
    float damping;
    float width;
    float sample_rate;

    std::vector<float> comb_buffers[4];
    std::vector<float> allpass_buffers[2];
    std::size_t comb_indices[4];
    std::size_t allpass_indices[2];

    void init_buffers();
    float process_sample(float input_sample);
};

#endif // REVERB_H

