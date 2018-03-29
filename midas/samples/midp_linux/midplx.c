#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "midasdll.h"
#include "midasrc.h"

#include <curses.h>

//#define NO_THREADS

#define MIDPVERSION 0.2.0
#define MIDPVERSTR "0.2.0"

MIDASmodule         *module = NULL;        /* current playing module */
MIDASmoduleInfo     info;                  /* current module playing info */
MIDASinstrumentInfo inst;
MIDASplayStatus     status;
MIDASechoSet        *echoset = NULL;
MIDASechoSet        *echoset2;
const char *midplx_rcsid = "$Id: midplx.c,v 1.15 1997/08/02 00:58:20 jpaana Exp $";

char            *title =
"\n\rMIDAS Module Player for Linux, version " MIDPVERSTR "\n\r"
"Copyright 1996-1997 Jarno Paananen";


char            *usage =
"Usage:\tMIDP\t[options] <filename> [options]\n\r"\
"Options:\n\r"\
"\t-?, -h\t\tThis information\n\r"\
"\t-mx\t\tMixing rate x Hz\n\r"\
"\t-ox\t\tOutput mode (8=8-bit, 1=16-bit, s=stereo, m=mono\n\r"\
"\t-fx\t\tUse filter (0 = none, 1=less, 2=more)\n\r"\
"\t-l \t\tDon't loop module\n\r"\
"\t-i \t\tUse oversampling (interpolation)\n\r"\
"\t-px\t\tSet default pan separation (0-64)\n\r"\
"\t-e <name>\tUse echo effect\n\n\r"\
"Keys while playing:\n\r"\
"\tESC\tExit\n\r"\
"\t+/-\tMaster volume control\n\r"\
"\t./,\tJump forward/backward in the song\n\r"\
"\n\r";


static int end;

/* Magic configuration variables from MIDAS: */
extern int mLoopModule;


