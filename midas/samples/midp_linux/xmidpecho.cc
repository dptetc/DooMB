#include "xmidp.hh"

const char *xmidpecho_rcsid = "$Id: xmidpecho.cc,v 1.2 1997/08/04 01:24:09 jpaana Exp $";


/* Default echoes */



/* Default echo structure types */
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

#define NUMDEFAULTEFFECTS 5

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
    { "None", 0.0, 1.0, 0, NULL }, 
    { "Hall Reverb?", 0.75, 0.85, 5, defaultEchoes1 },
    { "PC Speaker", 0.6, 1.0, 1, defaultEchoes2 },
    { "Stereo", 0.0, 0.8, 1, defaultEchoes3 },
    { "Superkaiku", 0.8, 1.0, 1, defaultEchoes4 }
};






/* Echo dialog stuff */

GtkWidget       *echowindow = NULL;
GtkWidget	*echohbox;
GtkWidget	*echovbox1;
GtkWidget	*echovbox2;

/* Left side */
GtkWidget	*echoframe1;
GtkWidget	*echoframe2;
GtkWidget	*echotable1;
GtkWidget	*echotable2;

typedef	struct EchoMenuItem
{
    struct EchoMenuItem *next;
    GtkWidget		*item;
}EchoMenuItem;


/* Top frame */
GtkWidget	*echonamelabel;
GtkWidget	*echoname;
GtkWidget	*echonamem;
EchoMenuItem	*echonamei;
EchoMenuItem	*echonamel = NULL;

GtkWidget	*echonumlabel;
GtkWidget	*echonum;
GtkWidget	*echofeedlabel;
GtkWidget	*echofeed;
GtkWidget	*echotgainlabel;
GtkWidget	*echotgain;

/* Lower frame */
GtkWidget	*echodelaylabel;
GtkWidget	*echogainlabel;
GtkWidget	*echoreverselabel;
GtkWidget	*echofilterlabel;
GtkWidget	*echodelay[8];
GtkWidget	*echogain[8];
GtkWidget	*echoreverse[8];
GtkWidget	*echofilter[8][2];


char		*echofilternames[3] = {
                "None",
                "Low pass",
                "High pass" };

GtkWidget	*echofilters[8][3];


/* right side buttons */
GtkWidget	*echoclose;
GtkWidget	*echoapply;
GtkWidget	*echonew;
GtkWidget	*echodelete;
GtkWidget	*echosave;
GtkWidget	*echorename;
GtkWidget	*echosep;
GtkTooltips	*echoclosetip;



#define TEXT_WIDTH 35

static void hide_echo(void)
{
    gtk_widget_hide(echowindow);
}

static void apply_echo(void)
{
}

static void new_echo(void)
{
}

static void save_echo(void)
{
}

static void delete_echo(void)
{
}

static void rename_echo(void)
{
}


static void echoname_selected(GtkWidget *w, DefaultEffect *effect)
{
    char	buf[80];
    unsigned	i;

    (void)w;
    
    sprintf(buf, "%i", effect->numEchoes);
    gtk_entry_set_text(GTK_ENTRY(echonum), buf);

    sprintf(buf, "%f", effect->feedback);
    gtk_entry_set_text(GTK_ENTRY(echofeed), buf);

    sprintf(buf, "%f", effect->gain);
    gtk_entry_set_text(GTK_ENTRY(echotgain), buf);

    for ( i = 0; i < 8; i++ )
    {
	if ( i < effect->numEchoes )
	{
	    sprintf(buf, "%f", effect->echoes[i].delay);
	    gtk_entry_set_text(GTK_ENTRY(echodelay[i]), buf);
	    sprintf(buf, "%f", effect->echoes[i].gain);
	    gtk_entry_set_text(GTK_ENTRY(echogain[i]), buf);
	    gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(echoreverse[i]), effect->echoes[i].reverse);
	    gtk_option_menu_set_history(GTK_OPTION_MENU(echofilter[i][0]), effect->echoes[i].filterType);
	}
	else
	{
	    gtk_entry_set_text(GTK_ENTRY(echodelay[i]), "----");
	    gtk_entry_set_text(GTK_ENTRY(echogain[i]), "----");
	    gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(echoreverse[i]), 0);
	    gtk_option_menu_set_history(GTK_OPTION_MENU(echofilter[i][0]), 0);
	}
    }
}


