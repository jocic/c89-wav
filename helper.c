#include <stdio.h>
#include <stdbool.h>

#include "helper.h"
#include "wav/format.h"

void wav_details(WAV_FILE *wf) {

    printf("---- RIFF CHUNK DESCRIPTOR ----\n\n");

    printf("Chunk ID: 0x%X\n", wav_get_ChunkID(wf));
    printf("Chunk Size: 0x%X\n", wav_get_ChunkSize(wf));
    printf("Format: 0x%X\n", wav_get_Format(wf));

    printf("\n---- FMT SUB-CHUNK ----\n\n");

    printf("Subchunk ID #1: 0x%X\n", wav_get_Subchunk1ID(wf));
    printf("Subchunk Size #1: 0x%X\n", wav_get_Subchunk1Size(wf));
    printf("Audio Format: 0x%X\n", wav_get_AudioFormat(wf));
    printf("Number of Channels: 0x%X\n", wav_get_NumChannels(wf));
    printf("Sample Rate: 0x%X\n", wav_get_SampleRate(wf));
    printf("Byte Rate: 0x%X\n", wav_get_ByteRate(wf));
    printf("Block Align: 0x%X\n", wav_get_BlockAlign(wf));
    printf("Bits per Sample: 0x%X\n", wav_get_BitsPerSample(wf));

    printf("\n---- DATA SUB-CHUNK ----\n\n");

    printf("Subchunk ID #2: 0x%X\n", wav_get_Subchunk2ID(wf));
    printf("Subchunk Size #2: 0x%X\n", wav_get_Subchunk2Size(wf));

    printf("\n---- Other Information ----\n\n");

    printf("Duration in MS: %d\n", wav_est_duration(wf));
    printf("No. of Samples: %d\n", wav_sample_count(wf));
}
