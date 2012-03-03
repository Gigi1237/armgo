#include <string.h>
#include <malloc.h>
#include "window.h"

GTKWINDOW window;

// 字符模式下刷新函数
static gboolean refreshWindow(GtkWidget * widget)
{
  if( !window.mode && window.refresh)
  {
    gtk_text_buffer_set_text(window.buffer, window.charBuffer, window.count);
    window.refresh = 0;
  }
  return TRUE;
}


// 图形模式下刷新函数
static gboolean windowExpose(GtkWidget * da, GdkEventExpose * event, gpointer data )
{

  //GdkGC * gc1, * gc2, * gc;
  //GdkColor color;

  //gc1 = gdk_gc_new( da->window );
  //color.red = 30000;
  //color.green = 0;
  //color.blue = 30000;
  //gdk_gc_set_rgb_fg_color(gc1, &color );

  //gc2 = gdk_gc_new( da->window );
  //color.red = 65535;
  //color.green = 0;
  //color.blue = 65535;
  //gdk_gc_set_rgb_fg_color( gc2, &color );

  //gc = gc1;
  //gdk_draw_rectangle( da->window, gc, TRUE,/*fill or not */ 5, 5, 100, 100 );

  //gc = gc2;
  //gdk_draw_point( da->window, gc, 10,10 );


  //gc = gc2;
  //gdk_draw_arc( da->window, gc, FALSE, /*don't fill*/ 105, 5, 100, 100, 0, 60 * 64 );

  //PangoLayout *layout = gtk_widget_create_pango_layout( da, "I'm a demon\nfrom hell.\nhahaha." );
  //PangoFontDescription *fontdesc = pango_font_description_from_string( "Luxi Mono 12" );
  //pango_layout_set_font_description( layout, fontdesc );
  //gdk_draw_layout( da->window, gc1, 5, 105, layout );
  //pango_font_description_free( fontdesc );
  //g_object_unref( layout );

  //g_object_unref( G_OBJECT(gc1) );
  //g_object_unref( G_OBJECT(gc2) );

  int x, y;
  GdkGC * gc;
  GdkColor color;

  gc = gdk_gc_new( da->window );

  for(x=0; x<window.currentX; ++x)
  {
    for(y=0; y<window.currentY; ++y)
    {
      color.red   = window.pixel[x][y].r;
      color.green = window.pixel[x][y].g;
      color.blue  = window.pixel[x][y].b;
      gdk_gc_set_rgb_fg_color(gc, &color);

      gdk_draw_point( da->window, gc, x, y);
    }
  }

  g_object_unref( G_OBJECT(gc) );

  return TRUE;
}


void createWindow()
{
  gtk_init(&armgo.argc, &armgo.argv);

  window.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window.window), "Armgo LCD");
  gtk_window_set_default_size(GTK_WINDOW(window.window), 500, 500);
  gtk_window_set_position(GTK_WINDOW(window.window), GTK_WIN_POS_CENTER);
  g_signal_connect(G_OBJECT(window.window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  if(window.mode == 0)
  {
    window.scrolled = gtk_scrolled_window_new(NULL,NULL); /*创建滚动窗口构件*/
    gtk_container_add(GTK_CONTAINER(window.window), window.scrolled);/*将滚动窗口构件加入窗体*/
    gtk_widget_show(window.scrolled);/*显示滚动窗口构件*/

    window.buffer = gtk_text_buffer_new(NULL);/*创建文本框的缓冲区*/
    window.textView = gtk_text_view_new_with_buffer(window.buffer);/*创建文本框构件*/
    gtk_container_add(GTK_CONTAINER(window.scrolled), window.textView);

    g_timeout_add(200, (GSourceFunc)refreshWindow, (gpointer)window.window);
  }
  else
  {
	window.drawArea = gtk_drawing_area_new();
	/* set a minimum size */
	gtk_widget_set_size_request(window.drawArea, 300, 300 );

	gtk_container_add( GTK_CONTAINER(window.window), window.drawArea );

	g_signal_connect(window.drawArea, "expose_event", G_CALLBACK(windowExpose), NULL);

	//temp!
	int x, y;
	int sx = 65535 / window.currentX;
	int sy = 65535 / window.currentY;
	for(x=0; x<window.currentX; ++x)
	{
	  for(y=0; y<window.currentY; ++y)
	  {
	    window.pixel[x][y].r = sx * x;
	    window.pixel[x][y].g = sx * y;
	    window.pixel[x][y].b = sx * x;
	  }
	}
  }

  gtk_widget_show_all(window.window);


  gtk_main();
}


// mode=0 字符模式   mode=1 图形模式
int initWindow(int mode)
{
  window.charBuffer = (char *)malloc(WINDOWBUFFERSIZE);
  memset(window.charBuffer, 0, WINDOWBUFFERSIZE);
  window.count = 0;
  window.refresh = 0;
  window.mode = mode;
  window.currentX = 300;
  window.currentY = 300;

  if(pthread_create(&window.id, NULL, (void *)createWindow, NULL))
  {
	return 1;
  }

  return 0;
}
