#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "format.h"
#include "tones.h"

bool wav_gen_square(char *loc, uint32_t freq, uint32_t dur) {

    WAV_FILE wf = wav_open(loc, WAV_NEW);

    if (wf.bin.open) {

        uint32_t srate = 8000;
        uint32_t scnt  = (srate * dur) / 1000;
        uint32_t fint  = srate / freq;

        wav_set_NumChannels(&wf, 1);
        wav_set_BitsPerSample(&wf, 16);
        wav_set_SampleRate(&wf, srate);

        for (int i = 0; i < scnt; i++) {

            if (i % fint == 0) {
                wav_push_sample(&wf, INT16_MAX);
            } else {
                wav_push_sample(&wf, INT16_MIN);
            }
        }

        return wav_close(&wf);
    }

    return false;
}

bool wav_gen_noise(char *loc, uint32_t dur) {

    WAV_FILE wf = wav_open(loc, WAV_NEW);

    if (wf.bin.open) {

        uint32_t srate = 8000;
        uint32_t scnt  = (srate * dur) / 1000;

        wav_set_NumChannels(&wf, 1);
        wav_set_BitsPerSample(&wf, 16);
        wav_set_SampleRate(&wf, srate);

        srand(time(NULL));

        for (int i = 0; i < scnt; i++) {
            wav_push_sample(&wf, rand());
        }

        return wav_close(&wf);
    }

    return false;
}
