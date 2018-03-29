#include "gtk--.hh"
#include <stdarg.h>

Gtk_Object::Gtk_Object(guint type, ...)
{
     va_list args;
     va_start(args, type);
     gtkobject = gtk_object_new(type, args);
     ref();
}

Gtk_Object::Gtk_Object(GtkObject *castitem)
{
     gtkobject = castitem;
//     g_print("Gtk_Object::Gtk_Object(%X)\n", gtkobject);
     ref();
}

Gtk_Object::Gtk_Object(const Gtk_Object &obj)
{
     gtkobject = obj.gtkobject;
//     g_print("copy Gtk_Object::Gtk_Object(%X)\n", gtkobject);
     ref();
}


void
Gtk_Object::ref(void)
{
     gtk_object_ref(gtkobject);
}

void
Gtk_Object::unref(void)
{
     gtk_object_unref(gtkobject);
}

void
Gtk_Object::set_user_data(gpointer data)
{
     gtk_object_set_user_data(gtkobject, data);
}

gpointer
Gtk_Object::get_user_data(void)
{
     return gtk_object_get_user_data(gtkobject);
}

gint
Gtk_Object::signal_connect(gchar *name, GtkSignalFunc callback_func,
			   gpointer cbdata)
{
     return gtk_signal_connect(gtkobject, name, callback_func, cbdata);
}

void
Gtk_Object::signal_disconnect(gint id)
{
     gtk_signal_disconnect(gtkobject, id);
}

void
Gtk_Object::signal_emit(gint signal_type, ...)
{
     va_list args;
     va_start(args, signal_type);
     gtk_signal_emit(GTK_OBJECT(gtkobject), signal_type, args);
}

void
Gtk_Object::signal_emit_by_name(gchar *name, ...)
{
     va_list args;
     va_start(args, name);
     gtk_signal_emit_by_name(GTK_OBJECT(gtkobject), name, args);
}

Gtk_Object::~Gtk_Object(void)
{
	unref();
//     g_print("Gtk_Object::~Gtk_Object(%X)\n", gtkobject);
     gtk_object_destroy(gtkobject);
}

guint8
Gtk_Object::isGtkObject(Gtk_Object *checkcast)
{
     return GTK_IS_OBJECT(checkcast->gtkobject);
}

guint8 operator ==(Gtk_Object& a, Gtk_Object& b)
{
     return a.gtkobject == b.gtkobject;
}

guint8 operator !=(Gtk_Object& a, Gtk_Object& b)
{
     return a.gtkobject != b.gtkobject;
}


Gtk_AcceleratorTable::Gtk_AcceleratorTable(void)
{
     gtkacceleratortable = gtk_accelerator_table_new();
     ref();
}

void
Gtk_AcceleratorTable::ref(void)
{
     gtk_accelerator_table_ref(gtkacceleratortable);
}

void
Gtk_AcceleratorTable::unref(void)
{
     gtk_accelerator_table_unref(gtkacceleratortable);
}

Gtk_AcceleratorTable::Gtk_AcceleratorTable(GtkAcceleratorTable *castitem)
{
     gtkacceleratortable = castitem;
     ref();
}

Gtk_AcceleratorTable::Gtk_AcceleratorTable(Gtk_Object *object,
					   gchar *signal_name,
					   gchar accelerator_key,
					   guint8 accelerator_mods)
{
     gtkacceleratortable  = 
	  gtk_accelerator_table_find(object->gtkobject,
				     signal_name,
				     accelerator_key,
				     accelerator_mods);
}

Gtk_AcceleratorTable::~Gtk_AcceleratorTable(void)
{
     unref();
     gtk_accelerator_table_destroy(gtkacceleratortable);
}

void
Gtk_AcceleratorTable::install(Gtk_Object *object,
			      gchar *signal_name,
			      gchar accelerator_key,
			      guint8 accelerator_mods)
{
     gtk_accelerator_table_install(gtkacceleratortable,
				   GTK_OBJECT(object->gtkobject),
				   signal_name,
				   accelerator_key,
				   accelerator_mods);
}

void
Gtk_AcceleratorTable::remove(Gtk_Object *object, gchar *signal_name)
{
     gtk_accelerator_table_remove(gtkacceleratortable, object->gtkobject, signal_name);
}

gint
Gtk_AcceleratorTable::check(gchar accelerator_key,
			    guint8 accelerator_mods)
{
     return gtk_accelerator_table_check(gtkacceleratortable,
					accelerator_key,
					accelerator_mods);
}

Gtk_Adjustment::Gtk_Adjustment(gfloat value, gfloat lower, gfloat upper,
			       gfloat step_increment, gfloat page_increment,
			       gfloat page_size)
     : Gtk_Data::Gtk_Data(GTK_DATA(gtk_adjustment_new(value, lower, upper,
						      step_increment,
						      page_increment,
						      page_size)))
{
}

Gtk_Adjustment::Gtk_Adjustment(GtkAdjustment *castitem)
     : Gtk_Data::Gtk_Data(GTK_DATA(castitem))
{
}

guint8
Gtk_Adjustment::isGtkAdjustment(Gtk_Object *checkcast)
{
     return GTK_IS_ADJUSTMENT(checkcast->gtkobject);
}

Gtk_Alignment::Gtk_Alignment(gfloat xalign, gfloat yalign, gfloat xscale,
			     gfloat yscale)
     : Gtk_Bin::Gtk_Bin(GTK_BIN(gtk_alignment_new(xalign, yalign, xscale, yscale)))
{
}

Gtk_Alignment::Gtk_Alignment(GtkAlignment *castitem)
     : Gtk_Bin::Gtk_Bin(GTK_BIN(castitem))
{
}

void
Gtk_Alignment::set(gfloat xalign, gfloat yalign, gfloat xscale,
		   gfloat yscale)
{
     gtk_alignment_set(GTK_ALIGNMENT(gtkobject), xalign, yalign,
		       xscale, yscale);
}

Gtk_Widget::Gtk_Widget(GtkWidget *castitem)
     : Gtk_Object::Gtk_Object(GTK_OBJECT(castitem))
{
}

