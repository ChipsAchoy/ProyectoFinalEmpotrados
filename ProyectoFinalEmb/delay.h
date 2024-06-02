#ifndef DELAY_H
#define DELAY_H

#include <vector>
#include <cstddef>

class Delay {
public:
    Delay(float delay_time = 0.5f, float feedback = 0.5f, float mix = 0.5f, float sample_rate = 44100.0f);
    void process(const std::vector<float>& input, std::vector<float>& output);

private:
    float delay_time;
    float feedback;
    float mix;
    float sample_rate;

    std::vector<float> delay_buffer;
    std::size_t delay_index;

    void init_buffer();
    float process_sample(float input_sample);
};

#endif // DELAY_H

