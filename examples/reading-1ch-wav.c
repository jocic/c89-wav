#include <stdio.h>
#include <stdint.h>

#include "c89-wav/format.h"

void main() {
    
    WAV_FILE file = wav_open("/path/to/file.wav", WAV_RD);
    
    if (wav_is_open(&file)) {
        
        while (wav_has_next(&file)) { // Loop Through Samples
            
            int32_t sample = wav_next_sample(&file);
            
            // ...
        }
        
        if (!wav_close(&file)) {
            // Handle I/O Error
        }
    }
}