void
Gtk_Widget::unparent(void)
{
     gtk_widget_unparent(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::reparent(Gtk_Widget *newparent)
{
     gtk_widget_reparent(GTK_WIDGET(gtkobject), GTK_WIDGET(newparent->gtkobject));
}

void
Gtk_Widget::activate(void)
{
     gtk_widget_activate(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::popup(gint x, gint y)
{
     gtk_widget_popup(GTK_WIDGET(gtkobject), x, y);
}

void
Gtk_Widget::grab_focus(void)
{
     gtk_widget_grab_focus(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::grab_default(void)
{
     gtk_widget_grab_default(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::set_uposition(gint x, gint y)
{
     gtk_widget_set_uposition(GTK_WIDGET(gtkobject), x, y);
}

void
Gtk_Widget::set_usize(gint width, gint height)
{
     gtk_widget_set_usize(GTK_WIDGET(gtkobject), width, height);
}

void
Gtk_Widget::set_name(gchar *name)
{
     gtk_widget_set_name(GTK_WIDGET(gtkobject), name);
}

gchar *
Gtk_Widget::get_name(void)
{
     return gtk_widget_get_name(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::set_sensitive(gint sensitive)
{
     gtk_widget_set_sensitive(GTK_WIDGET(gtkobject), sensitive);
}

void
Gtk_Widget::set_parent(Gtk_Widget *parent)
{
     gtk_widget_set_parent(GTK_WIDGET(gtkobject),
			   GTK_WIDGET(parent->gtkobject));
}

void
Gtk_Widget::show(void)
{
     gtk_widget_show(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::hide(void)
{
     gtk_widget_hide(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::map(void)
{
     gtk_widget_map(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::unmap(void)
{
     gtk_widget_unmap(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::realize(void)
{
     gtk_widget_realize(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::unrealize(void)
{
     gtk_widget_unrealize(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::draw_focus(void)
{
     gtk_widget_draw_focus(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::draw_default(void)
{
     gtk_widget_draw_default(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::draw_children(void)
{
     gtk_widget_draw_children(GTK_WIDGET(gtkobject));
}

void
Gtk_Widget::size_request(GtkRequisition *requisition)
{
     gtk_widget_size_request(GTK_WIDGET(gtkobject), requisition);
}

void
Gtk_Widget::size_allocate(GtkAllocation *allocation)
{
     gtk_widget_size_allocate(GTK_WIDGET(gtkobject), allocation);
}

void
Gtk_Widget::install_accelerator(Gtk_AcceleratorTable *gtkacceleratortable,
				gchar *signal_name, gchar key, guint8 modifiers)
{
     gtk_widget_install_accelerator (GTK_WIDGET(gtkobject),
                                     gtkacceleratortable->gtkacceleratortable,
				     signal_name,
                                     key,
                                     modifiers);

}

void
Gtk_Widget::remove_accelerator(Gtk_AcceleratorTable *gtkacceleratortable,
			       gchar *signal_name)
{
     gtk_widget_remove_accelerator(GTK_WIDGET(gtkobject),
				   gtkacceleratortable->gtkacceleratortable,
				   signal_name);
}

guint8
Gtk_Widget::isGtkWidget(Gtk_Object *checkcast)
{
     return GTK_IS_WIDGET(checkcast->gtkobject);
}

Gtk_Widget::~Gtk_Widget(void)
{
     unref();
     gtk_widget_destroy(GTK_WIDGET(gtkobject));
}

Gtk_Data::Gtk_Data(GtkData *castitem)
     : Gtk_Object::Gtk_Object(GTK_OBJECT(castitem))
{
}

guint8
Gtk_Data::isGtkData(Gtk_Object *checkcast)
{
     return GTK_IS_DATA(checkcast->gtkobject);
}

Gtk_Arrow::Gtk_Arrow(GtkArrowType arrow_type, GtkShadowType shadow_type)
     : Gtk_Misc::Gtk_Misc(GTK_MISC(gtk_arrow_new(arrow_type, shadow_type)))
{
}

Gtk_Arrow::Gtk_Arrow(GtkArrow *castitem)
     : Gtk_Misc::Gtk_Misc(GTK_MISC(castitem))
{
}

void
Gtk_Arrow::set(GtkArrowType arrow_type,
	       GtkShadowType shadow_type)
{
     gtk_arrow_set(GTK_ARROW(gtkobject), arrow_type, shadow_type);
}

guint8
Gtk_Arrow::isGtkArrow(Gtk_Object *checkcast)
{
     return GTK_IS_ARROW(checkcast->gtkobject);
}

Gtk_AspectFrame::Gtk_AspectFrame(gchar *label,
				 gfloat xalign,
				 gfloat yalign,
				 gfloat ratio,
				 gint obey_child)
     : Gtk_Frame::Gtk_Frame(GTK_FRAME(gtk_aspect_frame_new(label,
							   xalign,
							   yalign,
							   ratio,
							   obey_child)))
{
}

Gtk_AspectFrame::Gtk_AspectFrame(GtkAspectFrame *castitem)
     :     Gtk_Frame::Gtk_Frame(GTK_FRAME(castitem))
{
}

void
Gtk_AspectFrame::set(gfloat xalign,
		     gfloat yalign,
		     gfloat ratio,
		     gint obey_child)
{
     gtk_aspect_frame_set(GTK_ASPECT_FRAME(gtkobject),
			  xalign, yalign,
			  ratio, obey_child);
}

guint8
Gtk_AspectFrame::isGtkAspectFrame(Gtk_Object *checkcast)
{
     return GTK_IS_ASPECT_FRAME(checkcast->gtkobject);
}

Gtk_Bin::Gtk_Bin(GtkBin *castitem)
     : Gtk_Container::Gtk_Container(GTK_CONTAINER(castitem))
{
}

guint8
Gtk_Bin::isGtkBin(Gtk_Object *checkcast)
{
     return GTK_IS_BIN(checkcast->gtkobject);
}

Gtk_Box::Gtk_Box(GtkBox *castitem)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(castitem))
{
}

guint8
Gtk_Box::isGtkBox(Gtk_Object *checkcast)
{
     return GTK_IS_BOX(checkcast->gtkobject);
}

void
Gtk_Box::pack_start(Gtk_Widget *child,
		    gint expand,
		    gint fill,
		    gint padding)
{
     gtk_box_pack_start(GTK_BOX(gtkobject),
			GTK_WIDGET(child->gtkobject),
			expand, fill, padding);
}

void
Gtk_Box::pack_end(Gtk_Widget *child,
		  gint expand,
		  gint fill,
		  gint padding)
{
     gtk_box_pack_end(GTK_BOX(gtkobject),
		      GTK_WIDGET(child->gtkobject),
		      expand, fill, padding);
}

void
Gtk_Box::pack_start(Gtk_Widget *child)
{
     gtk_box_pack_start_defaults(GTK_BOX(gtkobject),
				 GTK_WIDGET(child->gtkobject));
}

void
Gtk_Box::pack_end(Gtk_Widget *child)
{
     gtk_box_pack_end_defaults(GTK_BOX(gtkobject),
			       GTK_WIDGET(child->gtkobject));
}

void
Gtk_Box::set_homogeneous(gint homogenous)
{
     gtk_box_set_homogeneous(GTK_BOX(gtkobject),
			     homogenous);
}

void
Gtk_Box::set_spacing(gint spacing)
{
     gtk_box_set_spacing(GTK_BOX(gtkobject),
			 spacing);
}

Gtk_Button::Gtk_Button(void)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(gtk_button_new()))
{
}

Gtk_Button::Gtk_Button(gchar *label)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(gtk_button_new_with_label(label)))
{
}

Gtk_Button::Gtk_Button(GtkButton *castitem)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(castitem))
{
}

guint8
Gtk_Button::isGtkButton(Gtk_Object *checkcast)
{
     return GTK_IS_BUTTON(checkcast->gtkobject);
}

void
Gtk_Button::pressed(void)
{
     gtk_button_pressed(GTK_BUTTON(gtkobject));
}

void
Gtk_Button::released(void)
{
     gtk_button_released(GTK_BUTTON(gtkobject));
}

void
Gtk_Button::enter(void)
{
     gtk_button_enter(GTK_BUTTON(gtkobject));
}

void
Gtk_Button::leave(void)
{
     gtk_button_leave(GTK_BUTTON(gtkobject));
}

Gtk_CheckButton::Gtk_CheckButton(void)
     :     Gtk_ToggleButton::Gtk_ToggleButton(GTK_TOGGLE_BUTTON(gtk_check_button_new()))
{
}

Gtk_CheckButton::Gtk_CheckButton(gchar *label)
     :     Gtk_ToggleButton::Gtk_ToggleButton(GTK_TOGGLE_BUTTON(gtk_check_button_new_with_label(label)))
{
}

Gtk_CheckButton::Gtk_CheckButton(GtkCheckButton *castitem)
     :     Gtk_ToggleButton::Gtk_ToggleButton(GTK_TOGGLE_BUTTON(castitem))
{
}

guint8
Gtk_CheckButton::isGtkCheckButton(Gtk_Object *checkcast)
{
     return GTK_IS_CHECK_BUTTON(checkcast->gtkobject);
}

Gtk_CheckMenuItem::Gtk_CheckMenuItem(void)
     :     Gtk_MenuItem::Gtk_MenuItem(GTK_MENU_ITEM(gtk_check_menu_item_new()))
{
}

Gtk_CheckMenuItem::Gtk_CheckMenuItem(GtkCheckMenuItem *castitem)
     :     Gtk_MenuItem::Gtk_MenuItem(GTK_MENU_ITEM(castitem))
{
}

Gtk_CheckMenuItem::Gtk_CheckMenuItem(gchar *label)
     :     Gtk_MenuItem::Gtk_MenuItem(GTK_MENU_ITEM(gtk_check_menu_item_new_with_label(label)))
{
}

void
Gtk_CheckMenuItem::set_state(gint state)
{
     gtk_check_menu_item_set_state(GTK_CHECK_MENU_ITEM(gtkobject),
				   state);
}

void
Gtk_CheckMenuItem::toggled(void)
{
     gtk_check_menu_item_toggled(GTK_CHECK_MENU_ITEM(gtkobject));
}

guint8
Gtk_CheckMenuItem::isGtkCheckMenuItem(Gtk_Object *checkcast)
{
     return GTK_IS_CHECK_MENU_ITEM(checkcast->gtkobject);
}

Gtk_ColorSelection::Gtk_ColorSelection(void)
     :     Gtk_Window::Gtk_Window(GTK_WINDOW(gtk_color_selection_new()))
{
}

Gtk_ColorSelection::Gtk_ColorSelection(GtkColorSelection *castitem)
     :     Gtk_Window::Gtk_Window(GTK_WINDOW(castitem))
{
}

guint8
Gtk_ColorSelection::isGtkColorSelection(Gtk_Object *checkcast)
{
     return GTK_IS_COLOR_SELECTION(checkcast->gtkobject);
}

void
Gtk_ColorSelection::set_update_policy(GtkUpdateType policy)
{
     gtk_color_selection_set_update_policy(GTK_COLOR_SELECTION(gtkobject),
					   policy);
}

void
Gtk_ColorSelection::set_opacity(gint use_opacity)
{
     gtk_color_selection_set_opacity(GTK_COLOR_SELECTION(gtkobject),
				     use_opacity);
}

void
Gtk_ColorSelection::set_color(gdouble *color)
{
     gtk_color_selection_set_color(GTK_COLOR_SELECTION(gtkobject),
				   color);
}

void
Gtk_ColorSelection::get_color(gdouble *color)
{
     gtk_color_selection_get_color(GTK_COLOR_SELECTION(gtkobject),
				   color);
}

Gtk_ColorSelectionDialog::Gtk_ColorSelectionDialog(gchar *title)
     :     Gtk_Window::Gtk_Window(GTK_WINDOW(gtk_color_selection_dialog_new(title)))
{
}

Gtk_ColorSelectionDialog::Gtk_ColorSelectionDialog(GtkColorSelectionDialog *castitem)
     :     Gtk_Window::Gtk_Window(GTK_WINDOW(castitem))
{
}

guint8
Gtk_ColorSelectionDialog::isGtkColorSelectionDialog(Gtk_Object *checkcast)
{
     return GTK_IS_COLOR_SELECTION_DIALOG(checkcast->gtkobject);
}

Gtk_Container::Gtk_Container(GtkContainer *castitem)
     : Gtk_Widget::Gtk_Widget(GTK_WIDGET(castitem))
{
}

guint8
Gtk_Container::isGtkContainer(Gtk_Object *checkcast)
{
     return GTK_IS_CONTAINER(checkcast->gtkobject);
}

void
Gtk_Container::add(Gtk_Widget *widget)
{
     gtk_container_add(GTK_CONTAINER(gtkobject),
		       GTK_WIDGET(widget->gtkobject));
}

void
Gtk_Container::remove(Gtk_Widget *widget)
{
     gtk_container_remove(GTK_CONTAINER(gtkobject),
			  GTK_WIDGET(widget->gtkobject));

}

void
Gtk_Container::disable_resize(void)
{
     gtk_container_disable_resize(GTK_CONTAINER(gtkobject));
}

void
Gtk_Container::enable_resize(void)
{
     gtk_container_enable_resize(GTK_CONTAINER(gtkobject));
}

void
Gtk_Container::block_resize(void)
{
     gtk_container_block_resize(GTK_CONTAINER(gtkobject));
}

void
Gtk_Container::unblock_resize(void)
{
     gtk_container_unblock_resize(GTK_CONTAINER(gtkobject));
}

void
Gtk_Container::need_resize(Gtk_Widget *widget)
{
     gtk_container_need_resize(GTK_CONTAINER(gtkobject), GTK_WIDGET(widget->gtkobject));
}

void
Gtk_Container::check_resize(Gtk_Widget *widget)
{
     gtk_container_check_resize(GTK_CONTAINER(gtkobject),
				GTK_WIDGET(widget->gtkobject));
}

void
Gtk_Container::foreach(GtkCallback callback,
		       gpointer callback_data)
{
     gtk_container_foreach(GTK_CONTAINER(gtkobject),
			   callback,
			   callback_data);
}

void
Gtk_Container::focus(GtkDirectionType direction)
{
     gtk_container_focus(GTK_CONTAINER(gtkobject), direction);
}

GList *
Gtk_Container::children(void)
{
     return gtk_container_children(GTK_CONTAINER(gtkobject));
}

Gtk_Curve::Gtk_Curve(void)
     :     Gtk_VBox::Gtk_VBox(GTK_VBOX(gtk_curve_new()))
{
}

Gtk_Curve::Gtk_Curve(GtkCurve *castitem)
     :     Gtk_VBox::Gtk_VBox(GTK_VBOX(castitem))
{
}

guint8
Gtk_Curve::isGtkCurve(Gtk_Object *checkcast)
{
     return GTK_IS_CURVE(checkcast->gtkobject);
}

void
Gtk_Curve::reset(void)
{
     gtk_curve_reset(GTK_CURVE(gtkobject));
}

void
Gtk_Curve::set_range(gfloat min_x, gfloat max_x,
		     gfloat min_y, gfloat max_y)
{
     gtk_curve_set_range(GTK_CURVE(gtkobject),
			 min_x, max_x, min_y, max_y);
}

void
Gtk_Curve::set_vector(int veclen, gfloat *vector)
{
     gtk_curve_set_vector(GTK_CURVE(gtkobject),
			  veclen, vector);
}

Gtk_Dialog::Gtk_Dialog(void)
     :     Gtk_Window::Gtk_Window(GTK_WINDOW(gtk_dialog_new()))
{
}

Gtk_Dialog::Gtk_Dialog(GtkDialog *castitem)
     :     Gtk_Window::Gtk_Window(GTK_WINDOW(castitem))
{
}

guint8
Gtk_Dialog::isGtkDialog(Gtk_Object *checkcast)
{
     return GTK_IS_DIALOG(checkcast->gtkobject);
}

Gtk_VBox *
Gtk_Dialog::vbox(void)
{
     return new Gtk_VBox(GTK_VBOX(GTK_DIALOG(gtkobject)->vbox));
}

Gtk_HBox *
Gtk_Dialog::action_area(void)
{
     return new Gtk_HBox(GTK_HBOX(GTK_DIALOG(gtkobject)->action_area));
}

Gtk_DrawingArea::Gtk_DrawingArea(void)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(gtk_drawing_area_new()))
{
}

Gtk_DrawingArea::Gtk_DrawingArea(GtkDrawingArea *castitem)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(castitem))
{
}

guint8
Gtk_DrawingArea::isGtkDrawingArea(Gtk_Object *checkcast)
{
     return GTK_IS_DRAWING_AREA(checkcast->gtkobject);
}

void
Gtk_DrawingArea::size(gint width, gint height)
{
     gtk_drawing_area_size(GTK_DRAWING_AREA(gtkobject),
			   width, height);
}

Gtk_Entry::Gtk_Entry(void)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(gtk_entry_new()))
{
}

Gtk_Entry::Gtk_Entry(GtkEntry *castitem)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(castitem))
{
}

guint8
Gtk_Entry::isGtkEntry(Gtk_Object *checkcast)
{
     return GTK_IS_ENTRY(checkcast->gtkobject);
}

void
Gtk_Entry::set_text(gchar *text)
{
     gtk_entry_set_text(GTK_ENTRY(gtkobject), text);
			
}

gchar *
Gtk_Entry::get_text(void)
{
     return gtk_entry_get_text(GTK_ENTRY(gtkobject));
}

void
Gtk_Entry::append_text(gchar *text)
{
     gtk_entry_append_text(GTK_ENTRY(gtkobject), text);
}

void
Gtk_Entry::prepend_text(gchar *text)
{
     gtk_entry_prepend_text(GTK_ENTRY(gtkobject), text);
}

void
Gtk_Entry::set_position(gint position)
{
     gtk_entry_set_position(GTK_ENTRY(gtkobject), position);
}

Gtk_FileSelection::Gtk_FileSelection(gchar *title)
     :     Gtk_Window::Gtk_Window(GTK_WINDOW(gtk_file_selection_new(title)))
{
}

Gtk_FileSelection::Gtk_FileSelection(GtkFileSelection *castitem)
     :     Gtk_Window::Gtk_Window(GTK_WINDOW(castitem))
{
}

guint8
Gtk_FileSelection::isGtkFileSelection(Gtk_Object *checkcast)
{
     return GTK_IS_FILE_SELECTION(checkcast->gtkobject);
}

void
Gtk_FileSelection::set_filename(gchar *filename)
{
     gtk_file_selection_set_filename(GTK_FILE_SELECTION(gtkobject),
				     filename);
}

gchar *
Gtk_FileSelection::get_filename(void)
{
     return gtk_file_selection_get_filename(GTK_FILE_SELECTION(gtkobject));
}

Gtk_Frame::Gtk_Frame(gchar *label)
     :     Gtk_Bin::Gtk_Bin(GTK_BIN(gtk_frame_new(label)))
{
}

Gtk_Frame::Gtk_Frame(GtkFrame *castitem)
     :     Gtk_Bin::Gtk_Bin(GTK_BIN(castitem))
{
}

guint8
Gtk_Frame::isGtkFrame(Gtk_Object *checkcast)
{
     return GTK_IS_FRAME(checkcast->gtkobject);
}

void
Gtk_Frame::set_label(gchar *label)
{
     gtk_frame_set_label(GTK_FRAME(gtkobject),
			 label);
}

void
Gtk_Frame::set_label_align(gfloat xalign, gfloat yalign)
{
     gtk_frame_set_label_align(GTK_FRAME(gtkobject),
			       xalign, yalign);
}

void
Gtk_Frame::set_shadow_type(GtkShadowType type)
{
     gtk_frame_set_shadow_type(GTK_FRAME(gtkobject),
			       type);
}

Gtk_HBox::Gtk_HBox(gint homogenous, gint spacing)
     :     Gtk_Box::Gtk_Box(GTK_BOX(gtk_hbox_new(homogenous, spacing)))
{
}

Gtk_HBox::Gtk_HBox(GtkHBox *castitem)
     :     Gtk_Box::Gtk_Box(GTK_BOX(castitem))
{
}

guint8
Gtk_HBox::isGtkHBox(Gtk_Object *checkcast)
{
     return GTK_IS_HBOX(checkcast->gtkobject);
}


Gtk_VBox::Gtk_VBox(gint homogenous, gint spacing)
     :     Gtk_Box::Gtk_Box(GTK_BOX(gtk_vbox_new(homogenous, spacing)))
{
}

Gtk_VBox::Gtk_VBox(GtkVBox *castitem)
     :     Gtk_Box::Gtk_Box(GTK_BOX(castitem))
{
}

guint8
Gtk_VBox::isGtkVBox(Gtk_Object *checkcast)
{
     return GTK_IS_VBOX(checkcast->gtkobject);
}

Gtk_HRuler::Gtk_HRuler(void)
     :     Gtk_Ruler::Gtk_Ruler(GTK_RULER(gtk_hruler_new()))
{
}

Gtk_HRuler::Gtk_HRuler(GtkHRuler *castitem)
     :     Gtk_Ruler::Gtk_Ruler(GTK_RULER(castitem))
{
}

guint8
Gtk_HRuler::isGtkHRuler(Gtk_Object *checkcast)
{
     return GTK_IS_HRULER(checkcast->gtkobject);
}

Gtk_VRuler::Gtk_VRuler(void)
     :     Gtk_Ruler::Gtk_Ruler(GTK_RULER(gtk_vruler_new()))
{
}

Gtk_VRuler::Gtk_VRuler(GtkVRuler *castitem)
     :     Gtk_Ruler::Gtk_Ruler(GTK_RULER(castitem))
{
}

guint8
Gtk_VRuler::isGtkVRuler(Gtk_Object *checkcast)
{
     return GTK_IS_VRULER(checkcast->gtkobject);
}

Gtk_HScale::Gtk_HScale(Gtk_Adjustment *gtkadjustment)
     : Gtk_Scale::Gtk_Scale(GTK_SCALE(gtk_hscale_new(GTK_ADJUSTMENT(
	  gtkadjustment->gtkobject))))
{
}

Gtk_HScale::Gtk_HScale(GtkHScale *castitem)
     :     Gtk_Scale::Gtk_Scale(GTK_SCALE(castitem))
{
}

guint8
Gtk_HScale::isGtkHScale(Gtk_Object *checkcast)
{
     return GTK_IS_HSCALE(checkcast->gtkobject);
}


Gtk_VScale::Gtk_VScale(Gtk_Adjustment *gtkadjustment)
     : Gtk_Scale::Gtk_Scale(GTK_SCALE(gtk_vscale_new(GTK_ADJUSTMENT(
	  gtkadjustment->gtkobject))))
{
}

Gtk_VScale::Gtk_VScale(GtkVScale *castitem)
     :     Gtk_Scale::Gtk_Scale(GTK_SCALE(castitem))
{
}

guint8
Gtk_VScale::isGtkVScale(Gtk_Object *checkcast)
{
     return GTK_IS_VSCALE(checkcast->gtkobject);
}

Gtk_HScrollbar::Gtk_HScrollbar(Gtk_Adjustment *gtkadjustment)
     :     Gtk_Scrollbar::Gtk_Scrollbar(GTK_SCROLLBAR(gtk_hscrollbar_new(GTK_ADJUSTMENT(gtkadjustment->gtkobject))))
{
}

Gtk_HScrollbar::Gtk_HScrollbar(GtkHScrollbar *castitem)
     :     Gtk_Scrollbar::Gtk_Scrollbar(GTK_SCROLLBAR(castitem))
{
}

guint8
Gtk_HScrollbar::isGtkHScrollbar(Gtk_Object *checkcast)
{
     return GTK_IS_HSCROLLBAR(checkcast->gtkobject);
}

Gtk_VScrollbar::Gtk_VScrollbar(Gtk_Adjustment *gtkadjustment)
     :     Gtk_Scrollbar::Gtk_Scrollbar(GTK_SCROLLBAR(gtk_vscrollbar_new(GTK_ADJUSTMENT(gtkadjustment->gtkobject))))
{
}

Gtk_VScrollbar::Gtk_VScrollbar(GtkVScrollbar *castitem)
     :     Gtk_Scrollbar::Gtk_Scrollbar(GTK_SCROLLBAR(castitem))
{
}

guint8
Gtk_VScrollbar::isGtkVScrollbar(Gtk_Object *checkcast)
{
     return GTK_IS_VSCROLLBAR(checkcast->gtkobject);
}

Gtk_HSeparator::Gtk_HSeparator(void)
     :     Gtk_Separator::Gtk_Separator(GTK_SEPARATOR(gtk_hseparator_new()))
{
}

Gtk_HSeparator::Gtk_HSeparator(GtkHSeparator *castitem)
     :     Gtk_Separator::Gtk_Separator(GTK_SEPARATOR(castitem))
{
}

guint8
Gtk_HSeparator::isGtkHSeparator(Gtk_Object *checkcast)
{
     return GTK_IS_HSEPARATOR(checkcast->gtkobject);
}


Gtk_VSeparator::Gtk_VSeparator(void)
     :     Gtk_Separator::Gtk_Separator(GTK_SEPARATOR(gtk_vseparator_new()))
{
}

Gtk_VSeparator::Gtk_VSeparator(GtkVSeparator *castitem)
     :     Gtk_Separator::Gtk_Separator(GTK_SEPARATOR(castitem))
{
}

guint8
Gtk_VSeparator::isGtkVSeparator(Gtk_Object *checkcast)
{
     return GTK_IS_VSEPARATOR(checkcast->gtkobject);
}

Gtk_Image::Gtk_Image(GdkImage *val, GdkBitmap *mask)
     :     Gtk_Misc::Gtk_Misc(GTK_MISC(gtk_image_new(val, mask)))
{
}

Gtk_Image::Gtk_Image(GtkImage *castitem)
     :     Gtk_Misc::Gtk_Misc(GTK_MISC(castitem))
{
}

guint8
Gtk_Image::isGtkImage(Gtk_Object *checkcast)
{
     return GTK_IS_IMAGE(checkcast->gtkobject);
}

void
Gtk_Image::set(GdkImage *val, GdkBitmap *mask)
{
     gtk_image_set(GTK_IMAGE(gtkobject), val, mask);
}

void
Gtk_Image::get(GdkImage **val, GdkBitmap **mask)
{
     gtk_image_get(GTK_IMAGE(gtkobject), val, mask);
}

Gtk_Item::Gtk_Item(GtkItem *castitem)
     :     Gtk_Bin::Gtk_Bin(GTK_BIN(castitem))
{
}

guint8
Gtk_Item::isGtkItem(Gtk_Object *checkcast)
{
     return GTK_IS_ITEM(checkcast->gtkobject);
}

void
Gtk_Item::select(void)
{
     gtk_item_select(GTK_ITEM(gtkobject));
}

void
Gtk_Item::deselect(void)
{
     gtk_item_deselect(GTK_ITEM(gtkobject));
}

void
Gtk_Item::toggle(void)
{
     gtk_item_toggle(GTK_ITEM(gtkobject));
}

Gtk_Label::Gtk_Label(gchar *str)
     :     Gtk_Misc::Gtk_Misc(GTK_MISC(gtk_label_new(str)))
{
}

Gtk_Label::Gtk_Label(GtkLabel *castitem)
     :     Gtk_Misc::Gtk_Misc(GTK_MISC(castitem))
{
}

guint8
Gtk_Label::isGtkLabel(Gtk_Object *checkcast)
{
     return GTK_IS_LABEL(checkcast->gtkobject);
}

void
Gtk_Label::set(gchar *str)
{
     gtk_label_set(GTK_LABEL(gtkobject), str);
}

void
Gtk_Label::get(gchar **str)
{
     gtk_label_get(GTK_LABEL(gtkobject), str);
}

Gtk_List::Gtk_List(void)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(gtk_list_new()))
{
}

Gtk_List::Gtk_List(GtkList *castitem)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(castitem))
{
}

guint8
Gtk_List::isGtkList(Gtk_Object *checkcast)
{
     return GTK_IS_LIST(checkcast->gtkobject);
}

void
Gtk_List::insert_items(GList *items, gint position)
{
     gtk_list_insert_items(GTK_LIST(gtkobject), items, position);
}

void
Gtk_List::append_items(GList *items)
{
     gtk_list_append_items(GTK_LIST(gtkobject), items);
}

void
Gtk_List::prepend_items(GList *items)
{
     gtk_list_prepend_items(GTK_LIST(gtkobject), items);
}

void
Gtk_List::remove_items(GList *items)
{
     gtk_list_remove_items(GTK_LIST(gtkobject), items);
}

void
Gtk_List::clear_items(gint start, gint end)
{
     gtk_list_clear_items(GTK_LIST(gtkobject), start, end);
}

void
Gtk_List::select_item(gint item)
{
     gtk_list_select_item(GTK_LIST(gtkobject), item);
}

void
Gtk_List::unselect_item(gint item)
{
     gtk_list_unselect_item(GTK_LIST(gtkobject), item);
}

void
Gtk_List::select_child(Gtk_Widget *child)
{
     gtk_list_select_child(GTK_LIST(gtkobject), GTK_WIDGET(child->gtkobject));
}

void
Gtk_List::unselect_child(Gtk_Widget *child)
{
     gtk_list_unselect_child(GTK_LIST(gtkobject),
			     GTK_WIDGET(child->gtkobject));
}

gint
Gtk_List::child_position(Gtk_Widget *child)
{
     return gtk_list_child_position(GTK_LIST(gtkobject),
				    GTK_WIDGET(child->gtkobject));
}

void
Gtk_List::set_selection_mode(GtkSelectionMode mode)
{
     gtk_list_set_selection_mode(GTK_LIST(gtkobject), mode);
}

Gtk_ListItem::Gtk_ListItem(void)
     :     Gtk_Item::Gtk_Item(GTK_ITEM(gtk_list_item_new()))
{
}

Gtk_ListItem::Gtk_ListItem(gchar *label)
     :     Gtk_Item::Gtk_Item(GTK_ITEM(gtk_list_item_new_with_label(label)))
{
}

Gtk_ListItem::Gtk_ListItem(GtkListItem *castitem)
     :     Gtk_Item::Gtk_Item(GTK_ITEM(castitem))
{
}

guint8
Gtk_ListItem::isGtkListItem(Gtk_Object *checkcast)
{
     return GTK_IS_LIST_ITEM(checkcast->gtkobject);
}

void
Gtk_ListItem::select(void)
{
     gtk_list_item_select(GTK_LIST_ITEM(gtkobject));
}

void
Gtk_ListItem::deselect(void)
{
     gtk_list_item_deselect(GTK_LIST_ITEM(gtkobject));
}

void
Gtk_Main::grab_add(Gtk_Widget *widget)
{
     gtk_grab_add(GTK_WIDGET(widget->gtkobject));
}

void
Gtk_Main::grab_remove(Gtk_Widget *widget)
{
     gtk_grab_remove(GTK_WIDGET(widget->gtkobject));
}

gint
Gtk_Main::timeout_add(guint32 interval, GtkFunction function,
		      gpointer data)
{
     return gtk_timeout_add(interval, function, data);
}

void
Gtk_Main::timeout_remove(gint tag)
{
     gtk_timeout_remove(tag);
}

gint
Gtk_Main::idle_add(GtkFunction function, gpointer data)
{
     return gtk_idle_add(function, data);
}

void
Gtk_Main::idle_remove(gint tag)
{
     gtk_idle_remove(tag);
}

Gtk_Menu::Gtk_Menu(void)
     :     Gtk_MenuShell::Gtk_MenuShell(GTK_MENU_SHELL(gtk_menu_new()))
{
}

Gtk_Menu::Gtk_Menu(GtkMenu *castitem)
     :     Gtk_MenuShell::Gtk_MenuShell(GTK_MENU_SHELL(castitem))
{
}

guint8
Gtk_Menu::isGtkMenu(Gtk_Object *checkcast)
{
     return GTK_IS_MENU(checkcast->gtkobject);
}

void
Gtk_Menu::append(Gtk_MenuItem *child)
{
     gtk_menu_append(GTK_MENU(gtkobject), GTK_WIDGET(child->gtkobject));
}

void
Gtk_Menu::prepend(Gtk_MenuItem *child)
{
     gtk_menu_prepend(GTK_MENU(gtkobject), GTK_WIDGET(child->gtkobject));
}

void
Gtk_Menu::insert(Gtk_MenuItem *child, gint position)
{
     gtk_menu_insert(GTK_MENU(gtkobject), GTK_WIDGET(child->gtkobject),
		     position);
}

void
Gtk_Menu::popup(Gtk_MenuShell *parent_menu_shell,
		Gtk_MenuItem *parent_menu_item,
		GtkMenuPositionFunc func,
		gpointer data,
		gint button,
		guint32 activate_time)
{
     gtk_menu_popup(GTK_MENU(gtkobject),
		    GTK_WIDGET(parent_menu_shell->gtkobject),
		    GTK_WIDGET(parent_menu_item->gtkobject),
		    func, data, button, activate_time);
}

void
Gtk_Menu::popdown(void)
{
     gtk_menu_popdown(GTK_MENU(gtkobject));
}

Gtk_Widget *
Gtk_Menu::get_active(void)
{
     return new Gtk_Widget(gtk_menu_get_active(GTK_MENU(gtkobject)));
}

void
Gtk_Menu::set_active(gint index)
{
     gtk_menu_set_active(GTK_MENU(gtkobject), index);
}

void
Gtk_Menu::set_accelerator_table(Gtk_AcceleratorTable *table)
{
     gtk_menu_set_accelerator_table(GTK_MENU(gtkobject),
				    table->gtkacceleratortable);
}

Gtk_MenuBar::Gtk_MenuBar(void)
     :     Gtk_MenuShell::Gtk_MenuShell(GTK_MENU_SHELL(gtk_menu_bar_new()))
{
}

Gtk_MenuBar::Gtk_MenuBar(GtkMenuBar *castitem)
     :     Gtk_MenuShell::Gtk_MenuShell(GTK_MENU_SHELL(castitem))
{
}

guint8
Gtk_MenuBar::isGtkMenuBar(Gtk_Object *checkcast)
{
     return GTK_IS_MENU_BAR(checkcast->gtkobject);
}

void
Gtk_MenuBar::append(Gtk_MenuItem *child)
{
     gtk_menu_bar_append(GTK_MENU_BAR(gtkobject), GTK_WIDGET(child->gtkobject));
}

void
Gtk_MenuBar::prepend(Gtk_MenuItem *child)
{
     gtk_menu_bar_prepend(GTK_MENU_BAR(gtkobject), GTK_WIDGET(child->gtkobject));
}

void
Gtk_MenuBar::insert(Gtk_MenuItem *child, gint position)
{
     gtk_menu_bar_insert(GTK_MENU_BAR(gtkobject), GTK_WIDGET(child->gtkobject), position);
}

Gtk_MenuPath::Gtk_MenuPath(GtkMenuPath *castitem)
{
     path = castitem->path;
     widget = new Gtk_Widget(GTK_WIDGET(castitem->widget));
     widget->ref();
}

Gtk_MenuPath::~Gtk_MenuPath(void)
{
     widget->unref();
     delete widget;
}


Gtk_MenuFactory::Gtk_MenuFactory(GtkMenuFactoryType type)
{
     gtkmenufactory = gtk_menu_factory_new(type);
}

Gtk_MenuFactory::Gtk_MenuFactory(GtkMenuFactory *castitem)
{
     gtkmenufactory = castitem;
}

Gtk_MenuFactory::~Gtk_MenuFactory(void)
{
     gtk_menu_factory_destroy(gtkmenufactory);
}

void
Gtk_MenuFactory::add_entries(GtkMenuEntry *entries, int nentries)
{
     /* XXX fix to use classes instead of gtk stuff */
     gtk_menu_factory_add_entries(gtkmenufactory, entries, nentries);
}

void
Gtk_MenuFactory::add_subfactory(Gtk_MenuFactory *subfactory, gchar *path)
{
     gtk_menu_factory_add_subfactory(gtkmenufactory,
				     subfactory->gtkmenufactory,
				     path);
}

void
Gtk_MenuFactory::remove_paths(gchar **paths, int npaths)
{
     gtk_menu_factory_remove_paths(gtkmenufactory, paths, npaths);
}

void
Gtk_MenuFactory::remove_entries(GtkMenuEntry *entries, int nentries)
{
     /* XXX fix to use classes instead of gtk stuff */
     gtk_menu_factory_remove_entries(gtkmenufactory, entries, nentries);
}

void
Gtk_MenuFactory::remove_subfactory(Gtk_MenuFactory *subfactory, char *path)
{
     gtk_menu_factory_remove_subfactory(gtkmenufactory,
					subfactory->gtkmenufactory,
					path);
}

Gtk_MenuPath *
Gtk_MenuFactory::factory_find(gchar *path)
{
     return new Gtk_MenuPath(gtk_menu_factory_find(gtkmenufactory, path));
}


Gtk_MenuItem::Gtk_MenuItem(void)
     :     Gtk_Item::Gtk_Item(GTK_ITEM(gtk_menu_item_new()))
{
}

Gtk_MenuItem::Gtk_MenuItem(GtkMenuItem *castitem)
     :     Gtk_Item::Gtk_Item(GTK_ITEM(castitem))
{
}

Gtk_MenuItem::Gtk_MenuItem(gchar *label)
     :     Gtk_Item::Gtk_Item(GTK_ITEM(gtk_menu_item_new_with_label(label)))
{
}

void
Gtk_MenuItem::set_submenu(Gtk_Menu *submenu)
{
     gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtkobject), GTK_WIDGET(submenu->gtkobject));
}

void
Gtk_MenuItem::set_placement(GtkSubmenuPlacement placement)
{
     gtk_menu_item_set_placement(GTK_MENU_ITEM(gtkobject), placement);
}

void
Gtk_MenuItem::accelerator_size(void)
{
     gtk_menu_item_accelerator_size(GTK_MENU_ITEM(gtkobject));
}

void
Gtk_MenuItem::accelerator_text(gchar *buffer)
{
     gtk_menu_item_accelerator_text(GTK_MENU_ITEM(gtkobject), buffer);
}

void
Gtk_MenuItem::configure(gint show_toggle_indicator, gint show_submenu_indicator)
{
     gtk_menu_item_configure(GTK_MENU_ITEM(gtkobject), show_toggle_indicator,
			     show_submenu_indicator);
}

void
Gtk_MenuItem::select(void)
{
     gtk_menu_item_select(GTK_MENU_ITEM(gtkobject));
}

void
Gtk_MenuItem::deselect(void)
{
     gtk_menu_item_deselect(GTK_MENU_ITEM(gtkobject));
}

void
Gtk_MenuItem::activate(void)
{
     gtk_menu_item_activate(GTK_MENU_ITEM(gtkobject));
}

guint8
Gtk_MenuItem::isGtkMenuItem(Gtk_Object *checkcast)
{
     return GTK_IS_MENU_ITEM(checkcast->gtkobject);
}

Gtk_MenuShell::Gtk_MenuShell(GtkMenuShell *castitem)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(castitem))
{
}

guint8
Gtk_MenuShell::isGtkMenuShell(Gtk_Object *checkcast)
{
     return GTK_IS_MENU_SHELL(checkcast->gtkobject);
}

void
Gtk_MenuShell::append(Gtk_MenuShell *child)
{
     gtk_menu_shell_append(GTK_MENU_SHELL(gtkobject), GTK_WIDGET(child->gtkobject));
}

void
Gtk_MenuShell::prepend(Gtk_MenuShell *child)
{
     gtk_menu_shell_prepend(GTK_MENU_SHELL(gtkobject), GTK_WIDGET(child->gtkobject));
}

void
Gtk_MenuShell::insert(Gtk_MenuShell *child, gint position)
{
     gtk_menu_shell_insert(GTK_MENU_SHELL(gtkobject), GTK_WIDGET(child->gtkobject), position);
}

void
Gtk_MenuShell::deactivate(void)
{
     gtk_menu_shell_deactivate(GTK_MENU_SHELL(gtkobject));
}

Gtk_Misc::Gtk_Misc(GtkMisc *castitem)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(castitem))
{
}

guint8
Gtk_Misc::isGtkMisc(Gtk_Object *checkcast)
{
     return GTK_IS_MISC(checkcast->gtkobject);
}
  
void
Gtk_Misc::set_alignment(gfloat xalign, gfloat yalign)
{
     gtk_misc_set_alignment(GTK_MISC(gtkobject), xalign, yalign);
}
  
void
Gtk_Misc::set_padding(gint xpad, gint ypad)
{
     gtk_misc_set_padding(GTK_MISC(gtkobject), xpad, ypad);
}

Gtk_Notebook::Gtk_Notebook(void)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(gtk_notebook_new()))
{
}

Gtk_Notebook::Gtk_Notebook(GtkNotebook *castitem)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(castitem))
{
}

guint8
Gtk_Notebook::isGtkNotebook(Gtk_Object *checkcast)
{
     return GTK_IS_NOTEBOOK(checkcast->gtkobject);
}

void
Gtk_Notebook::append_page(Gtk_Widget *child, Gtk_Widget *tab_label)
{
     gtk_notebook_append_page(GTK_NOTEBOOK(gtkobject),
			      GTK_WIDGET(child->gtkobject),
			      GTK_WIDGET(tab_label->gtkobject));
}

void
Gtk_Notebook::prepend_page(Gtk_Widget *child, Gtk_Widget *tab_label)
{
     gtk_notebook_prepend_page(GTK_NOTEBOOK(gtkobject),
			       GTK_WIDGET(child->gtkobject),
			       GTK_WIDGET(tab_label->gtkobject));
}

void
Gtk_Notebook::insert_page(Gtk_Widget *child, Gtk_Widget *tab_label, gint position)
{
     gtk_notebook_insert_page(GTK_NOTEBOOK(gtkobject),
			      GTK_WIDGET(child->gtkobject),
			      GTK_WIDGET(tab_label->gtkobject), position);
}

void
Gtk_Notebook::remove_page(gint page_num)
{
     gtk_notebook_remove_page(GTK_NOTEBOOK(gtkobject), page_num);
}

gint
Gtk_Notebook::current_page(void)
{
     return gtk_notebook_current_page(GTK_NOTEBOOK(gtkobject));
}

void
Gtk_Notebook::set_page(gint page_num)
{
     gtk_notebook_set_page(GTK_NOTEBOOK(gtkobject), page_num);
}

void
Gtk_Notebook::next_page(void)
{
     gtk_notebook_next_page(GTK_NOTEBOOK(gtkobject));
}

void
Gtk_Notebook::prev_page(void)
{
     gtk_notebook_prev_page(GTK_NOTEBOOK(gtkobject));
}

void
Gtk_Notebook::set_tab_pos(GtkPositionType pos)
{
     gtk_notebook_set_tab_pos(GTK_NOTEBOOK(gtkobject), pos);
}

void
Gtk_Notebook::set_show_tabs(gint show_tabs)
{
     gtk_notebook_set_show_tabs(GTK_NOTEBOOK(gtkobject), show_tabs);
}

void
Gtk_Notebook::set_show_border(gint show_border)
{
     gtk_notebook_set_show_border(GTK_NOTEBOOK(gtkobject), show_border);
}

Gtk_OptionMenu::Gtk_OptionMenu(void)
     :     Gtk_Button::Gtk_Button(GTK_BUTTON(gtk_option_menu_new()))
{
}

Gtk_OptionMenu::Gtk_OptionMenu(GtkOptionMenu *castitem)
     :     Gtk_Button::Gtk_Button(GTK_BUTTON(castitem))
{
}

guint8
Gtk_OptionMenu::isGtkOptionMenu(Gtk_Object *checkcast)
{
     return GTK_IS_OPTION_MENU(checkcast->gtkobject);
}

Gtk_Menu *
Gtk_OptionMenu::get_menu(void)
{
     return new Gtk_Menu(GTK_MENU(gtk_option_menu_get_menu(GTK_OPTION_MENU(gtkobject))));
}

void
Gtk_OptionMenu::set_menu(Gtk_Menu *menu)
{
     gtk_option_menu_set_menu(GTK_OPTION_MENU(gtkobject), GTK_WIDGET(menu->gtkobject));
}

void
Gtk_OptionMenu::remove_menu(void)
{
     gtk_option_menu_remove_menu(GTK_OPTION_MENU(gtkobject));
}

void
Gtk_OptionMenu::set_history(gint index)
{
     gtk_option_menu_set_history(GTK_OPTION_MENU(gtkobject), index);
}

Gtk_Pixmap::Gtk_Pixmap(GdkPixmap *pixmap, GdkBitmap *mask)
     :     Gtk_Misc::Gtk_Misc(GTK_MISC(gtk_pixmap_new(pixmap, mask)))
{
}

Gtk_Pixmap::Gtk_Pixmap(GtkPixmap *castitem)
     :     Gtk_Misc::Gtk_Misc(GTK_MISC(castitem))
{
}

guint8
Gtk_Pixmap::isGtkPixmap(Gtk_Object *checkcast)
{
     return GTK_IS_PIXMAP(checkcast->gtkobject);
}

void
Gtk_Pixmap::set(GdkPixmap *pixmap, GdkBitmap *mask)
{
     gtk_pixmap_set(GTK_PIXMAP(gtkobject), pixmap, mask);
}

void
Gtk_Pixmap::get(GdkPixmap **pixmap, GdkBitmap **mask)
{
     gtk_pixmap_get(GTK_PIXMAP(gtkobject), pixmap, mask);
}

Gtk_Preview::Gtk_Preview(GtkPreviewType type)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(gtk_preview_new(type)))
{
}

Gtk_Preview::Gtk_Preview(GtkPreview *castitem)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(castitem))
{
}

guint8
Gtk_Preview::isGtkPreview(Gtk_Object *checkcast)
{
     return GTK_IS_PREVIEW(checkcast->gtkobject);
}

void
Gtk_Preview::size(gint width, gint height)
{
     gtk_preview_size(GTK_PREVIEW(gtkobject), width, height);
}

void
Gtk_Preview::put(GdkWindow *window, GdkGC *gc,
		 gint srcx, gint srcy, gint destx,
		 gint desty, gint width, gint height)
{
     gtk_preview_put(GTK_PREVIEW(gtkobject), window, gc, srcx, srcy, destx,
		     desty, width, height);
}

void
Gtk_Preview::put_row(guchar *src, guchar *dest, gint x,
		     gint y, gint w)
{
     gtk_preview_put_row(GTK_PREVIEW(gtkobject), src, dest, x, y, w);
}

void
Gtk_Preview::draw_row(guchar *data, gint x,
		      gint y, gint w)
{
     gtk_preview_draw_row(GTK_PREVIEW(gtkobject), data, x, y, w);
}

void
Gtk_Preview::set_expand(gint expand)
{
     gtk_preview_set_expand(GTK_PREVIEW(gtkobject), expand);
}

void
Gtk_Preview::set_gamma(double gamma)
{
     gtk_preview_set_gamma(gamma);
}

void
Gtk_Preview::set_color_cube(guint nred_shades, guint ngreen_shades,
			    guint nblue_shades, guint ngray_shades)
{
     gtk_preview_set_color_cube(nred_shades, ngreen_shades, nblue_shades, ngray_shades);
}

void
Gtk_Preview::set_install_cmap(gint install_cmap)
{
     gtk_preview_set_install_cmap(install_cmap);
}

void
Gtk_Preview::set_reserved(gint nreserved)
{
     gtk_preview_set_reserved(nreserved);
}

GdkVisual *
Gtk_Preview::get_visual(void)
{
     return gtk_preview_get_visual();
}

GdkColormap *
Gtk_Preview::get_cmap(void)
{
     return gtk_preview_get_cmap();
}

GtkPreviewInfo *
Gtk_Preview::get_info(void)
{
     return gtk_preview_get_info();
}

Gtk_ProgressBar::Gtk_ProgressBar(void)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(gtk_progress_bar_new()))
{
}

Gtk_ProgressBar::Gtk_ProgressBar(GtkProgressBar *castitem)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(castitem))
{
}

guint8
Gtk_ProgressBar::isGtkProgressBar(Gtk_Object *checkcast)
{
     return GTK_IS_PROGRESS_BAR(checkcast->gtkobject);
}

void
Gtk_ProgressBar::update(gfloat percentage)
{
     gtk_progress_bar_update(GTK_PROGRESS_BAR(gtkobject),
			     percentage);
}

Gtk_RadioButton::Gtk_RadioButton(GSList *group)
     :     Gtk_CheckButton::Gtk_CheckButton(GTK_CHECK_BUTTON(gtk_radio_button_new(group)))
{
}

Gtk_RadioButton::Gtk_RadioButton(GSList *group, gchar *label)
     :     Gtk_CheckButton::Gtk_CheckButton(GTK_CHECK_BUTTON(gtk_radio_button_new_with_label(group, label)))
{
}

Gtk_RadioButton::Gtk_RadioButton(GtkRadioButton *castitem)
     :     Gtk_CheckButton::Gtk_CheckButton(GTK_CHECK_BUTTON(castitem))
{
}

guint8
Gtk_RadioButton::isGtkRadioButton(Gtk_Object *checkcast)
{
     return GTK_IS_RADIO_BUTTON(checkcast->gtkobject);
}

GSList *
Gtk_RadioButton::group(void)
{
     return gtk_radio_button_group(GTK_RADIO_BUTTON(gtkobject));
}


Gtk_RadioMenuItem::Gtk_RadioMenuItem(GSList *group)
     :     Gtk_CheckMenuItem::Gtk_CheckMenuItem(GTK_CHECK_MENU_ITEM(gtk_radio_menu_item_new(group)))
{
}

Gtk_RadioMenuItem::Gtk_RadioMenuItem(GSList *group, gchar *label)
     :     Gtk_CheckMenuItem::Gtk_CheckMenuItem(GTK_CHECK_MENU_ITEM(gtk_radio_menu_item_new_with_label(group, label)))
{
}

Gtk_RadioMenuItem::Gtk_RadioMenuItem(GtkRadioMenuItem *castitem)
     :     Gtk_CheckMenuItem::Gtk_CheckMenuItem(GTK_CHECK_MENU_ITEM(castitem))
{
}

guint8
Gtk_RadioMenuItem::isGtkRadioMenuItem(Gtk_Object *checkcast)
{
     return GTK_IS_RADIO_MENU_ITEM(checkcast->gtkobject);
}

GSList *
Gtk_RadioMenuItem::group(void)
{
     return gtk_radio_menu_item_group(GTK_RADIO_MENU_ITEM(gtkobject));
}

Gtk_Range::Gtk_Range(GtkRange *castitem)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(castitem))
{
}

guint8
Gtk_Range::isGtkRange(Gtk_Object *checkcast)
{
     return GTK_IS_RANGE(checkcast->gtkobject);
}

Gtk_Adjustment *
Gtk_Range::get_adjustment(void)
{
     return new Gtk_Adjustment(GTK_ADJUSTMENT(gtk_range_get_adjustment(GTK_RANGE(gtkobject))));
}

void
Gtk_Range::set_adjustment(Gtk_Adjustment *adjustment)
{
     gtk_range_set_adjustment(GTK_RANGE(gtkobject), GTK_ADJUSTMENT(adjustment->gtkobject));
}

void
Gtk_Range::set_update_policy(GtkUpdateType policy)
{
     gtk_range_set_update_policy(GTK_RANGE(gtkobject), policy);
}

void
Gtk_Range::draw_background(void)
{
     gtk_range_draw_background(GTK_RANGE(gtkobject));
}

void
Gtk_Range::draw_trough(void)
{
     gtk_range_draw_trough(GTK_RANGE(gtkobject));
}

void
Gtk_Range::draw_slider(void)
{
     gtk_range_draw_slider(GTK_RANGE(gtkobject));
}

void
Gtk_Range::draw_step_forw(void)
{
     gtk_range_draw_step_forw(GTK_RANGE(gtkobject));
}

void
Gtk_Range::draw_step_back(void)
{
     gtk_range_draw_step_back(GTK_RANGE(gtkobject));
}

void
Gtk_Range::slider_update(void)
{
     gtk_range_slider_update(GTK_RANGE(gtkobject));
}

gint
Gtk_Range::trough_click(gint x, gint y)
{
     return gtk_range_trough_click(GTK_RANGE(gtkobject), x, y);
}

void
Gtk_Range::default_hslider_update(void)
{
     gtk_range_default_hslider_update(GTK_RANGE(gtkobject));
}

void
Gtk_Range::default_vslider_update(void)
{
     gtk_range_default_vslider_update(GTK_RANGE(gtkobject));
}

void
Gtk_Range::default_htrough_click(gint x, gint y)
{
     gtk_range_default_htrough_click(GTK_RANGE(gtkobject), x, y);
}

void
Gtk_Range::default_vtrough_click(gint x, gint y)
{
     gtk_range_default_vtrough_click(GTK_RANGE(gtkobject), x, y);
}

void
Gtk_Range::default_hmotion(gint xdelta, gint ydelta)
{
     gtk_range_default_hmotion(GTK_RANGE(gtkobject), xdelta, ydelta);
}

void
Gtk_Range::default_vmotion(gint xdelta, gint ydelta)
{
     gtk_range_default_vmotion(GTK_RANGE(gtkobject), xdelta, ydelta);
}

gfloat
Gtk_Range::calc_value(gint position)
{
     return gtk_range_calc_value(GTK_RANGE(gtkobject), position);
}

Gtk_Ruler::Gtk_Ruler(GtkRuler *castitem)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(castitem))
{
}

guint8
Gtk_Ruler::isGtkRuler(Gtk_Object *checkcast)
{
     return GTK_IS_RULER(checkcast->gtkobject);
}

void
Gtk_Ruler::set_metric(GtkMetricType metric)
{
     gtk_ruler_set_metric(GTK_RULER(gtkobject), metric);
}

void
Gtk_Ruler::set_range(gfloat lower, gfloat upper,
		     gfloat position, gfloat max_size)
{
     gtk_ruler_set_range(GTK_RULER(gtkobject), lower, upper, position, max_size);
}

void
Gtk_Ruler::draw_ticks(void)
{
     gtk_ruler_draw_ticks(GTK_RULER(gtkobject));
}

void
Gtk_Ruler::draw_pos(void)
{
     gtk_ruler_draw_pos(GTK_RULER(gtkobject));
}

Gtk_Scale::Gtk_Scale(GtkScale *castitem)
     :     Gtk_Range::Gtk_Range(GTK_RANGE(castitem))
{
}

guint8
Gtk_Scale::isGtkScale(Gtk_Object *checkcast)
{
     return GTK_IS_SCALE(checkcast->gtkobject);
}

void
Gtk_Scale::set_digits(gint digits)
{
     gtk_scale_set_digits(GTK_SCALE(gtkobject), digits);
}

void
Gtk_Scale::set_draw_value(gint draw_value)
{
     gtk_scale_set_draw_value(GTK_SCALE(gtkobject), draw_value);
}

void
Gtk_Scale::set_value_pos(GtkPositionType pos)
{
     gtk_scale_set_value_pos(GTK_SCALE(gtkobject), pos);
}

gint
Gtk_Scale::value_width(void)
{
     return gtk_scale_value_width(GTK_SCALE(gtkobject));
}

void
Gtk_Scale::draw_value(void)
{
     gtk_scale_draw_value(GTK_SCALE(gtkobject));
}

Gtk_Scrollbar::Gtk_Scrollbar(GtkScrollbar *castitem)
     :     Gtk_Range::Gtk_Range(GTK_RANGE(castitem))
{
}

guint8
Gtk_Scrollbar::isGtkScrollbar(Gtk_Object *checkcast)
{
     return GTK_IS_SCROLLBAR(checkcast->gtkobject);
}

Gtk_ScrolledWindow::Gtk_ScrolledWindow(Gtk_Adjustment *hadjustment,
				       Gtk_Adjustment *vadjustment)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(gtk_scrolled_window_new(GTK_ADJUSTMENT(hadjustment->gtkobject), GTK_ADJUSTMENT(vadjustment->gtkobject))))
{
}

Gtk_ScrolledWindow::Gtk_ScrolledWindow(GtkScrolledWindow *castitem)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(castitem))
{
}

guint8
Gtk_ScrolledWindow::isGtkScrolledWindow(Gtk_Object *checkcast)
{
     return GTK_IS_SCROLLED_WINDOW(checkcast->gtkobject);
}

Gtk_Adjustment *
Gtk_ScrolledWindow::get_hadjustment(void)
{
     return new Gtk_Adjustment(GTK_ADJUSTMENT(gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(gtkobject))));
}

Gtk_Adjustment *
Gtk_ScrolledWindow::get_vadjustment(void)
{
     return new Gtk_Adjustment(GTK_ADJUSTMENT(gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(gtkobject))));
}

