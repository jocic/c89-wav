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
    
    printf("TEST: Binary I/O -> Core\n");
    
    // File Management (1 by 1)
    
    for (i = 0; i < 5; i++) {
        
        single = bin_open(files[i], BIN_RDWR);
        assert(single.open && "File couldn't be opened.");
        
        assert(bin_close(&single) && "File couldn't be closed");
        assert(!single.open && "Invalid open flag.");
    }
    
    // File Management (Multiple)
    
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
    
    uint16_t i;
    
    BIN_FILE file;
    
    struct target rd_targets[26] = {
        // 8-Bit Values
        { 0x000, 0x97, 0x1, false }, { 0x001, 0x68, 0x1, false },
        { 0x002, 0xAB, 0x1, false }, { 0x003, 0xD9, 0x1, false },
        { 0x0C6, 0xA0, 0x1, false }, { 0x0C7, 0x3A, 0x1, false },
        { 0x0C8, 0xA9, 0x1, false }, { 0x0C9, 0xDB, 0x1, false },
        { 0x1FF, 0xFE, 0x1, false }, { 0x2FF, 0x00, 0x1, false },
        // 16-Bit Values
        { 0x000, 0x6897, 0x2, false }, { 0x002, 0xABD9, 0x2, true },
        { 0x004, 0x295E, 0x2, false }, { 0x006, 0x6768, 0x2, true },
        { 0x126, 0xDA01, 0x2, false }, { 0x1FE, 0xB4FE, 0x2, true },
        { 0x2FF, 0x0000, 0x2, false }, { 0x3FF, 0x0000, 0x2, true },
        // 32-Bit Values
        { 0x000, 0xD9AB6897, 0x4, false }, { 0x004, 0x5E296768, 0x4, true },
        { 0x0B5, 0x5A1BE627, 0x4, false }, { 0x0D9, 0x4D4B8679, 0x4, true },
        { 0x1FC, 0xFEB44AC2, 0x4, false }, { 0x1FC, 0xC24AB4FE, 0x4, true },
        { 0x2FF, 0x00000000, 0x4, false }, { 0x3FF, 0x00000000, 0x4, true },
    };
    
    printf("TEST: Binary I/O -> Read\n");
    
    file = bin_open("test-files/binary-io/1.dat", BIN_RDWR);
    assert(file.open && "File couldn't be opened.");
    
    for (i = 0; i < 26; i++) {
        
        if (rd_targets[i].len == 1) {
            assert((bin_r8(&file, rd_targets[i].off) ==
                rd_targets[i].val)&& "Invalid read value - 8");
        } else if (rd_targets[i].len == 2) {
            if (rd_targets[i].big) {
                assert((bin_r16b(&file, rd_targets[i].off) ==
                    rd_targets[i].val) && "Invalid read value - 16B");
            } else {
                assert((bin_r16l(&file, rd_targets[i].off) ==
                    rd_targets[i].val) && "Invalid read value - 16L");
            }
        } else if (rd_targets[i].len == 4) {
            if (rd_targets[i].big) {
                assert((bin_r32b(&file, rd_targets[i].off) ==
                    rd_targets[i].val) && "Invalid read value - 32B");
            } else {
                assert((bin_r32l(&file, rd_targets[i].off) ==
                    rd_targets[i].val) && "Invalid read value - 32L");
            }
        }
    }
    
    assert(bin_close(&file) && "File couldn't be closed");
    assert(!file.open && "Invalid open flag.");
}

void test_bin_write() {
    
    uint16_t i, j;
    
    const uint8_t dummy_data[44] = {
        0x52, 0x49, 0x46, 0x46, 0xcc, 0xba, 0x06, 0x00, 0x57, 0x41, 0x56,
        0x45, 0x66, 0x6d, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00,
        0x01, 0x00, 0x44, 0xac, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00, 0x02,
        0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 0xa8, 0xba, 0x06, 0x00
    };
    
    printf("TEST: Binary I/O -> Write\n");
    
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
    
    int i;
    
    printf("TEST: Binary I/O -> Block Read\n");
    
    BIN_FILE file = bin_open("test-files/binary-io/1.dat", BIN_RD);
    assert(file.open && "File couldn't be opened.");
    
    BIN_DATA data = {
        .buff = malloc(44),
        .len  = 44,
        .off  = 0
    };
    
    assert(bin_rblk(&file, &data) && "Block read failed.");
    
    const uint8_t header_data[44] = {
        0x97, 0x68, 0xAB, 0xD9, 0x5E, 0x29, 0x67, 0x68, 0xC9, 0x8E, 0x7C,
        0xBC, 0xA9, 0x2D, 0x41, 0xED, 0xCA, 0x26, 0x6F, 0xDD, 0xC7, 0x29,
        0x7A, 0x60, 0x34, 0x19, 0x79, 0xFC, 0x77, 0x3E, 0x76, 0x48, 0xB9,
        0xC0, 0x6D, 0xEB, 0xC3, 0x4B, 0x07, 0x20, 0x0E, 0x29, 0x8B, 0xAA
    };
    
    for (i = 0; i < 44; i++) {
        assert(header_data[i] == data.buff[i] && "Block data mismatch.");
    }
    
    assert(bin_close(&file) && "File couldn't be closed");
    assert(!file.open && "Invalid open flag.");
}

void test_bin_block_write() {
    
    uint32_t i;
    
    printf("TEST: Binary I/O -> Block Write\n");
    
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
