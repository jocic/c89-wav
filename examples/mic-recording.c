#include <stdio.h>

#include <portaudio.h>
#include <format.h>

#define DURATION           10
#define BUFFER_LEN         1000
#define SAMPLE_RATE        44100
#define TARGET_FRAME_COUNT SAMPLE_RATE * DURATION

#define ERR_NONE         0
#define ERR_INIT         1
#define ERR_NO_DEVICE    2
#define ERR_NO_INFO      3
#define ERR_TERM         4
#define ERR_FILE_OPEN    5
#define ERR_FILE_META    6
#define ERR_FILE_CLOSE   7
#define ERR_STREAM_OPEN  8
#define ERR_STREAM_START 9
#define ERR_STREAM_CLOSE 10

PaError cb_record(
    const void *input, void *output,
    unsigned long frame_count,
    const PaStreamCallbackTimeInfo* time_info,
    PaStreamCallbackFlags status_flags,
    void *user_data) {
    
    static int total_frame_count = 0;
    
    WAV_PCM16* iptr = (WAV_PCM16*)input;
    
    for (int i = 0; i < frame_count; i++) {
        WAV_PCM16 sample = *iptr++;
        wav_push_1ch_sample((WAV_FILE*)user_data, &sample);
    }
    
    wav_commit((WAV_FILE*)user_data);
    
    total_frame_count += frame_count;
    if (total_frame_count >= TARGET_FRAME_COUNT) {
        return paComplete;
    }
    
    return paContinue;
}

int main() {
    
    PaError            dev_error;
    PaDeviceIndex      dev_index;
    PaDeviceInfo*      dev_info;
    PaStream*          dev_stream;
    PaStreamParameters dev_params;
    
    WAV_FILE wav_file;
    
    dev_error = Pa_Initialize();
    if (dev_error != paNoError) {
        return ERR_INIT;
    }
    
    dev_index = Pa_GetDefaultInputDevice();
    if (dev_index == paNoDevice) {
        return ERR_NO_DEVICE;
    }
    
    dev_info = (PaDeviceInfo*)Pa_GetDeviceInfo(dev_index);
    if (dev_info == NULL) {
        return ERR_NO_INFO;
    }
    
    printf("Default Sample Rate: %.0f\n", dev_info->defaultSampleRate);
    printf("Channel Count: %d\n", dev_info->maxInputChannels);
    printf("High Latency: %f\n", dev_info->defaultHighInputLatency);
    printf("Low Latency: %f\n", dev_info->defaultLowInputLatency);
    
    wav_file = wav_open("test-file.wav", WAV_NEW);
    if (!wav_is_open(&wav_file)) {
        return ERR_FILE_OPEN;
    }
    
    if (!wav_set_1ch_defaults(&wav_file)) {
        return ERR_FILE_META;
    }
    
    dev_params.channelCount              = 1;
    dev_params.device                    = dev_index;
    dev_params.hostApiSpecificStreamInfo = NULL;
    dev_params.sampleFormat              = paInt16;
    dev_params.suggestedLatency          = dev_info->defaultLowInputLatency;
    
    dev_error = Pa_OpenStream(&dev_stream, &dev_params, NULL,
        SAMPLE_RATE, BUFFER_LEN, paNoFlag, &cb_record, &wav_file);
    
    if (dev_error != paNoError) {
        return ERR_STREAM_OPEN;
    }
    
    dev_error = Pa_StartStream(dev_stream);
    if (dev_error != paNoError) {
        return ERR_STREAM_START;
    }
    
    while (Pa_IsStreamActive(dev_stream)) {
        Pa_Sleep(1000);
    }
    
    dev_error = Pa_CloseStream(dev_stream);
    if (dev_error != paNoError) {
        return ERR_STREAM_CLOSE;
    }
    
    if (!wav_close(&wav_file)) {
        return ERR_FILE_CLOSE;
    }
    
    dev_error = Pa_Terminate();
    if (dev_error != paNoError) {
        return ERR_TERM;
    }
    
    return ERR_NONE;
}