void
Gtk_ScrolledWindow::set_policy(GtkPolicyType hscrollbar_policy,
			       GtkPolicyType vscrollbar_policy)
{
     gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gtkobject),
				    hscrollbar_policy,
				    vscrollbar_policy);
}

Gtk_Separator::Gtk_Separator(GtkSeparator *castitem)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(castitem))
{
}

guint8
Gtk_Separator::isGtkSeparator(Gtk_Object *checkcast)
{
     return GTK_IS_SEPARATOR(checkcast->gtkobject);
}

Gtk_Table::Gtk_Table(gint rows, gint columns, gint homogeneous)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(gtk_table_new(rows, columns, homogeneous)))
{
}

Gtk_Table::Gtk_Table(GtkTable *castitem)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(castitem))
{
}

guint8
Gtk_Table::isGtkTable(Gtk_Object *checkcast)
{
     return GTK_IS_TABLE(checkcast->gtkobject);
}

void
Gtk_Table::attach(Gtk_Widget *child,
		  gint left_attach,
		  gint right_attach,
		  gint top_attach,
		  gint bottom_attach,
		  gint xoptions, gint yoptions,
		  gint xpadding, gint ypadding)
{
     gtk_table_attach(GTK_TABLE(gtkobject), GTK_WIDGET(child->gtkobject),
		      left_attach, right_attach, top_attach, bottom_attach,
		      xoptions, yoptions, xpadding, ypadding);
}

