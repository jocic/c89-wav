#ifndef C89_WAV_BIN_H
    
    #include <stdbool.h>
    #include <stdint.h>
    #include <sys/file.h>
    
    #define C89_WAV_BIN_H
    
    typedef struct {
        int fd;
        bool open;
    } BIN_FILE;
    
    typedef struct {
        uint8_t *buff;
        uint32_t off;
        uint32_t len;
    } BIN_DATA;
    
    BIN_FILE bin_open(char *loc, uint32_t modes);
    bool bin_close(BIN_FILE *bf);
    bool bin_rblk(BIN_FILE *bf, BIN_DATA *bd);
    bool bin_wblk(BIN_FILE *bf, BIN_DATA *bd);
    uint8_t bin_r8(BIN_FILE *bf, uint32_t off);
    bool bin_w8(BIN_FILE *bf, uint32_t off, uint8_t val);
    uint16_t bin_r16b(BIN_FILE *bf, uint32_t off);
    uint16_t bin_r16l(BIN_FILE *bf, uint32_t off);
    bool bin_w16b(BIN_FILE *bf, uint32_t off, uint16_t val);
    bool bin_w16l(BIN_FILE *bf, uint32_t off, uint16_t val);
    uint32_t bin_r32b(BIN_FILE *bf, uint32_t off);
    uint32_t bin_r32l(BIN_FILE *bf, uint32_t off);
    bool bin_w32b(BIN_FILE *bf, uint32_t off, uint32_t val);
    bool bin_w32l(BIN_FILE *bf, uint32_t off, uint32_t val);
    
#endif // BIN_H
