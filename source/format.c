#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/file.h>

#include "bin.h"
#include "format.h"

WAV_FILE wav_open(char *loc, uint8_t mode) {
    
    WAV_FILE wf;
    
    if (mode == WAV_READ) {
        wf.bin = bin_open(loc, O_RDONLY);
    } else if (mode == WAV_ALTER) {
        wf.bin = bin_open(loc, O_RDWR);
    } else if (mode == WAV_NEW) {
        wf.bin = bin_open(loc, O_RDWR | O_CREAT);
        wav_set_defaults(&wf);
    } else {
        printf("[x] Invalid mode provided.\n");
    }
    
    wf.curr = 0;
    
    return wf;
}

bool wav_close(WAV_FILE *wf) {
    
    if ((*wf).alt) {
        wav_set_ChunkID(wf, 0x52494646);
        wav_set_ChunkSize(wf, (*wf).curr + 36);
        wav_set_ByteRate(wf, wav_get_SampleRate(wf) * wav_get_NumChannels(wf) * wav_get_BitsPerSample(wf) / 8);
        wav_set_BlockAlign(wf, wav_get_NumChannels(wf) * wav_get_BitsPerSample(wf) / 8);
        wav_set_Subchunk2Size(wf, (*wf).curr);
    }
    
    return bin_close(&(*wf).bin);
}

uint32_t wav_sample_count(WAV_FILE *wf) {
    return wav_get_Subchunk2Size(wf) / (wav_get_BitsPerSample(wf) / 8);
}

uint32_t wav_est_duration(WAV_FILE *wf) {
    return (wav_get_Subchunk2Size(wf) * 1000) / wav_get_NumChannels(wf) /
        (wav_get_BitsPerSample(wf) / 8) / wav_get_SampleRate(wf);
}

void wav_rewind(WAV_FILE *wf) {
    (*wf).curr = 0;
}

bool wav_has_next(WAV_FILE *wf) {
    return (*wf).curr < wav_get_Subchunk2Size(wf);
}

int32_t wav_next_sample(WAV_FILE *wf) {
    
    uint8_t bps = wav_get_BitsPerSample(wf);
    (*wf).curr += bps / 2;
    
    return wav_get_sample(wf, (*wf).curr);
}

bool wav_push_sample(WAV_FILE *wf, int32_t val) {
    
    uint8_t bps = wav_get_BitsPerSample(wf);
    
    if (wav_set_sample(wf, (*wf).curr, val)) {
        
        (*wf).curr += bps / 8;
        (*wf).alt   = true;
        
        return true;
    }
    
    return false;
}

int32_t wav_get_sample(WAV_FILE *wf, uint32_t off) {
    
    uint8_t bps = wav_get_BitsPerSample(wf);
    
    switch(bps) {
        case 8:
            return (int8_t)bin_r8(&(*wf).bin, 44 + off);
        case 16:
            return (int16_t)bin_r16l(&(*wf).bin, 44 + off);
        case 32:
            return (int32_t)bin_r32l(&(*wf).bin, 44 + off);
    }
    
    return 0;
}

bool wav_set_sample(WAV_FILE *wf, uint32_t off, int32_t val) {
    
    uint8_t bps = wav_get_BitsPerSample(wf);
    
    switch(bps) {
        case 8:
            return bin_w8(&(*wf).bin, 44 + off, (uint8_t)val);
        case 16:
            return bin_w16l(&(*wf).bin, 44 + off, (int16_t)val);
        case 32:
            return bin_w32l(&(*wf).bin, 44 + off, (int32_t)val);
    }
    
    return false;
}

bool wav_is_valid(WAV_FILE *wf) {
    
    uint32_t tmp;
    bool     sts;
    
    if ((sts = ((tmp = wav_get_ChunkID(wf)) != 0x52494646))) {
        printf("[X] Invalid Chunk ID: 0x%X\n", tmp);
    } else if ((sts = ((tmp = wav_get_Format(wf)) != 0x57415645))) {
        printf("[X] Invalid file format: 0x%X\n", tmp);
    } else if ((sts = ((tmp = wav_get_Subchunk1ID(wf)) != 0x666D7420))) {
        printf("[X] Invalid Subchunk1 ID: 0x%X\n", tmp);
    } else if ((sts = ((tmp = wav_get_Subchunk2ID(wf)) != 0x64617461))) {
        printf("[X] Invalid Subchunk2 ID: 0x%X\n", tmp);
    } else if ((sts = ((tmp = wav_get_AudioFormat(wf)) != 0x1))) {
        printf("[X] Not a PCM audio format: 0x%X\n", tmp);
    } else if ((sts = ((tmp = wav_get_NumChannels(wf)) < 1 || tmp > 2))) {
        printf("[X] Invalid number of channels: 0x%X\n", tmp);
    } else if ((sts = ((tmp = wav_get_BitsPerSample(wf)) % 8 != 0))) {
        printf("[X] Invalid bits per sample: 0x%X\n", tmp);
    }
    
    return !sts;
}

