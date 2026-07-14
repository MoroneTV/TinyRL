#ifndef PicoWav_h
#define PicoWav_h

#include <Arduino.h>

class PicoWav{

public:
    PicoWav(const char* audioFilename, uint32_t duration, uint16_t num_channels, uint32_t sampling_rate, uint16_t bits_per_sample);

    bool begin();
     
private:
    const char* _audioFilename;
    uint32_t _duration;
    uint16_t _num_channels;
    uint32_t _sampling_rate;
    uint16_t _bits_per_sample;
};

#endif