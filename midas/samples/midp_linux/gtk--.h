/**********************************************************
 * Copyright (C) 1997 Elliot Lee <sopwith@redhat.com      *
 * Licensed under the LGPL.                               *
 * Please send corrections and improvements to the me     *
 *                                                        *
 * New Callback stuff written by Phil Dawes (1997)        *
 *  using ideas from code by Tero Pulkkinen.              *
 *                                                        *
 **********************************************************/

#ifndef GTK_MINUS_MINUS_H
#define GTK_MINUS_MINUS_H 1
extern "C" {
#include <gtk/gtk.h>
}

class Gtk_AcceleratorTable;
class Gtk_Adjustment;
class Gtk_Alignment;
class Gtk_Arrow;
class Gtk_AspectFrame;
class Gtk_Bin;
class Gtk_Box;
class Gtk_Button;
class Gtk_CheckButton;
class Gtk_CheckMenuItem;
class Gtk_ColorSelection;
class Gtk_ColorSelectionDialog;
class Gtk_Container;
class Gtk_Curve;
class Gtk_Data;
class Gtk_Dialog;
class Gtk_DrawingArea;
class Gtk_Entry;
class Gtk_FileSelection;
class Gtk_Frame;
class Gtk_HBox;
class Gtk_HRuler;
class Gtk_HScale;
class Gtk_HScrollbar;
class Gtk_HSeparator;
class Gtk_Image;
class Gtk_Item;
class Gtk_Label;
class Gtk_List;
class Gtk_ListItem;
class Gtk_Main;
class Gtk_Menu;
class Gtk_MenuBar;
class Gtk_MenuFactory;
class Gtk_MenuItem;
class Gtk_MenuPath;
class Gtk_MenuShell;
class Gtk_Misc;
class Gtk_Notebook;
class Gtk_Object;
class Gtk_OptionMenu;
class Gtk_Pixmap;
class Gtk_Preview;
class Gtk_ProgressBar;
class Gtk_RadioButton;
class Gtk_RadioMenuItem;
class Gtk_Range;
class Gtk_Ruler;
class Gtk_Scale;
class Gtk_Scrollbar;
class Gtk_ScrolledWindow;
class Gtk_Separator;
class Gtk_Table;
class Gtk_Text;
class Gtk_ToggleButton;
class Gtk_Tooltips;
class Gtk_Tree;
class Gtk_TreeItem;
class Gtk_VBox;
class Gtk_VRuler;
class Gtk_VScale;
class Gtk_Viewport;
class Gtk_Widget;
class Gtk_Window;


/***************************************************
 * Phil's Callback stuff  (Part 1)                 *
 ***************************************************

 See the bottom of this file for part2
 
 */

/** Allows universal access to all slots
  */
class SlotInterface
{
};

/** A simple linked list to contain slots.
  I'd use the STL list, but at the moment this results in code bloat
  for many compilers, and many C++ programmers won't want that.
  */
class SlotList
{
	class SlotNode	{
	  private:
		SlotInterface* slot;
		SlotNode* next;
	  public:
		SlotNode(SlotInterface* slot,SlotNode* next=0)
			:slot(slot),next(next) {}
		/// recursively deletes the whole list
		~SlotNode() { delete next; delete slot; }
	};

  private:
	SlotNode* list;
  public:
	SlotList():list(0){}
	void insert(SlotInterface* item){new SlotNode(item,list);}
	~SlotList() {delete list;}
};

/***********************************************************/


/* Most of the classes in here are children of this one */
class Gtk_Object {
	
	// Added by Phil for the new signal handling scheme 
  private:	
	SlotList slots; 
  public:
	void insert_slot(SlotInterface* item) {slots.insert(item);}

	// original Gtk_Object code:
	
    Gtk_Object(guint type, ...);
    Gtk_Object(GtkObject *castitem);
    Gtk_Object(const Gtk_Object &obj);
    virtual ~Gtk_Object(void);
    virtual void set_user_data(gpointer data);
    virtual gpointer get_user_data(void);

    virtual gint signal_connect(gchar *name, GtkSignalFunc callback_func,
				gpointer cbdata);
	virtual void signal_emit(gint signal_type, ...);
    virtual void signal_emit_by_name(gchar *name, ...);
    virtual void signal_disconnect(gint id);
    static guint8 isGtkObject(Gtk_Object *checkcast);
    friend guint8 operator ==(Gtk_Object& a, Gtk_Object& b);
    friend guint8 operator !=(Gtk_Object& a, Gtk_Object& b);
  protected:
    void ref(void);
    void unref(void);
    GtkObject *gtkobject;
    Gtk_Object(void) { }

