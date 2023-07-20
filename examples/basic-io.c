#include <stdio.h>
#include <stdlib.h>

#include "c89-wav/format.h"

void main() {
    
    WAV_FILE file = wav_open("/path/to/file.wav", WAV_READ);
    
    if (wav_is_open(&file)) {
        
        // Do Stuff
        
        if (!wav_close(&file)) {
            // Handle I/O Error
        }
    }
}
