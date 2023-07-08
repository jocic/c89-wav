#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>

#include "bin.h"
#include "binary-io.h"

void test_bin_open_close() {
    
    printf("TEST: Binary I/O -> File Open/Close\n");
    
    BIN_FILE file = bin_open("test-files/60hz-5s-16bit-signed-44100.wav", BIN_RDWR);
    assert(file.open && "File couldn't be opened.");
    
    bin_close(&file);
    assert(!file.open && "File couldn't be closed.");
}

void test_bin_read_block() {
    
    int i;
    
    printf("TEST: Binary I/O -> Block Read\n");
    
    BIN_FILE file = bin_open("test-files/60hz-5s-16bit-signed-44100.wav", BIN_RD);
    assert(file.open && "File couldn't be opened.");
    
    BIN_DATA data = {
        .buff = malloc(44),
        .len  = 44,
        .off  = 0
    };
    
    assert(bin_rblk(&file, &data) && "Block read failed.");
    
    const uint8_t header_data[44] = {
        0x52, 0x49, 0x46, 0x46, 0xcc, 0xba, 0x06, 0x00, 0x57, 0x41, 0x56,
        0x45, 0x66, 0x6d, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00,
        0x01, 0x00, 0x44, 0xac, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00, 0x02,
        0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 0xa8, 0xba, 0x06, 0x00
    };
    
    for (i = 0; i < 44; i++) {
        assert(header_data[i] == data.buff[i] && "Block data mismatch.");
    }
    
    bin_close(&file);
    assert(!file.open && "File couldn't be closed.");
}

void test_bin_write_block() {
    
    int i;
    
    printf("TEST: Binary I/O -> Block Write\n");
    
    BIN_FILE file = bin_open("test-files/test.dat", BIN_RDWR);
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
    
    bin_close(&file);
    assert(!file.open && "File couldn't be closed.");
}

void test_bin_rw_byte() {
    
    printf("TEST: Binary I/O -> Byte Read/Write\n");
    
    /* Read Test */
    
    BIN_FILE file_rd = bin_open("test-files/60hz-5s-16bit-signed-44100.wav", BIN_RD);
    assert(file_rd.open && "File couldn't be opened.");
    
    assert(bin_r8(&file_rd, 0) == 'R' && "Data mismatch, offset = 0");
    assert(bin_r8(&file_rd, 1) == 'I' && "Data mismatch, offset = 1");
    assert(bin_r8(&file_rd, 2) == 'F' && "Data mismatch, offset = 2");
    assert(bin_r8(&file_rd, 3) == 'F' && "Data mismatch, offset = 3");
    
    bin_close(&file_rd);
    assert(!file_rd.open && "File couldn't be closed.");
    
    /* Write Test */
    
    BIN_FILE file_wr = bin_open("test-files/test.dat", BIN_RDWR);
    assert(file_wr.open && "File couldn't be opened.");
    
    assert(bin_w8(&file_wr, 0, 0xAA) && "Write fail, data = 0xAA");
    assert(bin_w8(&file_wr, 1, 0xBB) && "Write fail, data = 0xBB");
    assert(bin_w8(&file_wr, 1, 0x0A) && "Write fail, data = 0x0A");
    assert(bin_w8(&file_wr, 2, 0x33) && "Write fail, data = 0x33");
    
    assert(bin_r8(&file_wr, 0) == 0xAA && "Data mismatch, offset = 0");
    assert(bin_r8(&file_wr, 1) == 0x0A && "Data mismatch, offset = 1");
    assert(bin_r8(&file_wr, 2) == 0x33 && "Data mismatch, offset = 2");
    
    bin_close(&file_wr);
    assert(!file_wr.open && "File couldn't be closed.");
}

