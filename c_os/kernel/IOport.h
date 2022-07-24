/*****************************************************************************
 *                                                                           *
 *                               I O P O R T                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Diese Klasse dient dem Zugriff auf die Ein-/Ausgabe      *
 *                  Ports des PCs. Beim PC gibt es einen gesonderten I/O-    *
 *                  Adressraum, der nur mittels der Maschineninstruktionen   *
 *                  'in' und 'out' angesprochen werden kann. Ein IOport-     *
 *                  Objekt wird beim Erstellen an eine Adresse des I/O-      *
 *                  Adressraums gebunden und kann dann fuer byte- oder       *
 *                  wortweise Ein- oder Ausgaben verwendet werden.           *       
 *                                                                           *
 * Autor:           Michael Schoettner, 28.8.2016                            *
 *****************************************************************************/
#ifndef IOport_include__
#define IOport_include__

class IOport {
private:
    // 16-Bit Adresse im I/O-Adressraum
    const unsigned short address;

public:
    // Konstruktor, speichert Port-Adresse
    explicit IOport(unsigned short a) : address(a) {};

    // Byteweise Ausgabe eines Wertes ueber einen I/O-Port.
    void outb(unsigned char val) const {
        asm volatile("outb %0, %1"
                     :
                     : "a"(val), "Nd"(address));
    }

    // NOTE: I added this for easier init of COM1 port
    void outb(unsigned char offset, unsigned char val) const {
        asm volatile("outb %0, %1"
                     :
                     : "a"(val), "Nd"(static_cast<unsigned short>(address + offset)));
    }

    // Wortweise Ausgabe eines Wertes ueber einen I/O-Port.
    void outw(unsigned short val) const {
        asm volatile("outw %0, %1"
                     :
                     : "a"(val), "Nd"(address));
    }

    // 32-Bit Ausgabe eines Wertes ueber einen I/O-Port.
    void outdw(unsigned int val) const {
        asm volatile("outl %0, %1"
                     :
                     : "a"(val), "Nd"(address));
    }

    // Byteweises Einlesen eines Wertes ueber einen I/O-Port.
    unsigned char inb() const {
        unsigned char ret;

        asm volatile("inb %1, %0"
                     : "=a"(ret)
                     : "Nd"(address));
        return ret;
    }

    // NOTE: I added this for COM1 port
    unsigned char inb(unsigned char offset) const {
        unsigned char ret;

        asm volatile("inb %1, %0"
                     : "=a"(ret)
                     : "Nd"(static_cast<unsigned short>(address + offset)));
        return ret;
    }

    // Wortweises Einlesen eines Wertes ueber einen I/O-Port.
    unsigned short inw() const {
        unsigned short ret;

        asm volatile("inw %1, %0"
                     : "=a"(ret)
                     : "Nd"(address));
        return ret;
    }

    // 32-Bit Einlesen eines Wertes ueber einen I/O-Port.
    unsigned int indw() const {
        unsigned int ret;

        asm volatile("inl %1, %0"
                     : "=a"(ret)
                     : "Nd"(address));
        return ret;
    }
};

#endif
