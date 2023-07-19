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
    assert(wav_last_error(&file) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_close(&file) && "File couldn't be closed.");
    assert(wav_last_error(&file) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_commit() {
    
    printf("[*] TEST: WAV Format -> Commit\n");
    
    WAV_FILE file = wav_open("test-files/wav-format/commit-test.wav", WAV_NEW);
    
    assert(file.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file) == WAV_ERR_NONE && "Error flag set.");
    
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
    assert(wav_last_error(&file) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_general() {
    
    printf("[*] TEST: WAV Format -> General\n");
    
    // 1 Channel
    
    WAV_FILE file_1ch = wav_open("test-files/wav-format/16bit-test.wav", WAV_READ);
    
    assert(file_1ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert((wav_est_duration(&file_1ch) == 5000) && "Invalid duration.");
    assert((wav_sample_count(&file_1ch) == 50000) && "Invalid sample count.");
    
    assert(wav_close(&file_1ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    // 2 Channel
    
    WAV_FILE file_2ch = wav_open("test-files/wav-format/mic-rec.wav", WAV_READ);
    
    assert(file_2ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert((wav_est_duration(&file_2ch) == 3599) && "Invalid duration.");
    assert((wav_sample_count(&file_2ch) == 158747) && "Invalid sample count.");
    
    assert(wav_close(&file_2ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_sugar() {
    
    printf("[*] TEST: WAV Format -> Sugar\n");
    
    WAV_FILE file = wav_open("test-files/wav-format/sugar-test.wav", WAV_NEW);
    
    assert(file.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file) == WAV_ERR_NONE && "Error flag set.");
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
    assert(wav_last_error(&file) == WAV_ERR_NONE && "Error flag set.");
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
        assert(wav_last_error(&file) == WAV_ERR_NONE && "Error flag set.");
        
        assert(wav_is_valid(&file) == test_files[i].valid
            && "Status doesn't match.");
        
        assert(wav_close(&file) && "File couldn't be closed.");
    }
}

void test_wav_defaults() {
    
    printf("[*] TEST: WAV Format -> Defaults\n");
    
    // 1 Channel
    
    WAV_FILE file_1ch = wav_open("test-files/wav-format/1ch-defaults.wav", WAV_NEW);
    
    assert(file_1ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_set_1ch_defaults(&file_1ch) && "Defaults couldn't be set.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_get_ChunkID(&file_1ch) == 0x52494646 && "Invalid header data.");
    assert(wav_get_Format(&file_1ch) == 0x57415645 && "Invalid header data.");
    assert(wav_get_Subchunk1ID(&file_1ch) == 0x666D7420 && "Invalid header data.");
    assert(wav_get_Subchunk1Size(&file_1ch) == 0x10 && "Invalid header data.");
    assert(wav_get_AudioFormat(&file_1ch) == 0x1 && "Invalid header data.");
    assert(wav_get_BitsPerSample(&file_1ch) == 0x10 && "Invalid header data.");
    assert(wav_get_NumChannels(&file_1ch) == 0x1 && "Invalid header data.");
    assert(wav_get_SampleRate(&file_1ch) == 0xAC44 && "Invalid header data.");
    assert(wav_get_ByteRate(&file_1ch) == 0x15888 && "Invalid header data.");
    assert(wav_get_BlockAlign(&file_1ch) == 0x2 && "Invalid header data.");
    assert(wav_get_Subchunk2ID(&file_1ch) == 0x64617461 && "Invalid header data.");
    
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_close(&file_1ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    // 2 Channel
    
    WAV_FILE file_2ch = wav_open("test-files/wav-format/2ch-defaults.wav", WAV_NEW);
    
    assert(wav_set_2ch_defaults(&file_1ch) && "Defaults couldn't be set.");
    
    assert(file_2ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_set_2ch_defaults(&file_1ch) && "Defaults couldn't be set.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_get_ChunkID(&file_1ch) == 0x52494646 && "Invalid header data.");
    assert(wav_get_Format(&file_1ch) == 0x57415645 && "Invalid header data.");
    assert(wav_get_Subchunk1ID(&file_1ch) == 0x666D7420 && "Invalid header data.");
    assert(wav_get_Subchunk1Size(&file_1ch) == 0x10 && "Invalid header data.");
    assert(wav_get_AudioFormat(&file_1ch) == 0x1 && "Invalid header data.");
    assert(wav_get_BitsPerSample(&file_1ch) == 0x10 && "Invalid header data.");
    assert(wav_get_NumChannels(&file_1ch) == 0x2 && "Invalid header data.");
    assert(wav_get_SampleRate(&file_1ch) == 0xAC44 && "Invalid header data.");
    assert(wav_get_ByteRate(&file_1ch) == 0x2B110 && "Invalid header data.");
    assert(wav_get_BlockAlign(&file_1ch) == 0x2 && "Invalid header data.");
    assert(wav_get_Subchunk2ID(&file_1ch) == 0x64617461 && "Invalid header data.");
    
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_close(&file_2ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_get() {
    
    struct test_sample {
        uint32_t num;
        int16_t  lval;
        int16_t  rval;
    };
    
    uint32_t i;
    int16_t  lval, rval;
    
    const struct test_sample samples_1ch[6] = {
        { 0,   0x3D73 }, { 1,   0x6E8F }, { 2,   0x6183 },
        { 199, 0x66E7 }, { 200, 0x65E7 }, { 201, 0x66E1 }
    };
    
    const struct test_sample samples_2ch[6] = {
        { 0,   0x0000, 0x0000 }, { 3,   0x0001, 0x0000 },
        { 4,   0xFFFE, 0x0000 }, { 5,   0x0004, 0x0000 },
        { 6,   0xFFFE, 0x0000 }, { 100, 0xFFFD, 0x0002 }
    };
    
    printf("[*] TEST: WAV Format -> Get\n");
    
    // 1 Channel
        
    WAV_FILE file_1ch = wav_open("test-files/wav-format/16bit-test.wav", WAV_READ);
    
    assert(file_1ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    for (i = 0; i < 6; i++) {
        break;
        assert((wav_get_sample(&file_1ch, samples_1ch[i].num) ==
            samples_1ch[i].lval) && "Invalid sample value.");
        
        wav_get_1ch_sample(&file_1ch, samples_1ch[i].num, &lval);
        assert((lval == samples_1ch[i].lval) && "Invalid sample value.");
    }
    
    assert(wav_close(&file_1ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    // 2 Channel
    
    WAV_FILE file_2ch = wav_open("test-files/wav-format/mic-rec.wav", WAV_READ);
    
    assert(file_2ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
    
    for (i = 0; i < 6; i++) {
        
        wav_get_2ch_sample(&file_2ch, samples_2ch[i].num, &lval, &rval);
        
        assert((lval == samples_2ch[i].lval) && "Invalid sample value.");
        assert((rval == samples_2ch[i].rval) && "Invalid sample value.");
    }
    
    assert(wav_close(&file_1ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_format() {
    
    test_wav_open();
    test_wav_commit();
    test_wav_general();
    test_wav_sugar();
    test_wav_valid();
    test_wav_defaults();
    test_wav_get();
}
