#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "format.h"
#include "wav-format.h"

void test_wav_open() {
    
    printf("[*] TEST: WAV Format -> Open\n");
    
    WAV_FILE file = wav_open("test-files/wav-format/16bit-test.wav", WAV_READ);
    
    assert(file.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file, false) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_close(&file) && "File couldn't be closed.");
    assert(wav_last_error(&file, false) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_commit() {
    
    printf("[*] TEST: WAV Format -> Commit\n");
    
    WAV_FILE file = wav_open("test-files/wav-format/commit-test.wav", WAV_NEW);
    
    assert(file.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file, false) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_set_1ch_defaults(&file) && "Error setting defaults.");
    
    assert(wav_get_Subchunk2Size(&file) == 0 && "Invalid subchunk2 size.");
    assert(wav_sample_count(&file) == 0 && "Invalid smaple count.");
    
    uint32_t i;
    
    for (i = 0; i < 44100; i++) {
        wav_push_1ch_sample(&file, 0xAA);
    }
    
    assert(wav_commit(&file) && "File couldn't be commited.");
    
    assert(wav_get_Subchunk2Size(&file) == 88200 && "Invalid subchunk2 size.");
    assert(wav_sample_count(&file) == 44100 && "Invalid smaple count.");
    
    assert(wav_close(&file) && "File couldn't be closed.");
    assert(wav_last_error(&file, false) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_general() {
    
    printf("[*] TEST: WAV Format -> General\n");
    
    // 1 Channel
    
    WAV_FILE file_1ch = wav_open("test-files/wav-format/16bit-test.wav", WAV_READ);
    
    assert(file_1ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_1ch, false) == WAV_ERR_NONE && "Error flag set.");
    
    assert((wav_est_duration(&file_1ch) == 5000) && "Invalid duration.");
    assert((wav_sample_count(&file_1ch) == 50000) && "Invalid sample count.");
    
    assert(wav_close(&file_1ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_1ch, false) == WAV_ERR_NONE && "Error flag set.");
    
    // 2 Channel
    
    WAV_FILE file_2ch = wav_open("test-files/wav-format/mic-rec.wav", WAV_READ);
    
    assert(file_2ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_2ch, false) == WAV_ERR_NONE && "Error flag set.");
    
    assert((wav_est_duration(&file_2ch) == 3599) && "Invalid duration.");
    assert((wav_sample_count(&file_2ch) == 158747) && "Invalid sample count.");
    
    assert(wav_close(&file_2ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_2ch, false) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_sugar() {
    
    printf("[*] TEST: WAV Format -> Sugar\n");
    
    WAV_FILE file = wav_open("test-files/wav-format/sugar-test.wav", WAV_NEW);
    
    assert(file.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file, false) == WAV_ERR_NONE && "Error flag set.");
    assert(wav_is_open(&file) && "Open - false flag.");
    
    assert(wav_set_1ch_defaults(&file) && "Error setting defaults.");
    
    assert(!wav_is_altered(&file) && "Altered - false flag.");    
    assert(file.curr == 0 && "Invalid pointer.");
    
    uint32_t i;
    
    for (i = 0; i < 44100; i++) {
        wav_push_1ch_sample(&file, 0xAA);
    }
    
    assert(wav_is_altered(&file) && "Altered - false flag.");
    assert(file.curr == 44100 && "Invalid pointer.");
    
    wav_rewind(&file);  
    assert(file.curr == 0 && "Invalid pointer.");
    
    assert(wav_close(&file) && "File couldn't be closed.");
    assert(wav_last_error(&file, false) == WAV_ERR_NONE && "Error flag set.");
    assert(!wav_is_open(&file) && "Open - false flag.");
}

void test_wav_valid() {
    
    struct target {
        char* file_path;
        bool  valid;
    };
    
    uint8_t i;
    
    struct target test_files[8] = {
        { "test-files/wav-format/8bit-test.wav",  true },
        { "test-files/wav-format/16bit-test.wav", true },
        { "test-files/wav-format/24bit-test.wav", true },
        { "test-files/wav-format/32bit-test.wav", true },
        { "test-files/binary-io/1.dat", false },
        { "test-files/binary-io/2.dat", false },
        { "test-files/binary-io/3.dat", false },
        { "test-files/binary-io/4.dat", false }
    };
    
    printf("[*] TEST: WAV Format -> Valid\n");
    
    for (i = 0; i < 8; i++) {
        
        WAV_FILE file = wav_open(test_files[i].file_path, WAV_READ);
        
        assert(file.bin.open && "File couldn't be opened.");
        assert(wav_last_error(&file, false) == WAV_ERR_NONE && "Error flag set.");
        
        assert(wav_is_valid(&file) == test_files[i].valid
            && "Status doesn't match.");
        
        assert(wav_close(&file) && "File couldn't be closed.");
    }
}

void test_wav_defaults() {
    
    printf("[*] TEST: WAV Format -> Defaults\n");
    
    // 1 Channel
    
    WAV_FILE file_1ch = wav_open("test-files/wav-format/16bit-test.wav", WAV_READ);
    
    assert(file_1ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_1ch, false) == WAV_ERR_NONE && "Error flag set.");
    

    
    assert(wav_close(&file_1ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_1ch, false) == WAV_ERR_NONE && "Error flag set.");
    
    // 2 Channel
    
    WAV_FILE file_2ch = wav_open("test-files/wav-format/mic-rec.wav", WAV_READ);
    
    assert(file_2ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_2ch, false) == WAV_ERR_NONE && "Error flag set.");
    

    
    assert(wav_close(&file_2ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_2ch, false) == WAV_ERR_NONE && "Error flag set.");
}

#define MAIN_TEST_FILE "test-files/60hz-5s-16bit-signed-44100.wav"


void test_wav_read() {
    
    printf("[*] TEST: WAV Format -> Reading\n");
    
    WAV_FILE file = wav_open(MAIN_TEST_FILE, WAV_READ);
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
    
    printf("[*] TEST: WAV Format -> Writing\n");
    
    WAV_FILE file = wav_open("test-files/test-tone.wav", WAV_NEW);
    assert(file.bin.open && "File couldn't be opened.");
    
    wav_set_defaults(&file);
    
    int val_high = pow(2, 15) - 1;
    int val_low  = val_high * -1;
    
    int i, j;
    
    for (i = 0, j = 0; i < 44100 * 5; i++) {
        
        if (j) {
            assert(wav_push_sample(&file, val_high) && "Couldn't push a sample: HIGH");
        } else {
            assert(wav_push_sample(&file, val_low) && "Couldn't push a sample: LOW");
        }
        
        if ((i % 133) == 0) {
            j = !j;
        }
    }
    
    wav_close(&file);
    assert(!file.bin.open && "File couldn't be closed.");
}

void test_wav_format() {
    
    test_wav_open();
    test_wav_commit();
    test_wav_general();
    test_wav_sugar();
    test_wav_valid();
    test_wav_defaults();
}
