#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>

enum waveShape { SINE, SQUARE, TRIANGLE, SAWTOOTH };

enum octaveInput { OCTAVE_0 = 1, OCTAVE_1 = 2, OCTAVE_2 = 3, OCTAVE_3 = 4,
    OCTAVE_4 = 5, OCTAVE_5 = 6, OCTAVE_6 = 7, OCTAVE_7 = 8, OCTAVE_8 = 9,
};

enum scale { CHROMATIC, MAJOR, MINOR };

void create16bit_char(unsigned char* buffer, std::string str){ buffer[0] = str[0]; buffer[1] = str[1]; }

void create32bit_char(unsigned char* buffer, std::string str){ buffer[0] = str[0]; buffer[1] = str[1]; buffer[2] = str[2]; buffer[3] = str[3]; }

void create16bit_Int(unsigned char* buffer, uint16_t value){ // little endian 16bits int writing
    buffer[0] = static_cast<unsigned char>(value & 0xFF);
    buffer[1] = static_cast<unsigned char>((value >> 8) & 0xFF);
}

void create32bit_Int(unsigned char* buffer, uint32_t value){ // little endian 32bits int writing
    buffer[0] = static_cast<unsigned char>(value & 0xFF);
    buffer[1] = static_cast<unsigned char>((value >> 8) & 0xFF);
    buffer[2] = static_cast<unsigned char>((value >> 16) & 0xFF);
    buffer[3] = static_cast<unsigned char>((value >> 24) & 0xFF);
}

void writingBits(unsigned char* buffer, uint32_t pcmValue, unsigned bits){ // this is writing the sin, sqr or tri value into the file, after the header
    const unsigned bytes = bits / 8;
    for (unsigned i = 0; i < bytes; ++i){
        buffer[i] = static_cast<unsigned char>((pcmValue >> (i * 8)) & 0xFF);
    }
}

int32_t floatToPCMConverter(float waveValue, unsigned bits){ // this is converting the calculated float value of each wave (tri, sqr, sin) into PCM values which is required to write with writingBits()
    if (waveValue > 1.0f) waveValue = 1.0f;
    if (waveValue < -1.0f) waveValue = -1.0f;
    if (bits == 8){
        int value = static_cast<int>(std::round(waveValue * 127.0f)) + 128;
        if (value < 0)  value = 0;
        if (value > 255) value = 255;
        return value;
    }
    const int64_t maxValue = (int64_t{1} << (bits - 1)) - 1;
    return static_cast<int32_t>(std::round(waveValue * maxValue));
}

void wavMaker(int channels, int bits, int sampleRate, const std::vector<unsigned char>& audioData){ // this is creating the WAV header
    std::vector<unsigned char> buffer(44);
    uint32_t dataSize = audioData.size();
    create32bit_char(&buffer[0], "RIFF"); // RIFF HEADER
    create32bit_Int(&buffer[4], dataSize + 36); // RIFF chunk size = file size - 8
    create32bit_char(&buffer[8], "WAVE"); // FORMAT
    create32bit_char(&buffer[12], "fmt "); // Subchunk1 ID
    create32bit_Int(&buffer[16], 16); // Subchunk 1 Size
    create16bit_Int(&buffer[20], 1); // Audio format : PCM lossless
    create16bit_Int(&buffer[22], channels); // CHANNELS
    create32bit_Int(&buffer[24], sampleRate); // SAMPLE RATE
    create32bit_Int(&buffer[28], sampleRate * channels * (bits / 8)); // BYTE RATE
    create16bit_Int(&buffer[32], channels * bits / 8); // BLOCK ALIGNMENT
    create16bit_Int(&buffer[34], bits); // BITS PER SAMPLE
    create32bit_char(&buffer[36], "data"); // Subchunk 2 ID (DATA)
    create32bit_Int(&buffer[40], dataSize); // Subchunk 2 Size (DATA SIZE)

    std::cout << "Successfully created the header \n";

    std::ofstream outputWav("audioData.wav", std::ios::binary);
    std::cout << "Empty wav file created \n";
    outputWav.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size());
    std::cout << "Header written on the wav \n";
    outputWav.write(reinterpret_cast<const char*>(&audioData[0]), dataSize);
    std::cout << "audioData written on the wav \n";
    outputWav.close();
}

