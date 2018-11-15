//
// Created by ezra on 11/3/18.
//

#include <algorithm>
#include <portaudio.h>

#include "PaWrapper.h"
#include "SoftCut.h"

using namespace softcut;

class PaWrapper::Imp {

    friend class PaWrapper;
private:
    enum { SAMPLE_RATE = 44100, FRAMES_PER_BUFFER = 1024 };
    PaStream *stream;
    const PaDeviceInfo* inputInfo;
    const PaDeviceInfo* outputInfo;
    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    int numChannels;

protected:
    SoftCut sc;

public:
    static int AudioCallback( const void *input, void *output,
                              unsigned long numFrames,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData ) {
        (void)timeInfo;
        float *in[2];
        float *out[2];
        in[0] = ((float**)input)[0];
        in[1] = ((float**)input)[1];
        out[0] = ((float**)output)[0];
        out[1] = ((float**)output)[1];

        auto* imp = static_cast<PaWrapper::Imp*>(userData);
        SoftCut& sc = imp->sc;

        sc.processBlock(in[0], in[1], out[0], out[1], numFrames);

        return paContinue;
    }

    void setup() {

        sc.setSampleRate(SAMPLE_RATE);

        PaError err = Pa_Initialize();
        if( err != paNoError ) {
            throw("failed to initialize portaudio");
        }

        inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
        printf( "Input device # %d.\n", inputParameters.device );
        inputInfo = Pa_GetDeviceInfo( inputParameters.device );
        printf( "    Name: %s\n", inputInfo->name );
        printf( "      LL: %g s\n", inputInfo->defaultLowInputLatency );
        printf( "      HL: %g s\n", inputInfo->defaultHighInputLatency );

        outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
        printf( "Output device # %d.\n", outputParameters.device );
        outputInfo = Pa_GetDeviceInfo( outputParameters.device );
        printf( "   Name: %s\n", outputInfo->name );
        printf( "     LL: %g s\n", outputInfo->defaultLowOutputLatency );
        printf( "     HL: %g s\n", outputInfo->defaultHighOutputLatency );

        numChannels = std::min(inputInfo->maxInputChannels, outputInfo->maxOutputChannels);
        printf( "Num channels = %d.\n", numChannels );
        if(numChannels < 2) { 
            printf("warning: not enough I/O channels!");
        }

        inputParameters.channelCount = numChannels;
        inputParameters.sampleFormat = paFloat32 | paNonInterleaved;
        inputParameters.suggestedLatency = inputInfo->defaultHighInputLatency ;
        inputParameters.hostApiSpecificStreamInfo = nullptr;

        outputParameters.channelCount = numChannels;
        outputParameters.sampleFormat = paFloat32 | paNonInterleaved;
        outputParameters.suggestedLatency = outputInfo->defaultHighOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = nullptr;

        err = Pa_OpenStream(
                &stream,
                &inputParameters,
                &outputParameters,
                SAMPLE_RATE,
                FRAMES_PER_BUFFER,
                paClipOff | paDitherOff,
                AudioCallback,
                this );
        if( err != paNoError ) {
            printf("failed to open stream\n");
            throw("failed to open stream");
        }
    }

    void start() {
        PaError err = Pa_StartStream( stream );
        if( err != paNoError ) { 
            printf("failed to start stream\n");
         }
    }

    void stop() {
        PaError err = Pa_StopStream(stream);
        if( err != paNoError ) { 
            printf("failed to stop stream\n");
        }
    }

    void cleanup() {
        PaError err = Pa_CloseStream(stream);
        if( err != paNoError ) { 
            printf("failed to close stream\n");
         }
        Pa_Terminate();
    }
};

PaWrapper::Imp PaWrapper::imp;

void PaWrapper::setup() {
    PaWrapper::imp.setup();
}

void PaWrapper::cleanup() {
    PaWrapper::imp.cleanup();
}

void PaWrapper::start() {
    PaWrapper::imp.start();
}

void PaWrapper::stop() {
    PaWrapper::imp.stop();
}

SoftCut* PaWrapper::getSoftCut() {
    return &PaWrapper::imp.sc;
}