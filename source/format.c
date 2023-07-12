#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/file.h>

#include "bin.h"
#include "format.h"

WAV_FILE wav_open(char *loc, uint8_t mode) {
    
    WAV_FILE wf;
    
    if (mode == WAV_READ) {
        wf.bin = bin_open(loc, BIN_RD);
    } else if (mode == WAV_ALTER) {
        wf.bin = bin_open(loc, BIN_RDWR);
    } else if (mode == WAV_NEW) {
        wf.bin = bin_open(loc, BIN_NEW);
        wav_set_defaults(&wf);
    } else {
        __wav_last_error = WAV_ERR_MODE;
    }
    
    wf.curr = 0;
    
    return wf;
}

bool wav_close(WAV_FILE *wf) {
    
    if (wf->alt) {
        wav_set_ChunkID(wf, 0x52494646);
        wav_set_ChunkSize(wf, wf->curr + 36);
        wav_set_ByteRate(wf, wav_get_SampleRate(wf) * wav_get_NumChannels(wf) * wav_get_BitsPerSample(wf) / 8);
        wav_set_BlockAlign(wf, wav_get_NumChannels(wf) * wav_get_BitsPerSample(wf) / 8);
        wav_set_Subchunk2Size(wf, wf->curr);
    }
    
    return bin_close(&wf->bin);
}

uint32_t wav_sample_count(WAV_FILE *wf) {
    return wav_get_Subchunk2Size(wf) / (wav_get_BitsPerSample(wf) / 8);
}

uint32_t wav_est_duration(WAV_FILE *wf) {
    return (wav_get_Subchunk2Size(wf) * 1000) / wav_get_NumChannels(wf) /
        (wav_get_BitsPerSample(wf) / 8) / wav_get_SampleRate(wf);
}

void wav_rewind(WAV_FILE *wf) {
    wf->curr = 0;
}

bool wav_has_next(WAV_FILE *wf) {
    return wf->curr < wav_sample_count(wf);
}

int32_t wav_next_sample(WAV_FILE *wf) {
    return wav_get_sample(wf, wf->curr++);
}

bool wav_push_sample(WAV_FILE *wf, int32_t val) {
    
    if (wav_set_sample(wf, wf->curr, val)) {
        
        wf->curr++;
        wf->alt = true;
        
        return true;
    }
    
    return false;
}

int32_t wav_get_sample(WAV_FILE *wf, uint32_t num) {
    
    uint8_t  bps = wav_get_BitsPerSample(wf);
    uint32_t off = num * (bps / 8);
    
    switch(bps) {
        case 8:
            return (int8_t)bin_r8(&wf->bin, 44 + off);
        case 16:
            return (int16_t)bin_r16l(&wf->bin, 44 + off);
        case 32:
            return (int32_t)bin_r32l(&wf->bin, 44 + off);
    }
    
    __wav_last_error = WAV_ERR_GET_SAMPLE;
    
    return 0;
}

bool wav_set_sample(WAV_FILE *wf, uint32_t num, int32_t val) {
    
    uint8_t  bps = wav_get_BitsPerSample(wf);
    uint32_t off = num * (bps / 8);
    
    switch(bps) {
        case 8:
            return bin_w8(&wf->bin, 44 + off, (uint8_t)val);
        case 16:
            return bin_w16l(&wf->bin, 44 + off, (int16_t)val);
        case 32:
            return bin_w32l(&wf->bin, 44 + off, (int32_t)val);
    }
    
    __wav_last_error = WAV_ERR_SET_SAMPLE;
    
    return false;
}

uint8_t wav_last_error(bool verbose) {
    
    if (verbose) {
        
        switch (__wav_last_error) {
            case WAV_ERR_MODE:
                fprintf(stdout, "Invalid mode selected.");
                break;
            case WAV_ERR_SET_SAMPLE:
                fprintf(stdout, "Couldn't replace the sample.");
                break;
            case WAV_ERR_GET_SAMPLE:
                fprintf(stdout, "Couldn't fetch the sample.");
                break;
            case WAV_ERR_NONE: default:
                fprintf(stdout, "No error occured.");
        }
    }
    
    return __wav_last_error;
}

uint8_t wav_is_valid(WAV_FILE *wf) {
    
    uint32_t tmp;
    
    if (wav_get_ChunkID(wf) != 0x52494646) {
        return WAV_INV_CHUNK_ID;
    } else if (wav_get_Format(wf) != 0x57415645) {
        return WAV_INV_FORMAT;
    } else if (wav_get_Subchunk1ID(wf) != 0x666D7420) {
        return WAV_INV_SUBCHUNK1_ID;
    } else if ( wav_get_Subchunk2ID(wf) != 0x64617461) {
        return WAV_INV_SUBCHUNK2_ID;
    } else if ( wav_get_AudioFormat(wf) != 0x1) {
        return WAV_INV_AUDIO_FORMAT;
    } else if ((tmp = wav_get_NumChannels(wf)) < 1 || tmp > 2) {
        return WAV_INV_CHANNEL_NUM;
    } else if ((wav_get_BitsPerSample(wf) % 8) != 0) {
        return WAV_INV_BPS;
    }
    
    return WAV_VALID;
}

