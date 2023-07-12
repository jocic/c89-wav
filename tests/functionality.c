#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "format.h"
#include "functionality.h"

void test_1ch_mutation() {
    
    printf("[*] TEST: Functionality -> One Channel Mutation\n");
    
    uint32_t i;
    
    uint8_t  duration       = 2;
    uint32_t sample_rate    = 44100;
    uint32_t total_samples  = duration * sample_rate * 2;
    uint16_t tone_frequency = 1000;
    
    int32_t tone_high   = (pow(2, 16) - 1) / 2;
    int32_t tone_low    = tone_high * -1;
    int32_t tone_spp    = (sample_rate / tone_frequency) / 2;
    bool    tone_hstate = false;
    
    WAV_FILE file = wav_open("test-files/1ch-test.wav", WAV_NEW);
    assert(file.bin.open && "File couldn't be opened.");
    
    wav_set_1ch_defaults(&file);
    
    for (i = 0; i < total_samples; i++) {
        
        if (tone_hstate) {
            wav_push_sample(&file, tone_high);
        } else {
            wav_push_sample(&file, tone_low);
        }
        
        if ((i % tone_spp) == 0) {
            tone_hstate = !tone_hstate;
        }
    }
    
    wav_close(&file);
    assert(!file.bin.open && "File couldn't be closed.");
}

void test_2ch_mutation() {
    
    printf("[*] TEST: Functionality -> Two Channel Mutation\n");
    
    uint32_t i;
    
    uint8_t  duration      = 2;
    uint32_t sample_rate   = 44100;
    uint32_t total_samples = duration * sample_rate * 2;
    uint16_t tone1_frequency = 1000;
    uint16_t tone2_frequency = 500;
    
    int32_t tone1_high   = (pow(2, 16) - 1) / 2;
    int32_t tone1_low    = tone1_high * -1;
    int32_t tone1_spp    = (sample_rate / tone1_frequency) / 2;
    bool    tone1_hstate = false;
    
    int32_t tone2_high   = (pow(2, 16) - 1) / 2;
    int32_t tone2_low    = tone2_high * -1;
    int32_t tone2_spp    = (sample_rate / tone2_frequency) / 2;
    bool    tone2_hstate = false;
    
    WAV_FILE file = wav_open("test-files/2ch-test.wav", WAV_NEW);
    assert(file.bin.open && "File couldn't be opened.");
    
    wav_set_2ch_defaults(&file);
    
    for (i = 0; i < total_samples; i++) {
        
        if (tone1_hstate) {
            wav_push_sample(&file, tone1_high);
        } else {
            wav_push_sample(&file, tone1_low);
        }
        
        if (tone2_hstate) {
            wav_push_sample(&file, tone2_high);
        } else {
            wav_push_sample(&file, tone2_low);
        }
        
        if ((i % tone1_spp) == 0) {
            tone1_hstate = !tone1_hstate;
        }
        
        if ((i % tone2_spp) == 0) {
            tone2_hstate = !tone2_hstate;
        }
    }
    
    wav_close(&file);
    assert(!file.bin.open && "File couldn't be closed.");
}

void test_functionality() {
    
    test_1ch_mutation();    
    test_2ch_mutation();
}
