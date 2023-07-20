#ifndef C89_WAV_FORMAT_H
    
    #include <stdint.h>
    #include <stdbool.h>
    #include <limits.h>
    
    #include "bin.h"
    
    #define C89_WAV_FORMAT_H
    
    #define WAV_ERR_NONE           0
    #define WAV_ERR_MODE           1
    #define WAV_ERR_COMMIT         2
    #define WAV_ERR_SET_SAMPLE     3
    #define WAV_ERR_GET_SAMPLE     4
    #define WAV_ERR_CHUNK_ID       5
    #define WAV_ERR_FORMAT         6
    #define WAV_ERR_SUBCHUNK1_ID   7
    #define WAV_ERR_SUBCHUNK1_SIZE 8
    #define WAV_ERR_SUBCHUNK2_ID   9
    #define WAV_ERR_AUDIO_FORMAT  10
    #define WAV_ERR_CHANNEL_NUM   11
    #define WAV_ERR_BPS           12
    #define WAV_ERR_SAMPLE_RATE   13
    #define WAV_ERR_BYTE_RATE     14
    #define WAV_ERR_BLOCK_ALIGN   15
    
    #define wav_is_open(wf)                 ((*wf).bin.open)
    #define wav_is_altered(wf)              ((*wf).alt)
    #define wav_rewind(wf)                  ((*wf).curr = 0)
    #define wav_csample(wf)                 ((*wf).curr)
    #define wav_has_next(wf)                ((*wf).curr < wav_sample_count(wf))
    #define wav_last_error(wf)              ((*wf).err)
    #define wav_set_defaults(wf)            (wav_set_1ch_defaults(wf))
    #define wav_get_sample(wf, n, v)        (wav_get_1ch_sample(wf, n, v))
    #define wav_set_sample(wf, n, v)        (wav_set_1ch_sample(wf, n, v))
    #define wav_push_sample(wf, v)          (wav_push_1ch_sample(wf, v))
    #define wav_next_sample(wf, v)          (wav_next_1ch_sample(wf, v))
    #define wav_set_psample(wf, v)          (wav_get_1ch_sample(wf, (*wf).curr - 1, v))
    #define wav_set_1ch_psample(wf, v)      (wav_get_1ch_sample(wf, (*wf).curr - 1, v))
    #define wav_set_2ch_psample(wf, lv, rv) (wav_get_1ch_sample(wf, (*wf).curr - 1, lv, rv))
    
    #define wav_get_ChunkID(wf)       (bin_r32b(&(*wf).bin, 0))
    #define wav_get_ChunkSize(wf)     (bin_r32l(&(*wf).bin, 4))
    #define wav_get_Format(wf)        (bin_r32b(&(*wf).bin, 8))
    #define wav_get_Subchunk1ID(wf)   (bin_r32b(&(*wf).bin, 12))
    #define wav_get_Subchunk1Size(wf) (bin_r32l(&(*wf).bin, 16))
    #define wav_get_AudioFormat(wf)   (bin_r16l(&(*wf).bin, 20))
    #define wav_get_NumChannels(wf)   (bin_r16l(&(*wf).bin, 22))
    #define wav_get_SampleRate(wf)    (bin_r32l(&(*wf).bin, 24))
    #define wav_get_ByteRate(wf)      (bin_r32l(&(*wf).bin, 28))
    #define wav_get_BlockAlign(wf)    (bin_r16l(&(*wf).bin, 32))
    #define wav_get_BitsPerSample(wf) (bin_r16l(&(*wf).bin, 34))
    #define wav_get_Subchunk2ID(wf)   (bin_r32b(&(*wf).bin, 36))
    #define wav_get_Subchunk2Size(wf) (bin_r32l(&(*wf).bin, 40))

    #define wav_set_ChunkID(wf, val)       (bin_w32b(&(*wf).bin, 0, val))
    #define wav_set_ChunkSize(wf, val)     (bin_w32l(&(*wf).bin, 4, val))
    #define wav_set_Format(wf, val)        (bin_w32b(&(*wf).bin, 8, val))
    #define wav_set_Subchunk1ID(wf, val)   (bin_w32b(&(*wf).bin, 12, val))
    #define wav_set_Subchunk1Size(wf, val) (bin_w32l(&(*wf).bin, 16, val))
    #define wav_set_AudioFormat(wf, val)   (bin_w16l(&(*wf).bin, 20, val))
    #define wav_set_NumChannels(wf, val)   (bin_w16l(&(*wf).bin, 22, val))
    #define wav_set_SampleRate(wf, val)    (bin_w32l(&(*wf).bin, 24, val))
    #define wav_set_ByteRate(wf, val)      (bin_w32l(&(*wf).bin, 28, val))
    #define wav_set_BlockAlign(wf, val)    (bin_w16l(&(*wf).bin, 32, val))
    #define wav_set_BitsPerSample(wf, val) (bin_w16l(&(*wf).bin, 34, val))
    #define wav_set_Subchunk2ID(wf, val)   (bin_w32b(&(*wf).bin, 36, val))
    #define wav_set_Subchunk2Size(wf, val) (bin_w32l(&(*wf).bin, 40, val))
    
    enum WAV_MODES {
        WAV_READ, WAV_ALTER, WAV_NEW
    };
    
    typedef struct {
        BIN_FILE bin;
        uint32_t curr;
        uint8_t  err;
        uint8_t  mod;
        bool     alt;
    } WAV_FILE;
    
    WAV_FILE wav_open(char *loc, uint8_t mode);
    bool wav_close(WAV_FILE *wf);
    bool wav_commit(WAV_FILE *wf);
    bool wav_copy_meta(WAV_FILE *in, WAV_FILE *out);
    uint32_t wav_est_duration(WAV_FILE *wf);
    uint32_t wav_sample_count(WAV_FILE *wf);
    void wav_get_1ch_sample(WAV_FILE *wf, uint32_t n, void* val);
    void wav_get_2ch_sample(WAV_FILE *wf, uint32_t n, void* lval, void* rval);
    bool wav_set_1ch_sample(WAV_FILE *wf, uint32_t n, void* val);
    bool wav_set_2ch_sample(WAV_FILE *wf, uint32_t n, void* lval, void* rval);
    bool wav_push_1ch_sample(WAV_FILE *wf, void* val);
    bool wav_push_2ch_sample(WAV_FILE *wf, void* lval, void* rval);
    void wav_next_1ch_sample(WAV_FILE *wf, void* val);
    void wav_next_2ch_sample(WAV_FILE *wf, void* lval, void* rval);
    bool wav_is_valid(WAV_FILE *wf);
    bool wav_set_1ch_defaults(WAV_FILE *wf);
    bool wav_set_2ch_defaults(WAV_FILE *wf);
    
#endif
