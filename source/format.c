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
    } else {
        wf.err = WAV_ERR_MODE;
    }
    
    wf.err  = WAV_ERR_NONE;
    wf.mod  = mode;
    wf.curr = 0;
    wf.alt  = false;
    
    return wf;
}

bool wav_close(WAV_FILE *wf) {
    
    if (!wf->bin.open) {
        return false;
    }
    
    if (!wav_commit(wf)) {
        wf->err = WAV_ERR_COMMIT;
    }
    
    return bin_close(&wf->bin);
}

bool wav_commit(WAV_FILE *wf) {
    
    if (!wf->bin.open) {
        return false;
    }
    
    if (wf->mod == WAV_READ) {
        return true;
    }
    
    uint16_t num_channels    = wav_get_NumChannels(wf);
    uint16_t bits_per_sample = wav_get_BitsPerSample(wf);
    
    uint32_t subchunk2_size = wf->curr * num_channels * bits_per_sample / 8;
    
    return wav_set_Subchunk2Size(wf, subchunk2_size)
        && wav_set_ChunkSize(wf, 36 + subchunk2_size);
}

uint32_t wav_est_duration(WAV_FILE *wf) {
    
    uint32_t sample_rate     = wav_get_SampleRate(wf);
    uint32_t subchunk2_size  = wav_get_Subchunk2Size(wf);
    uint16_t bits_per_sample = wav_get_BitsPerSample(wf);
    uint16_t num_channels    = wav_get_NumChannels(wf);
    
    uint32_t duration_ms = (subchunk2_size * 1000) / num_channels /
        (bits_per_sample / 8) / sample_rate;
    
    return duration_ms;
}

uint32_t wav_sample_count(WAV_FILE *wf) {
    
    uint32_t subchunk2_size  = wav_get_Subchunk2Size(wf);
    uint16_t bits_per_sample = wav_get_BitsPerSample(wf);
    uint16_t num_channels    = wav_get_NumChannels(wf);
    
    uint32_t sample_count = subchunk2_size / (bits_per_sample / 8) / num_channels;
    
    return sample_count;
}

int32_t wav_get_sample(WAV_FILE *wf, uint32_t n) {
    
    int32_t sample = 0;
    
    wav_get_1ch_sample(wf, n, &sample);
    
    return sample;
}

void wav_get_1ch_sample(WAV_FILE *wf, uint32_t n, void* val) {
    
    uint8_t  bps   = wav_get_BitsPerSample(wf);
    uint16_t bytes = bps / 8;
    
    uint32_t off = 44 + (n * bytes);
    
    switch(bps) {
        case 8:
            (*(int8_t*)val) = (int8_t)bin_r8(&wf->bin, off);
            break;
        case 16:
            (*(int16_t*)val) = (int16_t)bin_r16l(&wf->bin, off);
            break;
        case 32:
            (*(int32_t*)val) = (int32_t)bin_r32l(&wf->bin, off);
            break;
        default:
            wf->err = WAV_ERR_GET_SAMPLE;
    }
}

void wav_get_2ch_sample(WAV_FILE *wf, uint32_t n, void* lval, void* rval) {
    
    uint8_t  bps   = wav_get_BitsPerSample(wf);
    uint16_t bytes = bps / 8;
    
    uint32_t off_lch = 44 + (n * 2 * bytes);
    uint32_t off_rch = off_lch + bytes;
    
    switch(bps) {
        case 8:
            (*(int8_t*)lval) = (int8_t)bin_r8(&wf->bin, off_lch);
            (*(int8_t*)rval) = (int8_t)bin_r8(&wf->bin, off_rch);
            break;
        case 16:
            (*(int16_t*)lval) = (int16_t)bin_r16l(&wf->bin, off_lch);
            (*(int16_t*)rval) = (int16_t)bin_r16l(&wf->bin, off_rch);
            break;
        case 32:
            (*(int32_t*)lval) = (int32_t)bin_r32l(&wf->bin, off_lch);
            (*(int32_t*)rval) = (int32_t)bin_r32l(&wf->bin, off_rch);
            break;
        default:
            wf->err = WAV_ERR_GET_SAMPLE;
    }
}

