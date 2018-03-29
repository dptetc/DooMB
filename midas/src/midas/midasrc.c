/*      midasrc.c
 *
 * MIDAS Digital Audio System .midasrc-file configuration functions
 *
 * $Id: midasrc.c,v 1.5 1997/07/31 10:56:49 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/

#include <stdlib.h>
#include <string.h>
#include "midas.h"
#include "midasdll.h"
#include "midasrc.h"

RCSID(const char *midasrc_rcsid = "$Id: midasrc.c,v 1.5 1997/07/31 10:56:49 pekangas Exp $";);
#define NUMSECTIONS 3
#define MAXTOKENS 8


static char     secOptions[] = "Options";
static char     secEffects[] = "Effects";
static char     secModules[] = "Modules";
static char     *token, *value;
static char     *tokens[MAXTOKENS];

section sections[NUMSECTIONS] = {
    { optSec, secOptions },
    { effSec, secEffects },
    { modSec, secModules } };


static FILE             *f = NULL;
static int              fileOpened;
static char             *line = NULL;
static effect           *effects = NULL;
static effect           *leffect = NULL;
static option           *loption = NULL;
static optlist          *optlists = NULL;
static optlist          *loptlist = NULL;


static void splitLine(char *line)
{
    char *t;
    
    value = strstr(line, "=" );
    if ( value )
    {
        *value = '\0';
        value++;
        while ( *value == ' ' ) value++;
    }
    token = line;
    while ((*token == ' ') || (*token == '\t' ) ) token++;    
    t = token;
    while (( *t != ' ') && (*t != '\0' )) t++;
    *t = '\0';
    if ( value )
    {
        t = value + strlen(value);
	while (( *t == ' ') || (*t == '\0' ) || (*t == '\n')) t--;
        *(t+1) = '\0';
    }
}


static int checkToken(char *tok, char *curr)
{
    return !strncasecmp(tok, curr, strlen(curr) );
}


static effect *addEffect(effect** eff)
{
    if ( *eff )
        eff = &(leffect->next);
    return leffect = *eff = malloc(sizeof(effect));
}

static optlist *addOptList(optlist** opt)
{
    if ( *opt )
        opt = &(loptlist->next);
    return loptlist = *opt = malloc(sizeof(optlist));
}

static option *addOption(option** opt)
{
    if ( *opt )
        opt = &(loption->next);
    return loption = *opt = malloc(sizeof(option));
}

static void tokenizeLine(char *line)
{
    int         i;
    char        *t;

    t = line;
    for ( i = 0; i < MAXTOKENS; i++ )
    {
        if ( *t == '\0' ) return;
        
        while (( *t == ' ') || (*t == '\t' )) t++;
        if ( *t != '"' )
        {
            tokens[i] = t;
            while ( (*t != ' ') && (*t != '\0' ) && (*t != '\n' ) ) t++;
            if ( *t == '\0' )
                return;
            *t++ = '\0';
        }
        else
        {
            t++;
            tokens[i] = t;
            while ( (*t != '"') && (*t != '\0' ) && ( *t != '\n' ) ) t++;
            if ( *t == '\0' )
                return;
            *t++ = '\0';
        }
    }        
}




/****************************************************************************\
*
* Function:     void LoadError(void)
*
* Description:  Stops loading the file, deallocates everything and closes
*               the file.
*
\***************************************************************************/

#define condFree(x) { if ( x != NULL ) if ( memFree(x) != OK) return; }

static void LoadError(void)
{
    /* Close file if opened. Do not process errors. */
    if ( fileOpened )
        fclose(f);
}




