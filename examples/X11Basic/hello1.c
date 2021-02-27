/* Deklaracije potrebne za definicijo podatkov */
/* za uporabo z zahtevki okenskega sistema X */
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>

/* Deklaracije */
char hello[] = {"Hello, World."};
char hi[]    = {"Hi!"};


int main(int argc, char **argv)
{

Display  *mydisplay; /* Struktura nadzoruje povezavo s prikazovalnikom */
Window   mywindow;   /* Okno, ki ga aplikacija kreira */
GC       mygc;       /* Graficni kontekst */
XEvent   myevent;    /* Unija struktur za razlicne tipe dogodkov */
                     /* Postaja (dogodek), sistem X, opis dogodka */
KeySym   mykey;      /* Enolicno opise znak po "dogodku" */
                     /* s tipkovnice */
XSizeHints myhint;   /* Tu proces posreduje informacijo o oknu */
                     /* upravniku oken */
int      myscreen;   /* Stevilka zaslona */
unsigned long myforeground, mybackground;
                     /* Barvi ospredja in ozadja za risanje */
int      i;
char     text[10];
int      done;       /* */

/* Inicializacija */

XSetWindowAttributes attributes;

mydisplay = XOpenDisplay(""); /* Inicializira povezavo med */
                              /* aplikacijo in streznikom X, shrani */
                              /* opis povezave. Vzame ime prikazovalnika */

                    /* Kateri zaslon je v uporabi */
myscreen     = DefaultScreen(mydisplay);
mybackground = WhitePixel(mydisplay, myscreen);
myforeground = BlackPixel(mydisplay, myscreen);
                     /* Barve za risanje ozadja */
                     /* tock, teksta in robov */

         /* Kam ? levo zgoraj */
myhint.x = 200;     myhint.y = 300; 
         /* Sirina, dolzina */
myhint.width = 400; myhint.height = 250; 
         /* Katere komponente v strukturi XSizeHints */
         /* se uporabljajo. Program izbira pozicijo */
         /* in velikost okna */
myhint.flags = PPosition | PSize;

/* Zahteva postaji, da zgradi okno aplikacije */
/* Okno je zgrajeno v oknu, ki pokriva cel ekran */
/* Rob okna je sirok pet tock in crn */
mywindow = XCreateSimpleWindow(mydisplay,
                     DefaultRootWindow(mydisplay), /* Predhodnik */
                     myhint.x, myhint.y, myhint.width, myhint.height,
                     5, myforeground, mybackground);

attributes.backing_store = Always ;
XChangeWindowAttributes (mydisplay,mywindow,CWBackingStore,&attributes);

/* Informacija o na novo zgrajenem oknu za upravnika oken */
XSetStandardProperties(mydisplay, mywindow, hello, hello,
                       0, argv, argc, &myhint);

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
while (done == 0) {
   XNextEvent (mydisplay, &myevent);  /* naslednji dogodek */
      switch (myevent.type) {
         case KeyPress:     /* dogodek s tipkovnice */
             done = 1;
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
	    /* XDrawImageString(
                   myevent.xbutton.display,
                   myevent.xbutton.window,
                   mygc, myevent.xbutton.x, myevent.xbutton.y,
                   hi, strlen(hi));*/
	    /*Draw_Trig_Func();*/
	    for (i=0; i<360; i++) /*fprintf(stderr,"%lf %d %f\n",sin((double)i*3.14/180.0),i,(float)i*3.14/180.0);*/
	    XDrawPoint (mydisplay,mywindow,mygc,i+10,((int)( sin((double)i*3.14/180.0)*25.0))+50);
             break;
	  } /* switch */
 } /* while */

/* Razkroji graficni kontekst */
XFreeGC(mydisplay,mygc);
/* Razkroji okno */
XDestroyWindow(mydisplay, mywindow);
/* Prekine zvezo z X streznikom */
XCloseDisplay(mydisplay);

return 0;

} /* main */

