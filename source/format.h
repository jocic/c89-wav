#ifndef C89_WAV_FORMAT_H
    
    #include <stdint.h>
    #include <stdbool.h>
    #include <limits.h>
    
    #include "bin.h"
    
    #define C89_WAV_FORMAT_H
    
    #define WAV_ERR_NONE          0
    #define WAV_ERR_MODE          1
    #define WAV_ERR_SET_SAMPLE    2
    #define WAV_ERR_GET_SAMPLE    3
    #define WAV_ERR_CHUNK_ID      4
    #define WAV_ERR_FORMAT        5
    #define WAV_ERR_SUBCHUNK1_ID  6
    #define WAV_ERR_SUBCHUNK2_ID  7
    #define WAV_ERR_AUDIO_FORMAT  8
    #define WAV_ERR_CHANNEL_NUM   9
    #define WAV_ERR_BPS          10
    
    enum WAV_MODES {
        WAV_READ, WAV_ALTER, WAV_NEW
    };
    
    typedef struct {
        BIN_FILE bin;
        uint32_t curr;
        bool     alt;
    } WAV_FILE;
    
    static uint8_t __wav_last_error;
    
    WAV_FILE wav_open(char *loc, uint8_t mode);
    bool wav_close(WAV_FILE *wf);
    bool wav_is_open(WAV_FILE *wf);
    bool wav_is_altered(WAV_FILE *wf);
    uint32_t wav_est_duration(WAV_FILE *wf);
    uint32_t wav_sample_count(WAV_FILE *wf);
    void wav_rewind(WAV_FILE *wf);
    bool wav_has_next(WAV_FILE *wf);
    int32_t wav_next_sample(WAV_FILE *wf);
    bool wav_push_sample(WAV_FILE *wf, int32_t val);
    bool wav_push_1ch_sample(WAV_FILE *wf, int32_t val);
    bool wav_push_2ch_sample(WAV_FILE *wf, int32_t lval, int32_t rval);
    int32_t wav_get_sample(WAV_FILE *wf, uint32_t off);
    bool wav_set_sample(WAV_FILE *wf, uint32_t num, int32_t val);
    bool wav_set_1ch_sample(WAV_FILE *wf, uint32_t num, int32_t val);
    uint8_t wav_last_error(bool verbose);
    uint8_t wav_is_valid(WAV_FILE *wf);
    bool wav_set_defaults(WAV_FILE *wf);
    bool wav_set_1ch_defaults(WAV_FILE *wf);
    bool wav_set_2ch_defaults(WAV_FILE *wf);
    
    uint32_t wav_get_ChunkID(WAV_FILE *wf);
    uint32_t wav_get_ChunkSize(WAV_FILE *wf);
    uint32_t wav_get_Format(WAV_FILE *wf);
    uint32_t wav_get_Subchunk1ID(WAV_FILE *wf);
    uint32_t wav_get_Subchunk1Size(WAV_FILE *wf);
    uint16_t wav_get_AudioFormat(WAV_FILE *wf);
    uint16_t wav_get_NumChannels(WAV_FILE *wf);
    uint32_t wav_get_SampleRate(WAV_FILE *wf);
    uint32_t wav_get_ByteRate(WAV_FILE *wf);
    uint16_t wav_get_BlockAlign(WAV_FILE *wf);
    uint16_t wav_get_BitsPerSample(WAV_FILE *wf);
    uint32_t wav_get_Subchunk2ID(WAV_FILE *wf);
    uint32_t wav_get_Subchunk2Size(WAV_FILE *wf);
    
    bool wav_set_ChunkID(WAV_FILE *wf, uint32_t val);
    bool wav_set_ChunkSize(WAV_FILE *wf, uint32_t val);
    bool wav_set_Format(WAV_FILE *wf, uint32_t val);
    bool wav_set_Subchunk1ID(WAV_FILE *wf, uint32_t val);
    bool wav_set_Subchunk1Size(WAV_FILE *wf, uint32_t val);
    bool wav_set_AudioFormat(WAV_FILE *wf, uint16_t val);
    bool wav_set_NumChannels(WAV_FILE *wf, uint16_t val);
    bool wav_set_SampleRate(WAV_FILE *wf, uint32_t val);
    bool wav_set_ByteRate(WAV_FILE *wf, uint32_t val);
    bool wav_set_BlockAlign(WAV_FILE *wf, uint16_t val);
    bool wav_set_BitsPerSample(WAV_FILE *wf, uint16_t val);
    bool wav_set_Subchunk2ID(WAV_FILE *wf, uint32_t val);
    bool wav_set_Subchunk2Size(WAV_FILE *wf, uint32_t val);
    
#endif