void show_echo_dialog(GtkWidget *w, gpointer data)
{
    int i, j;

    (void)w;
    (void)data;
    
    if ( echowindow && GTK_WIDGET_VISIBLE(echowindow) )
        return;

    if ( !echowindow )
    {
        echowindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(echowindow), "Echo Effect Editor");
	gtk_window_set_policy(GTK_WINDOW(echowindow), FALSE, FALSE, TRUE);
        gtk_container_border_width(GTK_CONTAINER(echowindow), 5);
        gtk_signal_connect(GTK_OBJECT(echowindow), "destroy",
                       GTK_SIGNAL_FUNC(hide_echo), NULL);
	
	echohbox = gtk_hbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(echowindow), echohbox);
	gtk_widget_show(echohbox);

	echovbox1 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(echohbox), echovbox1, FALSE, FALSE, 2);
	gtk_widget_show(echovbox1);

	echosep = gtk_vseparator_new();
	gtk_box_pack_start(GTK_BOX(echohbox), echosep, TRUE, TRUE, 2);
	gtk_widget_show(echosep);
	
	echovbox2 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(echohbox), echovbox2, FALSE, FALSE, 2);
	gtk_widget_show(echovbox2);


/* Right side buttons */

#define DOBUTTON(name, tlabel, func) \
	name = gtk_button_new_with_label(tlabel);\
	gtk_box_pack_start(GTK_BOX(echovbox2), name, FALSE, FALSE, 3);\
	gtk_widget_show(name);\
	gtk_signal_connect(GTK_OBJECT(name), "clicked",\
			   GTK_SIGNAL_FUNC(func), NULL);


	DOBUTTON(echoclose, "Close", hide_echo)
	DOBUTTON(echoapply, "Apply", apply_echo)
	DOBUTTON(echonew, "New", new_echo)
	DOBUTTON(echodelete, "Delete", delete_echo)
	DOBUTTON(echosave, "Save", save_echo)
	DOBUTTON(echorename, "Rename", rename_echo)

/* ObTooltip ;) */	    
	echoclosetip = gtk_tooltips_new();
	gtk_tooltips_set_tips(echoclosetip, echoclose, "Closes the dialog");

/* Left side */

/* Top frame */
	
	echoframe1 = gtk_frame_new("Effect Information");
	gtk_box_pack_start(GTK_BOX(echovbox1), echoframe1, TRUE, TRUE, 2);
        gtk_container_border_width(GTK_CONTAINER(echoframe1), 5);
	gtk_widget_show(echoframe1);

	echotable1 = gtk_table_new( 4, 2, 0);
        gtk_container_border_width(GTK_CONTAINER(echotable1), 10);
	gtk_table_set_row_spacings(GTK_TABLE(echotable1), 2);
	gtk_table_set_col_spacings(GTK_TABLE(echotable1), 10);
	gtk_container_add(GTK_CONTAINER(echoframe1), echotable1);
	gtk_widget_show(echotable1);
/*
  Effect name combo
  Echoes number box
  Feedback number box
  Gain number box

 */
#define DOLABEL(name, tlabel, x1, x2, y1, y2) \
	name = gtk_label_new(tlabel);\
	gtk_misc_set_alignment(GTK_MISC(name), 1.0, 0.5);\
	gtk_widget_show(name);\
	gtk_table_attach_defaults(GTK_TABLE(echotable1), name,\
				  x1,x2,y1,y2);

	DOLABEL(echonamelabel, "Effect name", 0,1,0,1)
	DOLABEL(echonumlabel, "Number of echoes", 0,1,1,2)
	DOLABEL(echofeedlabel, "Feedback", 0,1,2,3)
	DOLABEL(echotgainlabel, "Total gain", 0,1,3,4)

	echoname = gtk_option_menu_new();
	gtk_widget_show(echoname);
	gtk_table_attach_defaults(GTK_TABLE(echotable1), echoname,
				  1,2,0,1);

	echonamem = gtk_menu_new();
	gtk_widget_show(echonamem);

	for ( i = 0; i < NUMDEFAULTEFFECTS; i++ )
	{
	    if ( echonamel == NULL )
	    {
		echonamei = (EchoMenuItem*)malloc(sizeof(EchoMenuItem));
		echonamel = echonamei;
	    }
	    else
	    {
		echonamel->next = (EchoMenuItem*)malloc(sizeof(EchoMenuItem));
		echonamel = echonamel->next;
	    }
	    echonamel->next = NULL;
	    echonamel->item = gtk_menu_item_new_with_label(defaultEffects[i].name);
	    gtk_signal_connect(GTK_OBJECT(echonamel->item), "activate",
			       GTK_SIGNAL_FUNC(echoname_selected),
			       (gpointer)&defaultEffects[i]);
	    gtk_widget_show(echonamel->item);
	    gtk_menu_append(GTK_MENU(echonamem), echonamel->item);
	}
	gtk_option_menu_set_menu(GTK_OPTION_MENU(echoname), echonamem);

	
	echonum = gtk_entry_new();
	gtk_widget_set_usize(echonum, TEXT_WIDTH, 25);
	gtk_widget_show(echonum);
	gtk_table_attach_defaults(GTK_TABLE(echotable1), echonum,
				  1,2,1,2);
	
	echofeed = gtk_entry_new();
	gtk_widget_set_usize(echofeed, TEXT_WIDTH, 25);
	gtk_widget_show(echofeed);
	gtk_table_attach_defaults(GTK_TABLE(echotable1), echofeed,
				  1,2,2,3);
	
	echotgain = gtk_entry_new();
	gtk_widget_set_usize(echotgain, TEXT_WIDTH, 25);
	gtk_widget_show(echotgain);
	gtk_table_attach_defaults(GTK_TABLE(echotable1), echotgain,
				  1,2,3,4);
	

