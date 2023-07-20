#include <stdio.h>
#include <stdint.h>

#include "c89-wav/format.h"

void main() {
    
    WAV_FILE file = wav_open("/path/to/file.wav", WAV_READ);
    
    if (wav_is_open(&file)) {
        
        uint32_t chunk_id        = wav_get_ChunkID(&file);
        uint32_t chunk_size      = wav_get_ChunkSize(&file);
        uint32_t format          = wav_get_Format(&file);
        uint32_t subchunk1_id    = wav_get_Subchunk1ID(&file);
        uint32_t subchunk1_size  = wav_get_Subchunk1Size(&file);
        uint16_t audio_format    = wav_get_AudioFormat(&file);
        uint16_t num_channels    = wav_get_NumChannels(&file);
        uint32_t sample_rate     = wav_get_SampleRate(&file);
        uint32_t byte_rate       = wav_get_ByteRate(&file);
        uint16_t block_align     = wav_get_BlockAlign(&file);
        uint16_t bits_per_sample = wav_get_BitsPerSample(&file);
        uint32_t subchunk2_id    = wav_get_Subchunk2ID(&file);
        uint32_t subchunk2_size  = wav_get_Subchunk2Size(&file);
        
        if (!wav_close(&file)) {
            // Handle I/O Error
        }
    }
}
