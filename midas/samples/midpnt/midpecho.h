/*      midpecho.h
 *
 * MidpNT echo effect manager
 *
 * $Id: midpecho.h,v 1.3 1997/07/12 11:58:11 pekangas Exp $
 *
 * Copyright 1997 Petteri Kangaslampi
*/

#ifndef __midpecho_h
#define __midpecho_h


#define MAXECHOES 8
#define ECHONAMELEN 32



class midpEcho
{
private:
    double delay, gain;
    int reverseChannels;
    unsigned filterType;
public:
    midpEcho();
    ~midpEcho() {}
    void SetDelay(double newDelay);
    double GetDelay() { return delay; }
    void SetGain(double newGain);
    double GetGain() { return gain; }
    void SetReverse(int newReverse) { reverseChannels = newReverse; }
    int GetReverse() { return reverseChannels; }
    void SetFilterType(unsigned newFilter) { filterType = newFilter; }
    unsigned GetFilterType() { return filterType; }
};



class midpEchoEffect : public midpListItem
{
private:
    double feedback;
    double gain;
    unsigned numEchoes;
    midpEcho echoes[MAXECHOES];
    int inRegistry;
    int noEffect;
    static MIDASechoHandle currentEcho;
    char name[ECHONAMELEN + 1];
    char registryName[ECHONAMELEN + 1];
public:
    midpEchoEffect(int noEffect = 0);
    ~midpEchoEffect();
    midpEcho *GetEcho(int num) { return &echoes[num]; }
    void SetNumEchoes(unsigned num);
    unsigned GetNumEchoes() { return numEchoes; }
    void ReadFromRegistry(char *name);
    void SaveToRegistry();
    void DeleteFromRegistry();
    void Rename(char *name);
    void Activate();
    void Deactivate();
    char *GetName() { return name; }
    void SetFeedback(double newFeedback);
    double GetFeedback() { return feedback; }
    void SetGain(double gain);
    double GetGain() { return gain; }
};



class midpEchoEffectList : public midpList
{
private:
    midpEchoEffect *noEffect;
public:
    midpEchoEffectList();
    ~midpEchoEffectList();
    midpEchoEffect *FirstEffect();
    midpEchoEffect *NextEffect();
    midpEchoEffect *NewEffect();
    void DeleteEffect(midpEchoEffect *effect);
};



extern midpEchoEffect *currentEffect;


#endif

/*
 * $Log: midpecho.h,v $
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
