/*      all.c
 *
 * A great all-in-one MIDAS API example
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#if defined(__NT__) || defined(__WINDOWS__) || defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#ifndef __linux__
#include <conio.h>
#endif
#include <stdlib.h>
#include "midasdll.h"

/* No stream support in DOS: */
#if defined(__DOS__) || defined(__DJGPP__)
#define NOSTREAMS
#define DOS
#endif
/* Actually polling stream support is there, just no support for playing
   stream files automatically */


/* We'll use a maximum of 2 stream channels, 4 sample channels and
   16 channels for music 1 and 2 channels for music 2: */
#define NUMSTREAMCHANNELS 2
#define NUMSAMPLECHANNELS 4
#define NUMMUSICCHANNELS 16
#define NUMMUSIC2CHANNELS 2


/****************************************************************************\
*
* Function:     void MIDASerror(void)
*
* Description:  Handles a MIDAS error - displays an error message and exits
*
\****************************************************************************/

void MIDASerror(void)
{
    int         error;

    error = MIDASgetLastError();
    printf("\nMIDAS error: %s\n", MIDASgetErrorMessage(error));
    if ( !MIDASclose() )
    {
        printf("\nBIG PANIC! MIDASclose Failed: %s\n", MIDASgetErrorMessage(
            MIDASgetLastError()));
    }
    exit(EXIT_FAILURE);
}



/****************************************************************************\
*
*       Post-processing stuff for a cheap "robotize" effect
*
\****************************************************************************/

MIDASpostProcessor *postProc = NULL;

#define ONSAMPLES 300
#define OFFSAMPLES 200


/* Floating-point mono post-processing function: */
void MIDAS_CALL PostProcFloatMono(void *data, unsigned numSamples, void *user)
{
    float *d = (float*) data;
    static unsigned count = ONSAMPLES;
    static int on = 1;
    
    user = user;

    while ( numSamples )
    {
        if ( on )
        {
            d++;
            if ( (--count) == 0 ) { on = 0; count = OFFSAMPLES; }
        }
        else
        {
            *(d++) = 0.0f;
            if ( (--count) == 0 ) { on = 1; count = ONSAMPLES; }
        }
        numSamples--;
    }
}


/* Floating-point stereo post-processing function: */
void MIDAS_CALL PostProcFloatStereo(void *data, unsigned numSamples,
                                    void *user)
{
    float *d = (float*) data;
    static unsigned count = ONSAMPLES;
    static int on = 1;
    
    user = user;

    while ( numSamples )
    {
        if ( on )
        {
            d += 2;
            if ( (--count) == 0 ) { on = 0; count = OFFSAMPLES; }
        }
        else
        {
            *(d++) = 0.0f;
            *(d++) = 0.0f;
            if ( (--count) == 0 ) { on = 1; count = ONSAMPLES; }
        }
        numSamples--;
    }
}


/* Integer mono post-processing function: */
void MIDAS_CALL PostProcIntMono(void *data, unsigned numSamples, void *user)
{
    int *d = (int*) data;
    static unsigned count = ONSAMPLES;
    static int on = 1;
    
    user = user;

    while ( numSamples )
    {
        if ( on )
        {
            d++;
            if ( (--count) == 0 ) { on = 0; count = OFFSAMPLES; }
        }
        else
        {
            *(d++) = 0;
            if ( (--count) == 0 ) { on = 1; count = ONSAMPLES; }
        }
        numSamples--;
    }
}



/* Integer stereo post-processing function: */
void MIDAS_CALL PostProcIntStereo(void *data, unsigned numSamples, void *user)
{
    int *d = (int*) data;
    static unsigned count = ONSAMPLES;
    static int on = 1;
    
    user = user;

    while ( numSamples )
    {
        if ( on )
        {
            d += 2;
            if ( (--count) == 0 ) { on = 0; count = OFFSAMPLES; }
        }
        else
        {
            *(d++) = 0;
            *(d++) = 0;
            if ( (--count) == 0 ) { on = 1; count = ONSAMPLES; }
        }
        numSamples--;
    }
}