    /* Why all these friend class declarations?  Because C++ is too
       anal to realize that static member functions of child classes
       should be allowed the same access as regular methods are.  I
       guess it's a "I hate your children even if you're my friend
       thing".  Go figure */
    friend class Gtk_AcceleratorTable;
    friend class Gtk_Adjustment;
    friend class Gtk_Alignment;
    friend class Gtk_Arrow;
    friend class Gtk_AspectFrame;
    friend class Gtk_Bin;
    friend class Gtk_Box;
    friend class Gtk_Button;
    friend class Gtk_CheckButton;
    friend class Gtk_CheckMenuItem;
    friend class Gtk_ColorSelection;
    friend class Gtk_ColorSelectionDialog;
    friend class Gtk_Container;
    friend class Gtk_Curve;
    friend class Gtk_Data;
    friend class Gtk_Dialog;
    friend class Gtk_DrawingArea;
    friend class Gtk_Entry;
    friend class Gtk_FileSelection;
    friend class Gtk_Frame;
    friend class Gtk_HBox;
    friend class Gtk_HRuler;
    friend class Gtk_HScale;
    friend class Gtk_HScrollbar;
    friend class Gtk_HSeparator;
    friend class Gtk_Image;
    friend class Gtk_Item;
    friend class Gtk_Label;
    friend class Gtk_List;
    friend class Gtk_ListItem;
    friend class Gtk_Menu;
    friend class Gtk_MenuBar;
    friend class Gtk_MenuFactory;
    friend class Gtk_MenuItem;
    friend class Gtk_MenuPath;
    friend class Gtk_MenuShell;
    friend class Gtk_Misc;
    friend class Gtk_Notebook;
    friend class Gtk_OptionMenu;
    friend class Gtk_Pixmap;
    friend class Gtk_Preview;
    friend class Gtk_ProgressBar;
    friend class Gtk_RadioButton;
    friend class Gtk_RadioMenuItem;
    friend class Gtk_Range;
    friend class Gtk_Ruler;
    friend class Gtk_Scale;
    friend class Gtk_Scrollbar;
    friend class Gtk_ScrolledWindow;
    friend class Gtk_Separator;
    friend class Gtk_Table;
    friend class Gtk_Text;
    friend class Gtk_ToggleButton;
    friend class Gtk_Tree;
    friend class Gtk_TreeItem;
    friend class Gtk_VBox;
    friend class Gtk_VRuler;
    friend class Gtk_VScale;
    friend class Gtk_VScrollbar;
    friend class Gtk_VSeparator;
    friend class Gtk_Viewport;
    friend class Gtk_Widget;
    friend class Gtk_Window;
};

class Gtk_AcceleratorTable {
  public:
    friend class Gtk_Object;
    friend class Gtk_Widget;
    friend class Gtk_Menu;
    friend class Gtk_Window;
    Gtk_AcceleratorTable(void);
    Gtk_AcceleratorTable(GtkAcceleratorTable *castitem);
    virtual ~Gtk_AcceleratorTable(void);
    Gtk_AcceleratorTable(Gtk_Object *object, gchar *signal_name,
			 gchar accelerator_key,
			 guint8 accelerator_mods);
    virtual void install(Gtk_Object *object, gchar *signal_name,
			 gchar accelerator_key, guint8 accelerator_mods);
    virtual void remove(Gtk_Object *object, gchar *signal_name);
    virtual gint check(gchar accelerator_key, guint8 accelerator_mods);
  protected:
    void ref(void);
    void unref(void);
    GtkAcceleratorTable *gtkacceleratortable;
};

class Gtk_Widget : public Gtk_Object {
  public:
    friend class Gtk_Main;
    friend class Gtk_Tooltips;
    Gtk_Widget(GtkWidget *castitem);
    virtual void unparent(void);
    virtual void reparent(Gtk_Widget *newparent);
    virtual void activate(void);
    virtual void popup(gint x, gint y);
    virtual void grab_focus(void);
    virtual void grab_default(void);
    virtual void set_uposition(gint x, gint y);
    virtual void set_usize(gint width, gint height);
    virtual void set_name(gchar *name);
    virtual gchar *get_name(void);
    virtual void set_sensitive(gint sensitive);
    virtual void set_parent(Gtk_Widget *parent);
    virtual void show(void);
    virtual void hide(void);
    virtual void map(void);
    virtual void unmap(void);
    virtual void realize(void);
    virtual void unrealize(void);
    virtual void draw_focus(void);
    virtual void draw_default(void);
    virtual void draw_children(void);
    virtual void size_request(GtkRequisition *requisition);
    virtual void size_allocate(GtkAllocation *allocation);
    virtual void install_accelerator(Gtk_AcceleratorTable *gtkacceleratortable,
				     gchar *signal_name,
				     gchar key,
				     guint8 modifiers);
    virtual void remove_accelerator(Gtk_AcceleratorTable *gtkacceleratortable,
				    gchar *signal_name);
    static guint8 isGtkWidget(Gtk_Object *checkcast);
    virtual ~Gtk_Widget(void);
  protected:
    Gtk_Widget(void) {}
};

class Gtk_Misc : public Gtk_Widget {
  public:
    Gtk_Misc(GtkMisc *castitem);
    static guint8 isGtkMisc(Gtk_Object *checkcast);
    virtual void set_alignment(gfloat xalign, gfloat yalign);
    virtual void set_padding(gint xpad, gint ypad);
  protected:
    Gtk_Misc(void) {}
};

class Gtk_Data : public Gtk_Object {
  public:
    Gtk_Data(GtkData *castitem);
    static guint8 isGtkData(Gtk_Object *checkcast);
  protected:
    Gtk_Data(void) {}
};

class Gtk_Container : public Gtk_Widget {
  public:
    Gtk_Container(GtkContainer *castitem);
    static guint8 isGtkContainer(Gtk_Object *checkcast);
    virtual void add(Gtk_Widget *widget);
    virtual void remove(Gtk_Widget *widget);
    virtual void disable_resize(void);
    virtual void enable_resize(void);
    virtual void block_resize(void);
    virtual void unblock_resize(void);
    virtual void need_resize(Gtk_Widget *widget);
    virtual void check_resize(Gtk_Widget *widget);
    virtual void foreach(GtkCallback callback,
			 gpointer callback_data);
    virtual void focus(GtkDirectionType direction);
    virtual GList *children(void);
  protected:
    Gtk_Container(void) {}
};

