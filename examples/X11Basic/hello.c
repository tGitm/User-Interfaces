#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

char hello[] = {"Hello, World."};
char hi[]    = {"Hi!"};

int main(int argc, char **argv)
{  
  Display  *mydisplay; /* Struktura nadzoruje povezavo s prikazovalnikom */
  Window   mywindow;   /* Okno, ki ga aplikacija zgradi */
  GC       mygc;       /* Graficni kontekst */
  XEvent   myevent;    /* Unija struktur za razlicne tipe dogodkov */
  KeySym   mykey;      /* Enolicno opise znak po "dogodku" */
                       /* s tipkovnice */
  XSizeHints myhint;   /* Tu proces posreduje informacijo o oknu */
                       /* upravniku oken */
int      myscreen;     /* stevilka zaslona */
unsigned long myforeground, mybackground;
                       /* Barvi ospredja in ozadja za risanje */
int      i;
char     text[10];
int      done;      

XSetWindowAttributes  myattr;   /* Atributi okna */
unsigned long         mymask=0; /* Maska za atribute */

/* Inicializacija */
mydisplay = XOpenDisplay(""); /* Inicializira povezavo med */
                              /* aplikacijo in streznikom X, shrani */
                              /* opis povezave, vzame ime prikazovalnika */

/* Kateri zaslon je v uporabi */
myscreen     = DefaultScreen(mydisplay);
mybackground = WhitePixel(mydisplay, myscreen);
myforeground = BlackPixel(mydisplay, myscreen);
                     /* Barve za risanje ozadja */
                     /* tock, teksta in robov */

/* Kam ? levo zgoraj */
myhint.x = 200;     myhint.y = 300; 
/* Sirina, dolzina */
myhint.width = 350; myhint.height = 250; 

/* Katere komponente v strukturi XSizeHints */
/* se uporabljajo. Program izbira pozicijo */
/* in velikost okna */
myhint.flags = PPosition | PSize;

/* Zahteva postaji, da zgradi okno aplikacije */
/* Okno je zgrajeno v osnovnem oknu */
/* Rob okna je sirok pet tock in crn */
mywindow = XCreateSimpleWindow(mydisplay,
                     DefaultRootWindow(mydisplay), /* predhodnik */
                     myhint.x, myhint.y, myhint.width, myhint.height,
                     5, myforeground, mybackground);

/* Informacija o na novo zgrajenem oknu za upravnika oken */
XSetStandardProperties(mydisplay, mywindow, hello, hello,
                       None, argv, argc, &myhint);

/* Zgradi graficni kontekst */
mygc = XCreateGC(mydisplay, mywindow, 0, 0);
XSetBackground(mydisplay, mygc, mybackground);
XSetForeground(mydisplay, mygc, myforeground);

/* Program zeli biti obvescen o vsakem dogodku z miske, */
/* tipkovnice, ali ob osvezevanju zaslona */
XSelectInput(mydisplay, mywindow,
              ( ButtonPressMask | KeyPressMask | ExposureMask));

/* Prikaz okna na zaslon */
XMapRaised(mydisplay, mywindow);

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
         case ButtonPress:    /* dogodek z miske */
              XDrawImageString(
                   myevent.xbutton.display,
                   myevent.xbutton.window,
                   mygc, myevent.xbutton.x, myevent.xbutton.y,
                   hi, strlen(hi));
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
