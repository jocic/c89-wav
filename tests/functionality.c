#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "format.h"
#include "functionality.h"

void test_func_1ch_mutation() {
    
    printf("[*] TEST: Functionality -> 1-Channel Mutation\n");
    
    uint32_t i;
    
    uint8_t  duration       = 2;
    uint32_t sample_rate    = 44100;
    uint32_t total_samples  = duration * sample_rate * 2;
    uint16_t tone_frequency = 1000;
    
    int16_t  tone_high   = (pow(2, 16) - 1) / 2;
    int16_t  tone_low    = tone_high * -1;
    int16_t  tone_spp    = (sample_rate / tone_frequency) / 2;
    int16_t* tone_val    = &tone_low;
    bool     tone_hstate = false;
    
    WAV_FILE file = wav_open("test-files/1ch-mutation.wav", WAV_NEW);
    
    if (wav_is_open(&file)) {
        
        wav_set_1ch_defaults(&file);
        
        for (i = 0; i < total_samples; i++) {
            
            wav_push_sample(&file, tone_val);
            
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
            fprintf(stderr, "File couldn't be closed.");
        }
    }
}

void test_func_2ch_mutation() {
    
    printf("[*] TEST: Functionality -> 2-Channel Mutation\n");
    
    uint32_t i;
    
    uint8_t  duration      = 2;
    uint32_t sample_rate   = 44100;
    uint32_t total_samples = duration * sample_rate * 2;
    uint16_t tone1_frequency = 1000;
    uint16_t tone2_frequency = 500;
    
    int32_t  tone1_high    = (pow(2, 16) - 1) / 2;
    int32_t  tone1_low     = tone1_high * -1;
    int32_t* tone1_current = &tone1_low;
    int32_t  tone1_spp     = (sample_rate / tone1_frequency) / 2;
    bool     tone1_hstate  = false;
    
    int32_t  tone2_high    = (pow(2, 16) - 1) / 2;
    int32_t  tone2_low     = tone2_high * -1;
    int32_t* tone2_current = &tone2_low;
    int32_t  tone2_spp     = (sample_rate / tone2_frequency) / 2;
    bool     tone2_hstate  = false;
    
    WAV_FILE file = wav_open("test-files/2ch-mutation.wav", WAV_NEW);
    
    if (wav_is_open(&file)) {
        
        wav_set_2ch_defaults(&file);
        
        for (i = 0; i < total_samples; i++) {
            
            wav_push_2ch_sample(&file, tone1_current, tone2_current);
            
            if ((i % tone1_spp) == 0) {
                
                tone1_hstate = !tone1_hstate;
                
                if (tone1_hstate) {
                    tone1_current = &tone1_high;
                } else {
                    tone1_current = &tone1_low;
                }
            }
            
            if ((i % tone2_spp) == 0) {
                
                tone2_hstate = !tone2_hstate;
                
                if (tone2_hstate) {
                    tone2_current = &tone2_high;
                } else {
                    tone2_current = &tone2_low;
                }
            }
        }
        
        if (!wav_close(&file)) {
            fprintf(stderr, "File couldn't be closed.");
        }
    }
}

void test_func_1ch_edit() {
    
    printf("[*] TEST: Functionality -> 1-Channel Edit\n");
    
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

void test_func_2ch_edit() {
    
    printf("[*] TEST: Functionality -> 2-Channel Edit\n");
    
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

void test_functionality() {
    
    test_func_1ch_mutation();
    test_func_2ch_mutation();
    test_func_1ch_edit();
    test_func_2ch_edit();
}