class Gtk_Bin : public Gtk_Container {
  public:
    Gtk_Bin(GtkBin *castitem);
    static guint8 isGtkBin(Gtk_Object *checkcast);
  protected:
    Gtk_Bin(void) {}
};

class Gtk_Window : public Gtk_Bin {
  public:
    Gtk_Window(GtkWindowType type);
    Gtk_Window(GtkWindow *castitem);
    static guint8 isGtkWindow(Gtk_Object *checkcast);
    virtual void set_title(gchar *title);
    virtual void set_focus(Gtk_Widget *focus);
    virtual void set_default(Gtk_Widget *defaultw);
    virtual void set_policy(gint allow_shrink,
			    gint allow_grow,
			    gint auto_shrink);
    virtual void add_accelerator_table(Gtk_AcceleratorTable *table);
    virtual void remove_accelerator_table(Gtk_AcceleratorTable *table);
    virtual void position(GtkWindowPosition position);
};

class Gtk_Frame : public Gtk_Bin {
  public:
    Gtk_Frame(gchar *label);
    Gtk_Frame(GtkFrame *castitem);
    static guint8 isGtkFrame(Gtk_Object *checkcast);
    virtual void set_label(gchar *label);
    virtual void set_label_align(gfloat xalign,
				 gfloat yalign);
    virtual void set_shadow_type(GtkShadowType type);
  protected:
    Gtk_Frame(void){}
};

/* gtkadjustment.h */
class Gtk_Adjustment : public Gtk_Data {
  public:
    friend class Gtk_Range;
    friend class Gtk_HScrollbar;
    friend class Gtk_VScrollbar;
    Gtk_Adjustment(gfloat value, gfloat lower, gfloat upper,
		   gfloat step_increment, gfloat page_increment,
		   gfloat page_size);
    Gtk_Adjustment(GtkAdjustment *castitem);
    static guint8 isGtkAdjustment(Gtk_Object *obj);
};

/* gtkalignment.h */
class Gtk_Alignment : public Gtk_Bin {
  public:
    Gtk_Alignment(gfloat xalign, gfloat yalign, gfloat xscale,
		  gfloat yscale);
    Gtk_Alignment(GtkAlignment *castitem);
    virtual void set(gfloat xalign, gfloat yalign, gfloat xscale,
		     gfloat yscale);
};

class Gtk_Arrow : public Gtk_Misc {
  public:
    Gtk_Arrow(GtkArrowType arrow_type,
	      GtkShadowType shadow_type);
    Gtk_Arrow(GtkArrow *castitem);
    virtual void set(GtkArrowType arrow_type,
		     GtkShadowType shadow_type);
    static guint8 isGtkArrow(Gtk_Object *checkcast);
};

class Gtk_AspectFrame : public Gtk_Frame {
  public:
    Gtk_AspectFrame(gchar *label,
		    gfloat xalign,
		    gfloat yalign,
		    gfloat ratio,
		    gint obey_child);
    Gtk_AspectFrame(GtkAspectFrame *castitem);
    virtual void set(gfloat xalign,
		     gfloat yalign,
		     gfloat ratio,
		     gint obey_child);
    static guint8 isGtkAspectFrame(Gtk_Object *checkcast);
};

class Gtk_Box : public Gtk_Container {
  public:
    Gtk_Box(GtkBox *castitem);
    static guint8 isGtkBox(Gtk_Object *checkcast);
    virtual void pack_start(Gtk_Widget *child,
			    gint expand,
			    gint fill,
			    gint padding);
    virtual void pack_end(Gtk_Widget *child,
			  gint expand,
			  gint fill,
			  gint padding);
    virtual void pack_start(Gtk_Widget *child);
    virtual void pack_end(Gtk_Widget *child);
    virtual void set_homogeneous(gint homogeneous);
    virtual void set_spacing(gint spacing);
  protected:
    Gtk_Box(void) {}
};

class Gtk_Button : public Gtk_Container {
  public:
    Gtk_Button(void);
    Gtk_Button(GtkButton *castitem);
    Gtk_Button(gchar *label);
    static guint8 isGtkButton(Gtk_Object *checkcast);
    virtual void pressed(void);
    virtual void released(void);
    virtual void enter(void);
    virtual void leave(void);
};

class Gtk_ToggleButton : public Gtk_Button {
  public:
    Gtk_ToggleButton(void);
    Gtk_ToggleButton(gchar *label);
    Gtk_ToggleButton(GtkToggleButton *castitem);
    static guint8 isGtkToggleButton(Gtk_Object *checkcast);
    virtual void set_mode(gint draw_indicator);
    virtual void set_state(gint state);
    virtual void toggled(void);
};

class Gtk_CheckButton : public Gtk_ToggleButton {
  public:
    Gtk_CheckButton(void);
    Gtk_CheckButton(gchar *label);
    Gtk_CheckButton(GtkCheckButton *castitem);
    static guint8 isGtkCheckButton(Gtk_Object *checkcast);
};

