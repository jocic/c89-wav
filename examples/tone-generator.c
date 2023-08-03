#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "c89-wav/format.h"

void main() {
    
    WAV_FILE file = wav_open("/path/to/file.wav", WAV_NEW);
    
    if (wav_is_open(&file)) {
        
        uint32_t i;
        
        uint8_t  duration       = 2;
        uint32_t sample_rate    = 44100;
        uint32_t total_samples  = duration * sample_rate;
        uint16_t tone_frequency = 1000;
        uint16_t tone_spp       = (sample_rate / tone_frequency) / 2;
        bool     tone_hstate    = false;
        
        WAV_PCM16  tone_high = WAV_PCM16_MAX;
        WAV_PCM16  tone_low  = WAV_PCM16_MIN;
        WAV_PCM16* tone_val  = &tone_low;
        
        if (wav_is_open(&file)) {
            
            wav_set_defaults(&file, 1);
            
            for (i = 0; i < total_samples; i++) {
                
                wav_push_sample(&file, tone_val, NULL);
                
                if ((i % tone_spp) == 0) {
                    
                    if (tone_hstate) {
                        tone_val = &tone_low;
                    } else {
                        tone_val = &tone_high;
                    }
                    
                    tone_hstate = !tone_hstate;
                }
            }
            
            if (!wav_close(&file)) {
                // Handle I/O Error
            }
        }
    }
}
