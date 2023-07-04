#ifndef C89_WAV_TONES_H
    
    #include <stdint.h>
    #include <stdbool.h>
    
    #define C89_WAV_TONES_H
    
    bool wav_gen_square(char *loc, uint32_t freq, uint32_t dur);
    bool wav_gen_sine(char *loc, uint32_t freq, uint32_t dur);
    bool wav_gen_tringle(char *loc, uint32_t freq, uint32_t dur);
    bool wav_gen_sawtooth(char *loc, uint32_t freq, uint32_t dur);
    bool wav_gen_noise(char *loc, uint32_t dur);
    
#endif // WAV_TONES_H