bool wav_set_1ch_sample(WAV_FILE *wf, uint32_t n, void* val) {
    
    uint8_t  bps = wav_get_BitsPerSample(wf);
    uint16_t bytes = bps / 8;
    
    uint32_t off = 44 + (n * bytes);
   
    switch(bps) {
        case 8:
            return bin_w8(&wf->bin, off, *((int8_t*)val));
        case 16:
            return bin_w16l(&wf->bin, off, *((int16_t*)val));
        case 32:
            return bin_w32l(&wf->bin, off, *((int32_t*)val));
    }
    
    wf->err = WAV_ERR_SET_SAMPLE;
    
    return false;
}

bool wav_set_2ch_sample(WAV_FILE *wf, uint32_t n, void* lval, void* rval) {
    
    uint8_t  bps   = wav_get_BitsPerSample(wf);
    uint16_t bytes = bps / 8;
    
    uint32_t off_lch = 44 + (n * 2 * bytes);
    uint32_t off_rch = off_lch + bytes;
    
    switch(bps) {
        case 8:
            return bin_w8(&wf->bin, off_lch, *((int8_t*)lval))
                && bin_w8(&wf->bin, off_rch, *((int8_t*)rval));
        case 16:
            return bin_w16l(&wf->bin, off_lch, *((int16_t*)lval))
                && bin_w16l(&wf->bin, off_rch, *((int8_t*)rval));
        case 32:
            return bin_w32l(&wf->bin, off_lch, *((int32_t*)lval))
                && bin_w32l(&wf->bin, off_rch, *((int8_t*)rval));
    }
    
    wf->err = WAV_ERR_SET_SAMPLE;
    
    return false;
}

bool wav_set_psample(WAV_FILE *wf, int32_t val) {
    return wav_set_sample(wf, wf->curr - 1, &val);
}

bool wav_set_1ch_psample(WAV_FILE *wf, int32_t val) {
    return wav_set_psample(wf, val);
}

bool wav_set_2ch_psample(WAV_FILE *wf, int32_t lval, int32_t rval) {
    return wav_set_sample(wf, wf->curr - 2, &lval) && wav_set_sample(wf, wf->curr - 1, &rval);
}

bool wav_has_next(WAV_FILE *wf) {
    return wf->curr < wav_sample_count(wf);
}

int32_t wav_next_sample(WAV_FILE *wf) {
    return wav_get_sample(wf, wf->curr++);
}

void wav_next_1ch_sample(WAV_FILE *wf, int32_t* val) {
    *val = wav_get_sample(wf, wf->curr++);
}

void wav_next_2ch_sample(WAV_FILE *wf, int32_t* lval, int32_t* rval) {
    
    int32_t left  = wav_get_sample(wf, wf->curr++);
    int32_t right = wav_get_sample(wf, wf->curr++);
    
    *lval = left;
    *rval = right;
}

bool wav_push_sample(WAV_FILE *wf, int32_t val) {
    
    if (wav_set_sample(wf, wf->curr, &val)) {
        
        wf->curr++;
        wf->alt = true;
        
        return true;
    }
    
    return false;
}

bool wav_push_1ch_sample(WAV_FILE *wf, int32_t val) {
    return wav_push_sample(wf, val);
}

bool wav_push_2ch_sample(WAV_FILE *wf, int32_t lval, int32_t rval) {
    return wav_push_sample(wf, lval) && wav_push_sample(wf, rval);
}

bool wav_is_valid(WAV_FILE *wf) {
    
    uint32_t tmp;
    
    if (wav_get_ChunkID(wf) != 0x52494646) {
        wf->err = WAV_ERR_CHUNK_ID;
        return false;
    } else if (wav_get_Format(wf) != 0x57415645) {
        wf->err = WAV_ERR_FORMAT;
        return false;
    } else if (wav_get_Subchunk1ID(wf) != 0x666D7420) {
        wf->err = WAV_ERR_SUBCHUNK1_ID;
        return false;
    } else if (wav_get_Subchunk2ID(wf) != 0x64617461) {
        wf->err = WAV_ERR_SUBCHUNK2_ID;
        return false;
    } else if (wav_get_AudioFormat(wf) != 0x1) {
        wf->err = WAV_ERR_AUDIO_FORMAT;
        return false;
    } else if ((tmp = wav_get_NumChannels(wf)) < 1 || tmp > 2) {
        wf->err = WAV_ERR_CHANNEL_NUM;
        return false;
    } else if ((wav_get_BitsPerSample(wf) % 8) != 0) {
        wf->err = WAV_ERR_BPS;
        return false;
    }
    
    return true;
}

