#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include "bin.h"
#include "binary-io.h"

void test_bin_core() {
    
    uint16_t i;
    
    BIN_FILE single;
    BIN_FILE multiple[5];
    
    const char* files[5] = {
        "test-files/binary-io/1.dat",
        "test-files/binary-io/2.dat",
        "test-files/binary-io/3.dat",
        "test-files/binary-io/4.dat",
        "test-files/binary-io/5.dat",
    };
    
    printf("[*] TEST: Binary I/O -> Core\n");
    
    /* File Management (1 by 1) */
    
    for (i = 0; i < 5; i++) {
        
        single = bin_open(files[i], BIN_RDWR);
        assert(single.open && "File couldn't be opened.");
        
        assert(bin_close(&single) && "File couldn't be closed");
        assert(!single.open && "Invalid open flag.");
    }
    
    /* File Management (Multiple) */
    
    for (i = 0; i < 5; i++) {
        multiple[i] = bin_open(files[i], BIN_RDWR);
        assert(multiple[i].open && "File couldn't be opened.");
    }
    
    for (i = 0; i < 5; i++) {
        assert(bin_close(&multiple[i]) && "File couldn't be closed");
        assert(!multiple[i].open && "Invalid open flag.");
    }
}

void test_bin_read() {
    
    struct target {
        uint32_t off;
        uint32_t val;
        uint8_t  len;
        bool     big;
    };
    
    uint16_t i, j;
    char bin_filename[255];
    
    BIN_FILE file;
    
    struct target rd_targets[2][34] = { {
        /* 8-Bit Values */
        { 0x000, 0x97, 0x1, false }, { 0x001, 0x68, 0x1, false },
        { 0x002, 0xAB, 0x1, false }, { 0x003, 0xD9, 0x1, false },
        { 0x0C6, 0xA0, 0x1, false }, { 0x0C7, 0x3A, 0x1, false },
        { 0x0C8, 0xA9, 0x1, false }, { 0x0C9, 0xDB, 0x1, false },
        { 0x1FF, 0xFE, 0x1, false }, { 0x2FF, 0x00, 0x1, false },
        /* 16-Bit Values */
        { 0x000, 0x6897, 0x2, false }, { 0x002, 0xABD9, 0x2, true },
        { 0x004, 0x295E, 0x2, false }, { 0x006, 0x6768, 0x2, true },
        { 0x126, 0xDA01, 0x2, false }, { 0x1FE, 0xB4FE, 0x2, true },
        { 0x2FF, 0x0000, 0x2, false }, { 0x3FF, 0x0000, 0x2, true },
        /* 24-Bit Values */
        { 0x000, 0xAB6897, 0x3, false }, { 0x003, 0xD95E29, 0x3, true },
        { 0x006, 0xC96867, 0x3, false }, { 0x009, 0x8E7CBC, 0x3, true },
        { 0x126, 0x5BDA01, 0x3, false }, { 0x1FD, 0x4AB4FE, 0x3, true },
        { 0x2FF, 0x000000, 0x3, false }, { 0x3FF, 0x000000, 0x3, true },
        /* 32-Bit Values */
        { 0x000, 0xD9AB6897, 0x4, false }, { 0x004, 0x5E296768, 0x4, true },
        { 0x0B5, 0x5A1BE627, 0x4, false }, { 0x0D9, 0x4D4B8679, 0x4, true },
        { 0x1FC, 0xFEB44AC2, 0x4, false }, { 0x1FC, 0xC24AB4FE, 0x4, true },
        { 0x2FF, 0x00000000, 0x4, false }, { 0x3FF, 0x00000000, 0x4, true },
    }, {
        /* 8-Bit Values */
        { 0x000, 0x9B, 0x1, false }, { 0x001, 0x5F, 0x1, false },
        { 0x002, 0xFC, 0x1, false }, { 0x003, 0x24, 0x1, false },
        { 0x0C6, 0x57, 0x1, false }, { 0x0C7, 0x9C, 0x1, false },
        { 0x0C8, 0xA8, 0x1, false }, { 0x0C9, 0xC6, 0x1, false },
        { 0x1FF, 0xC1, 0x1, false }, { 0x2FF, 0x00, 0x1, false },
        /* 16-Bit Values */
        { 0x000, 0x5F9B, 0x2, false }, { 0x002, 0xFC24, 0x2, true },
        { 0x004, 0xB8CD, 0x2, false }, { 0x006, 0xAC47, 0x2, true },
        { 0x126, 0x1DF3, 0x2, false }, { 0x1FE, 0x8CC1, 0x2, true },
        { 0x2FF, 0x0000, 0x2, false }, { 0x3FF, 0x0000, 0x2, true },
        /* 24-Bit Values */
        { 0x000, 0xFC5F9B, 0x3, false }, { 0x003, 0x24CDB8, 0x3, true },
        { 0x006, 0x8947AC, 0x3, false }, { 0x009, 0xB6B371, 0x3, true },
        { 0x126, 0x041DF3, 0x3, false }, { 0x1FD, 0xA28CC1, 0x3, true },
        { 0x2FF, 0x000000, 0x3, false }, { 0x3FF, 0x000000, 0x3, true },
        /* 32-Bit Values */
        { 0x000, 0x24FC5F9B, 0x4, false }, { 0x004, 0xCDB8AC47, 0x4, true },
        { 0x0B5, 0xEF3E1476, 0x4, false }, { 0x0D9, 0x053D9412, 0x4, true },
        { 0x1FC, 0xC18CA22B, 0x4, false }, { 0x1FC, 0x2BA28CC1, 0x4, true },
        { 0x2FF, 0x00000000, 0x4, false }, { 0x3FF, 0x00000000, 0x4, true },
    } };
    
    printf("[*] TEST: Binary I/O -> Read\n");
    
    for (i = 0; i < 2; i++) {
        
        sprintf(bin_filename, "test-files/binary-io/%d.dat", (i + 1));
        
        file = bin_open(bin_filename, BIN_RDWR);
        assert(file.open && "File couldn't be opened.");
        
        for (j = 0; j < 34; j++) {
            
            if (rd_targets[i][j].len == 1) {
                assert((bin_r8(&file, rd_targets[i][j].off) ==
                    rd_targets[i][j].val)&& "Invalid read value - 8");
            } else if (rd_targets[i][j].len == 2) {
                if (rd_targets[i][j].big) {
                    assert((bin_r16b(&file, rd_targets[i][j].off) ==
                        rd_targets[i][j].val) && "Invalid read value - 16B");
                } else {
                    assert((bin_r16l(&file, rd_targets[i][j].off) ==
                        rd_targets[i][j].val) && "Invalid read value - 16L");
                }
            } else if (rd_targets[i][j].len == 3) {
                if (rd_targets[i][j].big) {
                    assert((bin_r24b(&file, rd_targets[i][j].off) ==
                        rd_targets[i][j].val) && "Invalid read value - 24B");
                } else {
                    assert((bin_r24l(&file, rd_targets[i][j].off) ==
                        rd_targets[i][j].val) && "Invalid read value - 24L");
                }
            }  else if (rd_targets[i][j].len == 4) {
                if (rd_targets[i][j].big) {
                    assert((bin_r32b(&file, rd_targets[i][j].off) ==
                        rd_targets[i][j].val) && "Invalid read value - 32B");
                } else {
                    assert((bin_r32l(&file, rd_targets[i][j].off) ==
                        rd_targets[i][j].val) && "Invalid read value - 32L");
                }
            }
        }
        
        assert(bin_close(&file) && "File couldn't be closed");
        assert(!file.open && "Invalid open flag.");
    }
}

