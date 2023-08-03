#include <stdio.h>
#include <stdint.h>

#include "c89-wav/format.h"

void main() {
    
    WAV_FILE file = wav_open("/path/to/file.wav", WAV_READ);
    
    WAV_PCM16 sample;
    
    if (wav_is_open(&file)) {
        
        wav_get_sample(&file, 2, &sample, NULL); // Fetch N-th Sample (0-Indexed)
        // wav_get_1ch_sample(&file, 2, &sample);
        
        while (wav_has_next(&file)) { // Loop Through Samples
            
            wav_next_sample(&file, &sample, NULL);
            
            // ...
        }
        
        wav_rewind(&file); // Rewind To Loop Again
        
        if (!wav_close(&file)) {
            // Handle I/O Error
        }
    }
}
