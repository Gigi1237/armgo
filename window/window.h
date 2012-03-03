


#ifndef WINDOW_H_
#define WINDOW_H_

#include <gtk-2.0/gtk/gtk.h>
#include <pthread.h>
#include "common.h"


#define WINDOWBUFFERSIZE 10000
#define WINDOWGRAPHICXMAX 300
#define WINDOWGRAPHICYMAX 300

typedef struct
{
  int r;
  int g;
  int b;
}WINDOWPIXEL;

typedef struct
{
  pthread_t id;        //线程ID
  int refresh;         //是否要刷新屏幕

  GtkWidget * window;

  /* 字符模式 */
  GtkWidget * textView;
  GtkWidget * scrolled;
  GtkTextBuffer * buffer;
  char * charBuffer;   //字符缓冲区
  int count;           //字符缓冲区中已有字符个数

  /* 图形模式 */
  GtkWidget * drawArea;
  WINDOWPIXEL pixel[WINDOWGRAPHICXMAX][WINDOWGRAPHICYMAX];
  int currentX;         //当前屏幕长(pixel)
  int currentY;         //当前屏幕宽(pixel)

  int mode;

}GTKWINDOW;



extern GTKWINDOW window;



int initWindow(int mode);


#endif