/****************************************************************************\
*
* Function:     int mrcReadFile(char *fileName)
*
* Description:  Reads the file specified and parses it as a MIDAS RC file
*
* Input:        char *fileName          RC file name
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int mrcReadFile(char *fileName)
{
    unsigned    i, state;
    effect      *eff;
    option      *opt;
    optlist     *optl = NULL;
    int         currEcho = 0, numEchoes = 0;
    MIDASechoSet *echoset;
    MIDASecho   *echoes = NULL;
    
    /* Open the file: */
    if ( ( f = fopen(fileName, "rt")) == NULL )
        return errFileNotFound;

    fileOpened = TRUE;

    if ( ( line = malloc(256)) == NULL )
    {
        LoadError();
        return errOutOfMemory;
    }


    state = noSec;    /* no section yet */
    
    while ( !feof(f) )
    {
        fgets(line, 256, f);

        switch ( line[0] )
        {
            case ';':
            case '\n':
            case '\r':
                /* Comment or empty line, skip */
                break;

            case '[':
                /* New section begins */
                for ( i = 0; i < NUMSECTIONS; i++ )
                {
                    if ( !(strncasecmp(&line[1], sections[i].id, strlen(sections[i].id))))
                    {
                        state = sections[i].token;
                        break;
                    }                        
                }
                break;

            case ' ':
            case '\t':
                for ( i = strlen(line)-1; i; i-- )
                {
                    if (( line[i] != ' ') || (line[i] != '\t') ||
                        (line[i] != '\n') || (line != '\0') )
                        break;
                }
                if ( !i )
                    break;
                /* there was something on the line, fall-through */

            default:
                switch ( state )
                {
                    case noSec:
                        /* no section yet */
                        break;

                    case optSec:                        
                        /* Options section */
                        splitLine(line);
                        if ( checkToken(token, "mixrate" ) )
                            MIDASsetOption(MIDAS_OPTION_MIXRATE, atoi(value));

                        if ( checkToken(token, "stereo" ) )
                        {
                            if (checkToken(value, "on" ) )
                                MIDASsetOption(MIDAS_OPTION_OUTPUTMODE, MIDAS_MODE_STEREO);
                            else
                                MIDASsetOption(MIDAS_OPTION_OUTPUTMODE, MIDAS_MODE_MONO);
                        }                                

                        if ( checkToken(token, "bits" ) )
                        {
                            if (checkToken(value, "16" ) )
                                MIDASsetOption(MIDAS_OPTION_OUTPUTMODE, MIDAS_MODE_16BIT);
                            else
                                MIDASsetOption(MIDAS_OPTION_OUTPUTMODE, MIDAS_MODE_8BIT);
                        }                                

                        if ( checkToken(token, "interpolation" ) )
                        {
                            if (checkToken(value, "on" ) )
                                MIDASsetOption(MIDAS_OPTION_MIXING_MODE, TRUE);
                            else
                                MIDASsetOption(MIDAS_OPTION_MIXING_MODE, FALSE);
                        }                                

                        if ( checkToken(token, "separation" ) )
                                MIDASsetOption(MIDAS_OPTION_DEFAULT_STEREO_SEPARATION, atoi(value));

                        if ( checkToken(token, "loop" ) )
                            mLoopModule = checkToken(value, "on" );
                        break;
                        
                    case effSec:
                        /* Effects section */
                        if (line[0] == '{' )
                        {
                            state = echoDef;
                            break;
                        }
                        if (!(strncasecmp(line, "echo", 4)))
                        {
                            tokenizeLine(line);
                            eff = addEffect(&effects);
                            currEcho = 0;
                            numEchoes = atoi(tokens[4]);
                            eff->id = malloc(strlen(tokens[1]) + 1);
                            strcpy(eff->id, tokens[1]);
                            eff->echoset = echoset = (MIDASechoSet*) malloc(sizeof(MIDASechoSet) + numEchoes * sizeof(MIDASecho));
                            echoset->numEchoes = numEchoes;
                            echoset->feedback = 65536L * atof(tokens[2]);
                            echoset->gain = 65536L * atof(tokens[3]);
                            /* This is a nasty one :) */
                            echoset->echoes = echoes = (MIDASecho*)echoset + 1;
                        }
                        break;

                    case modSec:
                        /* Modules section */
                        if (line[0] == '{' )
                        {
                            state = modDef;
                            break;
                        }
                        tokenizeLine(line);
                        optl = addOptList(&optlists);
                        optl->id = malloc(strlen(tokens[0]) + 1);
                        strcpy(optl->id, tokens[0]);
                        optl->options = NULL;
                        loption = NULL;
                        break;

                    case echoDef:
                        /* Echo definition */
                        if (line[0] == '}' )
                        {
                            state = effSec;
                            break;
                        }
                        if ( currEcho < numEchoes )
                        {
                            tokenizeLine(line);
                            echoes[currEcho].delay = 65536L * atof(tokens[0]);
                            echoes[currEcho].gain = 65536L * atof(tokens[1]);
                            echoes[currEcho].reverseChannels = atoi(tokens[2]);
                            echoes[currEcho].filterType = atoi(tokens[3]);
                            currEcho++;
                        }                        
                        break;

                    case modDef:
                        /* Module definition */
                        if (line[0] == '}' )
                        {
                            state = modSec;
                            break;
                        }
                        splitLine(line);
                        opt = addOption(&(optl->options));
                        if ( checkToken(token, "mixrate" ) )
                        {
                            opt->opt = optMixRate;
                            opt->val.value = atoi(value);
                        }
                        if ( checkToken(token, "stereo" ) )
                        {
                            opt->opt = optStereo;
                            opt->val.value = checkToken(value, "on" );
                        }                                
                        if ( checkToken(token, "bits" ) )
                        {
                            opt->opt = optBits;
                            opt->val.value = checkToken(value, "16" );
                        }                                

                        if ( checkToken(token, "interpolation" ) )
                        {
                            opt->opt = optInterpolation;
                            opt->val.value = checkToken(value, "on" );
                        }                                

                        if ( checkToken(token, "separation" ) )
                        {
                            opt->opt = optSeparation;
                            opt->val.value = atoi(value);
                        }
                        
                        if ( checkToken(token, "loop" ) )
                        {
                            opt->opt = optLoop;
                            opt->val.value = checkToken(value, "on" );
                        }
                        if ( checkToken(token, "echo" ) )
                        {
                            opt->opt = optEcho;
                            opt->val.str = malloc(strlen(value) + 1);
                            strcpy(opt->val.str, value);
                        }
                        break;
                }                
        }
    }
    
    free(line);
    fclose(f);
   
    return OK;
}