class Gtk_ColorSelection : public Gtk_Window {
  public:
    Gtk_ColorSelection(void);
    Gtk_ColorSelection(GtkColorSelection *castitem);
    static guint8 isGtkColorSelection(Gtk_Object *checkcast);
    virtual void set_update_policy(GtkUpdateType policy);
    virtual void set_opacity(gint use_opacity);
    virtual void set_color(gdouble *color);
    virtual void get_color(gdouble *color);
};

class Gtk_ColorSelectionDialog : public Gtk_Window {
  public:
    Gtk_ColorSelectionDialog(gchar *title);
    Gtk_ColorSelectionDialog(GtkColorSelectionDialog *castitem);
    static guint8 isGtkColorSelectionDialog(Gtk_Object *checkcast);
};

class Gtk_Dialog : public Gtk_Window {
  public:
    Gtk_Dialog(void);
    Gtk_Dialog(GtkDialog *castitem);
    static guint8 isGtkDialog(Gtk_Object *checkcast);
    virtual Gtk_VBox *vbox(void);
    virtual Gtk_HBox *action_area(void);
};

class Gtk_DrawingArea : public Gtk_Widget {
  public:
    Gtk_DrawingArea(void);
    Gtk_DrawingArea(GtkDrawingArea *castitem);
    static guint8 isGtkDrawingArea(Gtk_Object *checkcast);
    virtual void size(gint width, gint height);
};

class Gtk_Entry : public Gtk_Widget {
  public:
    Gtk_Entry(void);
    Gtk_Entry(GtkEntry *castitem);
    static guint8 isGtkEntry(Gtk_Object *checkcast);
    virtual void set_text(gchar *text);
    virtual gchar* get_text(void);
    virtual void append_text(gchar *text);
    virtual void prepend_text(gchar *text);
    virtual void set_position(gint position);
};

class Gtk_FileSelection : public Gtk_Window {
  public:
    Gtk_FileSelection(gchar *title);
    Gtk_FileSelection(GtkFileSelection *castitem);
    static guint8 isGtkFileSelection(Gtk_Object *checkcast);
    virtual void set_filename(gchar *filename);
    virtual gchar * get_filename(void);
};

class Gtk_HBox : public Gtk_Box {
  public:
    Gtk_HBox(gint homogeneous, gint spacing);
    Gtk_HBox(GtkHBox *castitem);
    static guint8 isGtkHBox(Gtk_Object *checkcast);
};


class Gtk_VBox : public Gtk_Box {
  public:
    Gtk_VBox(gint homogeneous, gint spacing);
    Gtk_VBox(GtkVBox *castitem);
    static guint8 isGtkVBox(Gtk_Object *checkcast);
  protected:
    Gtk_VBox(void) {}
};

class Gtk_Curve : public Gtk_VBox {
  public:
    Gtk_Curve(void);
    Gtk_Curve(GtkCurve *castitem);
    static guint8 isGtkCurve(Gtk_Object *checkcast);
    virtual void reset(void);
    virtual void set_range(gfloat min_x, gfloat max_x,
			   gfloat min_y, gfloat max_y);
    virtual void set_vector(int veclen, gfloat *vector);
};

class Gtk_Image : public Gtk_Misc {
  public:
    Gtk_Image(GdkImage *val, GdkBitmap *mask);
    Gtk_Image(GtkImage *castitem);
    static guint8 isGtkImage(Gtk_Object *checkcast);
    virtual void set(GdkImage *val, GdkBitmap *mask);
    virtual void get(GdkImage **val, GdkBitmap **mask);
};

class Gtk_Item : public Gtk_Bin {
  public:
    Gtk_Item(GtkItem *castitem);
    static guint8 isGtkItem(Gtk_Object *checkcast);
    virtual void select(void);
    virtual void deselect(void);
    virtual void toggle(void);
  protected:
    Gtk_Item(void) {}
};

class Gtk_Label : public Gtk_Misc {
  public:
    Gtk_Label(gchar *str);
    Gtk_Label(GtkLabel *castitem);
    static guint8 isGtkLabel(Gtk_Object *checkcast);
    virtual void set(gchar *str);
    virtual void get(gchar **str);
};

class Gtk_List : public Gtk_Container {
  public:
    Gtk_List(void);
    Gtk_List(GtkList *castitem);
    static guint8 isGtkList(Gtk_Object *checkcast);
    virtual void insert_items(GList *items, gint position);
    virtual void append_items(GList *items);
    virtual void prepend_items(GList *items);
    virtual void remove_items(GList *items);
    virtual void clear_items(gint start, gint end);
    virtual void select_item(gint item);
    virtual void unselect_item(gint item);
    virtual void select_child(Gtk_Widget *child);
    virtual void unselect_child(Gtk_Widget *child);
    virtual gint child_position(Gtk_Widget *child);
    virtual void set_selection_mode(GtkSelectionMode  mode);
}; 

class Gtk_ListItem : public Gtk_Item {
  public:
    Gtk_ListItem(void);
    Gtk_ListItem(gchar *label);
    Gtk_ListItem(GtkListItem *castitem);
    static guint8 isGtkListItem(Gtk_Object *checkcast);
    virtual void select(void);
    virtual void deselect(void);
};

