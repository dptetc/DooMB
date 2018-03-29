#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "midasdll.h"
#include "midasrc.h"
#include "xmidp.hh"


MIDASmodule             *module = NULL;         /* current playing module */
MIDASmoduleInfo         info;                   /* current module playing info */
MIDASinstrumentInfo     inst;
MIDASplayStatus         status;
MIDASechoSet            *echoset = NULL;
MIDASechoSet            *echoset2;
MIDASmodulePlayHandle   handle;
char                    *fileName = NULL;

int                     mInitialized = FALSE;
int                     masterVolume = 64;
int                     currFilter = 0;

const char *xmidp_rcsid = "$Id: xmidp.cc,v 1.3 1997/08/04 01:20:22 jpaana Exp $";

char            *title =
"\n\rMIDAS Module Player for Linux/X, version " MIDPVERSTR "\n\r"
"Copyright 1996-1997 Jarno Paananen";


/* Magic configuration variables from MIDAS: */
extern int mLoopModule;



/* GUI stuff */


GtkWidget       *mainwindow;
GtkWidget	*filesel = NULL;
GtkWidget       *exitb;
GtkWidget       *loadb;
GtkWidget       *vbox;
GtkWidget       *menubar;



void close_module(GtkWidget *w, gpointer data)
{
    (void)w;
    (void)data;
    if ( module )
    {
        MIDASstopBackgroundPlay();
        MIDASstopModule(handle);
        MIDASfreeModule(module);
        handle = 0;
        module = NULL;
    }
}


/* Exit midp */

void exit_midp(GtkWidget *w, gpointer data)
{
    (void)w;
    (void)data;
    char *rcPath;

    close_module(NULL, NULL);

    if ( mInitialized )
        MIDASclose();

    rcPath = (char*)malloc(strlen(getenv("HOME")) + 14);
    strcpy(rcPath, getenv("HOME"));
    strcat(rcPath, "/.xmidpmenurc");
    menus_save(rcPath);
    free(rcPath);
    
    gtk_main_quit();
}


static void loadAndStartPlaying(char *fileName)
{
    int                mode, rate;
        
    if ( !mInitialized )
    {
        if ( !MIDASinit() )
        {
            printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
            exit(1);
        }

        mInitialized = TRUE;
    }

    close_module(NULL, NULL);
    
    module = (MIDASmodule*)MIDASloadModule(fileName);
    if ( module == NULL )
    {
        printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
        exit(1);
        
    }
    if( !MIDASgetModuleInfo(module, &info))
    {
	printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
	exit(1);
    }
    printf("Module: %s\n\r", info.songName);
    printf("Length: %x, patterns: %x, instruments: %x, channels: %i\n\r", info.songLength, 
	   info.numPatterns, info.numInstruments, info.numChannels);

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

    puts("\n\r");

    if ( echoset )
        MIDASaddEchoEffect(echoset);
	

    /* Start playing thread */
    if( !MIDASstartBackgroundPlay(50))
    {
	printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
	exit(1);
    }
}

static void ok_filesel(void)
{
    fileName = gtk_file_selection_get_filename(GTK_FILE_SELECTION(filesel));
    loadAndStartPlaying(fileName);
    gtk_widget_hide(filesel);
}

static void cancel_filesel(void)
{
    gtk_widget_hide(filesel);
}

void show_filesel(GtkWidget *w, gpointer data)
{
    (void)w;
    (void)data;
    if ( filesel && GTK_WIDGET_VISIBLE(filesel) )
        return;

    if ( !filesel )
    {
        filesel = gtk_file_selection_new("Load module");
        gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button),
                            "clicked", (GtkSignalFunc) ok_filesel, NULL);
        gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filesel)->cancel_button),
                            "clicked", (GtkSignalFunc) cancel_filesel, NULL);
    }
    gtk_widget_show(filesel);
}



void next_pos(GtkWidget *w, gpointer data)
{
    (void)w;
    (void)data;
    if ( module )
    {
        MIDASgetPlayStatus(handle, &status);
        MIDASsetPosition(handle, status.position + 1 );
    }
}