void
Gtk_Table::attach_defaults(Gtk_Widget *child,
			   gint left_attach,
			   gint right_attach,
			   gint top_attach,
			   gint bottom_attach)
{
     gtk_table_attach_defaults(GTK_TABLE(gtkobject), GTK_WIDGET(child->gtkobject), left_attach, right_attach, top_attach, bottom_attach);
}

void
Gtk_Table::set_row_spacing(gint row, gint spacing)
{
     gtk_table_set_row_spacing(GTK_TABLE(gtkobject), row, spacing);
}

void
Gtk_Table::set_col_spacing(gint row, gint spacing)
{
     gtk_table_set_col_spacing(GTK_TABLE(gtkobject), row, spacing);
}

void
Gtk_Table::set_row_spacings(gint spacing)
{
     gtk_table_set_row_spacings(GTK_TABLE(gtkobject), spacing);
}

void
Gtk_Table::set_col_spacings(gint spacing)
{
     gtk_table_set_col_spacings(GTK_TABLE(gtkobject), spacing);
}

Gtk_Text::Gtk_Text(Gtk_Adjustment *hadj, Gtk_Adjustment *vadj)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(gtk_text_new(GTK_ADJUSTMENT(hadj->gtkobject), GTK_ADJUSTMENT(vadj->gtkobject))))
{
}