class Gtk_Main {
  public:
    Gtk_Main(int *argc, char ***argv) { gtk_init(argc, argv); }
    virtual ~Gtk_Main(void) { gtk_exit(0); }
    virtual void run(void) { gtk_main(); }
    virtual void quit(void) { gtk_main_quit(); }
    virtual void iteration(void) { gtk_main_iteration(); }
    virtual void grab_add(Gtk_Widget *widget);
    virtual void grab_remove(Gtk_Widget *widget);
    virtual gint timeout_add(guint32 interval, GtkFunction function,
		     gpointer data);
    virtual void timeout_remove(gint tag);
    virtual gint idle_add(GtkFunction function, gpointer data);
    virtual void idle_remove(gint tag);
  protected:
    Gtk_Main(void) {}
};

class Gtk_MenuShell : public Gtk_Container {
  public:
    Gtk_MenuShell(GtkMenuShell *castitem);
    static guint8 isGtkMenuShell(Gtk_Object *checkcast);
    virtual void append(Gtk_MenuShell *child);
    virtual void prepend(Gtk_MenuShell *child);
    virtual void insert(Gtk_MenuShell *child, gint position);
    virtual void deactivate(void);
  protected:
    Gtk_MenuShell(void) {}
};

class Gtk_Menu : public Gtk_MenuShell {
  public:
    Gtk_Menu(void);
    Gtk_Menu(GtkMenu *castitem);
    static guint8 isGtkMenu(Gtk_Object *checkcast);
    virtual void append(Gtk_MenuItem *child);
    virtual void prepend(Gtk_MenuItem *child);
    virtual void insert(Gtk_MenuItem *child, gint position);
    virtual void popup(Gtk_MenuShell *parent_menu_shell,
		       Gtk_MenuItem *parent_menu_item,
		       GtkMenuPositionFunc func,
		       gpointer data,
		       gint button,
		       guint32 activate_time);
    virtual void popdown(void);
    virtual Gtk_Widget *get_active(void);
    virtual void set_active(gint index);
    virtual void set_accelerator_table(Gtk_AcceleratorTable *table);
};

class Gtk_MenuBar : public Gtk_MenuShell {
  public:
    Gtk_MenuBar(void);
    Gtk_MenuBar(GtkMenuBar *castitem);
    static guint8 isGtkMenuBar(Gtk_Object *checkcast);
    virtual void append(Gtk_MenuItem *child);
    virtual void prepend(Gtk_MenuItem *child);
    virtual void insert(Gtk_MenuItem *child, gint position);    
};

class Gtk_MenuPath {
  public:
    Gtk_MenuPath(GtkMenuPath *castitem);
    virtual ~Gtk_MenuPath(void);
    gchar *path;
    Gtk_Widget *widget;
};

class Gtk_MenuFactory {
  public:
    Gtk_MenuFactory(GtkMenuFactoryType type);
    Gtk_MenuFactory(GtkMenuFactory *castitem);
    virtual ~Gtk_MenuFactory(void);
    static guint8 isGtkMenuFactory(Gtk_Object *checkcast);
    virtual void add_entries(GtkMenuEntry *entries, int nentries); /* XXX fix */
    virtual void add_subfactory(Gtk_MenuFactory *subfactory, gchar *path);
    virtual void remove_paths(gchar **paths, int npaths);
    virtual void remove_entries(GtkMenuEntry *entries, int nentries); /* XXX fix */
    virtual void remove_subfactory(Gtk_MenuFactory *subfactory, gchar *path);
    virtual Gtk_MenuPath *factory_find(gchar *path);
  protected:
    GtkMenuFactory *gtkmenufactory;
};

class Gtk_MenuItem : public Gtk_Item {
  public:
    Gtk_MenuItem(void);
    Gtk_MenuItem(GtkMenuItem *castitem);
    Gtk_MenuItem(gchar *label);
    static guint8 isGtkMenuItem(Gtk_Object *checkcast);
    virtual void set_submenu(Gtk_Menu *submenu);
    virtual void set_placement(GtkSubmenuPlacement placement);
    virtual void accelerator_size(void);
    virtual void accelerator_text(gchar *buffer);
    virtual void configure(gint show_toggle_indicator,
			   gint show_submenu_indicator);
    virtual void select(void);
    virtual void deselect(void);
    virtual void activate(void);
};


class Gtk_CheckMenuItem : public Gtk_MenuItem {
  public:
    Gtk_CheckMenuItem(void);
    Gtk_CheckMenuItem(GtkCheckMenuItem *castitem);
    Gtk_CheckMenuItem(gchar *label);
    virtual void set_state(gint state);
    virtual void toggled(void);
    static guint8 isGtkCheckMenuItem(Gtk_Object *checkcast);
};

class Gtk_Notebook : public Gtk_Container {
  public:
    Gtk_Notebook(void);
    Gtk_Notebook(GtkNotebook *castitem);
    static guint8 isGtkNotebook(Gtk_Object *checkcast);
    virtual void append_page(Gtk_Widget *child, Gtk_Widget *tab_label);
    virtual void prepend_page(Gtk_Widget *child, Gtk_Widget *tab_label);
    virtual void insert_page(Gtk_Widget *child, Gtk_Widget *tab_label, gint position);
    virtual void remove_page(gint page_num);
    virtual gint current_page(void);
    virtual void set_page(gint page_num);
    virtual void next_page(void);
    virtual void prev_page(void);
    virtual void set_tab_pos(GtkPositionType pos);
    virtual void set_show_tabs(gint show_tabs);
    virtual void set_show_border(gint show_border);
};

