#ifndef WAV_H

    #include <stdint.h>
    #include <stdbool.h>
    #include <limits.h>

    #include "../bin/bin.h"

    #define WAV_H

    enum WAV_MODES {
        WAV_READ, WAV_ALTER, WAV_NEW
    };

    typedef struct {
        BIN_FILE bin;
        uint32_t curr;
        bool alt;
    } WAV_FILE;

    WAV_FILE wav_open(char *loc, uint8_t mode);
    bool wav_close(WAV_FILE *wf);
    uint32_t wav_est_duration(WAV_FILE *wf);
    uint32_t wav_sample_count(WAV_FILE *wf);
    void wav_rewind(WAV_FILE *wf);
    bool wav_has_next(WAV_FILE *wf);
    int32_t wav_next_sample(WAV_FILE *wf);
    bool wav_push_sample(WAV_FILE *wf, int32_t val);
    int32_t wav_get_sample(WAV_FILE *wf, uint32_t off);
    bool wav_set_sample(WAV_FILE *wf, uint32_t off, int32_t val);

    bool wav_is_valid(WAV_FILE *wf);
    bool wav_set_defaults(WAV_FILE *wf);

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

#endif // WAV_H