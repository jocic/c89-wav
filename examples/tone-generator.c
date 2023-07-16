#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "c89-wav/format.h"

void main() {
    
    uint32_t smp_ptr            = 0;
    uint8_t  duration           = 5;
    uint32_t sample_rate        = 44100;
    uint32_t total_samples      = duration * sample_rate;
    uint16_t tone_frequency     = 3300;
    uint16_t samples_per_period = (sample_rate / tone_frequency) / 2;
    uint16_t tone_high          = (pow(2, 16) - 1) / 2;
    uint16_t tone_low           = tone_high * -1;
    bool     tone_state         = false;
    
    // Generate Tone
    
    WAV_FILE file = wav_open("test-files/1ch-edit.wav", WAV_NEW);
    
    wav_set_defaults(&file);
    
    for (smp_ptr = 0; smp_ptr < total_samples; smp_ptr++) {
        
        if (tone_state) {
            wav_push_sample(&file, tone_high);
        } else {
            wav_push_sample(&file, tone_low);
        }
        
        if ((smp_ptr % samples_per_period) == 0) {
            tone_state = !tone_state;
        }
    }
    
    wav_close(&file);
}