bool wav_set_defaults(WAV_FILE *wf) {
    
    (*wf).alt = true;
    
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

uint32_t wav_get_ChunkID(WAV_FILE *wf) {
    return bin_r32b(&(*wf).bin, 0);
}

uint32_t wav_get_ChunkSize(WAV_FILE *wf) {
    return bin_r32l(&(*wf).bin, 4);
}

uint32_t wav_get_Format(WAV_FILE *wf) {
    return bin_r32b(&(*wf).bin, 8);
}

uint32_t wav_get_Subchunk1ID(WAV_FILE *wf) {
    return bin_r32b(&(*wf).bin, 12);
}

uint32_t wav_get_Subchunk1Size(WAV_FILE *wf) {
    return bin_r32l(&(*wf).bin, 16);
}

uint16_t wav_get_AudioFormat(WAV_FILE *wf) {
    return bin_r16l(&(*wf).bin, 20);
}

uint16_t wav_get_NumChannels(WAV_FILE *wf) {
    return bin_r16l(&(*wf).bin, 22);
}

uint32_t wav_get_SampleRate(WAV_FILE *wf) {
    return bin_r32l(&(*wf).bin, 24);
}

uint32_t wav_get_ByteRate(WAV_FILE *wf) {
    return bin_r32l(&(*wf).bin, 28);
}

uint16_t wav_get_BlockAlign(WAV_FILE *wf) {
    return bin_r16l(&(*wf).bin, 32);
}

uint16_t wav_get_BitsPerSample(WAV_FILE *wf) {
    return bin_r16l(&(*wf).bin, 34);
}

uint32_t wav_get_Subchunk2ID(WAV_FILE *wf) {
    return bin_r32b(&(*wf).bin, 36);
}

uint32_t wav_get_Subchunk2Size(WAV_FILE *wf) {
    return bin_r32l(&(*wf).bin, 40);
}

bool wav_set_ChunkID(WAV_FILE *wf, uint32_t val) {
    return bin_w32b(&(*wf).bin, 0, val);
}

bool wav_set_ChunkSize(WAV_FILE *wf, uint32_t val) {
    return bin_w32l(&(*wf).bin, 4, val);
}

bool wav_set_Format(WAV_FILE *wf, uint32_t val) {
    return bin_w32b(&(*wf).bin, 8, val);
}

bool wav_set_Subchunk1ID(WAV_FILE *wf, uint32_t val) {
    return bin_w32b(&(*wf).bin, 12, val);
}

bool wav_set_Subchunk1Size(WAV_FILE *wf, uint32_t val) {
    return bin_w32l(&(*wf).bin, 16, val);
}

bool wav_set_AudioFormat(WAV_FILE *wf, uint16_t val) {
    return bin_w16l(&(*wf).bin, 20, val);
}

bool wav_set_NumChannels(WAV_FILE *wf, uint16_t val) {
    return bin_w16l(&(*wf).bin, 22, val);
}

bool wav_set_SampleRate(WAV_FILE *wf, uint32_t val) {
    return bin_w32l(&(*wf).bin, 24, val);
}

bool wav_set_ByteRate(WAV_FILE *wf, uint32_t val) {
    return bin_w32l(&(*wf).bin, 28, val);
}

bool wav_set_BlockAlign(WAV_FILE *wf, uint16_t val) {
    return bin_w16l(&(*wf).bin, 32, val);
}

bool wav_set_BitsPerSample(WAV_FILE *wf, uint16_t val) {
    return bin_w16l(&(*wf).bin, 34, val);
}

bool wav_set_Subchunk2ID(WAV_FILE *wf, uint32_t val) {
    return bin_w32b(&(*wf).bin, 36, val);
}

bool wav_set_Subchunk2Size(WAV_FILE *wf, uint32_t val) {
    return bin_w32l(&(*wf).bin, 40, val);
}
