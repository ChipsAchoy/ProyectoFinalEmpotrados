#ifndef TREMOLO_H
#define TREMOLO_H

#include <vector>
#include <cstddef>

class Tremolo {
public:
    Tremolo(float frequency = 5.0f, float depth = 0.5f, float sample_rate = 44100.0f);
    void process(const std::vector<float>& input, std::vector<float>& output);

private:
    float frequency;
    float depth;
    float sample_rate;
    float phase;

    void init();
    float process_sample(float input_sample);
};

#endif // TREMOLO_H