bool wav_set_1ch_defaults(WAV_FILE *wf) {
    
    if (!wav_set_ChunkID(wf, 0x52494646)) {
        wf->err = WAV_ERR_CHUNK_ID;
        return false;
    } else if (!wav_set_Format(wf, 0x57415645)) {
        wf->err = WAV_ERR_FORMAT;
        return false;
    } else if (!wav_set_Subchunk1ID(wf, 0x666D7420)) {
        wf->err = WAV_ERR_SUBCHUNK1_ID;
        return false;
    } else if (!wav_set_Subchunk1Size(wf, 0x10)) {
        wf->err = WAV_ERR_SUBCHUNK1_SIZE;
        return false;
    } else if (!wav_set_AudioFormat(wf, 0x1)) {
        wf->err = WAV_ERR_AUDIO_FORMAT;
        return false;
    } else if (!wav_set_BitsPerSample(wf, 0x10)) {
        wf->err = WAV_ERR_BPS;
        return false;
    } else if (!wav_set_NumChannels(wf, 0x1)) {
        wf->err = WAV_ERR_CHANNEL_NUM;
        return false;
    } else if (!wav_set_SampleRate(wf, 0xAC44)) {
        wf->err = WAV_ERR_SAMPLE_RATE;
        return false;
    } else if (!wav_set_ByteRate(wf, 0x15888)) {
        wf->err = WAV_ERR_BYTE_RATE;
        return false;
    } else if (!wav_set_BlockAlign(wf, 0x2)) {
        wf->err = WAV_ERR_BLOCK_ALIGN;
        return false;
    } else if (!wav_set_Subchunk2ID(wf, 0x64617461)) {
        wf->err = WAV_ERR_SUBCHUNK2_ID;
        return false;
    }
    
    return true;
}

bool wav_set_2ch_defaults(WAV_FILE *wf) {
    
    if (!wav_set_ChunkID(wf, 0x52494646)) {
        wf->err = WAV_ERR_CHUNK_ID;
        return false;
    } else if (!wav_set_Format(wf, 0x57415645)) {
        wf->err = WAV_ERR_FORMAT;
        return false;
    } else if (!wav_set_Subchunk1ID(wf, 0x666D7420)) {
        wf->err = WAV_ERR_SUBCHUNK1_ID;
        return false;
    } else if (!wav_set_Subchunk1Size(wf, 0x10)) {
        wf->err = WAV_ERR_SUBCHUNK1_SIZE;
        return false;
    } else if (!wav_set_AudioFormat(wf, 0x1)) {
        wf->err = WAV_ERR_AUDIO_FORMAT;
        return false;
    } else if (!wav_set_BitsPerSample(wf, 0x10)) {
        wf->err = WAV_ERR_BPS;
        return false;
    } else if (!wav_set_NumChannels(wf, 0x2)) {
        wf->err = WAV_ERR_CHANNEL_NUM;
        return false;
    } else if (!wav_set_SampleRate(wf, 0xAC44)) {
        wf->err = WAV_ERR_SAMPLE_RATE;
        return false;
    } else if (!wav_set_ByteRate(wf, 0x2B110)) {
        wf->err = WAV_ERR_BYTE_RATE;
        return false;
    } else if (!wav_set_BlockAlign(wf, 0x2)) {
        wf->err = WAV_ERR_BLOCK_ALIGN;
        return false;
    } else if (!wav_set_Subchunk2ID(wf, 0x64617461)) {
        wf->err = WAV_ERR_SUBCHUNK2_ID;
        return false;
    }
    
    return true;
}