void test_bin_write() {
    
    uint16_t i, j;
    
    const uint8_t dummy_data[44] = {
        0x52, 0x49, 0x46, 0x46, 0xcc, 0xba, 0x06, 0x00, 0x57, 0x41, 0x56,
        0x45, 0x66, 0x6d, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00,
        0x01, 0x00, 0x44, 0xac, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00, 0x02,
        0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 0xa8, 0xba, 0x06, 0x00
    };
    
    printf("[*] TEST: Binary I/O -> Write\n");
    
    BIN_FILE file = bin_open("test-files/binary-io/write-test.dat", BIN_NEW);
    assert(file.open && "File couldn't be opened.");
    
    for (i = 0; i < 44; i++) {
        assert(bin_w8(&file, i, dummy_data[i]) && "Byte write failed.");
        assert((bin_r8(&file, i) == dummy_data[i]) && "Byte read failed");
    }
    
    for (i = 0, j = 0; i < 22; i++, j+=2) {
        
        uint16_t data = ((dummy_data[i] << 8) | dummy_data[i + 1]);
        
        assert(bin_w16b(&file, i, data) && "Byte write failed.");
        assert((bin_r16b(&file, i) == data) && "Byte read failed");
        
        assert(bin_w16l(&file, i, data) && "Byte write failed.");
        assert((bin_r16l(&file, i) == data) && "Byte read failed");
    }
    
    for (i = 0, j = 0; i < 11; i++, j+=3) {
        
        uint32_t data = (dummy_data[i] << 16) |
            (dummy_data[i] << 8) | dummy_data[i];
        
        assert(bin_w24b(&file, i, data) && "Byte write failed.");
        assert((bin_r24b(&file, i) == data) && "Byte read failed");
        
        assert(bin_w24l(&file, i, data) && "Byte write failed.");
        assert((bin_r24l(&file, i) == data) && "Byte read failed");
    }
    
    for (i = 0, j = 0; i < 11; i++, j+=4) {
        
        uint32_t data = (dummy_data[i] << 24) | (dummy_data[i] << 16) |
            (dummy_data[i] << 8) | dummy_data[i];
        
        assert(bin_w32b(&file, i, data) && "Byte write failed.");
        assert((bin_r32b(&file, i) == data) && "Byte read failed");
        
        assert(bin_w32l(&file, i, data) && "Byte write failed.");
        assert((bin_r32l(&file, i) == data) && "Byte read failed");
    }
    
    assert(bin_close(&file) && "File couldn't be closed");
    assert(!file.open && "Invalid open flag.");
}