int main(int argc, char *argv[])
{
    unsigned           i, j, k;
    char               *fileName = NULL;
    char               *name;
    int                key = 0, oldRow = 0;
    int                mode, rate;
    int                masterVolume = 64;
    char               *rcPath;
    MIDASmodulePlayHandle handle;

    
    puts(title);
    printf("\rUsing MIDAS Digital Audio System version %s\n\n\r",MIDASgetVersionString());
    
    if ( argc < 2 )
    {
        puts(usage);
        exit(EXIT_SUCCESS);
    }

    if ( !MIDASstartup() )
    {
	printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
	exit(1);
    }

    rcPath = malloc(strlen(getenv("HOME")) + 10);
    strcpy(rcPath, getenv("HOME"));
    strcat(rcPath, "/.midasrc");    
    mrcReadFile(rcPath);
    free(rcPath);
    
    for ( i = 1 ; i < (unsigned)argc; i++ )
    {
        if ( argv[i][0] == '-' )
	{
	    switch ( argv[i][1] )
	    {
	        case 'm':
                    MIDASsetOption(MIDAS_OPTION_MIXRATE, atoi(&argv[i][2]));
		    break;

	        case 'o':
		    mode = MIDAS_MODE_16BIT_STEREO;
		    for ( j = 2; j < 4; j++ )			
		    {
		        switch ( argv[i][j] )
			{
			    case '1':
				mode &= ~MIDAS_MODE_8BIT;
				mode |= MIDAS_MODE_16BIT;
			        break;
			    case '8':
				mode &= ~MIDAS_MODE_16BIT;
				mode |= MIDAS_MODE_8BIT;
			        break;
			    case 's':
			        break;
			    case 'm':
				mode &= ~MIDAS_MODE_STEREO;
				mode |= MIDAS_MODE_MONO;
			        break;
                        }			
		    }
		    MIDASsetOption(MIDAS_OPTION_OUTPUTMODE, mode);
		    break;

                case 'l':
                    mLoopModule = FALSE;
                    break;

	        case 'i':
		    MIDASsetOption(MIDAS_OPTION_MIXING_MODE,
                                   MIDAS_MIX_HIGH_QUALITY);
		    break;
		    
                case 'f':
                    MIDASsetOption(MIDAS_OPTION_FILTER_MODE, atoi(&argv[i][2]));
                    break;

                case 'p':
                    MIDASsetOption(MIDAS_OPTION_DEFAULT_STEREO_SEPARATION, atoi(&argv[i][2]));
                    break;
                    
                case 'e':
                    if ( !(echoset = mrcFindEcho(argv[++i])) )
                    {
                        puts("No such echo defined!\n\r");
                        return 1;
                    }
                    break;
                    
                default:
		    fileName = argv[i];  // :)
                    echoset2 = mrcMatchFilename(fileName);
                    if ( echoset2 )
                        echoset = echoset2;
                    break;
	    }
	}
	else
        {
	    fileName = argv[i];
            echoset2 = mrcMatchFilename(fileName);
	    if ( echoset2 )
	        echoset = echoset2;
        }
    }

    if ( fileName == NULL )
    {
        puts("No filename given!\n\r");
	return 1;
    }

    if ( !MIDASinit() )
    {
	printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
	exit(1);
    }

    module = MIDASloadModule(fileName);
    if ( module == NULL )
    {
	    printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
	    exit(1);
    }

/* Initialize ncurses (clears the screen */
    if ( !initscr() )
        exit(1);

    atexit((void*)endwin);
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();

    getch();

    puts(title);
    printf("\rUsing MIDAS Digital Audio System version %s\n\n\r",MIDASgetVersionString());

    if( !MIDASgetModuleInfo(module, &info))
    {
	printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
	exit(1);
    }
    printf("Module: %s\n\r", info.songName);
    printf("Length: %x, patterns: %x, instruments: %x, channels: %i\n\r", info.songLength, 
	   info.numPatterns, info.numInstruments, info.numChannels);

    k = ( info.numInstruments + 1) / 2;

    for ( i = 0; i < k; i++ )
    {
        if(!MIDASgetInstrumentInfo(module, i, &inst))
        {
            printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
            exit(1);
        }

	name = inst.instName;
	while ( *name != 0 )
	{
	    if ( *name < 32 )
	        *name = 32;
	    name++;
	}

        printf( "\n\r%02x: %-35s", i + 1, inst.instName );
        if ( i + k < info.numInstruments )
        {
	    MIDASgetInstrumentInfo(module, i+k, &inst);
	    name = inst.instName;
	    while ( *name != 0 )
	    {
	        if ( *name < 32 )
	            *name = 32;
		name++;
	    }
            printf( "%02x: %s", i + k + 1, inst.instName );
        }
    }

    printf("\n\n\r");
    handle = MIDASplayModule(module, mLoopModule);
    if ( handle == 0 )
    {
	printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
	exit(1);
    }

    rate = MIDASgetOption(MIDAS_OPTION_MIXRATE);
    printf("Playing at %d Hz", rate);
    
    mode = MIDASgetOption(MIDAS_OPTION_OUTPUTMODE);
    if ( mode & MIDAS_MODE_8BIT )
        printf(" 8-bit");
    else
        printf(" 16-bit");

    if ( mode & MIDAS_MODE_MONO )
        printf(" mono");
    else
        printf(" stereo");
        
    end = 0;

    puts("\n\r");

    if ( echoset )
        MIDASaddEchoEffect(echoset);
	

#ifndef NO_THREADS
    /* Start playing thread */
    if( !MIDASstartBackgroundPlay(50))
    {
	printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
	exit(1);
    }
#endif

    fflush(stdout);

    /* The Main Loop */
    while( !end )
    {
        MIDASgetPlayStatus(handle, &status);

	if ( status.row != (unsigned)oldRow )
	{
	    oldRow = status.row;
	    printf("Position: %02x Pattern: %02x Row: %02x\r", status.position,
                   status.pattern, status.row);

	    fflush(stdout);
	}

	if ( (status.songLoopCount != 0) && (mLoopModule == FALSE ) ) 
	    end = 1;

	key = getch();

        switch ( key )
        {
            case 27:
                end = 1;
                break;

            case '-':
                if ( masterVolume > 0 )
                    masterVolume--;
                MIDASsetMusicVolume(handle, masterVolume);
                break;

            case '+':
                if ( masterVolume < 64 )
                    masterVolume++;
                MIDASsetMusicVolume(handle, masterVolume);
                break;

             case '.':
	        MIDASsetPosition(handle, status.position + 1 );
		break;

	    case ',':
	        MIDASsetPosition(handle, status.position - 1 );
		break;
        }
#ifdef NO_THREADS
	MIDASpoll();
#else
	usleep(50*1000);
#endif
    }

    /* Wait for the playing thread to return */
#ifndef NO_THREADS
    MIDASstopBackgroundPlay();
#endif

    /* And off we go... */
    printf("\n\r");
    MIDASstopModule(handle);
    MIDASfreeModule(module);

    MIDASclose();
    return 0;
}
