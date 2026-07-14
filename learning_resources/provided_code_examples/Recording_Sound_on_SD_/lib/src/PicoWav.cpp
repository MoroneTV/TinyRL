#include <Arduino.h>
#include "PicoWav.h"
#include "SD.h"

PicoWav:: PicoWav(const char* audioFilename, uint32_t duration, uint16_t num_channels, uint32_t sampling_rate, uint16_t bits_per_sample){
    _audioFilename  = audioFilename;
    _duration       = duration;
    _num_channels   = num_channels;
    _sampling_rate  = sampling_rate;
    _bits_per_sample= bits_per_sample;
}

bool PicoWav:: begin(){
    
  // data size in bytes - > this amount of data should be recorded from microphone later
  
  uint32_t data_size = _sampling_rate * _num_channels * _bits_per_sample * _duration/8;

  File new_audio_file = SD.open(_audioFilename, FILE_WRITE);
  if (new_audio_file == NULL) {
    //Serial.println("Unable to open "+String(_audioFilename)+" to create wav header.");
    return false;
  }
  
 // add ".wav" HEADER parameters 
  uint8_t CHUNK_ID[4] = {'R', 'I', 'F', 'F'};
  new_audio_file.write(CHUNK_ID, 4);

  uint32_t chunk_size = data_size + 36;
  uint8_t CHUNK_SIZE[4] = {chunk_size, chunk_size >> 8, chunk_size >> 16, chunk_size >> 24};
  new_audio_file.write(CHUNK_SIZE, 4);

  uint8_t FORMAT[4] = {'W', 'A', 'V', 'E'};
  new_audio_file.write(FORMAT, 4);

  uint8_t SUBCHUNK_1_ID[4] = {'f', 'm', 't', ' '};
  new_audio_file.write(SUBCHUNK_1_ID, 4);

  uint8_t SUBCHUNK_1_SIZE[4] = {0x10, 0x00, 0x00, 0x00};
  new_audio_file.write(SUBCHUNK_1_SIZE, 4);

  uint8_t AUDIO_FORMAT[2] = {0x01, 0x00};
  new_audio_file.write(AUDIO_FORMAT, 2);

  uint8_t NUM_CHANNELS[2] = {_num_channels, _num_channels >> 8};
  new_audio_file.write(NUM_CHANNELS, 2);

  uint8_t SAMPLING_RATE[4] = {_sampling_rate, _sampling_rate >> 8, _sampling_rate >> 16, _sampling_rate >> 24};
  new_audio_file.write(SAMPLING_RATE, 4);

  uint32_t byte_rate = _num_channels * _sampling_rate * _bits_per_sample / 8;
  uint8_t BYTE_RATE[4] = {byte_rate, byte_rate >> 8, byte_rate >> 16, byte_rate >> 24};
  new_audio_file.write(BYTE_RATE, 4);

  uint16_t block_align = _num_channels * _bits_per_sample / 8;
  uint8_t BLOCK_ALIGN[2] = {block_align, block_align >> 8};
  new_audio_file.write(BLOCK_ALIGN, 2);

  uint8_t BITS_PER_SAMPLE[2] = {_bits_per_sample, _bits_per_sample >> 8};
  new_audio_file.write(BITS_PER_SAMPLE, 2);

  uint8_t SUBCHUNK_2_ID[4] = {'d', 'a', 't', 'a'};
  new_audio_file.write(SUBCHUNK_2_ID, 4);

  uint8_t SUBCHUNK_2_SIZE[4] = {data_size, data_size >> 8, data_size >> 16, data_size >> 24};
  new_audio_file.write(SUBCHUNK_2_SIZE, 4);

  //actual audio data should be appended after this point later
  new_audio_file.close();
  return true;
}