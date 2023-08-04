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
        0xC5A399DB, 0x9DE1AEC7, 0xC5A30D06, 0x9DE1FE4D, 0xC5A3C130, 0x9DE1AD46,
        0xC5A33F3B, 0x9DE14B3A, 0xC5A31168, 0x9DE11972, 0xC5A3FBE2, 0x9DE1BC73,
        0xC5A3E156, 0x9DE1FF30, 0xC5A35119, 0x9DE14F37, 0xC5A3421D, 0x9DE1DD79,
        0xC5A33281, 0x9DE1FC21, 0xC5A3BD43, 0x9DE1AA06, 0xC5A34982, 0x9DE1B65A,
        0xC5A32B1D, 0x9DE14E4C, 0xC5A302C6, 0x9DE1526A, 0xC5A3D7DB, 0x9DE10D67,
        0xC5A39CAC, 0x9DE1050B, 0xC5A3AC52, 0x9DE15B6E, 0xC5A3E43B, 0x9DE147EA,
        0xC5A3B7C9, 0x9DE15689, 0xC5A3BB90, 0x9DE15EEC, 0xC5A36410, 0x9DE13184,
        0xC5A3F4DE, 0x9DE1C2EE, 0xC5A3B1FE, 0x9DE1F467, 0xC5A39A8E, 0x9DE1DDA0,
        0xC5A39B6D, 0x9DE1501E, 0xC5A3E6FE, 0x9DE145A9, 0xC5A3067B, 0x9DE12AB3,
        0xC5A30E95, 0x9DE1247D, 0xC5A314D7, 0x9DE1E843, 0xC5A315F3, 0x9DE152A8,
        0xC5A3BBA4, 0x9DE1FD66, 0xC5A32165, 0x9DE11E23, 0xC5A3F5EC, 0x9DE1567C,
        0xC5A3E1B7, 0x9DE14BAE, 0xC5A3A548, 0x9DE1AE13, 0xC5A3160D, 0x9DE1045A,
        0xC5A33A07, 0x9DE1F1F3, 0xC5A33DF9, 0x9DE1B744, 0xC5A30412, 0x9DE1D1BD,
        0xC5A3D772, 0x9DE13823, 0xC5A34CFC, 0x9DE19DD0, 0xC5A32E71, 0x9DE1D9B3
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
