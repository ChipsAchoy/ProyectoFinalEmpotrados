#include <iostream>
#include <sndfile.h>
#include <mpg123.h>
#include <lame/lame.h>
#include <vector>
#include <cstring>
#include "reverb.h"
#include "delay.h"
#include "tremolo.h"
#include "chorus.h"
#include "audio_socket.h"
#include <fstream>

const char* input_filename = "input.mp3";
const char* output_filename = "output.mp3";

void apply_reverb(const std::vector<float>& input, std::vector<float>& output, float room_size, float damping, float width) {
    Reverb reverb(room_size, damping, width);
    reverb.process(input, output);
}

void apply_delay(const std::vector<float>& input, std::vector<float>& output, float delay_time, float feedback, float mix) {
    Delay delay(delay_time, feedback, mix);
    delay.process(input, output);
}

void apply_tremolo(const std::vector<float>& input, std::vector<float>& output, float frequency, float depth) {
    Tremolo tremolo(frequency, depth);
    tremolo.process(input, output);
}

void apply_chorus(const std::vector<float>& input, std::vector<float>& output, float delay_time, float depth, float frequency) {
    Chorus chorus(delay_time, depth, frequency);
    chorus.process(input, output);
}

std::vector<unsigned char> read_file(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return data;
}

void write_file(const char* filename, const std::vector<unsigned char>& data) {
    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

int main() {
    while(1){
    try {
        // Listen on all interfaces
        AudioSocket audioSocket("0.0.0.0", 8777);
        std::vector<unsigned char> input_data;
        std::vector<unsigned char> chunk;
        size_t total_bytes_received = 0;

        // Receive audio data in chunks of 1024 bytes
        std::cout << "Waiting for audio data" << std::endl;
        while ((chunk = audioSocket.receive()).size() > 0) {
            input_data.insert(input_data.end(), chunk.begin(), chunk.end());
            
            total_bytes_received += chunk.size();
            std::cout << "Received chunk of size: " << chunk.size() << " bytes" << std::endl;
            if (chunk.size() < 1024){
               break;
            }
            
        }
        std::cout << "Total audio data received: " << total_bytes_received << " bytes" << std::endl;
        write_file(input_filename, input_data);

        // Initialize mpg123
        mpg123_init();
        mpg123_handle* mh = mpg123_new(nullptr, nullptr);
        if (mpg123_open(mh, input_filename) != MPG123_OK) {
            std::cerr << "Failed to open input file: " << input_filename << std::endl;
            mpg123_delete(mh);
            mpg123_exit();
            return 1;
        }

        // Get audio format information
        long rate;
        int channels, encoding;
        mpg123_getformat(mh, &rate, &channels, &encoding);

        // Read and decode MP3 file
        std::vector<float> input;
        unsigned char buffer[4096];
        size_t bytes_read;
        while (mpg123_read(mh, buffer, sizeof(buffer), &bytes_read) == MPG123_OK) {
            for (size_t i = 0; i < bytes_read; i += 2) { // Assuming 16-bit PCM
                int16_t sample = 0;
                std::memcpy(&sample, buffer + i, 2);
                input.push_back(sample / 32768.0f); // Convert to float
            }
        }

        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();

        // Apply effects
        std::vector<float> output(input.size());
        // Uncomment one of the following lines to apply a specific effect
        //apply_delay(input, output, 0.08f, 0.08f, 0.2f);
        apply_reverb(input, output, 0.8f, 0.5f, 0.5f);
        //apply_tremolo(input, output, 5.0f, 0.5f);
        //apply_chorus(input, output, 0.025f, 0.002f, 1.0f);

        // Encode output to MP3
        lame_t lame = lame_init();
        lame_set_in_samplerate(lame, rate);
        lame_set_num_channels(lame, channels);
        lame_set_VBR(lame, vbr_default);
        lame_init_params(lame);

        FILE* mp3file = fopen(output_filename, "wb");
        if (!mp3file) {
            std::cerr << "Failed to open output file: " << output_filename << std::endl;
            lame_close(lame);
            return 1;
        }

        std::vector<unsigned char> mp3_buffer(7200 + output.size() * 2);
        int mp3_bytes = lame_encode_buffer_interleaved_ieee_float(lame, output.data(), output.size() / channels, mp3_buffer.data(), mp3_buffer.size());
        fwrite(mp3_buffer.data(), 1, mp3_bytes, mp3file);
        mp3_bytes = lame_encode_flush(lame, mp3_buffer.data(), mp3_buffer.size());
        fwrite(mp3_buffer.data(), 1, mp3_bytes, mp3file);

        fclose(mp3file);
        lame_close(lame);

        std::cout << "Effects applied and output saved to MP3 successfully!" << std::endl;

        std::vector<unsigned char> output_data = read_file(output_filename);

        // Send processed audio data in chunks of 1024 bytes
        size_t total_bytes_sent = 0;
        size_t chunk_size = 1024;
        for (size_t i = 0; i < output_data.size(); i += chunk_size) {
            size_t end = std::min(i + chunk_size, output_data.size());
            std::vector<unsigned char> chunk(output_data.begin() + i, output_data.begin() + end);
            audioSocket.send(chunk);
            total_bytes_sent += chunk.size();
            std::cout << "Sent chunk of size: " << chunk.size() << " bytes" << std::endl;
        }
        std::cout << "Total processed audio data sent: " << total_bytes_sent << " bytes" << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    }
    return 0;
}

