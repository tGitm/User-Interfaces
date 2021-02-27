/* Deklaracije potrebne za definicijo podatkov */
/* za uporabo z zahtevki okenskega sistema X */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <malloc.h>
#include <X11/X.h>
#include <X11/Xatom.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* Deklaracije */
char hello[] = {"Hello, World."};
char hi[]    = {"Hi!"};
#define  BEST 1

int main(int argc, char **argv)
{

Display  *mydisplay; /* Struktura nadzoruje povezavo s prikazovalnikom */
Window   mywindow;   /* Okno, ki ga aplikacija kreira */
GC       mygc;       /* Graficni kontekst */
XEvent   myevent;    /* Unija struktur za razlicne tipe dogodkov */
                     /* Postaja (dogodek), sistem X, opis dogodka */
XSizeHints myhint;   /* Tu proces posreduje informacijo o oknu */
                     /* upravniku oken */
int      myscreen;   /* Stevilka zaslona */
unsigned long myforeground, mybackground;
                     /* Barvi ospredja in ozadja za risanje */
int      i;
char     text[10];
int      done;       /* */

/* Relevantne spremenljivke, strukture in kazalci */
XSetWindowAttributes  myattr;   /* Atributi okna */
unsigned long         mymask=0; /* Maska za atribute */
unsigned long pixel, whitepixel, blackpixel;
unsigned long r,g,b;            /* Barve */
Status        result;
Visual        *myvisual;        /* Zaslon */
XStandardColormap stdcolor;     /* Iskalna tabela s paleto */
int           depth, format, bitmap_pad;
XImage        *image;           /* Slika v spominu */
int           wwidth=256, wheight=256;
unsigned int  width=256, height=256;
int           j, k, l=0, jprev, status;

int Draw_Image(int l)
  {
    /* ??????????????????? */

  }

/* Inicializacija */

if (BEST == 1) 
  system("xstdcmap -best");
 else
  system("xstdcmap -default");

mydisplay = XOpenDisplay(""); /* Inicializira povezavo med */
                              /* aplikacijo in X streznikom, shrani */
                              /* opis povezave. Vzame ime prikazovalnika */

                     /* Kateri zaslon je v uporabi */
myscreen     = DefaultScreen(mydisplay);
mybackground = WhitePixel(mydisplay, myscreen);
myforeground = BlackPixel(mydisplay, myscreen);
                     /* Barve za risanje ozadja */
                     /* tock, teksta in robov */

         /* Kam ? levo zgoraj */
myhint.x = wwidth;     myhint.y = wheight; 
         /* Sirina, dolzina */
myhint.width = width; myhint.height = height; 
         /* Katere komponente v strukturi XSizeHints */
         /* se uporabljajo. Program izbira pozicijo */
         /* in velikost okna */
myhint.flags = PPosition | PSize;

/* Zahteva postaji, da zgradi okno aplikacije */
/* Okno je zgrajeno v oknu, ki pokriva cel ekran */
/* Rob okna je sirok pet tock in crn */
mywindow = XCreateSimpleWindow(mydisplay,
                     DefaultRootWindow(mydisplay), /* predhodnik */
                     myhint.x, myhint.y, myhint.width, myhint.height,
                     5, myforeground, mybackground);

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

/* Globina okna, stevilo bitov na piksel (8) */
depth        = DefaultDepth(mydisplay, myscreen);

/* Informacija o barvnih sposobnostih zaslona */
myvisual     = DefaultVisual(mydisplay, myscreen);

printf("\nAfter DefaultVisual:\n");
/* Tri barve v iskalni tabeli, paleta se lahko dinamicno spreminja */
if      (myvisual->class == PseudoColor)
         printf("PseudoColor, %d\n",PseudoColor);

/* Tri barve, paleta se ne more spreminjati */
else if (myvisual->class == StaticColor)
         printf("StaticColor, %d\n",StaticColor);

/* Kot PseudoColor, le da prikazuje sive nivoje */
else if (myvisual->class == GrayScale) 
         printf("GrayScale, %d\n",GrayScale);

/* Sivih nivojev ni mozno spreminjati */
else if (myvisual->class == StaticGray)
         printf("StaticGray, %d\n",StaticGray);

/* Tri locene palete v iskalni tabeli, R, G, B */
else if (myvisual->class == DirectColor)
         printf("DirectColor, %d\n",DirectColor);

/* Palet R, G, B ni mozno spreminjati */
else if (myvisual->class == TrueColor) 
         printf("TrueColor, %d\n",TrueColor);
/*
printf("%d %d %d %d %d %d\n",
         myvisual->class,
         myvisual->red_mask,
         myvisual->green_mask,
         myvisual->blue_mask,
         myvisual->bits_per_rgb,
         myvisual->map_entries);
*/

/* Izbira ene od standardnih palet (DEFAULT, BEST (R(8), G(8), B(8))*/
/* RED, GREEN, BLUE, GRAY) */
if (BEST == 1)
    result = XGetStandardColormap(mydisplay, 
          RootWindow(mydisplay,DefaultScreen(mydisplay)),
          &stdcolor,
          XA_RGB_BEST_MAP);
 else 
   result = XGetStandardColormap(mydisplay, 
          RootWindow(mydisplay,DefaultScreen(mydisplay)),
          &stdcolor,
          XA_RGB_DEFAULT_MAP); 

/* Kako so razporejene barvne celice v paleti */
printf("\nAfter XGetStandardColormap:\n");
printf("Base pixel: %ld\nRed_max:    %ld Red_mult:   %ld\n",
       stdcolor.base_pixel, stdcolor.red_max, stdcolor.red_mult);
printf("Green_max:  %ld Green_mult: %ld\n",
           stdcolor.green_max, stdcolor.green_mult);
printf("Blue_max:   %ld Blue_mult:  %ld\n",
           stdcolor.blue_max, stdcolor.blue_mult);
/* base_pixel, red_mult, green_mult, blue_mult: */
/* Konstante za izracun vrednosti piksla pri dani RGB kombinaciji */

/* Barva ozadja */
whitepixel   = stdcolor.base_pixel +
               (stdcolor.red_max * stdcolor.red_mult ) +
               (stdcolor.green_max * stdcolor.green_mult ) +
               (stdcolor.blue_max * stdcolor.blue_mult );

/* Standardna paleta in barva ozadja */
myattr.background_pixel = whitepixel;
myattr.backing_store = Always;

/* Popravljanje atributov okna */
mymask = CWBackPixel|CWBackingStore;
//mymask = CWBackPixel;
XChangeWindowAttributes(mydisplay,mywindow,mymask,&myattr);

mymask = CWColormap;
myattr.colormap = stdcolor.colormap;
XConfigureWindow(mydisplay,mywindow,mymask,(XWindowChanges *)&myattr);

/* Inicializacija slike */
format       = ZPixmap; /* Slika je organizirana kot polje pikslov */
bitmap_pad   = 8;       /* Kako zacne vsaka vrstica slike v pomnilniku */

/* Gradnja slike */
image        = XCreateImage(mydisplay, myvisual,
               depth, format, 0, 0, width, height,
               bitmap_pad, 0);
if (image == 0) { printf("%d\n No image", 1); return 1; }

/* Rezervacija spomina */
image->data = malloc(image->bytes_per_line * height);
if (image->data == 0) { printf("%d No buffer\n", 2); return 2; }

/* Bela barva - barva ozadja */
 for ( j = 0 ; j < 256 ; j ++ ) {
   for ( k = 0 ; k < 256 ; k ++ )  {
     status = XPutPixel( image, j, k, mybackground);
    }
}


XPutImage(mydisplay, mywindow, mygc, image, 0, 0, 0, 0, width, height);

done = 0;
while (done == 0) {
   XNextEvent (mydisplay, &myevent);  /* naslednji dogodek */
      switch (myevent.type) {
         case KeyPress:     /* dogodek s tipkovnice */
             done = 1;
             break;
         case Expose:       /* dogodek ob zagonu programa */
            if (myevent.xexpose.count == 0)
	      Draw_Image(l); /* ????????????????? */
	    XPutImage(mydisplay,mywindow,mygc,image,0,0,0,0,width,height);
         break;
          case ButtonPress:    /* dogodek z miske */
	    l+=1;
	    if (l>=256) l=(l%256);

	    Draw_Image(l); /* ????????????????? */

	    XPutImage(mydisplay,mywindow,mygc,image,0,0,0,0,width,height);

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