void test_bin_block_read() {
    
    char bin_filename[255];
    int i, j;
    
    printf("[*] TEST: Binary I/O -> Block Read\n");
    
    const uint8_t data_targets[2][44] = { {
        0x97, 0x68, 0xAB, 0xD9, 0x5E, 0x29, 0x67, 0x68, 0xC9, 0x8E, 0x7C,
        0xBC, 0xA9, 0x2D, 0x41, 0xED, 0xCA, 0x26, 0x6F, 0xDD, 0xC7, 0x29,
        0x7A, 0x60, 0x34, 0x19, 0x79, 0xFC, 0x77, 0x3E, 0x76, 0x48, 0xB9,
        0xC0, 0x6D, 0xEB, 0xC3, 0x4B, 0x07, 0x20, 0x0E, 0x29, 0x8B, 0xAA
    }, {
        0x9B, 0x5F, 0xFC, 0x24, 0xCD, 0xB8, 0xAC, 0x47, 0x89, 0xB6, 0xB3,
        0x71, 0x2A, 0x71, 0x2D, 0x9F, 0x18, 0x13, 0x28, 0xC8, 0xF5, 0x7D,
        0x62, 0xD4, 0x58, 0x97, 0x84, 0x30, 0xFF, 0xFF, 0x11, 0x8B, 0xFA,
        0xE2, 0xD2, 0xB9, 0xC7, 0xDA, 0xD1, 0x65, 0x0D, 0x6B, 0xB0, 0x6A
    } };
    
    for (i = 0; i < 2; i++) {
        
        sprintf(bin_filename, "test-files/binary-io/%d.dat", (i + 1));
        
        BIN_FILE file = bin_open(bin_filename, BIN_RD);
        assert(file.open && "File couldn't be opened.");
        
        BIN_DATA data = {
            .buff = malloc(44),
            .len  = 44,
            .off  = 0
        };
        
        assert(bin_rblk(&file, &data) && "Block read failed.");
        
        for (j = 0; j < 44; j++) {
            assert(data_targets[i][j] == data.buff[j] && "Block data mismatch.");
        }
        
        assert(bin_close(&file) && "File couldn't be closed");
        assert(!file.open && "Invalid open flag.");
    }
}

void test_bin_block_write() {
    
    uint32_t i;
    
    printf("[*] TEST: Binary I/O -> Block Write\n");
    
    BIN_FILE file = bin_open("test-files/binary-io/bwrite-test.dat", BIN_NEW);
    assert(file.open && "File couldn't be opened.");
    
    BIN_DATA data_wr = {
        .buff = malloc(512),
        .len  = 512,
        .off  = 0
    };
    
    srand(time(NULL));
    
    for (i = 0; i < 512; i++) {
        data_wr.buff[i] = rand() % 255;
    }
    
    assert(bin_wblk(&file, &data_wr) && "Block write failed.");
    
    BIN_DATA data_rd = {
        .buff = malloc(512),
        .len  = 512,
        .off  = 0
    };
    
    assert(bin_rblk(&file, &data_rd) && "Block read failed.");
    
    for (i = 0; i < 512; i++) {
        assert(data_wr.buff[i] == data_rd.buff[i] && "Block data mismatch.");
    }
    
    assert(bin_close(&file) && "File couldn't be closed");
    assert(!file.open && "Invalid open flag.");
}

void test_bin_io() {
    
    test_bin_core();
    test_bin_read();
    test_bin_write();
    test_bin_block_read();
    test_bin_block_write();
}
