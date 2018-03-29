#include <string.h>
#include "xmidp.hh"

const char *xmidpmenu_rcsid = "$Id: xmidpmenu.cc,v 1.2 1997/08/04 01:24:10 jpaana Exp $";


/* Menu stuff */

GtkMenuFactory  *factory = NULL;
GtkMenuFactory  *subfactories[4];
GHashTable *entry_ht = NULL;

/* Menu keyboard accelerator table definition */
typedef struct menuaccel
{
    struct menuaccel    *next;
    char                *path;
    char                *accel;
}menuaccel;

static menuaccel        *menuitems = NULL;
static menuaccel        *lmenuitem;




/* Main menu items */

static GtkMenuEntry    menu_items[] =
{
    { "<Main>/File/Open...", "<alt>O", show_filesel, NULL },
    { "<Main>/File/Close", "<alt>W", close_module, NULL },
    { "<Main>/File/<separator>", NULL, NULL, NULL },
    { "<Main>/File/Exit", "<alt>X", exit_midp, NULL },
    { "<Main>/Playing/Next Position", "<control>.", next_pos, NULL },
    { "<Main>/Playing/Prev. Position", "<control>,", prev_pos, NULL },
    { "<Main>/Playing/Center Channels", "<control>C", center_chans, NULL },
    { "<Main>/Playing/List Instruments", "<control>L", list_insts, NULL },
    { "<Main>/Playing/Change Filter", "<control>F", change_filter, NULL },
    { "<Main>/Options/Preferences...", "<control>P", NULL, NULL },
    { "<Main>/Options/Sound Options...", "<control>O", NULL, NULL },
    { "<Main>/Views/Song Information...", "<control>S", NULL, NULL },
    { "<Main>/Views/Instrument List...", "<control>I", NULL, NULL },
    { "<Main>/Views/Echo Effects...", "<control>E", show_echo_dialog, NULL },
    { "<Main>/Help/About...", "<alt>A", show_about, NULL }
};

static int nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);



/* Here comes the menu code */


static gint menus_install_accel(GtkWidget *widget, gchar *signal_name,
				gchar key, gchar modifiers, gchar *path)
{
    char accel[64];
    char *t1, t2[2];
    (void)widget;
    (void)signal_name;
    
    accel[0] = '\0';
    if (modifiers & GDK_CONTROL_MASK)
        strcat (accel, "<control>");
    if (modifiers & GDK_SHIFT_MASK)
        strcat (accel, "<shift>");
    if (modifiers & GDK_MOD1_MASK)
        strcat (accel, "<alt>");

    t2[0] = key;
    t2[1] = '\0';
    strcat (accel, t2);

    if (entry_ht)
    {
        t1 = (char*)g_hash_table_lookup (entry_ht, path);
        g_free (t1);
    }
    else
        entry_ht = g_hash_table_new (g_string_hash, g_string_equal);

    g_hash_table_insert (entry_ht, path, g_strdup (accel));

    return TRUE;
}

static void menus_remove_accel (GtkWidget *widget, gchar *signal_name,
                                gchar *path)
{
    char *t;
    (void)widget;
    (void)signal_name;
    
    if (entry_ht)
    {
        t = (char*)g_hash_table_lookup (entry_ht, path);
        g_free (t);

        g_hash_table_insert (entry_ht, path, g_strdup (""));
    }
}



void menus_create(void)
{
    char 		*accelerator;
    int 		i;
    GtkMenuEntry 	*entries;
    int			nmenu_entries;

    entries = menu_items;
    nmenu_entries  = nmenu_items;
    
    if (entry_ht)
    {
        for (i = 0; i < nmenu_entries; i++)
        {
            accelerator = (char*)g_hash_table_lookup (entry_ht, entries[i].path);
            if (accelerator)
            {
                if (accelerator[0] == '\0')
                    entries[i].accelerator = NULL;
                else
                    entries[i].accelerator = accelerator;
            }
        }
    }
    gtk_menu_factory_add_entries (factory, entries, nmenu_entries);

    for (i = 0; i < nmenu_entries; i++)
    {
        if (entries[i].widget)
        {
            gtk_signal_connect (GTK_OBJECT (entries[i].widget),
                                "install_accelerator",
                                (GtkSignalFunc) menus_install_accel,
                                entries[i].path);
            gtk_signal_connect (GTK_OBJECT (entries[i].widget),
                                "remove_accelerator",
                                (GtkSignalFunc) menus_remove_accel,
                                entries[i].path);
        }
    }
}

static void menus_add_path(char *path, char *accelerator)
{
    if (!entry_ht)
        entry_ht = g_hash_table_new (g_string_hash, g_string_equal);

    g_hash_table_insert (entry_ht, path, accelerator);
}

void menus_load(char *filename)
{
    FILE        *fp;
    char        item[80];
    char        *path;
    char        *accel;
    char        *t;
    menuaccel   **tuu;
    
    if ( !(fp = fopen(filename, "r")) )
        return;
    while ( !feof(fp) )
    {
        item[0] = '\0';
        fgets(item, 80, fp);
        if ( item[0] != '(' )
            return;
        path = item;
        while ( (*path != '"') && (*path != '\n') && (*path != '\0' ) ) path++;
        path++;
        accel = path;
        while ( (*accel != '"') && (*accel != '\n') && (*accel != '\0' ) ) accel++;
        *accel = '\0';
        accel++;
        while ( (*accel != '"') && (*accel != '\n') && (*accel != '\0' ) ) accel++;
        accel++;
        t = accel;
        while ( (*t != '"') && (*t != '\n') && (*t != '\0' ) ) t++;
        *t = '\0';

        if ( menuitems )
            tuu = &(lmenuitem->next);
        else
            tuu = &menuitems;
       
        *tuu = (menuaccel*)malloc(sizeof(menuaccel));
        lmenuitem = *tuu;
        (*tuu)->next = NULL;
        (*tuu)->path = (char*)malloc(strlen(path) + 1);
        strcpy((*tuu)->path, path);
        (*tuu)->accel = (char*)malloc(strlen(accel) + 1);
        strcpy((*tuu)->accel, accel);
        
        menus_add_path((*tuu)->path, (*tuu)->accel);
    }
}

static void menus_foreach(gpointer key, gpointer value, gpointer user_data)
{
    fprintf ((FILE*) user_data, "(menu-path \"%s\" \"%s\")\n", (char*) key,
           (char*) value);
}

void menus_save(char *filename)
{
    FILE        *fp;

    if (!entry_ht)
        return;

    if ( !(fp = fopen(filename, "w")) )
        return;

    g_hash_table_foreach (entry_ht, menus_foreach, fp);

    fclose (fp);
}