void prev_pos(GtkWidget *w, gpointer data)
{
    (void)w;
    (void)data;

    if ( module )
    {
        MIDASgetPlayStatus(handle, &status);
        MIDASsetPosition(handle, status.position - 1 );
    }
}


void center_chans(GtkWidget *w, gpointer data)
{
    unsigned i;
    (void)w;
    (void)data;

    if ( module )
    {
        for ( i = 0; i < info.numChannels; i++ )
        {
            /* Kluge: */
            MIDASsetSamplePanning((MIDASsamplePlayHandle) i + 1,
                MIDAS_PAN_MIDDLE);
        }
    }
}

void change_filter(GtkWidget *w, gpointer data)
{
    (void)w;
    (void)data;
    if ( module )
    {
        if ( ++currFilter > 2 ) currFilter = 0;
	printf("Using filter mode %i\n", currFilter);
        MIDASsetOption(MIDAS_OPTION_FILTER_MODE, currFilter);
    }
}


void list_insts(GtkWidget *w, gpointer data)
{
    unsigned i, k;
    char               *name;

    (void)w;
    (void)data;
    
    if ( !module )
      return;

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
}




int main(int argc, char *argv[])
{    
    unsigned           i, j;
    char               *rcPath;
    int                mode;

    
    gtk_init(&argc, &argv);
    	
    puts(title);

    
    if ( !MIDASstartup() )
    {
	printf("%s\n\r", MIDASgetErrorMessage(MIDASgetLastError()));
	exit(1);
    }

    rcPath = (char*)malloc(strlen(getenv("HOME")) + 10);
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
                            case  '8':
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
		    MIDASsetOption(MIDAS_OPTION_MIXING_MODE, TRUE);
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


    if ( fileName )
        loadAndStartPlaying(fileName);
    
    
    /* Do the main window */
    mainwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(mainwindow), "MIDP/Linux");
    gtk_window_set_policy(GTK_WINDOW(mainwindow), FALSE, FALSE, TRUE);
    gtk_signal_connect(GTK_OBJECT(mainwindow), "destroy",
                       GTK_SIGNAL_FUNC(exit_midp), NULL);
    gtk_container_border_width(GTK_CONTAINER(mainwindow), 0);

    
    /* Do the vbox  */
    
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(mainwindow), vbox);
    gtk_widget_show(vbox);        
    
    /* Do the menus */

    factory = gtk_menu_factory_new (GTK_MENU_FACTORY_MENU_BAR);
    subfactories[0] = gtk_menu_factory_new (GTK_MENU_FACTORY_MENU_BAR);
    gtk_menu_factory_add_subfactory (factory, subfactories[0], "<Main>");
    rcPath = (char*)malloc(strlen(getenv("HOME")) + 14);
    strcpy(rcPath, getenv("HOME"));
    strcat(rcPath, "/.xmidpmenurc");
    menus_load(rcPath);
    free(rcPath);
    menus_create();
    menubar = subfactories[0]->widget;

    
    gtk_window_add_accelerator_table(GTK_WINDOW(mainwindow),
				     subfactories[0]->table);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, TRUE,0);
    gtk_widget_show(menubar);
    
    /* Do the buttons */
/*    
    exitb = gtk_button_new_with_label("Exit");
    gtk_signal_connect(GTK_OBJECT(exitb), "clicked",
		       GTK_SIGNAL_FUNC(exit_midp), NULL);
    gtk_container_add(GTK_CONTAINER(vbox), exitb);
    gtk_widget_show(exitb);

    
    loadb = gtk_button_new_with_label("Load");
    gtk_signal_connect(GTK_OBJECT(loadb), "clicked",
		       GTK_SIGNAL_FUNC(show_filesel), NULL);
    gtk_container_add(GTK_CONTAINER(vbox), loadb);
    gtk_widget_show(loadb);
    */
    
    gtk_widget_show(mainwindow);

    gtk_main();

    return 0;
}
