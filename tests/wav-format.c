#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "format.h"
#include "wav-format.h"

void test_wav_types() {
    
    assert(sizeof(WAV_PCM8) == 1 && "Invalid PCM8 size.");
    assert(sizeof(WAV_PCM16) == 2 && "Invalid PCM16 size.");
    assert(sizeof(WAV_PCM24) == 4 && "Invalid PCM32 size.");
    assert(sizeof(WAV_PCM32) == 4 && "Invalid PCM32 size.");
    
    assert(WAV_PCM8_MIN == 0 && "Invalid PCM8 min. range.");
    assert(WAV_PCM8_MAX == 255 && "Invalid PCM8 max. range.");
    
    assert(WAV_PCM16_MIN == -32768 && "Invalid PCM16 min. range.");
    assert(WAV_PCM16_MAX == 32767 && "Invalid PCM16 max. range.");
    
    assert(WAV_PCM24_MIN == -8388607 && "Invalid PCM24 min. range.");
    assert(WAV_PCM24_MAX == 8388607 && "Invalid PCM24 max. range.");
    
    assert(WAV_PCM32_MIN == -2147483647 && "Invalid PCM32 min. range.");
    assert(WAV_PCM32_MAX == 2147483647 && "Invalid PCM32 max. range.");
}

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
    
    assert(wav_set_defaults(&file, 1) && "Error setting defaults.");
    
    assert(wav_get_Subchunk2Size(&file) == 0 && "Invalid subchunk2 size.");
    assert(wav_sample_count(&file) == 0 && "Invalid smaple count.");
    
    uint32_t i;
    uint16_t dummy_sample = 0xAA;
    
    for (i = 0; i < 44100; i++) {
        wav_push_1ch_sample(&file, &dummy_sample);
    }
    
    assert(wav_commit(&file) && "File couldn't be commited.");
    
    assert(wav_get_Subchunk2Size(&file) == 88200 && "Invalid subchunk2 size.");
    assert(wav_sample_count(&file) == 44100 && "Invalid smaple count.");
    
    assert(wav_close(&file) && "File couldn't be closed.");
    assert(wav_last_error(&file) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_general() {
    
    printf("[*] TEST: WAV Format -> General\n");
    
    /* 1 Channel */
    
    WAV_FILE file_1ch = wav_open("test-files/wav-format/16bit-test.wav", WAV_READ);
    
    assert(file_1ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert((wav_est_duration(&file_1ch) == 5000) && "Invalid duration.");
    assert((wav_sample_count(&file_1ch) == 50000) && "Invalid sample count.");
    
    assert(wav_close(&file_1ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    /* 2 Channel */
    
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
    
    assert(wav_set_defaults(&file, 1) && "Error setting defaults.");
    
    assert(!wav_is_altered(&file) && "Altered - false flag.");    
    assert(file.curr == 0 && "Invalid pointer.");
    
    uint32_t i;
    uint16_t dummy_sample = 0xAA;
    
    for (i = 0; i < 44100; i++) {
        wav_push_1ch_sample(&file, &dummy_sample);
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
    
    /* 1 Channel */
    
    WAV_FILE file_1ch = wav_open("test-files/wav-format/1ch-defaults.wav", WAV_NEW);
    
    assert(file_1ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_set_defaults(&file_1ch, 1) && "Defaults couldn't be set.");
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
    
    /* 2 Channel */
    
    WAV_FILE file_2ch = wav_open("test-files/wav-format/2ch-defaults.wav", WAV_NEW);
    
    assert(file_2ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_set_defaults(&file_2ch, 2) && "Defaults couldn't be set.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_get_ChunkID(&file_2ch) == 0x52494646 && "Invalid header data.");
    assert(wav_get_Format(&file_2ch) == 0x57415645 && "Invalid header data.");
    assert(wav_get_Subchunk1ID(&file_2ch) == 0x666D7420 && "Invalid header data.");
    assert(wav_get_Subchunk1Size(&file_2ch) == 0x10 && "Invalid header data.");
    assert(wav_get_AudioFormat(&file_2ch) == 0x1 && "Invalid header data.");
    assert(wav_get_BitsPerSample(&file_2ch) == 0x10 && "Invalid header data.");
    assert(wav_get_NumChannels(&file_2ch) == 0x2 && "Invalid header data.");
    assert(wav_get_SampleRate(&file_2ch) == 0xAC44 && "Invalid header data.");
    assert(wav_get_ByteRate(&file_2ch) == 0x2B110 && "Invalid header data.");
    assert(wav_get_BlockAlign(&file_2ch) == 0x2 && "Invalid header data.");
    assert(wav_get_Subchunk2ID(&file_2ch) == 0x64617461 && "Invalid header data.");
    
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
    
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
    
    /* 1 Channel */
    
    WAV_FILE file_1ch = wav_open("test-files/wav-format/16bit-test.wav", WAV_READ);
    
    assert(file_1ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    for (i = 0; i < 6; i++) {
        
        wav_get_sample(&file_1ch, samples_1ch[i].num, &lval, NULL);
        assert((lval == samples_1ch[i].lval) && "Invalid sample value.");
        
        wav_get_1ch_sample(&file_1ch, samples_1ch[i].num, &lval);
        assert((lval == samples_1ch[i].lval) && "Invalid sample value.");
    }
    
    assert(wav_close(&file_1ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    /* 2 Channel */
    
    WAV_FILE file_2ch = wav_open("test-files/wav-format/mic-rec.wav", WAV_READ);
    
    assert(file_2ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
    
    for (i = 0; i < 6; i++) {
        
        wav_get_2ch_sample(&file_2ch, samples_2ch[i].num, &lval, &rval);
        
        assert((lval == samples_2ch[i].lval) && "Invalid sample value.");
        assert((rval == samples_2ch[i].rval) && "Invalid sample value.");
    }
    
    assert(wav_close(&file_2ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_set() {
    
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
    
    printf("[*] TEST: WAV Format -> Set\n");
    
    /* 1 Channel */
        
    WAV_FILE file_1ch = wav_open("test-files/wav-format/1ch-set-test.wav", WAV_NEW);
    
    assert(file_1ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_set_1ch_defaults(&file_1ch) && "Couldn't set defaults.");
    
    for (i = 0; i < 6; i++) {
        
        lval = samples_1ch[i].lval;
        assert(wav_set_sample(&file_1ch, samples_1ch[i].num, &lval, NULL)
            && "Couldn't set a sample.");
        
        wav_get_1ch_sample(&file_1ch, samples_1ch[i].num, &lval);
        assert((lval == samples_1ch[i].lval) && "Invalid sample value.");
    }
    
    assert(wav_close(&file_1ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    /* 2 Channel */
    
    WAV_FILE file_2ch = wav_open("test-files/wav-format/2ch-set-test.wav", WAV_NEW);
    
    assert(file_2ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_set_2ch_defaults(&file_2ch) && "Couldn't set defaults.");
    
    for (i = 0; i < 6; i++) {
        
        lval = samples_2ch[i].lval;
        rval = samples_2ch[i].rval;
        
        assert(wav_set_sample(&file_2ch, samples_2ch[i].num, &lval, &rval)
            && "Couldn't set a sample.");
        
        wav_get_2ch_sample(&file_2ch, samples_2ch[i].num, &lval, &rval);
        
        assert((lval == samples_2ch[i].lval) && "Invalid sample value.");
        assert((rval == samples_2ch[i].rval) && "Invalid sample value.");
    }
    
    assert(wav_close(&file_2ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_push() {
    
    struct test_sample {
        int16_t  lval;
        int16_t  rval;
    };
    
    uint32_t i;
    int16_t  lval, rval;
    
    const struct test_sample samples_1ch[6] = {
        { 0x3D73, 0x0000 }, { 0x6E8F, 0x0000 }, { 0x6183, 0x0000 },
        { 0x66E7, 0x0000 }, { 0x65E7, 0x0000 }, { 0x66E1, 0x0000 }
    };
    
    const struct test_sample samples_2ch[6] = {
        { 0x6E8F, 0x3D73 }, { 0x0001, 0x66E1 },
        { 0x3D73, 0xFFFE }, { 0x0004, 0x0002 },
        { 0xFFFE, 0xFFFD }, { 0xFFFD, 0x0002 }
    };
    
    printf("[*] TEST: WAV Format -> Push\n");
    
    /* 1 Channel */
        
    WAV_FILE file_1ch = wav_open("test-files/wav-format/1ch-push-test.wav", WAV_NEW);
    
    assert(file_1ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_set_1ch_defaults(&file_1ch) && "Couldn't set defaults.");
    
    for (i = 0; i < 6; i++) {
        
        lval = samples_1ch[i].lval;
        assert(wav_push_sample(&file_1ch, &lval, NULL) && "Couldn't set a sample.");
        
        wav_get_1ch_sample(&file_1ch, i, &lval);
        assert((lval == samples_1ch[i].lval) && "Invalid sample value.");
    }
    
    assert(wav_close(&file_1ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    /* 2 Channel */
    
    WAV_FILE file_2ch = wav_open("test-files/wav-format/2ch-push-test.wav", WAV_NEW);
    
    assert(file_2ch.bin.open && "File couldn't be opened.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
    
    assert(wav_set_2ch_defaults(&file_2ch) && "Couldn't set defaults.");
    
    for (i = 0; i < 6; i++) {
        
        lval = samples_2ch[i].lval;
        rval = samples_2ch[i].rval;
        assert(wav_push_sample(&file_2ch, &lval, &rval) && "Couldn't set a sample.");
        
        wav_get_2ch_sample(&file_2ch, i, &lval, &rval);
        assert((lval == samples_2ch[i].lval) && "Invalid sample value.");
        assert((rval == samples_2ch[i].rval) && "Invalid sample value.");
    }
    
    assert(wav_close(&file_2ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
    
}

void test_wav_next() {
    
    printf("[*] TEST: WAV Format -> Next\n");
    
    uint32_t i;
    
    bool test_general = false;
    
    WAV_PCM32 buffer_1ch[42];
    WAV_PCM32 buffer_2ch[84];
    WAV_PCM32 lsample, rsample;
    
    const WAV_PCM32 data_1ch[42] = {
        0x0A174730, 0x1510F740, 0x44815580, 0xBF037E00, 0x474C8E80, 0x29AF8680,
        0xB5883800, 0x2475E280, 0x0425A198, 0xB4003500, 0x3AC60F40, 0xC091E780,
        0x498C9300, 0x25691800, 0x42261D80, 0x36713980, 0x34513580, 0xAEB9C400,
        0xD67BACC0, 0x5A864E80, 0x9BA00400, 0xCCF53380, 0xDFB3BF80, 0x251F7D80,
        0xB969D900, 0x36613980, 0x41DFB700, 0x08927730, 0x0E6DB660, 0x48149000,
        0x1498F5A0, 0x6484C900, 0xBB684380, 0xE0B08E00, 0x5FB18C00, 0xD00ED340,
        0xEF8F1260, 0x2EB790C0, 0xC9BEC680, 0x27778280, 0xA5C1B200, 0xC5A38B40
    };
    
    const WAV_PCM32 data_2ch[84] = {
        0xAD502700, 0xF3D71AD0, 0x158A91A0, 0x57FFE300, 0xCEFED140, 0x1588F800,
        0x02F8D268, 0xE26A2B40, 0xD51876C0, 0x20CF74C0, 0xCDA6CE80, 0x55FFDF00,
        0xD24D3E80, 0x13A42740, 0x14E5C340, 0x38313CC0, 0x08F744D0, 0xEC623F40,
        0xBD851500, 0xBB684380, 0x3A414180, 0x65033100, 0xDB988400, 0xFE13FC00,
        0xFCDBF998, 0x16C0FA60, 0x0BDA7E60, 0xCA26C780, 0xFE6BFCCC, 0xEF93DF40,
        0xC6CEC0C0, 0xB254FE80, 0x5E165580, 0xFB4A5CD0, 0x54D64300, 0x543E4280,
        0xC4552280, 0x5DD7EF00, 0xDE0EEF40, 0x27344E80, 0x60EE5B00, 0x148DC260,
        0xE7AF0260, 0x49495F00, 0x0D45B400, 0x296DECC0, 0xB7383B00, 0xAFD9C600,
        0xF6B58730, 0xC2505180, 0x0AEF48D0, 0xB13E9580, 0xB619D280, 0x5E2FEF00,
        0xDD655480, 0x241C4800, 0x1900FF40, 0xBE69E300, 0x013268CC, 0x9ABCCF00,
        0x9C480500, 0xB5850500, 0xC8485D80, 0x0E2DB5A0, 0xCA06C740, 0xE0555A60,
        0xBB137680, 0x1CFDD340, 0xD4EEDD40, 0x43EC8800, 0xEA423AC0, 0x225C44C0,
        0x03EDA198, 0x02E0D268, 0x23177980, 0x26EAB400, 0x15CDC5A0, 0xBEB1E400,
        0xC226B780, 0x20C774C0, 0x9D233A80, 0x455AF100, 0x56C7E100, 0x08474400
    };
    
    /* 1-Channel */
    
    WAV_FILE  file_1ch = wav_open("test-files/wav-format/32bit-test.wav", WAV_READ);
    
    assert(wav_is_open(&file_1ch) && "File couldn't be opened.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    test_general = false;
    
    i = 0;
    
    while (wav_has_next(&file_1ch)) {
        
        if (test_general) {
            wav_next_sample(&file_1ch, &lsample, NULL);
        } else {
            wav_next_1ch_sample(&file_1ch, &lsample);
        }
        
        assert(data_1ch[i] == lsample && "Invalid sample value.");
        
        test_general = !test_general;
        
        buffer_1ch[i++] = lsample;
        
        if (i == 42) {
            break;
        }
    }
    
    assert(wav_close(&file_1ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_1ch) == WAV_ERR_NONE && "Error flag set.");
    
    /* 2-Channel */
    
    WAV_FILE  file_2ch = wav_open("test-files/wav-format/2ch-test.wav", WAV_READ);
    
    assert(wav_is_open(&file_2ch) && "File couldn't be opened.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
    
    test_general = false;
    
    i = 0;
    
    while (wav_has_next(&file_2ch)) {
        
        if (test_general) {
            wav_next_sample(&file_2ch, &lsample, &rsample);
        } else {
            wav_next_2ch_sample(&file_2ch, &lsample, &rsample);
        }
        
        assert(data_2ch[i    ] == lsample && "Invalid sample value - Left.");        
        assert(data_2ch[i + 1] == rsample && "Invalid sample value - Right.");
        
        test_general = !test_general;
        
        buffer_2ch[i++] = lsample;
        buffer_2ch[i++] = rsample;
        
        if (i == 84) {
            break;
        }
    }
    
    assert(wav_close(&file_2ch) && "File couldn't be closed.");
    assert(wav_last_error(&file_2ch) == WAV_ERR_NONE && "Error flag set.");
}

void test_wav_format() {
    
    test_wav_types();
    test_wav_open();
    test_wav_commit();
    test_wav_general();
    test_wav_sugar();
    test_wav_valid();
    test_wav_defaults();
    test_wav_get();
    test_wav_set();
    test_wav_push();
    test_wav_next();
}