class Gtk_OptionMenu : public Gtk_Button {
  public:
    Gtk_OptionMenu(void);
    Gtk_OptionMenu(GtkOptionMenu *castitem);
    static guint8 isGtkOptionMenu(Gtk_Object *checkcast);
    virtual Gtk_Menu *get_menu(void);
    virtual void set_menu(Gtk_Menu *menu);
    virtual void remove_menu(void);
    virtual void set_history(gint index);
};

class Gtk_Pixmap : public Gtk_Misc {
  public:
    Gtk_Pixmap(GdkPixmap *pixmap, GdkBitmap *mask);
    Gtk_Pixmap(GtkPixmap *castitem);
    static guint8 isGtkPixmap(Gtk_Object *checkcast);
    virtual void set(GdkPixmap *pixmap, GdkBitmap *mask);
    virtual void get(GdkPixmap **pixmap, GdkBitmap **mask);
};

class Gtk_Preview : public Gtk_Widget {
  public:
    Gtk_Preview(GtkPreviewType type);
    Gtk_Preview(GtkPreview *castitem);
    static guint8 isGtkPreview(Gtk_Object *checkcast);
    virtual void size(gint width, gint height);
    virtual void put(GdkWindow *window, GdkGC *gc,
		     gint srcx, gint srcy, gint destx,
		     gint desty, gint width, gint height);
    virtual void put_row(guchar *src, guchar *dest, gint x,
			 gint y, gint w);
    virtual void draw_row(guchar *data, gint x,
			  gint y, gint w);
    virtual void set_expand(gint expand);
    static void set_gamma(double gamma);
    static void set_color_cube(guint nred_shades, guint ngreen_shades,
			       guint nblue_shades, guint ngray_shades);
    static void set_install_cmap(gint install_cmap);
    static void set_reserved(gint nreserved);
    static GdkVisual * get_visual(void);
    static GdkColormap * get_cmap(void);
    static GtkPreviewInfo * get_info(void);
};

class Gtk_ProgressBar : public Gtk_Widget {
  public:
    Gtk_ProgressBar(void);
    Gtk_ProgressBar(GtkProgressBar *castitem);
    static guint8 isGtkProgressBar(Gtk_Object *checkcast);
    virtual void update(gfloat percentage);
};

class Gtk_RadioButton : public Gtk_CheckButton {
  public:
    Gtk_RadioButton(GSList *group);
    Gtk_RadioButton(GSList *group, gchar *label);
    Gtk_RadioButton(GtkRadioButton *castitem);
    static guint8 isGtkRadioButton(Gtk_Object *checkcast);
    virtual GSList * group(void);
};

class Gtk_RadioMenuItem : public Gtk_CheckMenuItem {
  public:
    Gtk_RadioMenuItem(GSList *group);
    Gtk_RadioMenuItem(GSList *group, gchar *label);
    Gtk_RadioMenuItem(GtkRadioMenuItem *castitem);
    static guint8 isGtkRadioMenuItem(Gtk_Object *checkcast);
    virtual GSList * group(void);
};

class Gtk_Range : public Gtk_Widget {
  public:
    Gtk_Range(GtkRange *castitem);
    static guint8 isGtkRange(Gtk_Object *checkcast);
    virtual Gtk_Adjustment *get_adjustment(void);
    virtual void set_adjustment(Gtk_Adjustment *adjustment);
    virtual void set_update_policy(GtkUpdateType policy);
    virtual void draw_background(void);
    virtual void draw_trough(void);
    virtual void draw_slider(void);
    virtual void draw_step_forw(void);
    virtual void draw_step_back(void);
    virtual void slider_update(void);
    virtual gint trough_click(gint x, gint y);
    virtual void default_hslider_update(void);
    virtual void default_vslider_update(void);
    virtual void default_htrough_click(gint x, gint y);
    virtual void default_vtrough_click(gint x, gint y);
    virtual void default_hmotion(gint xdelta, gint ydelta);
    virtual void default_vmotion(gint xdelta, gint ydelta);
    virtual gfloat calc_value(gint position);
  protected:
    Gtk_Range(void) {}
};

class Gtk_Ruler : public Gtk_Widget {
  public:
    Gtk_Ruler(GtkRuler *castitem);
    static guint8 isGtkRuler(Gtk_Object *checkcast);
    virtual void set_metric(GtkMetricType metric);
    virtual void set_range(gfloat lower, gfloat upper,
			   gfloat position, gfloat max_size);
    virtual void draw_ticks(void);
    virtual void draw_pos(void);
  protected:
    Gtk_Ruler(void) {}
};

class Gtk_HRuler : public Gtk_Ruler {
  public:
    Gtk_HRuler(void);
    Gtk_HRuler(GtkHRuler *castitem);
    static guint8 isGtkHRuler(Gtk_Object *checkcast);
};


class Gtk_VRuler : public Gtk_Ruler {
  public:
    Gtk_VRuler(void);
    Gtk_VRuler(GtkVRuler *castitem);
    static guint8 isGtkVRuler(Gtk_Object *checkcast);
};

class Gtk_Scale : public Gtk_Range {
  public:
    Gtk_Scale(GtkScale *castitem);
    static guint8 isGtkScale(Gtk_Object *checkcast);
    virtual void set_digits(gint digits);
    virtual void set_draw_value(gint draw_value);
    virtual void set_value_pos(GtkPositionType pos);
    virtual gint value_width(void);
    virtual void draw_value(void);
  protected:
    Gtk_Scale(void) {}
};

