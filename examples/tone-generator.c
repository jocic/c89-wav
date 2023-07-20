#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "c89-wav/format.h"

void main() {
    
    uint32_t smp_ptr;
    
    uint8_t  duration       = 2;
    uint32_t sample_rate    = 44100;
    uint32_t total_samples  = duration * sample_rate * 2;
    uint16_t tone_frequency = 1000;
    
    int16_t  tone_high  = (pow(2, 16) - 1) / 2;
    int16_t  tone_low   = tone_high * -1;
    int16_t  tone_spp   = (sample_rate / tone_frequency) / 2;
    int16_t* tone_val   = &tone_low;
    bool     tone_state = false;
    
    // Generate Tone
    
    WAV_FILE file = wav_open("/path/to/file.wav", WAV_NEW);
    
    if (wav_is_open(&file)) {
        
        wav_set_1ch_defaults(&file);
        
        for (i = 0; i < total_samples; i++) {
            
            wav_push_sample(&file, tone_val);
            
            if ((i % tone_spp) == 0) {
                
                if (tone_state) {
                    tone_val = &tone_low;
                } else {
                    tone_val = &tone_high;
                }
                
                tone_state = !tone_state;
            }
        }
        
        if (!wav_close(&file)) {
            // Handle I/O Error
        }
    }
}
