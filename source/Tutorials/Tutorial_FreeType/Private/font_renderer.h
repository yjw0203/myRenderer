// Chinese Font Renderer for FreeType
#pragma once

#ifndef CHINESE_FONT_RENDERER_H
#define CHINESE_FONT_RENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

class ChineseFontRenderer {
public:
    struct TextBitmap {
        int width = 0;
        int height = 0;
        std::vector<uint8_t> data;
    };

    struct GlyphInfo {
        uint32_t unicode = 0;
        int width = 0;
        int height = 0;
        int bearingX = 0;
        int bearingY = 0;
        int advance = 0;
        std::vector<uint8_t> bitmap;
    };

    ChineseFontRenderer();
    ~ChineseFontRenderer();

    bool loadFont(const std::string& fontPath, int fontSize = 24);
    TextBitmap renderText(const std::string& text, int padding = 2);
    TextBitmap renderUTF8(const std::string& utf8Text, int padding = 2);
    GlyphInfo renderGlyph(uint32_t unicode);
    void getTextSize(const std::string& text, int& width, int& height);
    bool saveAsPNM(const TextBitmap& bitmap, const std::string& filename);
    void setFontSize(int size);

    static std::u32string utf8ToUtf32(const std::string& utf8);
    static std::string utf32ToUtf8(const std::u32string& utf32);

private:
    FT_Library library_ = nullptr;
    FT_Face face_ = nullptr;
    int fontSize_ = 24;
    bool fontLoaded_ = false;

    void initFreeType();
    void cleanup();
    bool loadGlyph(uint32_t unicode);
};

#endif // CHINESE_FONT_RENDERER_H