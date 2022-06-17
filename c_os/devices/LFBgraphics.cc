/*****************************************************************************
 *                                                                           *
 *                            L F B G R A P H I C S                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Zeichenfunktionen fuer Grafikmodi, die auf einem         *
 *                  linearen Framebuffer basieren. Verwendet in VESA und     *
 *                  QemuVGA.                                                 *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 19.9.2016                       *
 *                  Der Code fuer das Zeichnen der Linie ist von Alan Wolfe  *
 *         https://blog.demofox.org/2015/01/17/bresenhams-drawing-algorithms *
 *****************************************************************************/

#include "devices/LFBgraphics.h"
#include "kernel/Globals.h"


/* Hilfsfunktionen */
void swap(unsigned int *a, unsigned int *b);
int abs(int a);


/*****************************************************************************
 * Methode:         LFBgraphics::drawMonoBitmap                              *
 *---------------------------------------------------------------------------*
 * Parameter:       x,y     Startpunkt ab dem Text ausgegeben wird.          *
 *                  width   Breite in Pixel                                  *
 *                  height  Hoehe in Pixel                                   *
 *                  bitmap  Zeiger auf Pixel der monochromen Rastergrafik    *
 *                  col     Farbe der Pixel                                  *
 *                                                                           *
 * Beschreibung:    Gibt die gegebene monochrome Rastergrafik an der Position*
 *                  x,y zeilenweise aus. (x,y) ist der linke obere Punkt;    *
 *                  ist in der bitmap eine '1', so wird ein Pixel mit der    *
 *                  Farbe col ausgegeben, ansonsten bei '0' nichts.          *
 *                  Diese Funktion basiert auf dem Format der Fonts, welche  *
 *                  mit cpi2fnt (AmigaOS) erzeugt wurden. Das Format erklaert*
 *                  sich in den C-Dateien in fonts/ von selbst.              *
 *****************************************************************************/
inline void LFBgraphics::drawMonoBitmap( unsigned int x, unsigned int y,
                                         unsigned int width,
                                         unsigned int height,
                                         unsigned char* bitmap, 
                                         unsigned int color) {
    // Breite in Bytes
    unsigned short width_byte = width/8 + ((width%8 != 0) ? 1 : 0);
    
    for(unsigned int yoff=0; yoff<height; ++yoff) {
        int xpos=x;
        int ypos=y+yoff;
        for(unsigned int xb=0; xb < width_byte; ++xb) {
            for( int src=7; src>=0; --src) {
          		if ((1 << src) & *bitmap) {
                   drawPixel(xpos, ypos, color);
                }
                xpos++;
            }
            bitmap++;
        }
    }
}


/*****************************************************************************
 * Methode:         LFBgraphics::drawString                                  *
 *---------------------------------------------------------------------------*
 * Parameter:       fnt     Schrift                                          *
 *                  x,y     Startpunkt ab dem Text ausgegeben wird.          *
 *                  col     Farbe des Textes                                 *
 *                  str     Zeiger auf Zeichenkette                          *
 *                  len     Laenge der Zeichenkette                          *
 *                                                                           *
 * Beschreibung:    Gibt eine Zeichenkette mit gewaehlter Schrift an der     *
 *                  Position x,y aus.                                        *
 *****************************************************************************/
void LFBgraphics::drawString(Font &fnt, unsigned int x, unsigned int y,  
                             unsigned int col, char* str, unsigned int len) {
    unsigned int i;
        
    for(i = 0; i < len; ++i) {
		drawMonoBitmap(x, y, fnt.get_char_width(), fnt.get_char_height(),
                       fnt.getChar( *(str+i) ), col);
        x += fnt.get_char_width();
    }
}


/*****************************************************************************
 * Methode:         LFBgraphics::drawPixel                                   *
 *---------------------------------------------------------------------------*
 * Parameter:       x, y    Koordinaten des Pixels                           *
 *                  col     Farbe                                            *
 *                                                                           *
 * Beschreibung:    Zeichnen eines Pixels.                                   *
 *****************************************************************************/
