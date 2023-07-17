#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "format.h"
#include "functionality.h"

void test_func_1ch_mutation() {
    
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
    
    WAV_FILE file = wav_open("test-files/1ch-mutation.wav", WAV_NEW);
    assert(file.bin.open && "File couldn't be opened.");
    
    wav_set_1ch_defaults(&file);
    
    for (i = 0; i < total_samples; i++) {
        
        if (tone_hstate) {
            wav_push_1ch_sample(&file, tone_high);
        } else {
            wav_push_1ch_sample(&file, tone_low);
        }
        
        if ((i % tone_spp) == 0) {
            tone_hstate = !tone_hstate;
        }
    }
    
    wav_close(&file);
    assert(!file.bin.open && "File couldn't be closed.");
}

void test_func_2ch_mutation() {
    
    printf("[*] TEST: Functionality -> Two Channel Mutation\n");
    
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
    assert(file.bin.open && "File couldn't be opened.");
    
    wav_set_2ch_defaults(&file);
    
    for (i = 0; i < total_samples; i++) {
        
        wav_push_2ch_sample(&file, *tone1_current, *tone2_current);
        
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
    
    wav_close(&file);
    assert(!file.bin.open && "File couldn't be closed.");
}

void test_func_1ch_edit() {
    
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
    assert(file.bin.open && "File couldn't be opened - NEW.");
    
    assert(wav_set_defaults(&file) && "Couldn't set defaults.");
    
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
    assert(!file.bin.open && "File couldn't be closed - NEW.");
    
    // Alter Tone
    
    int32_t  current_sample      = 0;
    uint32_t attenuation_counter = 0;
    float    attenuation         = 0.0F;
    
    file = wav_open("test-files/1ch-edit.wav", WAV_ALTER);
    assert(file.bin.open && "File couldn't be opened - ALTER.");
    
    while (wav_has_next(&file)) {
        
        current_sample = wav_next_sample(&file);
        attenuation    = (attenuation_counter / (float)sample_rate);
        
        current_sample *= attenuation;
        
        if (++attenuation_counter == sample_rate) {
            attenuation_counter = 0;
        }
        
        wav_set_psample(&file, current_sample);
    }
    
    wav_close(&file);
    assert(!file.bin.open && "File couldn't be closed - ALTER.");
}

void test_func_2ch_edit() {
    
    uint8_t  duration                 = 3;
    uint32_t sample_rate              = 44100;
    uint32_t total_samples            = duration * sample_rate;
    int16_t  tone_high                = (pow(2, 16) - 1) / 2;
    int16_t  tone_low                 = tone_high * -1;
    uint16_t tone1_frequency          = 1000;
    uint16_t tone1_samples_per_period = (sample_rate / tone1_frequency) / 2;
    uint16_t tone1_current_value      = tone_high;
    uint16_t tone2_frequency          = 2000;
    uint16_t tone2_samples_per_period = (sample_rate / tone2_frequency) / 2;
    uint16_t tone2_current_value      = tone_high;
    
    // Generate Tone
    
    WAV_FILE file = wav_open("test-files/2ch-edit.wav", WAV_NEW);
    assert(file.bin.open && "File couldn't be opened - NEW.");
    
    assert(wav_set_2ch_defaults(&file) && "Couldn't set 2 channel defaults.");
    
    uint32_t smp_count;
    
    for (smp_count = 0; smp_count < total_samples; smp_count++) {
        
        assert(wav_push_2ch_sample(&file, tone1_current_value, tone2_current_value)
            && "Couldn't push 2 channel sample.");
        
        if ((smp_count % tone1_samples_per_period) == 0) {
            tone1_current_value = (tone1_current_value == tone_high) ?
                tone_low : tone_high;
        }
        
        if ((smp_count % tone2_samples_per_period) == 0) {
            tone2_current_value = (tone2_current_value == tone_high) ?
                tone_low : tone_high;
        }
    }
    
    wav_close(&file);
    assert(!file.bin.open && "File couldn't be closed - NEW.");
    
    // Alter Tone
    
    WAV_FILE in  = wav_open("test-files/2ch-edit.wav", WAV_READ);
    WAV_FILE out = wav_open("test-files/2ch-edit-alt.wav", WAV_NEW);
    
    assert(in.bin.open && "File couldn't be opened - IN.");
    assert(out.bin.open && "File couldn't be opened - OUT.");
    
    int32_t left_attenth  = 4410;
    int32_t right_attenth = 8820;
    float   left_atten;
    float   right_atten;
    int32_t left_ch;
    int32_t right_ch;
    
    wav_set_2ch_defaults(&out);
    
    while (wav_has_next(&in)) {
        
        wav_next_2ch_sample(&in, &left_ch, &right_ch);
        
        left_atten  = (in.curr % left_attenth) / (float)left_attenth;
        right_atten = (in.curr % right_attenth) / (float)right_attenth;
        
        if (left_atten > 1) {
            left_atten = 0;
        }
        
        if (right_atten > 1) {
            right_atten = 0;
        }
        
        left_ch  *= left_atten;
        right_ch *= right_atten;
        
        wav_push_2ch_sample(&out, left_ch, right_ch);
    }
    
    wav_close(&in);
    wav_close(&out);
    
    assert(!in.bin.open && "File couldn't be closed - IN.");
    assert(!out.bin.open && "File couldn't be closed - OUT.");
}

void test_functionality() {
    
    test_func_1ch_mutation();    
    test_func_2ch_mutation();
    test_func_1ch_edit();
    test_func_2ch_edit();
}
