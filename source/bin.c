#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>

#include "bin.h"

BIN_FILE bin_open(char *loc, uint32_t modes) {
    
    BIN_FILE bf;
    
    bf.fd   = open(loc, modes, 0777);
    bf.open = bf.fd > 0;
    
    return bf;
}

bool bin_close(BIN_FILE *bf) {
    
    if  (close(bf->fd) == 0) {
        bf->open = false;
        return true;
    }
    
    return false;
}

bool bin_rblk(BIN_FILE *bf, BIN_DATA *bd) {
    
    if ((*bf).fd) {
        lseek((*bf).fd, (*bd).off, SEEK_SET);
        return read((*bf).fd, &((*bd).buff)[0], (*bd).len) > 0;
    }
    
    fprintf(stderr, "[x] Block read failed, open an image first.\n");
    
    return false;
}

bool bin_wblk(BIN_FILE *bf, BIN_DATA *bd) {
    
    if ((*bf).fd) {
        lseek((*bf).fd, (*bd).off, SEEK_SET);
        return write((*bf).fd, &((*bd).buff)[0], (*bd).len) > 0;
    }
    
    fprintf(stderr, "[x] Block write failed, open an image first.\n");
    
    return false;
}

uint8_t bin_r8(BIN_FILE *bf, uint32_t off) {
    
    uint8_t buff[1];
    
    BIN_DATA bd = { &buff[0], off, 1 };
    
    if (bin_rblk(bf, &bd)) {
        return buff[0];
    }
    
    return 0;
}

bool bin_w8(BIN_FILE *bf, uint32_t off, uint8_t val) {
    
    BIN_DATA bd = { &val, off, 1 };
    
    return bin_wblk(bf, &bd);
}

uint16_t bin_r16b(BIN_FILE *bf, uint32_t off) {
    
    uint16_t val = 0;
    uint8_t temp[2];
    
    BIN_DATA bd = { &temp[0], off, 2 };
    
    bin_rblk(bf, &bd);
    
    val |= (temp[0] << 8);
    val |= (temp[1]     );
    
    return val;
}

uint16_t bin_r16l(BIN_FILE *bf, uint32_t off) {
    
    uint16_t val = 0;
    uint8_t temp[2];
    
    BIN_DATA bd = { &temp[0], off, 2 };
    
    bin_rblk(bf, &bd);
    
    val |= (temp[1] << 8);
    val |= (temp[0]     );
    
    return val;
}

bool bin_w16b(BIN_FILE *bf, uint32_t off, uint16_t val) {
    
    uint8_t buff[2];
    
    buff[0] = ((val & 0xFF00) >> 8);
    buff[1] = ((val & 0x00FF)     );
    
    BIN_DATA bd = { &buff[0], off, 2 };
    
    return bin_wblk(bf, &bd);
}

bool bin_w16l(BIN_FILE *bf, uint32_t off, uint16_t val) {
    
    uint8_t buff[2];
    
    buff[1] = ((val & 0xFF00) >> 8);
    buff[0] = ((val & 0x00FF)     );
    
    BIN_DATA bd = { &buff[0], off, 2 };
    
    return bin_wblk(bf, &bd);
}

uint32_t bin_r32b(BIN_FILE *bf, uint32_t off) {
    
    uint32_t val = 0;
    uint8_t  temp[4];
    
    BIN_DATA bd = { &temp[0], off, 4 };
    
    bin_rblk(bf, &bd);
    
    val |= (temp[0] << 24);
    val |= (temp[1] << 16);
    val |= (temp[2] << 8 );
    val |= (temp[3]      );
    
    return val;
}

uint32_t bin_r32l(BIN_FILE *bf, uint32_t off) {
    
    uint32_t val = 0;
    uint8_t  temp[4];
    
    BIN_DATA bd = { &temp[0], off, 4 };
    
    bin_rblk(bf, &bd);
    
    val |= (temp[3] << 24);
    val |= (temp[2] << 16);
    val |= (temp[1] << 8 );
    val |= (temp[0]      );
    
    return val;
}

bool bin_w32b(BIN_FILE *bf, uint32_t off, uint32_t val) {
    
    uint8_t buff[4];
    
    buff[0] = ((val & 0xFF000000) >> 24);
    buff[1] = ((val & 0x00FF0000) >> 16);
    buff[2] = ((val & 0x0000FF00) >> 8 );
    buff[3] = ((val & 0x000000FF)      );
    
    BIN_DATA bd = { &buff[0], off, 4 };
    
    return bin_wblk(bf, &bd);
}

bool bin_w32l(BIN_FILE *bf, uint32_t off, uint32_t val) {
    
    uint8_t buff[4];
    
    buff[3] = ((val & 0xFF000000) >> 24);
    buff[2] = ((val & 0x00FF0000) >> 16);
    buff[1] = ((val & 0x0000FF00) >> 8 );
    buff[0] = ((val & 0x000000FF)      );
    
    BIN_DATA bd = { &buff[0], off, 4 };
    
    return bin_wblk(bf, &bd);
}