Gtk_Text::Gtk_Text(GtkText *castitem)
     :     Gtk_Widget::Gtk_Widget(GTK_WIDGET(castitem))
{
}

guint8
Gtk_Text::isGtkText(Gtk_Object *checkcast)
{
     return GTK_IS_TEXT(checkcast->gtkobject);
}

void
Gtk_Text::set_editable(gint editable)
{
     gtk_text_set_editable(GTK_TEXT(gtkobject), editable);
}

void
Gtk_Text::set_adjustments(Gtk_Adjustment *hadj, Gtk_Adjustment *vadj)
{
     gtk_text_set_adjustments(GTK_TEXT(gtkobject),
			      GTK_ADJUSTMENT(hadj->gtkobject),
			      GTK_ADJUSTMENT(vadj->gtkobject));
}

void
Gtk_Text::set_point(guint index)
{
     gtk_text_set_point(GTK_TEXT(gtkobject), index);
}

guint
Gtk_Text::get_point(void)
{
     return gtk_text_get_point(GTK_TEXT(gtkobject));
}

guint
Gtk_Text::get_length(void)
{
     return gtk_text_get_length(GTK_TEXT(gtkobject));
}

void
Gtk_Text::freeze(void)
{
     gtk_text_freeze(GTK_TEXT(gtkobject));
}

