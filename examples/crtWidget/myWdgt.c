#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

char hello[] = {"Hello, World."};
char hi[]    = {"Hi!"};
GC  btgc;
unsigned long gcvm = 0;
XGCValues gcv;
char *bttxt;
int bth, btw;

Window MyCreateSimpleButton(Display *dspl, Window prt, GC mygc, 
                        int x, int y, int w, int h, char *txt){
  Window mw;
  Font font;
  
  mw  = XCreateSimpleWindow(dspl, prt, x, y, w, h, 3, 0x00ccde, 0xcccccc);
  gcv.foreground = 0xabcdef;
  gcv.background = 0x612345;
  gcv.font = XLoadFont (dspl, "9x15");
  XGrabButton(dspl, AnyButton, AnyModifier, mw, True,
	      ButtonPressMask | ButtonReleaseMask,
	      GrabModeAsync, GrabModeAsync, None, None);
  bttxt = (char *)malloc(sizeof(char)*strlen(txt)+1);
  sprintf(bttxt,"%s", txt);
  bth=h; btw=w;
  btgc = XCreateGC(dspl,RootWindow(dspl, DefaultScreen(dspl)) ,
		   GCFont | GCForeground | GCBackground , &gcv);
  XMapRaised(dspl, mw);
  
  XDrawImageString(dspl, mw, btgc, 22, (h+10)/2, txt, strlen(txt));
  return mw;
}

int ButtonPressed(Display *dspl, Window mw){
  XSetWindowBackground(dspl, mw, 0x040404);
  XClearWindow(dspl, mw);
  XSetBackground(dspl, btgc, 0xabcdef);
  XSetForeground(dspl, btgc, 0x612345);
  XDrawImageString(dspl, mw, btgc, 22, (bth+10)/2, bttxt, strlen(bttxt));
}

int ButtonReleased(Display *dspl, Window mw){
  XSetWindowBackground(dspl, mw, 0xcccccc);
  XClearWindow(dspl, mw);
  XSetBackground(dspl, btgc, 0x612345);
  XSetForeground(dspl, btgc, 0xabcdef);
  XDrawImageString(dspl, mw, btgc, 22, (bth+10)/2, bttxt, strlen(bttxt));
}

int main(int argc, char **argv){  
  Display  *mydisplay; 
  Window   mywindow, mybt;   
  GC       mygc;       
  XEvent   myevent;    
  KeySym   mykey;      
                       
  XSizeHints myhint;   
                       
  int      myscreen;     
  unsigned long myforeground, mybackground;

  int      i;
  char     text[10];
  int      done;      
  
  XSetWindowAttributes  myattr;   
  unsigned long         mymask=0; 
  
  mydisplay = XOpenDisplay(""); 
  
  myscreen     = DefaultScreen(mydisplay);
  mybackground = WhitePixel(mydisplay, myscreen);
  myforeground = BlackPixel(mydisplay, myscreen);

  myhint.x = 200;     myhint.y = 300; 
  myhint.width = 350; myhint.height = 250; 
  myhint.flags = PPosition | PSize;
  
  mywindow = XCreateSimpleWindow(mydisplay,
				 DefaultRootWindow(mydisplay), 
				 myhint.x, myhint.y,
				 myhint.width, myhint.height,
				 5, myforeground, mybackground);
  
  XSetStandardProperties(mydisplay, mywindow, hello, hello,
			 None, argv, argc, &myhint);

  mygc = XCreateGC(mydisplay, mywindow, 0, 0);
  XSetBackground(mydisplay, mygc, mybackground);
  XSetForeground(mydisplay, mygc, myforeground);
  XMapRaised(mydisplay, mywindow);

  XSelectInput(mydisplay, mywindow,
	       ButtonPressMask | KeyPressMask | ExposureMask);
  done = 0;
  while (done == 0){
    XNextEvent (mydisplay, &myevent);
    if (myevent.type == Expose) done =1;
  }
  
  mybt = MyCreateSimpleButton(mydisplay, mywindow, mygc, 70, 70, 160, 60, "Neko besedilo");

  done = 0;
  while (done == 0) {  /* Zanka dogodkov */
    XNextEvent (mydisplay, &myevent);  /* naslednji dogodek */
    switch (myevent.type) {
    case KeyPress:     /* dogodek s tipkovnice */
      i = XLookupString(&myevent.xkey, text, 10, &mykey, NULL);
      if (i == 1 && text[0] == 'q') done = 1;
      break;
    case Expose:       /* dogodek ob zagonu programa */
      if (myevent.xexpose.count == 0)
	XDrawImageString(
			 myevent.xexpose.display,
			 myevent.xexpose.window,
			 mygc, 50, 50,            /* kam v oknu zapise tekst */
			 hello, strlen(hello) );
      break;
    case ButtonPress:    
      if (myevent.xbutton.window == mywindow){
	XDrawImageString(myevent.xbutton.display,
			 myevent.xbutton.window,
			 mygc, myevent.xbutton.x, myevent.xbutton.y,
			 hi, strlen(hi));
      }
      else {	
	ButtonPressed(mydisplay, mybt);
      }
      break;
    case ButtonRelease:
      if (myevent.xbutton.window == mybt){
	ButtonReleased(mydisplay, mybt);
      }
      break;
    } /* switch */
  } /* while */

  /* Razkroji graficni kontekst */
  XFreeGC(mydisplay,mygc);
  /* Razkroji okno */
  XDestroyWindow(mydisplay, mywindow);
  /* Prekine povezavo s streznikom X */
  XCloseDisplay(mydisplay);
  
  return 0;
} /* main */
