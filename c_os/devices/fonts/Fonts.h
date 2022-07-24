// Jakob Falke, oostubs
// Github: https://gitlab.cs.fau.de/um15ebek/oostubs

// Schriften in Form von Rastergrafiken (separate Datein)
// Generiert mit cpi2fnt
// Keine Proportionalschriften
// Monochrome Speicherung: 1 Bit pro Pixel
// Je nach Breite wird auf Bytegrenzen aufgerundet:
//  8 Pixel -> 1 Byte; 12 Pixel -> 2 Byte

#ifndef FONTS_H__
#define FONTS_H__

#include "user/lib/Array.h"

class Font {
public:
    virtual ~Font() = default;

    virtual const unsigned char* getChar(int c) const = 0;
    virtual unsigned int get_char_width() const = 0;
    virtual unsigned int get_char_height() const = 0;
};

template<unsigned int width, unsigned int height, const unsigned char* data>
class FontInstance : public Font {
    const unsigned int char_width;
    const unsigned int char_height;
    const unsigned int char_mem_size;
    const unsigned char* font_data;

public:
    FontInstance() : char_width(width), char_height(height), char_mem_size((((char_width + (8 >> 1)) / 8) * char_height)), font_data(data) {}

    inline const unsigned char* getChar(int c) const override {
        return &font_data[char_mem_size * c];
    }
    inline unsigned int get_char_width() const override {
        return char_width;
    }
    inline unsigned int get_char_height() const override {
        return char_height;
    }
};

extern const unsigned char fontdata_8x16[];
extern const unsigned char fontdata_8x8[];
extern const unsigned char acorndata_8x8[];
extern const unsigned char fontdata_pearl_8x8[];
extern const unsigned char fontdata_sun_12x22[];
extern const unsigned char fontdata_sun_8x16[];

using Font_8x16 = FontInstance<8, 16, fontdata_8x16>;
using Font_8x8 = FontInstance<8, 8, fontdata_8x8>;
using Font_acorn_8x8 = FontInstance<8, 8, acorndata_8x8>;
using Font_pearl_8x8 = FontInstance<8, 8, fontdata_pearl_8x8>;
using Font_sun_12x22 = FontInstance<12, 22, fontdata_sun_12x22>;
using Font_sun_8x16 = FontInstance<8, 16, fontdata_sun_8x16>;

extern const Font_8x16 std_font_8x16;
extern const Font_8x8 std_font_8x8;
extern const Font_acorn_8x8 acorn_font_8x8;
extern const Font_pearl_8x8 pearl_font_8x8;
extern const Font_sun_12x22 sun_font_12x22;
extern const Font_sun_8x16 sun_font_8x16;

#endif
