#include <stdio.h>
#include <stdint.h>

#include "c89-wav/format.h"

void main() {
    
    WAV_FILE file = wav_open("/path/to/file.wav", WAV_READ);
    
    int16_t lsample, rsample;
    
    if (wav_is_open(&file)) {
        
        while (wav_has_next(&file)) { // Loop Through Samples
            
            wav_next_2ch_sample(&file, &lsample, &rsample);
            
            // ...
        }
        
        if (!wav_close(&file)) {
            // Handle I/O Error
        }
    }
}
