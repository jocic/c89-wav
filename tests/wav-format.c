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

void test_wav_read() {
    
    printf("[*] TEST: WAV Format -> Reading\n");
    
    WAV_FILE file = wav_open("test-files/60hz-5s-16bit-signed-44100.wav", WAV_READ);
    assert(file.bin.open && "File couldn't be opened.");
    
    assert(wav_get_ChunkID(&file)       == 0x52494646 && "Invalid header value: Chunk ID");
    assert(wav_get_ChunkSize(&file)     == 0x0006BACC && "Invalid header value: Chunk Size");
    assert(wav_get_Format(&file)        == 0x57415645 && "Invalid header value: Format");
    assert(wav_get_Subchunk1ID(&file)   == 0x666D7420 && "Invalid header value: Subchunk1 ID");
    assert(wav_get_Subchunk1Size(&file) == 0x00000010 && "Invalid header value: Subchunk1 Size");
    assert(wav_get_AudioFormat(&file)   == 0x00000001 && "Invalid header value: Audio Format");
    assert(wav_get_NumChannels(&file)   == 0x00000001 && "Invalid header value: Num Channels");
    assert(wav_get_SampleRate(&file)    == 0x0000AC44 && "Invalid header value: Sample Rate");
    assert(wav_get_ByteRate(&file)      == 0x00015888 && "Invalid header value: Byte Rate");
    assert(wav_get_BlockAlign(&file)    == 0x00000002 && "Invalid header value: Block Align");
    assert(wav_get_BitsPerSample(&file) == 0x00000010 && "Invalid header value: Bits Per Sample");
    assert(wav_get_Subchunk2ID(&file)   == 0x64617461 && "Invalid header value: Subchunk2 ID");
    assert(wav_get_Subchunk2Size(&file) == 0x0006BAA8 && "Invalid header value: Subchunk2 Size");
    
    assert(wav_get_sample(&file, 0)  == 0x6666 && "Invalid read, offset = 0");
    assert(wav_get_sample(&file, 1)  == 0x6667 && "Invalid read, offset = 1");
    assert(wav_get_sample(&file, 2)  == 0x6666 && "Invalid read, offset = 2");
    assert(wav_get_sample(&file, 15) == 0x6667 && "Invalid   read, offset = 15");
    assert(wav_get_sample(&file, 30) == 0x6666 && "Invalid read, offset = 30");
    assert(wav_get_sample(&file, 45) == 0x6665 && "Invalid read, offset = 45");
    
    wav_close(&file);
    assert(!file.bin.open && "File couldn't be closed.");
}

void test_wav_write() {
    
}

void test_wav_rewind() {
    
}

void test_wav_format() {
    
    test_wav_open_close();
    test_wav_functions();
    test_wav_read();
    test_wav_write();
    test_wav_rewind();
}