void test_bin_rw_int_be() {
    
    printf("TEST: Binary I/O -> BE  Read/Write\n");
    
    /* Read Test */
    
    BIN_FILE file_rd = bin_open("test-files/60hz-5s-16bit-signed-44100.wav", BIN_RD);
    assert(file_rd.open && "File couldn't be opened.");
    
    assert(bin_r16b(&file_rd, 0) == 0x5249 && "Invalid 16bit data, offset = 0");
    assert(bin_r16b(&file_rd, 4) == 0xCCBA && "Invalid 16bit data, offset = 4");
    assert(bin_r16b(&file_rd, 8) == 0x5741 && "Invalid 16bit data, offset = 8");
    
    assert(bin_r32b(&file_rd, 0)  == 0x52494646 && "Invalid 32bit data, offset = 0");
    assert(bin_r32b(&file_rd, 8)  == 0x57415645 && "Invalid 32bit data, offset = 8");
    assert(bin_r32b(&file_rd, 16) == 0x10000000 && "Invalid 32bit data, offset = 16");
    
    bin_close(&file_rd);
    assert(!file_rd.open && "File couldn't be closed.");
    
    /* Write Test */
    
    BIN_FILE file_wr = bin_open("test-files/test.dat", BIN_RDWR);
    assert(file_wr.open && "File couldn't be opened");
    
    assert(bin_w16b(&file_wr, 0, 0x5249) && "Invalid 16bit data, offset = 0");
    assert(bin_w16b(&file_wr, 4, 0xCCBA) && "Invalid 16bit data, offset = 4");
    assert(bin_w16b(&file_wr, 8, 0x5741) && "Invalid 16bit data, offset = 8");
    
    assert(bin_r16b(&file_wr, 4) == 0xCCBA && "Data mismatch.");
    
    assert(bin_w32b(&file_wr, 0, 0x52494646)  && "Invalid 32bit data, offset = 0");
    assert(bin_w32b(&file_wr, 8, 0x57415645)  && "Invalid 32bit data, offset = 8");
    assert(bin_w32b(&file_wr, 16, 0x10000000) && "Invalid 32bit data, offset = 16");
    
    assert(bin_r32b(&file_wr, 8) == 0x57415645 && "Data mismatch.");
    
    bin_close(&file_wr);
    assert(!file_wr.open && "File couldn't be closed.");
}

void test_bin_rw_int_le() {
    
    printf("TEST: Binary I/O -> LE  Read/Write\n");
    
    /* Read Test */
    
    BIN_FILE file_rd = bin_open("test-files/60hz-5s-16bit-signed-44100.wav", BIN_RD);
    assert(file_rd.open && "File couldn't be opened.");
    
    assert(bin_r16l(&file_rd, 0) == 0x4952 && "Invalid 16bit data, offset = 0");
    assert(bin_r16l(&file_rd, 4) == 0xBACC && "Invalid 16bit data, offset = 4");
    assert(bin_r16l(&file_rd, 8) == 0x4157 && "Invalid 16bit data, offset = 8");
    
    assert(bin_r32l(&file_rd, 0)  == 0x46464952 && "Invalid 16bit data, offset = 0");
    assert(bin_r32l(&file_rd, 8)  == 0x45564157 && "Invalid 16bit data, offset = 8");
    assert(bin_r32l(&file_rd, 16) == 0x00000010 && "Invalid 16bit data, offset = 16");
    
    bin_close(&file_rd);
    assert(!file_rd.open && "File couldn't be closed.");
    
    /* Write Test */
    
    BIN_FILE file_wr = bin_open("test-files/test.dat", BIN_WR);
    assert(file_wr.open && "File couldn't be opened");
    
    assert(bin_w16b(&file_wr, 0, 0x5249) && "Invalid 16bit data, offset = 0");
    assert(bin_w16b(&file_wr, 4, 0xCCBA) && "Invalid 16bit data, offset = 4");
    assert(bin_w16b(&file_wr, 8, 0x5741) && "Invalid 16bit data, offset = 8");
    
    assert(bin_w32b(&file_wr, 0, 0x46464952)  && "Invalid 32bit data, offset = 0");
    assert(bin_w32b(&file_wr, 8, 0x45564157)  && "Invalid 32bit data, offset = 8");
    assert(bin_w32b(&file_wr, 16, 0x00000010) && "Invalid 32bit data, offset = 16");
    
    bin_close(&file_wr);
    assert(!file_wr.open && "File couldn't be closed.");
}

void test_bin_io() {
    
    test_bin_open_close();
    test_bin_read_block();
    test_bin_write_block();
    test_bin_rw_byte();
    test_bin_rw_int_be();
    test_bin_rw_int_le();
}