void
Gtk_Text::thaw(void)
{
     gtk_text_thaw(GTK_TEXT(gtkobject));
}

void
Gtk_Text::insert(GdkFont *font, GdkColor *fore, GdkColor *back,
		 const char *chars, gint length)
{
     gtk_text_insert(GTK_TEXT(gtkobject), font, fore, back, chars, length);
}

gint
Gtk_Text::backward_delete(guint nchars)
{
     return gtk_text_backward_delete(GTK_TEXT(gtkobject), nchars);
}

gint
Gtk_Text::forward_delete(guint nchars)
{
     return gtk_text_foreward_delete(GTK_TEXT(gtkobject), nchars);
}

Gtk_ToggleButton::Gtk_ToggleButton(void)
     :     Gtk_Button::Gtk_Button(GTK_BUTTON(gtk_toggle_button_new()))
{
}

Gtk_ToggleButton::Gtk_ToggleButton(gchar *label)
     :     Gtk_Button::Gtk_Button(GTK_BUTTON(gtk_toggle_button_new_with_label(label)))
{
}

Gtk_ToggleButton::Gtk_ToggleButton(GtkToggleButton *castitem)
     :     Gtk_Button::Gtk_Button(GTK_BUTTON(castitem))
{
}

guint8
Gtk_ToggleButton::isGtkToggleButton(Gtk_Object *checkcast)
{
     return GTK_IS_TOGGLE_BUTTON(checkcast->gtkobject);
}

