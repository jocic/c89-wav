#include <stdio.h>
#include <stdint.h>

#include "c89-wav/format.h"

void main() {
    
    int16_t in_sample;
    int16_t out_sample;
    float   attenuation;
    
    WAV_FILE input  = wav_open("test-files/1ch-mutation.wav", WAV_READ);
    WAV_FILE output = wav_open("test-files/1ch-edit.wav", WAV_NEW);
    
    if (wav_is_open(&input) && wav_is_open(&output)) {
        
        wav_copy_meta(&input, &output);
        
        while (wav_has_next(&input)) {
            
            wav_next_1ch_sample(&input, &in_sample);
            
            attenuation = (wav_csample(&input) % 44100) / (float)44100;
            
            out_sample = in_sample * attenuation;
            
            wav_push_1ch_sample(&output, &out_sample);
        }
        
        if (!wav_close(&input)) {
            fprintf(stderr, "Input file couldn't be closed.");
        }
        
        if (!wav_close(&output)) {
            fprintf(stderr, "Output file couldn't be closed.");
        }
    }
}