void AddPostProcessor(void)
{
    if ( postProc != NULL )
        return;
    
    postProc = malloc(sizeof(MIDASpostProcessor));
    postProc->floatMono = &PostProcFloatMono;
    postProc->floatStereo = &PostProcFloatStereo;
    postProc->intMono = &PostProcIntMono;
    postProc->intStereo = &PostProcIntStereo;

    if ( !MIDASaddPostProcessor(postProc, MIDAS_POST_PROC_FIRST, NULL) )
        MIDASerror();    
}


void RemovePostProcessor(void)
{
    if ( postProc == NULL )
        return;

    if ( !MIDASremovePostProcessor(postProc) )
        MIDASerror();

    free(postProc);
    postProc = NULL;
}



static MIDASmodule module, module2;
static MIDASmodulePlayHandle modulePlayHandle;
static MIDASmodulePlayHandle module2PlayHandle;
static MIDASstreamHandle stream1, stream2;
static MIDASsample sample1, sample2;
static MIDASsamplePlayHandle playHandle1, playHandle2;


int main(void)
{
    int         key, exit = 0;

    MIDASstartup();

    setbuf(stdout, NULL);

    /* Flag that we don't have a module, effects or streams playing: */
    module = NULL;
    stream1 = stream2 = NULL;
    sample1 = sample2 = 0;
    playHandle1 = playHandle2 = 0;
    module2PlayHandle = 0;

    /* Decrease the size of buffer used: (not needed in DOS, the default
       buffers there are already much smaller) */
#ifndef DOS    
    MIDASsetOption(MIDAS_OPTION_MIXBUFLEN, 150);
    MIDASsetOption(MIDAS_OPTION_MIXBUFBLOCKS, 4);
#endif    

    /* Initialize MIDAS and start background playback (at 100 polls
       per second): */
    if ( !MIDASinit() )
        MIDASerror();
    if ( !MIDASstartBackgroundPlay(100) )
        MIDASerror();

    /* Open all channels: */
    if ( !MIDASopenChannels(NUMSTREAMCHANNELS + NUMSAMPLECHANNELS +
        NUMMUSICCHANNELS + NUMMUSIC2CHANNELS) )
        MIDASerror();

    /* Allocate a part of the channels for use as automatic sound effect
       channels: */
    if ( !MIDASallocAutoEffectChannels(NUMSAMPLECHANNELS) )
        MIDASerror();

    /* Load our samples: */
#ifdef __linux__
    if ( (sample1 = MIDASloadWaveSample("../data/explosi1.wav",
        MIDAS_LOOP_NO)) == 0 )
        MIDASerror();
    if ( (sample2 = MIDASloadWaveSample("../data/laugh1.wav",
        MIDAS_LOOP_YES)) == 0 )
        MIDASerror();
#else
    if ( (sample1 = MIDASloadWaveSample("..\\data\\explosi1.wav",
        MIDAS_LOOP_NO)) == 0 )
        MIDASerror();
    if ( (sample2 = MIDASloadWaveSample("..\\data\\laugh1.wav",
        MIDAS_LOOP_YES)) == 0 )
        MIDASerror();
#endif    

    /* Load module 2: */
#ifdef __linux__
    if ( (module2 = MIDASloadModule("../data/songsect.xm")) == NULL )
        MIDASerror();
#else    
    if ( (module2 = MIDASloadModule("..\\data\\songsect.xm")) == NULL )
        MIDASerror();
#endif    

    /* Loop, reading user input, until we should exit: */
    while ( !exit )
    {
        puts("Keys:     1/2     Play/Stop sample 1\n"
             "          q/w     Play/Stop sample 2\n"
#ifndef NOSTREAMS
             "          3/4     Play/Stop stream 1\n"
             "          e/r     Play/Stop stream 2\n"
#endif
             "          5/6     Play/Stop module\n"
             "          7/8     Play/Stop module 2 section 1\n"
             "          u/i     Play/Stop module 2 section 2\n"
             "          j/k     Play/Stop module 2 section 3\n"
             "          a/s     Add/Remove post-processor\n"
             "          Esc     Exit\n");

#ifdef __linux__
        key = getchar();
#else
        key = getch();
#endif        

        switch ( key )
        {
            case 27:
                exit = 1;
                break;

            case '1':
                if ( (playHandle1 = MIDASplaySample(sample1,
                    MIDAS_CHANNEL_AUTO, 0, 22050, 64, MIDAS_PAN_MIDDLE)) == 0)
                    MIDASerror();
                break;

            case '2':
                if ( playHandle1 != 0 )
                {
                    if ( !MIDASstopSample(playHandle1) )
                        MIDASerror();
                }
                break;

            case 'q':
                if ( (playHandle2 = MIDASplaySample(sample2,
                    MIDAS_CHANNEL_AUTO, 0, 16000, 64, -20)) == 0 )
                    MIDASerror();
                break;

            case 'w':
                if ( playHandle2 != 0 )
                {
                    if ( !MIDASstopSample(playHandle2) )
                        MIDASerror();
                }
                break;

#ifndef NOSTREAMS
            case '3':
                if ( stream1 != NULL )
                {
                    if ( !MIDASstopStream(stream1) )
                        MIDASerror();
                }
                if ( (stream1 = MIDASplayStreamWaveFile("e:\\fable-mono.wav",
                    500, 0)) == NULL )
                    MIDASerror();
                break;

            case '4':
                if ( stream1 != NULL )
                {
                    if ( !MIDASstopStream(stream1) )
                        MIDASerror();
                    stream1 = NULL;
                }
                break;

            case 'e':
                if ( stream2 != NULL )
                {
                    if ( !MIDASstopStream(stream2) )
                        MIDASerror();
                }
#ifdef __linux__
                if ( (stream2 = MIDASplayStreamWaveFile(
                    "../data/powerups.wav", 500, 1)) == NULL )
                    MIDASerror();
#else                
                if ( (stream2 = MIDASplayStreamWaveFile(
                    "..\\data\\powerups.wav", 500, 1)) == NULL )
                    MIDASerror();
#endif                
                break;

            case 'r':
                if ( stream2 != NULL )
                {
                    if ( !MIDASstopStream(stream2) )
                        MIDASerror();
                    stream2 = NULL;
                }
                break;
#endif /* #ifndef NOSTREAMS */

            case '5':
                if ( module != NULL )
                {
                    if ( !MIDASstopModule(modulePlayHandle) )
                        MIDASerror();
                    if ( !MIDASfreeModule(module) )
                        MIDASerror();
                }
#ifdef __linux__                
                if ( (module = MIDASloadModule("../data/templsun.xm"))
                    == NULL )
                    MIDASerror();
#else
                if ( (module = MIDASloadModule("..\\data\\templsun.xm"))
                    == NULL )
                    MIDASerror();
#endif
                if ( (modulePlayHandle = MIDASplayModule(module, TRUE)) == 0 )
                    MIDASerror();
                break;

            case '6':
                if ( module != NULL )
                {
                    if ( !MIDASstopModule(modulePlayHandle) )
                        MIDASerror();
                    if ( !MIDASfreeModule(module) )
                        MIDASerror();

                    module = NULL;
                }
                break;

            case '7':
                if ( module2PlayHandle != 0 )
                    if ( !MIDASstopModule(module2PlayHandle) )
                        MIDASerror();
                if ( (module2PlayHandle = MIDASplayModuleSection(module2,
                                                                 0, 0, 0,
                                                                 FALSE)) == 0)
                    MIDASerror();
                break;

            case '8':
            case 'i':
            case 'k':
                if ( module2PlayHandle != 0 )
                    if ( !MIDASstopModule(module2PlayHandle) )
                        MIDASerror();
                module2PlayHandle = 0;
                break;
                     
            case 'u':
                if ( module2PlayHandle != 0 )
                    if ( !MIDASstopModule(module2PlayHandle) )
                        MIDASerror();
                if ( (module2PlayHandle = MIDASplayModuleSection(module2,
                                                                 1, 1, 1,
                                                                 FALSE)) == 0)
                    MIDASerror();
                break;

            case 'j':
                if ( module2PlayHandle != 0 )
                    if ( !MIDASstopModule(module2PlayHandle) )
                        MIDASerror();
                if ( (module2PlayHandle = MIDASplayModuleSection(module2,
                                                                 2, 3, 3,
                                                                 TRUE)) == 0)
                    MIDASerror();
                break;

            case 'a':
                AddPostProcessor();
                break;

            case 's':
                RemovePostProcessor();
                break;
        }
    }

    /* Stop MIDAS: */
    if ( !MIDASstopBackgroundPlay() )
        MIDASerror();
    if ( !MIDASclose() )
        MIDASerror();

    return 0;
}
