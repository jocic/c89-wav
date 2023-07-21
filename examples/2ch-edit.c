#include <stdio.h>
#include <stdint.h>

#include "c89-wav/format.h"

void main() {
    
    int16_t lin_sample,  rin_sample;
    int16_t lout_sample, rout_sample;
    float   latten, ratten;
    
    WAV_FILE input  = wav_open("test-files/2ch-mutation.wav", WAV_READ);
    WAV_FILE output = wav_open("test-files/2ch-edit.wav", WAV_NEW);
    
    if (wav_is_open(&input) && wav_is_open(&output)) {
        
        wav_copy_meta(&input, &output);
        
        while (wav_has_next(&input)) {
            
            wav_next_2ch_sample(&input, &lin_sample, &rin_sample);
            
            latten = (wav_csample(&input) % 4410) / (float)4410;
            ratten = (wav_csample(&input) % 8820) / (float)8820;
            
            lout_sample = lin_sample * latten;
            rout_sample = rin_sample * ratten;
            
            wav_push_2ch_sample(&output, &lout_sample, &rout_sample);
        }
        
        if (!wav_close(&input)) {
            fprintf(stderr, "Input file couldn't be closed.");
        }
        
        if (!wav_close(&output)) {
            fprintf(stderr, "Output file couldn't be closed.");
        }
    }
}
