/*      midpecho.cpp
 *
 * MidpNT echo effect manager
 *
 * $Id: midpecho.cpp,v 1.3 1997/07/12 11:58:11 pekangas Exp $
 *
 * Copyright 1997 Petteri Kangaslampi
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "midasdll.h"
#include "midpnt.h"
#include "midplist.h"
#include "registry.h"
#include "midpecho.h"


midpEchoEffect *currentEffect = NULL;


MIDASechoHandle midpEchoEffect::currentEcho = NULL;

static char *effectKey = "Software\\Sahara Surfers\\MidpNT\\Effects";



typedef struct
{
    double delay;
    double gain;
    int reverse;
    unsigned filterType;
} DefaultEcho;

typedef struct
{
    char *name;
    double feedback;
    double gain;
    unsigned numEchoes;
    DefaultEcho *echoes;
} DefaultEffect;



#define NUMDEFAULTEFFECTS 4

DefaultEcho defaultEchoes1[5] =
{
    { 42.0, 0.16, 0, 1 },
    { 79.0, -0.19, 0, 1 },
    { 107.0, 0.24, 1, 1 },
    { 157.0, -0.27, 1, 1 },
    { 163.0, 0.28, 0, 1 }
};

DefaultEcho defaultEchoes2[1] =
{
    { 1.0, -1.4, 0, 0 }
};

DefaultEcho defaultEchoes3[1] =
{
    { 42.17, -0.6, 1, 0 }
};

DefaultEcho defaultEchoes4[1] =
{
    { 250.0, 0.5, 0, 0 }
};   

DefaultEffect defaultEffects[NUMDEFAULTEFFECTS] =
{
    { "Hall Reverb?", 0.75, 0.85, 5, defaultEchoes1 },
    { "PC Speaker", 0.6, 1.0, 1, defaultEchoes2 },
    { "Stereo", 0.0, 0.8, 1, defaultEchoes3 },
    { "Superkaiku", 0.8, 1.0, 1, defaultEchoes4 }
};



midpEcho::midpEcho()
{
    delay = 100.0f;
    gain = 0.0f;
    reverseChannels = 0;
    filterType = 0;
}


void midpEcho::SetDelay(double newDelay)
{
    delay = newDelay;
    if ( delay > 4000.0 )
        delay = 4000.0;
}


void midpEcho::SetGain(double newGain)
{
    gain = newGain;
    if ( gain > 5.0 )
        gain = 5.0;
    if ( gain < -5.0 )
        gain = 5.0;
}



midpEchoEffect::midpEchoEffect(int noEffect)
{
    this->noEffect = noEffect;
    inRegistry = 0;
    name[0] = 0;
    registryName[0] = 0;
    numEchoes = 0;
    feedback = 0.0;
    gain = 1.0;

    if ( noEffect )
        strcpy(name, "None");
}


midpEchoEffect::~midpEchoEffect()
{
}


void midpEchoEffect::SetNumEchoes(unsigned num)
{
    if ( noEffect )
        return;
    
    if ( num <= MAXECHOES )
        numEchoes = num;
    else
        numEchoes = MAXECHOES;
}



void midpEchoEffect::SetFeedback(double newFeedback)
{
    if ( !noEffect )
        feedback = newFeedback;
    if ( feedback > 5.0 )
        feedback = 5.0;
    if ( feedback < -5.0 )
        feedback = -5.0;
}


void midpEchoEffect::SetGain(double newGain)
{
    if ( !noEffect )
        gain = newGain;
    if ( gain > 5.0 )
        gain = 5.0;
    if ( gain < -5.0 )
        gain = -5.0;
}


void midpEchoEffect::ReadFromRegistry(char *name)
{
    Registry reg;
    char str[256 + ECHONAMELEN];
    char val[64];
    unsigned n;
    
    strncpy(registryName, name, ECHONAMELEN);
    registryName[ECHONAMELEN] = 0;
    strcpy(this->name, registryName);

    sprintf(str, "%s\\%s", effectKey, registryName);
    assert(reg.KeyExists(str));
    reg.OpenKey(str);

    numEchoes = reg.ValueDWORD("NumEchoes", 0);
    reg.ValueString("Feedback", "0.0", val, 64);
    feedback = atof(val);
    reg.ValueString("Gain", "1.0", val, 64);
    gain = atof(val);

    for ( n = 0; n < numEchoes; n++ )
    {
        sprintf(str, "%u Delay", n);
        reg.ValueString(str, "0.0", val, 64);
        echoes[n].SetDelay(atof(val));

        sprintf(str, "%u Gain", n);
        reg.ValueString(str, "0.0", val, 64);
        echoes[n].SetGain(atof(val));

        sprintf(str, "%u Reverse", n);
        echoes[n].SetReverse((int) reg.ValueDWORD(str, 0));

        sprintf(str, "%u FilterType", n);
        echoes[n].SetFilterType(reg.ValueDWORD(str, 0));
    }

    inRegistry = 1;

    reg.CloseKey();
}



void midpEchoEffect::SaveToRegistry()
{
    Registry reg;
    char str[256 + ECHONAMELEN];
    char val[64];
    unsigned n;

    if ( noEffect )
        return;

    if ( inRegistry && strcmp(name, registryName) )
    {
        reg.OpenKey(effectKey);
        reg.DeleteSubKey(registryName);
        reg.CloseKey();
    }

    strcpy(registryName, name);

    sprintf(str, "%s\\%s", effectKey, registryName);
    reg.CreateKey(str);

    reg.WriteDWORD("NumEchoes", numEchoes);
    sprintf(val, "%f", feedback);
    reg.WriteString("Feedback", val);
    sprintf(val, "%f", gain);
    reg.WriteString("Gain", val);

    for ( n = 0; n < numEchoes; n++ )
    {
        sprintf(str, "%u Delay", n);
        sprintf(val, "%f", echoes[n].GetDelay());
        reg.WriteString(str, val);

        sprintf(str, "%u Gain", n);
        sprintf(val, "%f", echoes[n].GetGain());
        reg.WriteString(str, val);

        sprintf(str, "%u Reverse", n);
        reg.WriteDWORD(str, (DWORD) echoes[n].GetReverse());

        sprintf(str, "%u FilterType", n);
        reg.WriteDWORD(str, echoes[n].GetFilterType());

    }

    reg.CloseKey();
}



void midpEchoEffect::DeleteFromRegistry()
{
    Registry reg;

    if ( inRegistry )
    {
        reg.OpenKey(effectKey);
        reg.DeleteSubKey(registryName);
        reg.CloseKey();
        inRegistry = 0;
    }
}


void midpEchoEffect::Rename(char *name)
{
    if ( noEffect )
        return;
    
    strncpy(this->name, name, ECHONAMELEN);
    this->name[ECHONAMELEN] = 0;
}


void midpEchoEffect::Activate()
{
    MIDASechoSet echoSet;
    MIDASecho *mechoes;
    unsigned n;
    
    if ( currentEcho != NULL )
    {
        if ( !MIDASremoveEchoEffect(currentEcho) )
            MIDASerror();
        currentEcho = NULL;
        currentEffect = NULL;
    }

    if ( noEffect )
        return;

    echoSet.numEchoes = numEchoes;
    echoSet.feedback = (int) (65536.0 * feedback);
    echoSet.gain = (int) (65536.0 * gain);
    mechoes = new MIDASecho[numEchoes];
    echoSet.echoes = mechoes;

    for ( n = 0; n < numEchoes; n++ )
    {
        mechoes[n].delay = (unsigned) (65536.0 * echoes[n].GetDelay());
        mechoes[n].gain = (int) (65536.0 * echoes[n].GetGain());
        mechoes[n].reverseChannels = echoes[n].GetReverse();
        mechoes[n].filterType = echoes[n].GetFilterType();
    }

    if  ( (currentEcho = MIDASaddEchoEffect(&echoSet)) == NULL )
        MIDASerror();

    delete [] mechoes;

    currentEffect = this;
}



void midpEchoEffect::Deactivate()
{
    if ( currentEcho != NULL )
    {
        if ( !MIDASremoveEchoEffect(currentEcho) )
            MIDASerror();
        currentEcho = NULL;
    }
    currentEffect = NULL;
}



midpEchoEffectList::midpEchoEffectList()
{
    Registry reg;
    char str[256 + ECHONAMELEN];
    unsigned numEffects;
    unsigned i, n;
    midpEchoEffect *effect;
    midpEcho *echo;
    DefaultEffect *defeff;
    DefaultEcho *defecho;
    
    noEffect = new midpEchoEffect(1);
    
    AddItem((midpListItem*) noEffect);

    if ( reg.KeyExists(effectKey) )
    {
        reg.OpenKey(effectKey);
        numEffects = reg.GetNumSubKeys();

        for ( i = 0; i < numEffects; i++ )
        {
            reg.GetSubKeyName(i, str, ECHONAMELEN+1);
            effect = NewEffect();
            effect->ReadFromRegistry(str);           
        }
        reg.CloseKey();
    }

    for ( i = 0; i < NUMDEFAULTEFFECTS; i++ )
    {
        defeff = &defaultEffects[i];
        sprintf(str, "%s\\%s", effectKey, defeff->name);
        if ( !reg.KeyExists(str) )
        {
            effect = NewEffect();
            effect->Rename(defeff->name);
            effect->SetFeedback(defeff->feedback);
            effect->SetGain(defeff->gain);
            effect->SetNumEchoes(defeff->numEchoes);

            for ( n = 0; n < defeff->numEchoes; n++ )
            {
                echo = effect->GetEcho(n);
                defecho = &defeff->echoes[n];
                echo->SetDelay(defecho->delay);
                echo->SetGain(defecho->gain);
                echo->SetReverse(defecho->reverse);
                echo->SetFilterType(defecho->filterType);
            }

            effect->SaveToRegistry();
        }
    }    
}


midpEchoEffectList::~midpEchoEffectList()
{
    midpEchoEffect *effect;

    while ( (effect = (midpEchoEffect*) GetFirst()) != NULL )
    {
        RemoveItem((midpListItem*) effect);
        delete effect;
    }
}


midpEchoEffect *midpEchoEffectList::FirstEffect()
{
    return (midpEchoEffect*) GetFirst();
}


midpEchoEffect *midpEchoEffectList::NextEffect()
{
    return (midpEchoEffect*) GetNext();
}


midpEchoEffect *midpEchoEffectList::NewEffect()
{
    midpEchoEffect *effect = new midpEchoEffect;

    effect->Rename("New");
    AddItemEnd((midpListItem*) effect);

    return effect;
}


void midpEchoEffectList::DeleteEffect(midpEchoEffect *effect)
{
    RemoveItem((midpListItem*) effect);
    effect->DeleteFromRegistry();
    delete effect;
}


/*
 * $Log: midpecho.cpp,v $
 * Revision 1.3  1997/07/12 11:58:11  pekangas
 * Added default effects plus clipping to echo values
 *
 * Revision 1.2  1997/07/11 13:36:39  pekangas
 * Added filters and total gain to echoes.
 * Fixed crashes when output mode was changed when an echo effect was
 * active.
 *
 * Revision 1.1  1997/07/10 18:41:09  pekangas
 * Initial revision
 *
*/
