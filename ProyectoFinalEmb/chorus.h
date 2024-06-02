#ifndef CHORUS_H
#define CHORUS_H

#include <vector>
#include <cstddef>

class Chorus {
public:
    Chorus(float delay_time = 0.025f, float depth = 0.002f, float frequency = 1.0f, float sample_rate = 44100.0f);
    void process(const std::vector<float>& input, std::vector<float>& output);

private:
    float delay_time;
    float depth;
    float frequency;
    float sample_rate;
    float phase;

    std::vector<float> delay_buffer;
    std::size_t delay_index;

    void init_buffer();
    float process_sample(float input_sample);
};

#endif // CHORUS_H