void LFBgraphics::drawPixel(unsigned int x, unsigned int y,unsigned int col) {
    unsigned char *ptr = (unsigned char *) lfb;
    
    if (hfb == 0 || lfb == 0) return ;

    if (mode == 0) ptr = (unsigned char *) hfb;
    
    // Pixel ausserhalb des sichtbaren Bereichs?
    if (x<0 || x>=xres || y<0 || y>yres)
        return;
    
    // Adresse des Pixels berechnen und Inhalt schreiben
    switch (bpp) {
        case 8:
            ptr += (x+y*xres);
            *ptr = col;
            return;
        case 15:
        case 16:
            ptr += (2*x+2*y*xres);
            *ptr = col;
            return;
        case 24:
            ptr += (3*x+3*y*xres);
            *ptr = (col & 0xFF); ptr++;
            *ptr = ((col>>8) & 0xFF); ptr++;
            *ptr = ((col>>16) & 0xFF); ptr++;
            return;
        case 32:
            ptr += (4*x+4*y*xres);
            *ptr = (col & 0xFF); ptr++;
            *ptr = ((col>>8) & 0xFF); ptr++;
            *ptr = ((col>>16) & 0xFF); ptr++;
            return;
    }
}


/*****************************************************************************
 * Methode:         LFBgraphics::clear                                       *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Bildschirm loeschen.                                     *
 *****************************************************************************/
void LFBgraphics::clear() {
    unsigned int *ptr = (unsigned int *)lfb;
    unsigned int i;

    
    if (hfb == 0 || lfb == 0) return ;

    if (mode == 0) ptr = (unsigned int *) hfb;
    
    switch (bpp) {
        case 8:
            for (i=0; i < ((xres/4)*yres); i++)
               *(ptr++) = 0;
            return;
        case 15:
        case 16:
            for (i=0; i < (2*(xres/4)*yres); i++)
               *(ptr++) = 0;
            return;
        case 24:
            for (i=0; i < (3*(xres/4)*yres); i++)
               *(ptr++) = 0;
            return;
        case 32:
            for (i=0; i < (4*(xres/4)*yres); i++)
               *(ptr++) = 0;
            return;
    }
}


/*****************************************************************************
 * Methode:         LFBgraphics::setDrawingBuff                              *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Stellt ein, ob in den sichtbaren Puffer gezeichnet wird. *
 *****************************************************************************/
void LFBgraphics::setDrawingBuff(int v) 
{
   mode = v;
}


/*****************************************************************************
 * Methode:         LFBgraphics::copyHiddenToVisible                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Kopiert den versteckten Puffer in den sichtbaren LFB.    *
 *****************************************************************************/
void LFBgraphics::copyHiddenToVisible() {
    unsigned int *sptr = (unsigned int *)hfb;
    unsigned int *dptr = (unsigned int *)lfb;
    unsigned int i;
    
    if (hfb == 0 || lfb == 0) return ;

    switch (bpp) {
        case 8:
            for (i=0; i < ((xres/4)*yres); i++)
               *(dptr++) = *(sptr++);
            return;
        case 15:
        case 16:
            for (i=0; i < (2*(xres/4)*yres); i++)
               *(dptr++) = *(sptr++);
            return;
        case 24:
            for (i=0; i < (3*(xres/4)*yres); i++)
               *(dptr++) = *(sptr++);
            return;
        case 32:
            for (i=0; i < (4*(xres/4)*yres); i++)
               *(dptr++) = *(sptr++);
            return;
    }
}



void swap(unsigned int *a, unsigned int *b) {
   int h;
   
   h = *a;
   
   *a = *b;
   *b = h;
}

int abs(int a) {
	if (a<0) return -a;
	return a;
}