class Gtk_Scrollbar : public Gtk_Range {
  public:
    Gtk_Scrollbar(GtkScrollbar *castitem);
    static guint8 isGtkScrollbar(Gtk_Object *checkcast);
  protected:
    Gtk_Scrollbar(void) {}
};

class Gtk_ScrolledWindow : public Gtk_Container {
  public:
    Gtk_ScrolledWindow(Gtk_Adjustment *hadjustment, Gtk_Adjustment *vadjustment);
    Gtk_ScrolledWindow(GtkScrolledWindow *castitem);
    static guint8 isGtkScrolledWindow(Gtk_Object *checkcast);
    virtual Gtk_Adjustment *get_hadjustment(void);
    virtual Gtk_Adjustment *get_vadjustment(void);
    virtual void set_policy(GtkPolicyType hscrollbar_policy,
			    GtkPolicyType vscrollbar_policy);
};

class Gtk_Separator : public Gtk_Widget {
  public:
    Gtk_Separator(GtkSeparator *castitem);
    static guint8 isGtkSeparator(Gtk_Object *checkcast);
  protected:
    Gtk_Separator(void) {}
};

class Gtk_Table : public Gtk_Container {
  public:
    Gtk_Table(gint rows, gint columns, gint homogeneous);
    Gtk_Table(GtkTable *castitem);
    static guint8 isGtkTable(Gtk_Object *checkcast);
    virtual void attach(Gtk_Widget *child,
			gint left_attach,
			gint right_attach,
			gint top_attach,
			gint bottom_attach,
			gint xoptions, gint yoptions,
			gint xpadding, gint ypadding);
    virtual void attach_defaults(Gtk_Widget *child,
				 gint left_attach,
				 gint right_attach,
				 gint top_attach,
				 gint bottom_attach);
    virtual void set_row_spacing(gint row, gint spacing);
    virtual void set_col_spacing(gint row, gint spacing);
    virtual void set_row_spacings(gint spacing);
    virtual void set_col_spacings(gint spacing);
};

class Gtk_Text : public Gtk_Widget {
  public:
    Gtk_Text(Gtk_Adjustment *hadj, Gtk_Adjustment *vadj);
    Gtk_Text(GtkText *castitem);
    static guint8 isGtkText(Gtk_Object *checkcast);
    virtual void set_editable(gint editable);
    virtual void set_adjustments(Gtk_Adjustment *hadj, Gtk_Adjustment *vadj);
    virtual void set_point(guint index);
    virtual guint get_point(void);
    virtual guint get_length(void);
    virtual void freeze(void);
    virtual void thaw(void);
    virtual void insert(GdkFont *font, GdkColor *fore, GdkColor *back,
			const char *chars, gint length);
    virtual gint backward_delete(guint nchars);
    virtual gint forward_delete(guint nchars);
};


class Gtk_Tooltips {
  public:
    Gtk_Tooltips(void);
    Gtk_Tooltips(GtkTooltips *castitem);
    virtual ~Gtk_Tooltips(void);
    virtual void enable(void);
    virtual void disable(void);
    virtual void set_delay(gint delay);
    virtual void set_tips(Gtk_Widget *widget, gchar *tips_text);
    virtual void set_colors(GdkColor *background, GdkColor *foreground);
  protected:
    GtkTooltips *gtktooltips;
};

class Gtk_Tree : public Gtk_Container {
  public:
    Gtk_Tree(void);
    Gtk_Tree(GtkTree *castitem);
    static guint8 isGtkTree(Gtk_Object *checkcast);
    virtual void append(Gtk_Widget *widget);
    virtual void prepend(Gtk_Widget *widget);
    virtual void insert(Gtk_Widget *widget, gint position);
};

class Gtk_TreeItem : public Gtk_Item {
  public:
    Gtk_TreeItem(void);
    Gtk_TreeItem(gchar *label);
    Gtk_TreeItem(GtkTreeItem *castitem);
    static guint8 isGtkTreeItem(Gtk_Object *checkcast);
    virtual void set_subtree(Gtk_Tree *subtree);
    virtual void select(void);
    virtual void deselect(void);
    virtual void expand(void);
    virtual void collapse(void);
};

class Gtk_Viewport : public Gtk_Bin {
  public:
    Gtk_Viewport(Gtk_Adjustment *hadjustment, Gtk_Adjustment *vadjustment);
    Gtk_Viewport(GtkViewport *castitem);
    static guint8 isGtkViewport(Gtk_Object *checkcast);
    virtual Gtk_Adjustment *get_hadjustment(void);
    virtual Gtk_Adjustment *get_vadjustment(void);
    virtual void set_hadjustment(Gtk_Adjustment *adjustment);
    virtual void set_vadjustment(Gtk_Adjustment *adjustment);
    virtual void set_shadow_type(GtkShadowType type);
};


class Gtk_HScale : public Gtk_Scale {
  public:
    Gtk_HScale(Gtk_Adjustment *gtkadjustment);
    Gtk_HScale(GtkHScale *castitem);
    static guint8 isGtkHScale(Gtk_Object *checkcast);
};


class Gtk_VScale : public Gtk_Scale {
  public:
    Gtk_VScale(Gtk_Adjustment *gtkadjustment);
    Gtk_VScale(GtkVScale *castitem);
    static guint8 isGtkVScale(Gtk_Object *checkcast);
};