/****************************************************************************\
*
* Function:     int mrcFindEcho(char *name)
*
* Description:  Matches the name with the echoset, created from the RC file
*
* Input:        char *name          echo name
*
* Returns:      MIDASechoSet pointer or NULL if not found.
*
\****************************************************************************/

MIDASechoSet *mrcFindEcho(char *name)
{
    effect      *t;


    t = effects;
    if ( !t )
        return NULL;
    
    do
    {
        if ( !(strncasecmp(name, t->id, strlen(name) ) ) )
            return t->echoset;

    } while ( (t = t->next) );
    return NULL;
}


/****************************************************************************\
*
* Function:     int mrcMatchFilename(char *filename)
*
* Description:  Matches the filename with the options created from the RC file
*               and implements them.
*
* Input:        char *name          filename
*
* Returns:      MIDASechoSet pointer or NULL if not found.
*
\****************************************************************************/

MIDASechoSet *mrcMatchFilename(char *name)
{
    optlist      *t;
    option       *t2;
    MIDASechoSet *echoset = NULL;

    t = optlists;
    if (!t)
        return NULL;
    
    do
    {
        if ( strstr(name, t->id ) )
        {
            t2 = t->options;
            if (!t2)
                break;
            do
            {
                switch (t2->opt)
                {
                    case optMixRate:
                        MIDASsetOption(MIDAS_OPTION_MIXRATE, t2->val.value);
                        break;

                    case optStereo:
                        if ( t2->val.value )
                            MIDASsetOption(MIDAS_OPTION_OUTPUTMODE, MIDAS_MODE_STEREO);
                        else
                            MIDASsetOption(MIDAS_OPTION_OUTPUTMODE, MIDAS_MODE_MONO);
                        break;

                    case optBits:
                        if ( t2->val.value )
                            MIDASsetOption(MIDAS_OPTION_OUTPUTMODE, MIDAS_MODE_16BIT);
                        else
                            MIDASsetOption(MIDAS_OPTION_OUTPUTMODE, MIDAS_MODE_8BIT);
                        break;

                    case optInterpolation:
                        if ( t2->val.value )
                            MIDASsetOption(MIDAS_OPTION_MIXING_MODE, TRUE);
                        else
                            MIDASsetOption(MIDAS_OPTION_MIXING_MODE, FALSE);
                        break;
                        
                    case optSeparation:
                        MIDASsetOption(MIDAS_OPTION_DEFAULT_STEREO_SEPARATION, t2->val.value);
                        break;

                    case optLoop:
                        mLoopModule = t2->val.value;
                        break;

                    case optEcho:
                        echoset = mrcFindEcho(t2->val.str);
                        break;
                                                    
                    default:
                        break;
                }
            } while( (t2 = t2->next ) );
        }
    } while ( (t = t->next) );
    return echoset;
}



/*
 * $Log: midasrc.c,v $
 * Revision 1.5  1997/07/31 10:56:49  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.4  1997/07/25 15:38:57  jpaana
 * Changed MIDAS_OPTION_OVERSAMPLING to MIDAS_OPTION_MIXING_MODE
 *
 * Revision 1.3  1997/07/17 09:54:11  pekangas
 * Removed a about half a thousand empty lines from the end
 *
 * Revision 1.2  1997/07/15 18:32:31  jpaana
 * Fixed some warnings
 *
 * Revision 1.1  1997/07/15 18:23:13  jpaana
 * Initial revision
 *
 *
*/
