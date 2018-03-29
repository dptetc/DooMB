#ifndef __XMIDP_HH
#define __XMIDP_HH

#include <gtk/gtk.h>

#define MIDPVERSION 0.3.0
#define MIDPVERSTR "0.3.0 alpha"


/* Variables */
extern GtkMenuFactory  *factory;
extern GtkMenuFactory  *subfactories[4];
extern GHashTable *entry_ht;


/* Function prototypes */

void show_filesel(GtkWidget *w, gpointer p);
void close_module(GtkWidget *w, gpointer p);
void exit_midp(GtkWidget *w, gpointer p);
void next_pos(GtkWidget *w, gpointer p);
void prev_pos(GtkWidget *w, gpointer p);
void center_chans(GtkWidget *w, gpointer p);
void list_insts(GtkWidget *w, gpointer p);
void change_filter(GtkWidget *w, gpointer p);
void show_echo_dialog(GtkWidget *w, gpointer p);
void show_about(GtkWidget *w, gpointer p);

void menus_create(void);
void menus_load(char *filename);
void menus_save(char *filename);

#endif