void
Gtk_ToggleButton::set_mode(gint draw_indicator)
{
     gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(gtkobject), draw_indicator);
}

void
Gtk_ToggleButton::set_state(gint state)
{
     gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(gtkobject), state);
}

void
Gtk_ToggleButton::toggled(void)
{
     gtk_toggle_button_toggled(GTK_TOGGLE_BUTTON(gtkobject));
}

Gtk_Tooltips::Gtk_Tooltips(void)
{
     gtktooltips = gtk_tooltips_new();
}

Gtk_Tooltips::Gtk_Tooltips(GtkTooltips *castitem)
{
     gtktooltips = castitem;
}

Gtk_Tooltips::~Gtk_Tooltips(void)
{
     gtk_tooltips_destroy(gtktooltips);
}

void
Gtk_Tooltips::enable(void)
{
     gtk_tooltips_enable(gtktooltips);
}

void
Gtk_Tooltips::disable(void)
{
     gtk_tooltips_disable(gtktooltips);
}

void
Gtk_Tooltips::set_delay(gint delay)
{
     gtk_tooltips_set_delay(gtktooltips, delay);
}

void
Gtk_Tooltips::set_tips(Gtk_Widget *widget, gchar *tips_text)
{
     gtk_tooltips_set_tips(gtktooltips, GTK_WIDGET(widget->gtkobject), tips_text);
}