class Gtk_HScrollbar : public Gtk_Scrollbar {
  public:
    Gtk_HScrollbar(Gtk_Adjustment *gtkadjustment);
    Gtk_HScrollbar(GtkHScrollbar *castitem);
    static guint8 isGtkHScrollbar(Gtk_Object *checkcast);    
};

class Gtk_VScrollbar : public Gtk_Scrollbar {
  public:
    Gtk_VScrollbar(Gtk_Adjustment *gtkadjustment);
    Gtk_VScrollbar(GtkVScrollbar *castitem);
    static guint8 isGtkVScrollbar(Gtk_Object *checkcast);    
};

class Gtk_HSeparator : public Gtk_Separator {
  public:
    Gtk_HSeparator(void);
    Gtk_HSeparator(GtkHSeparator *castitem);
    static guint8 isGtkHSeparator(Gtk_Object *checkcast);
};

class Gtk_VSeparator : public Gtk_Separator {
  public:
    Gtk_VSeparator(void);
    Gtk_VSeparator(GtkVSeparator *castitem);
    static guint8 isGtkVSeparator(Gtk_Object *checkcast);
};


/***************************************************
 * Callback stuff                                  *
 ***************************************************
 Doing callbacks in C++ is very tricky, this is an ugly hack. For sanity,
 use a real language.

 There is another, non-portable way to do this using a few functions and a lot of
 data; this is the portable many-static-functions way.
 */

/* DEFINE Usage:
   class myclass {
     public:
       mymethod(void);
       METHOD_CALLBACK_DEFINE_X(myclass, mymethod);
   };
   (where X is the number of args the signal function would ordinarily get)
 */

#define METHOD_CALLBACK_DEFINE_0(classname, methodname) \
static methodname ## _callback(classname *a) { (a)-> ## methodname(); }

#define METHOD_CALLBACK_DEFINE_1(classname, methodname) \
static methodname ## _callback(void *arg1, classname *a) { (a)-> ## methodname(arg1); }

#define METHOD_CALLBACK_DEFINE_2(classname, methodname) \
static methodname ## _callback(void *arg1, void *arg2, classname *a) \
{ (a)-> ## methodname(arg1, arg2); }

#define METHOD_CALLBACK_DEFINE_3(classname, methodname) \
static methodname ## _callback(void *arg1, void *arg2, void *arg3, classname *a) \
{ (a)-> ## methodname(arg1, arg2, arg3); }

#define METHOD_CALLBACK_DEFINE_4(classname, methodname) \
static methodname ## _callback(void *arg1, void *arg2, void *arg3, void *arg4, \
			       classname *a) \
{ (a)-> ## methodname(arg1, arg2, arg3, arg4); }

/* Usage:
   agtkobject->signal_connect("button_click", METHOD_CALLBACK(mymethod));
 */
#define METHOD_CALLBACK(methodname) \
GtkSignalFunc( ## methodname ## _callback), this

/* Usage:
   agtkobject->signal_connect("button_click", METHOD_CALLBACK_EXTERNAL(myobject,mymethod));
 */
#define METHOD_CALLBACK_EXTERNAL(instance, methodname) \
GtkSignalFunc((instance)-> ## methodname ## _callback), instance

#endif /* #ifndef GTK_MINUS_MINUS_H */



/***************************************************
 * Phil's Callback stuff  (part 2)                 *
 ***************************************************

 See the top of this file for the SlotList class
 
 */

// -------------------------- 0 args ----------------------------------

/** Keeps a record of the object and method to call in the event of a signal
  */
template <class T>
class Slot : public SlotInterface
{
  private:
	T* obj;
	void (T::*method)();
  public:
	Slot(T* obj,void (T::*method)()) : obj(obj),method(method) {}
	void call() {(obj->*method)();}
	static void callback(GtkWidget*,Slot<T>* s) {s->call();}	
};


/** The signal connect function.
  Ideally this would be a member of the Gtk_Object class, and called
  signal_connect, but at present the g++ compiler doesn't support member
  template functions. Roll on V2.8.0!
  */
template <class T>
void 
connect(Gtk_Object* sender, const char* name,
			   T* receiver, void (T::*method)() )
{
	SlotInterface* s = new Slot<T>(receiver,method);
	sender->insert_slot(s);
	sender->signal_connect(const_cast<gchar*>(name),
						  GtkSignalFunc(Slot<T>::callback),s);
}

// -------------------------- 1 arg ----------------------------------

template <class T, class Arg1>
class Slot1 : public SlotInterface
{
  private:
	T* obj;
	void (T::*method)(Arg1);
  public:
	Slot1(T* obj,void (T::*method)(Arg1)) : obj(obj),method(method) {}
	void call(Arg1 a1) {(obj->*method)(a1);}
	static void callback(GtkWidget*,Arg1 a1,Slot1<T,Arg1>* s) {s->call(a1);}	
};

template <class T, class Arg1>
void 
connect(Gtk_Object* sender, const char* name,
			   T* receiver, void (T::*method)(Arg1) )
{
	SlotInterface* s = new Slot1<T,Arg1>(receiver,method);
	sender->insert_slot(s);
	sender->signal_connect(const_cast<gchar*>(name),
						  GtkSignalFunc(Slot1<T,Arg1>::callback),s);
}