/* Lower frame */

	echoframe2 = gtk_frame_new("Echoes");
	gtk_box_pack_start(GTK_BOX(echovbox1), echoframe2, TRUE, TRUE, 2);
        gtk_container_border_width(GTK_CONTAINER(echoframe2), 5);
	gtk_widget_show(echoframe2);

	echotable2 = gtk_table_new( 9, 4, 0);
        gtk_container_border_width(GTK_CONTAINER(echotable2), 10);
	gtk_table_set_row_spacings(GTK_TABLE(echotable2), 2);
	gtk_table_set_col_spacings(GTK_TABLE(echotable2), 10);
	gtk_container_add(GTK_CONTAINER(echoframe2), echotable2);
	gtk_widget_show(echotable2);
	
/* Delay	gain	Reverse		filter
   number	number	check		combo
   x8
 */
#define DOLABEL2(name, tlabel, x1, x2, y1, y2) \
	name = gtk_label_new(tlabel);\
	gtk_widget_show(name);\
	gtk_table_attach_defaults(GTK_TABLE(echotable2), name,\
				  x1,x2,y1,y2);

	DOLABEL2(echodelaylabel, "Delay (ms)", 0,1,0,1)
	DOLABEL2(echogainlabel, "Gain", 1,2,0,1)
	DOLABEL2(echoreverselabel, "Reverse", 2,3,0,1)
	DOLABEL2(echofilterlabel, "Filter", 3,4,0,1)
	    
	for ( i = 0; i < 8; i++ )
	{
	    echodelay[i] = gtk_entry_new();
	    gtk_widget_set_usize(echodelay[i], TEXT_WIDTH, 25);
	    gtk_widget_show(echodelay[i]);
	    gtk_table_attach_defaults(GTK_TABLE(echotable2), echodelay[i],
				  0,1,i+1,i+2);

	    echogain[i] = gtk_entry_new();
	    gtk_widget_set_usize(echogain[i], TEXT_WIDTH, 25);
	    gtk_widget_show(echogain[i]);
	    gtk_table_attach_defaults(GTK_TABLE(echotable2), echogain[i],
				  1,2,i+1,i+2);

	    echoreverse[i] = gtk_check_button_new();
	    gtk_widget_show(echoreverse[i]);
	    gtk_table_attach(GTK_TABLE(echotable2), echoreverse[i],
			     2,3,i+1,i+2, GTK_EXPAND|GTK_SHRINK,
			     GTK_EXPAND|GTK_SHRINK, 0, 0);

	    echofilter[i][0] = gtk_option_menu_new();
	    gtk_widget_show(echofilter[i][0]);
	    gtk_table_attach_defaults(GTK_TABLE(echotable2), echofilter[i][0],
				  3,4,i+1,i+2);

	    echofilter[i][1] = gtk_menu_new();
	    gtk_widget_show(echofilter[i][1]);

	    for ( j = 0; j < 3; j++ )
	    {
		echofilters[i][j] = gtk_menu_item_new_with_label(echofilternames[j]);
		gtk_widget_show(echofilters[i][j]);
		gtk_menu_append(GTK_MENU(echofilter[i][1]), echofilters[i][j]);
	    }
	    gtk_option_menu_set_menu(GTK_OPTION_MENU(echofilter[i][0]),
				     echofilter[i][1]);
	}
	echoname_selected(NULL, &defaultEffects[0]);
    }
    gtk_widget_show(echowindow);
}
