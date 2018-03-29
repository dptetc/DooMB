#include "xmidp.hh"

const char *xmidpabout_rcsid = "$Id: xmidpabout.cc,v 1.2 1997/08/04 01:24:09 jpaana Exp $";


/* About dialog */

static GtkWidget       	*aboutwindow = NULL;
static GtkWidget       	*aboutvbox;
static GtkWidget       	*aboutsep;
static GtkWidget	*aboutok;

#define ABOUTTEXTROWS 5

static GtkWidget	*aboutlabel[ABOUTTEXTROWS];

static char		*texts[ABOUTTEXTROWS] = {
    "MIDP/Linux/X",
    "Version " MIDPVERSTR,
    __DATE__ " " __TIME__,
    "Copyright 1996-1997",
    "Jarno Paananen" };
    

static void hide_about(void)
{
    gtk_widget_hide(aboutwindow);
}

void show_about(GtkWidget *w, gpointer data)
{
    (void)w;
    (void)data;

    int	i;
    
    if ( aboutwindow && GTK_WIDGET_VISIBLE(aboutwindow) )
        return;

    if ( !aboutwindow )
    {
        aboutwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_container_border_width(GTK_CONTAINER(aboutwindow), 10);
        gtk_window_set_title(GTK_WINDOW(aboutwindow), "About MIDP/Linux");
        gtk_signal_connect(GTK_OBJECT(aboutwindow), "destroy",
                       GTK_SIGNAL_FUNC(hide_about), NULL);
	
	aboutvbox = gtk_vbox_new(FALSE, 5);
	gtk_widget_show(aboutvbox);
	gtk_container_add(GTK_CONTAINER(aboutwindow), aboutvbox);

        aboutok = gtk_button_new_with_label("OK");
        gtk_signal_connect(GTK_OBJECT(aboutok), "clicked",
		       GTK_SIGNAL_FUNC(hide_about), NULL);
        GTK_WIDGET_SET_FLAGS(aboutok, GTK_CAN_DEFAULT);
        gtk_box_pack_end(GTK_BOX(aboutvbox), aboutok, FALSE, FALSE, 0);
        gtk_widget_grab_default(aboutok);
        gtk_widget_show(aboutok);

	aboutsep = gtk_hseparator_new();
	gtk_widget_show(aboutsep);
	gtk_box_pack_end(GTK_BOX(aboutvbox), aboutsep, TRUE, TRUE, 0);
	
	for ( i = 0 ; i < ABOUTTEXTROWS; i++ )
	{
	    aboutlabel[i] = gtk_label_new(texts[i]);	    
	    gtk_widget_show(aboutlabel[i]);
	    gtk_box_pack_start(GTK_BOX(aboutvbox), aboutlabel[i], FALSE,
                           FALSE, 0);
	}

#if 0
        abouttab = gtk_table_new(1, 1, FALSE);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(aboutwindow)->vbox), abouttab, TRUE,
                           TRUE, 0);
        gtk_widget_show(abouttab);
        
        abouttext = gtk_text_new(NULL, NULL);
        gtk_table_attach_defaults (GTK_TABLE (abouttab), abouttext, 0, 1, 0, 1);
        gtk_widget_show(abouttext);
        gtk_text_freeze(GTK_TEXT(abouttext));
        gtk_widget_realize(abouttext);
        gtk_text_insert(GTK_TEXT(abouttext), NULL, &abouttext->style->black, NULL,
                        "MIDP/Linux\n" \
                        "Version " MIDPVERSTR "\n" \
                        __DATE__ " " __TIME__ "\n" \
                        "Copyright 1996-1997 Jarno Paananen\n", -1);
        gtk_text_thaw(GTK_TEXT(abouttext));
        gtk_text_set_editable(GTK_TEXT(abouttext), FALSE);

#endif
    }
    gtk_widget_show(aboutwindow);
}