bool wav_set_defaults(WAV_FILE *wf) {
    
    wf->alt = true;
    
    return     wav_set_ChunkID(wf, 0x52494646)
            && wav_set_Format(wf, 0x57415645)
            && wav_set_Subchunk1ID(wf, 0x666D7420)
            && wav_set_Subchunk1Size(wf, 0x10)
            && wav_set_AudioFormat(wf, 0x1)
            && wav_set_BitsPerSample(wf, 0x10)
            && wav_set_NumChannels(wf, 0x1)
            && wav_set_SampleRate(wf, 0xAC44)
            && wav_set_ByteRate(wf, 0x15888)
            && wav_set_BlockAlign(wf, 0x2)
            && wav_set_Subchunk2ID(wf, 0x64617461);
}

bool wav_set_1ch_defaults(WAV_FILE *wf) {
    return wav_set_defaults(wf);
}

bool wav_set_2ch_defaults(WAV_FILE *wf) {
    
    wf->alt = true;
    
    return     wav_set_ChunkID(wf, 0x52494646)
            && wav_set_Format(wf, 0x57415645)
            && wav_set_Subchunk1ID(wf, 0x666D7420)
            && wav_set_Subchunk1Size(wf, 0x10)
            && wav_set_AudioFormat(wf, 0x1)
            && wav_set_BitsPerSample(wf, 0x10)
            && wav_set_NumChannels(wf, 0x2)
            && wav_set_SampleRate(wf, 0xAC44)
            && wav_set_ByteRate(wf, 0x15888)
            && wav_set_BlockAlign(wf, 0x2)
            && wav_set_Subchunk2ID(wf, 0x64617461);
}

uint32_t wav_get_ChunkID(WAV_FILE *wf) {
    return bin_r32b(&wf->bin, 0);
}

uint32_t wav_get_ChunkSize(WAV_FILE *wf) {
    return bin_r32l(&wf->bin, 4);
}

uint32_t wav_get_Format(WAV_FILE *wf) {
    return bin_r32b(&wf->bin, 8);
}

uint32_t wav_get_Subchunk1ID(WAV_FILE *wf) {
    return bin_r32b(&wf->bin, 12);
}

uint32_t wav_get_Subchunk1Size(WAV_FILE *wf) {
    return bin_r32l(&wf->bin, 16);
}

uint16_t wav_get_AudioFormat(WAV_FILE *wf) {
    return bin_r16l(&wf->bin, 20);
}

uint16_t wav_get_NumChannels(WAV_FILE *wf) {
    return bin_r16l(&wf->bin, 22);
}

uint32_t wav_get_SampleRate(WAV_FILE *wf) {
    return bin_r32l(&wf->bin, 24);
}

uint32_t wav_get_ByteRate(WAV_FILE *wf) {
    return bin_r32l(&wf->bin, 28);
}

uint16_t wav_get_BlockAlign(WAV_FILE *wf) {
    return bin_r16l(&wf->bin, 32);
}

uint16_t wav_get_BitsPerSample(WAV_FILE *wf) {
    return bin_r16l(&wf->bin, 34);
}

uint32_t wav_get_Subchunk2ID(WAV_FILE *wf) {
    return bin_r32b(&wf->bin, 36);
}

uint32_t wav_get_Subchunk2Size(WAV_FILE *wf) {
    return bin_r32l(&wf->bin, 40);
}

bool wav_set_ChunkID(WAV_FILE *wf, uint32_t val) {
    return bin_w32b(&wf->bin, 0, val);
}

bool wav_set_ChunkSize(WAV_FILE *wf, uint32_t val) {
    return bin_w32l(&wf->bin, 4, val);
}

bool wav_set_Format(WAV_FILE *wf, uint32_t val) {
    return bin_w32b(&wf->bin, 8, val);
}

bool wav_set_Subchunk1ID(WAV_FILE *wf, uint32_t val) {
    return bin_w32b(&wf->bin, 12, val);
}

bool wav_set_Subchunk1Size(WAV_FILE *wf, uint32_t val) {
    return bin_w32l(&wf->bin, 16, val);
}

bool wav_set_AudioFormat(WAV_FILE *wf, uint16_t val) {
    return bin_w16l(&wf->bin, 20, val);
}

bool wav_set_NumChannels(WAV_FILE *wf, uint16_t val) {
    return bin_w16l(&wf->bin, 22, val);
}

bool wav_set_SampleRate(WAV_FILE *wf, uint32_t val) {
    return bin_w32l(&wf->bin, 24, val);
}

bool wav_set_ByteRate(WAV_FILE *wf, uint32_t val) {
    return bin_w32l(&wf->bin, 28, val);
}

bool wav_set_BlockAlign(WAV_FILE *wf, uint16_t val) {
    return bin_w16l(&wf->bin, 32, val);
}

bool wav_set_BitsPerSample(WAV_FILE *wf, uint16_t val) {
    return bin_w16l(&wf->bin, 34, val);
}

bool wav_set_Subchunk2ID(WAV_FILE *wf, uint32_t val) {
    return bin_w32b(&wf->bin, 36, val);
}

bool wav_set_Subchunk2Size(WAV_FILE *wf, uint32_t val) {
    return bin_w32l(&wf->bin, 40, val);
}