void
Gtk_Tooltips::set_colors(GdkColor *background, GdkColor *foreground)
{
     gtk_tooltips_set_colors(gtktooltips, background, foreground);
}

Gtk_Tree::Gtk_Tree(void)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(gtk_tree_new()))
{
}

Gtk_Tree::Gtk_Tree(GtkTree *castitem)
     :     Gtk_Container::Gtk_Container(GTK_CONTAINER(castitem))
{
}

guint8
Gtk_Tree::isGtkTree(Gtk_Object *checkcast)
{
     return GTK_IS_TREE(checkcast->gtkobject);
}

void
Gtk_Tree::append(Gtk_Widget *widget)
{
     gtk_tree_append(GTK_TREE(gtkobject),
		     GTK_WIDGET(widget->gtkobject));
}

void
Gtk_Tree::prepend(Gtk_Widget *widget)
{
     gtk_tree_prepend(GTK_TREE(gtkobject),
		      GTK_WIDGET(widget->gtkobject));
}

void
Gtk_Tree::insert(Gtk_Widget *widget, gint position)
{
     gtk_tree_insert(GTK_TREE(gtkobject),
		     GTK_WIDGET(widget->gtkobject),
		     position);
}

Gtk_TreeItem::Gtk_TreeItem(void)
     :     Gtk_Item::Gtk_Item(GTK_ITEM(gtk_tree_item_new()))
{
}

Gtk_TreeItem::Gtk_TreeItem(gchar *label)
     :     Gtk_Item::Gtk_Item(GTK_ITEM(gtk_tree_item_new_with_label(label)))
{
}

Gtk_TreeItem::Gtk_TreeItem(GtkTreeItem *castitem)
     :     Gtk_Item::Gtk_Item(GTK_ITEM(castitem))
{
}

guint8
Gtk_TreeItem::isGtkTreeItem(Gtk_Object *checkcast)
{
     return GTK_IS_TREE_ITEM(checkcast->gtkobject);
}

void
Gtk_TreeItem::set_subtree(Gtk_Tree *subtree)
{
     gtk_tree_item_set_subtree(GTK_TREE_ITEM(gtkobject), GTK_WIDGET(subtree->gtkobject));
}

void
Gtk_TreeItem::select(void)
{
     gtk_tree_item_select(GTK_TREE_ITEM(gtkobject));
}

void
Gtk_TreeItem::deselect(void)
{
     gtk_tree_item_deselect(GTK_TREE_ITEM(gtkobject));
}

void
Gtk_TreeItem::expand(void)
{
     gtk_tree_item_expand(GTK_TREE_ITEM(gtkobject));
}

void
Gtk_TreeItem::collapse(void)
{
     gtk_tree_item_collapse(GTK_TREE_ITEM(gtkobject));
}

Gtk_Viewport::Gtk_Viewport(Gtk_Adjustment *hadjustment, Gtk_Adjustment *vadjustment)
     :     Gtk_Bin::Gtk_Bin(GTK_BIN(gtk_viewport_new(GTK_ADJUSTMENT(hadjustment->gtkobject),GTK_ADJUSTMENT(vadjustment->gtkobject))))
{
}

Gtk_Viewport::Gtk_Viewport(GtkViewport *castitem)
     :     Gtk_Bin::Gtk_Bin(GTK_BIN(castitem))
{
}

guint8
Gtk_Viewport::isGtkViewport(Gtk_Object *checkcast)
{
     return GTK_IS_VIEWPORT(checkcast->gtkobject);
}

Gtk_Adjustment *
Gtk_Viewport::get_hadjustment(void)
{
     return new Gtk_Adjustment(GTK_ADJUSTMENT(gtk_viewport_get_hadjustment(GTK_VIEWPORT(gtkobject))));
}

Gtk_Adjustment *
Gtk_Viewport::get_vadjustment(void)
{
     return new Gtk_Adjustment(GTK_ADJUSTMENT(gtk_viewport_get_vadjustment(GTK_VIEWPORT(gtkobject))));
}

void
Gtk_Viewport::set_hadjustment(Gtk_Adjustment *adjustment)
{
     gtk_viewport_set_hadjustment(GTK_VIEWPORT(gtkobject), GTK_ADJUSTMENT(adjustment->gtkobject));
}

void
Gtk_Viewport::set_vadjustment(Gtk_Adjustment *adjustment)
{
     gtk_viewport_set_vadjustment(GTK_VIEWPORT(gtkobject), GTK_ADJUSTMENT(adjustment->gtkobject));
}

void
Gtk_Viewport::set_shadow_type(GtkShadowType type)
{
     gtk_viewport_set_shadow_type(GTK_VIEWPORT(gtkobject), type);
}

Gtk_Window::Gtk_Window(GtkWindowType type = GTK_WINDOW_TOPLEVEL)
     : Gtk_Bin::Gtk_Bin(GTK_BIN(gtk_window_new(type)))
{
}

Gtk_Window::Gtk_Window(GtkWindow *castitem)
     : Gtk_Bin(GTK_BIN(castitem))
{
}

guint8
Gtk_Window::isGtkWindow(Gtk_Object *checkcast)
{
     return GTK_IS_WINDOW(checkcast->gtkobject);
}

void
Gtk_Window::set_title(gchar *title)
{
     gtk_window_set_title(GTK_WINDOW(gtkobject), title);
}

void
Gtk_Window::set_focus(Gtk_Widget *focus)
{
     gtk_window_set_focus(GTK_WINDOW(gtkobject), GTK_WIDGET(focus->gtkobject));
}

void
Gtk_Window::set_default(Gtk_Widget *defaultw)
{
     gtk_window_set_default(GTK_WINDOW(gtkobject), GTK_WIDGET(defaultw->gtkobject));
}

void
Gtk_Window::set_policy(gint allow_shrink,
		       gint allow_grow,
		       gint auto_shrink)
{
     gtk_window_set_policy(GTK_WINDOW(gtkobject),
			   allow_shrink,
			   allow_grow,
			   auto_shrink);
}

void
Gtk_Window::add_accelerator_table(Gtk_AcceleratorTable *table)
{
     gtk_window_add_accelerator_table(GTK_WINDOW(gtkobject), table->gtkacceleratortable);
}

void
Gtk_Window::remove_accelerator_table(Gtk_AcceleratorTable *table)
{
     gtk_window_remove_accelerator_table(GTK_WINDOW(gtkobject), table->gtkacceleratortable);
}

void
Gtk_Window::position(GtkWindowPosition position)
{
     gtk_window_position(GTK_WINDOW(gtkobject), position);
}

#ifdef TEST
int main(int argc, char *argv[])
{
     Gtk_Main myapp(&argc, &argv);
}
#endif
