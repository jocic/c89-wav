#include <stdio.h>
#include <assert.h>

#include "format.h"
#include "wav-format.h"

void test_wav_open_close() {
    
    printf("[*] TEST: WAV Format -> File Open/Close\n");
    
    WAV_FILE file = wav_open("test-files/60hz-5s-16bit-signed-44100.wav", WAV_READ);
    assert(file.bin.open && "File couldn't be opened.");
    
    wav_close(&file);
    assert(!file.bin.open && "File couldn't be closed.");
}

void test_wav_functions() {
    
    printf("[*] TEST: WAV Format -> General Functions\n");
    
    int i;
    
    struct duration_map {
        char* location;
        int   duration;
        int   samples;
    };
    
    struct duration_map test_files[] = { {
        .location = "test-files/250ms-test.wav",
        .duration = 250,
        .samples  = 11025   
    }, {
        .location = "test-files/1500ms-test.wav",
        .duration = 1500,
        .samples  = 66150
    }, {
        .location = "test-files/10000ms-test.wav",
        .duration = 10000,
        .samples  = 100000
    } };
    
    for (i = 0; i < 3; i++) {
        
        WAV_FILE file = wav_open(test_files[i].location, WAV_READ);
        assert(file.bin.open && "File couldn't be opened.");
        
        assert(wav_is_valid(&file) && "Invalid WAV file.");
        
        int duration_ms = wav_est_duration(&file);
        assert(duration_ms == test_files[i].duration && "Invalid duration.");
        
        int sample_count = wav_sample_count(&file);
        assert(sample_count == test_files[i].samples && "Invalid sample count.");
        
        wav_close(&file);
        assert(!file.bin.open && "File couldn't be closed.");
    }
}

void test_wav_format() {
    
    test_wav_open_close();
    test_wav_functions();
}
