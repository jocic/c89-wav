# C89 WAV Library

WAV file management library written in ASNI C, suitable for use with resource-limited microcontrollers.

[![Buy Me Coffee](assets/images/buy-me-coffee.png)](https://www.paypal.me/DjordjeJocic)

## Examples

Following examples should cover all basic use-case scenarios.

### Opening & Closing

```c
WAV_FILE file = wav_open("/path/to/file.wav", WAV_RD);

if (file.open) {
    
    // Do Stuff
    
    if (!wav_close(&file)) {
        // Handle I/O Error
    }
}
```

### WAV Header

```c
WAV_FILE file = wav_open("/path/to/file.wav", WAV_RD);

if (file.open) {
    
    uint32_t chunk_id        = wav_get_ChunkID(&file);
    uint32_t chunk_size      = wav_get_ChunkSize(&file);
    uint32_t format          = wav_get_Format(&file);
    uint32_t subchunk1_id    = wav_get_Subchunk1ID(&file);
    uint32_t subchunk1_size  = wav_get_Subchunk1Size(&file);
    uint16_t audio_format    = wav_get_AudioFormat(&file);
    uint16_t num_channels    = wav_get_NumChannels(&file);
    uint32_t sample_rate     = wav_get_SampleRate(&file);
    uint32_t byte_rate       = wav_get_ByteRate(&file);
    uint16_t block_align     = wav_get_BlockAlign(&file);
    uint16_t bits_per_sample = wav_get_BitsPerSample(&file);
    uint32_t subchunk2_id    = wav_get_Subchunk2ID(&file);
    uint32_t subchunk2_size  = wav_get_Subchunk2Size(&file);
    
    if (!wav_close(&file)) {
        // Handle I/O Error
    }
}
```

### Reading Samples

```c
WAV_FILE file = wav_open("/path/to/file.wav", WAV_RD);

if (file.open) {
    
    uint32_t nth_sample = wav_get_sample(&file, 2); // You can fetch a specific sample - 0-indexed...
    
    while (wav_has_next(&file)) { // You can loop through all of the samples...
        
        int32_t sample = wav_next_sample(&file);
        
        // ...
    }
    
    wav_rewind(&file); // If you want to loop through the samples again...
    
    if (!wav_close(&file)) {
        // Handle I/O Error
    }
}
```

### Writing Samples

```c
WAV_FILE file = wav_open("/path/to/file.wav", WAV_NEW);

if (file.open) {
    
    int16_t sample_high = (pow(2, 15) - 1);
    int16_t sample_low  = sample_high * -1;
    
    int duration           = 5;
    int tone_frequency     = 1000;
    int samples_per_period = 44100 / tone_frequency;
    int total_samples      = duration * 44100;
    
    bool pos_period = false;
    
    for (int i = 0; i < total_samples; i++) {
        
        if (pos_period) {
            wav_push_sample(&file, sample_high);
        } else {
            wav_push_sample(&file, sample_low);
        }
        
        if ((i % samples_per_period) == 0) {
            pos_period = !pos_period;
        }
    }
    
    if (!wav_close(&file)) {
        // Handle I/O Error
    }
}
```

## Versioning Scheme

I use a 3-digit [Semantic Versioning](https://semver.org/spec/v2.0.0.html) identifier, for example 1.0.2. These digits have the following meaning:

*   The first digit (1) specifies the MAJOR version number.
*   The second digit (0) specifies the MINOR version number.
*   The third digit (2) specifies the PATCH version number.

## Copyright & License

Copyright (C) 2022 Đorđe Jocić

Licensed under the MIT license.