void writeWaveform(uint32_t samplerate, std::vector<unsigned char>& audioData, float frequency, unsigned bits, waveShape waveShape, float gain, float attack, float decay, float sustain, float release){
    const float pi = 3.14159265358979323846;
    const size_t bytesPerSample = bits / 8;
    const float phaseIncrement = frequency / samplerate;
    float phase = 0;
    unsigned int i;
    float PCMValue;
    float waveValue; // is determined in the switch case
    float envelope = 0.0f;
    float attackSlope = 1.0f / (attack * samplerate);
    float decaySlope = (1.0f - sustain) / (decay * samplerate);
    float releaseSlope = sustain / (release * samplerate);

    enum EnvStage { ATTACK, DECAY, SUSTAIN, RELEASE, DONE };
    EnvStage stage = ATTACK;

    for (i = 0; i < audioData.size(); i += bytesPerSample){
        switch (stage){
            case ATTACK: envelope += attackSlope;
                if (envelope >= 1.0f){
                    envelope = 1.0f;
                    stage = DECAY;
                }
                break;
            case DECAY: envelope -= decaySlope;
                if (envelope <= sustain){
                    envelope = sustain;
                    stage = SUSTAIN;
                }
                break;
            case SUSTAIN:
                if (envelope <= sustain){
                    envelope = sustain;
                    stage = SUSTAIN;
                }
                break;
            case RELEASE: envelope -= releaseSlope;
                if (envelope <= 0.0f){
                    envelope = 0.0f;
                    stage = DONE;
                }
                break;
            case DONE: envelope = 0.0f;
                break;
        }
        switch (waveShape){
            case SINE:
                waveValue = gain * envelope * sin(2.0f * pi * phase);
                break;
            case SQUARE:
                waveValue = gain * envelope * (phase < 0.5f ? -1.0f : 1.0f);
                break;
            case TRIANGLE:
                waveValue = gain * envelope * (1.0f - 4.0f * fabs(phase - 0.5f));
                break;
            case SAWTOOTH:
                waveValue = gain * envelope * (2.0f * phase - 1.0f);
                break;
        }
        PCMValue = floatToPCMConverter(waveValue, bits);
        writingBits(&audioData[i], static_cast<uint32_t>(PCMValue), bits);
        phase += phaseIncrement;
        if (phase >= 1.0f) phase -= 1.0f;
    }
}

float noteToFrequency(int midiNote){
    return 440.0f * pow(2.0f, (midiNote - 69) / 12.0f);
}

void initMapping(std::vector<int>& keyMap, octaveInput oct, int keyboardSize, scale scale){
    int i = 0;
    switch (scale){
        case CHROMATIC:
            for (i=0; i < keyboardSize;i++){
                keyMap[i] = oct * 12 + i;
            }
            break;
        case MAJOR:
            std::cout << "MAJOR SCALE HASNT BEEN DONE YET" << std::endl;
            break;
        case MINOR:
            std::cout << "MINOR SCALE HASNT BEEN DONE YET" << std::endl;
            break;
    }
}

int main(){
    // START OF VARIABLE DECLARATIONS | START OF VARIABLE DECLARATIONS
    int keyboardSize = 16;
    octaveInput octave = OCTAVE_4;
    std::vector<int> keyMap(keyboardSize); // crétion du tableau ou chaque index correspond à une touche du clavier
    initMapping(keyMap, octave, keyMap.size(), CHROMATIC);
    int midiNote = keyMap[9];
    std::cout << "Midi Note: " << midiNote << std::endl;
    float frequency = noteToFrequency(midiNote);
    std::cout << "Frequency: " << frequency << std::endl;

    float gain = 1;

    uint32_t samplerate = 48000;
    uint16_t channels = 1;
    uint16_t bits = 16;
    uint32_t durationSeconds = 10;
    uint32_t frames = durationSeconds * samplerate;
    uint32_t dataSize = frames * channels * (bits / 8);

    waveShape main = SINE;

    float attack = 1.0f;
    float decay = 0.1f;
    float sustain = 0.5f;
    float release = 0.1f;

    std::vector<unsigned char> audioData(dataSize); // création du tableau DATASIZE

    // END OF VARIABLE DECLARATIONS | END OF VARIABLE DECLARATIONS


    writeWaveform(samplerate, audioData, frequency, bits, main, gain, attack, decay, sustain, release);
    wavMaker(channels, bits, samplerate, audioData);

    return 0;
}
