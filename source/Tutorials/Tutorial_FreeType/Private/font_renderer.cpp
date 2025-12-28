#include "font_renderer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <codecvt>
#include <locale>

ChineseFontRenderer::ChineseFontRenderer() {
    initFreeType();
}

ChineseFontRenderer::~ChineseFontRenderer() {
    cleanup();
}

void ChineseFontRenderer::initFreeType() {
    FT_Error error = FT_Init_FreeType(&library_);
    if (error) {
        std::cerr << "Failed to initialize FreeType library: " << error << std::endl;
    }
}

bool ChineseFontRenderer::loadFont(const std::string& fontPath, int fontSize) {
    if (!library_) {
        std::cerr << "FreeType library not initialized" << std::endl;
        return false;
    }

    if (face_) {
        FT_Done_Face(face_);
        face_ = nullptr;
    }

    FT_Error error = FT_New_Face(library_, fontPath.c_str(), 0, &face_);
    if (error) {
        std::cerr << "Failed to load font: " << fontPath
            << " (Error: " << error << ")" << std::endl;
        return false;
    }

    fontSize_ = fontSize;
    error = FT_Set_Pixel_Sizes(face_, 0, fontSize_);
    if (error) {
        std::cerr << "Failed to set font size: " << fontSize << std::endl;
        FT_Done_Face(face_);
        face_ = nullptr;
        return false;
    }

    fontLoaded_ = true;

    // Print font info
    std::cout << "Font loaded: " << fontPath << std::endl;
    std::cout << "Font name: " << face_->family_name << " " << face_->style_name << std::endl;
    std::cout << "Font size: " << fontSize_ << "px" << std::endl;
    std::cout << "Num glyphs: " << face_->num_glyphs << std::endl;

    return true;
}

bool ChineseFontRenderer::loadGlyph(uint32_t unicode) {
    if (!face_) return false;

    FT_UInt glyph_index = FT_Get_Char_Index(face_, unicode);
    if (glyph_index == 0) {
        return false;
    }

    FT_Error error = FT_Load_Glyph(face_, glyph_index, FT_LOAD_RENDER);
    if (error) {
        return false;
    }

    return true;
}

ChineseFontRenderer::GlyphInfo ChineseFontRenderer::renderGlyph(uint32_t unicode) {
    GlyphInfo info;
    info.unicode = unicode;

    if (!loadGlyph(unicode)) {
        return info;
    }

    FT_GlyphSlot glyph = face_->glyph;
    FT_Bitmap* bitmap = &glyph->bitmap;

    info.width = bitmap->width;
    info.height = bitmap->rows;
    info.bearingX = glyph->bitmap_left;
    info.bearingY = glyph->bitmap_top;
    info.advance = glyph->advance.x >> 6;

    info.bitmap.resize(static_cast<size_t>(bitmap->width) * bitmap->rows);
    for (int y = 0; y < bitmap->rows; ++y) {
        for (int x = 0; x < bitmap->width; ++x) {
            size_t index = static_cast<size_t>(y) * bitmap->width + x;
            info.bitmap[index] = bitmap->buffer[y * bitmap->pitch + x];
        }
    }

    return info;
}

std::u32string ChineseFontRenderer::utf8ToUtf32(const std::string& utf8) {
    try {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        return converter.from_bytes(utf8);
    }
    catch (const std::range_error&) {
        return std::u32string();
    }
}

std::string ChineseFontRenderer::utf32ToUtf8(const std::u32string& utf32) {
    try {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        return converter.to_bytes(utf32);
    }
    catch (const std::range_error&) {
        return std::string();
    }
}

ChineseFontRenderer::TextBitmap ChineseFontRenderer::renderUTF8(const std::string& utf8Text, int padding) {
    TextBitmap result;

    if (!fontLoaded_ || !face_) {
        return result;
    }

    std::u32string utf32 = utf8ToUtf32(utf8Text);
    if (utf32.empty()) {
        return result;
    }

    int totalWidth = 0;
    int maxHeight = 0;
    int maxAscent = 0;
    int maxDescent = 0;

    std::vector<GlyphInfo> glyphs;

    for (char32_t ch : utf32) {
        GlyphInfo glyph = renderGlyph(ch);
        if (glyph.width > 0 && glyph.height > 0) {
            glyphs.push_back(glyph);

            totalWidth += glyph.advance;
            maxHeight = std::max(maxHeight, glyph.height);
            maxAscent = std::max(maxAscent, glyph.bearingY);
            maxDescent = std::max(maxDescent, glyph.height - glyph.bearingY);
        }
        else {
            glyph.width = fontSize_ / 2;
            glyph.height = maxHeight;
            glyph.advance = fontSize_ / 2;
            glyphs.push_back(glyph);
            totalWidth += glyph.advance;
        }
    }

    if (glyphs.empty()) {
        return result;
    }

    result.width = totalWidth + padding * 2;
    result.height = maxAscent + maxDescent + padding * 2;
    result.data.resize(static_cast<size_t>(result.width) * result.height, 0);

    int x = padding;
    int baseline = padding + maxAscent;

    for (const auto& glyph : glyphs) {
        if (glyph.bitmap.empty()) {
            x += glyph.advance;
            continue;
        }

        int glyphX = x + glyph.bearingX;
        int glyphY = baseline - glyph.bearingY;

        for (int y = 0; y < glyph.height; ++y) {
            for (int x2 = 0; x2 < glyph.width; ++x2) {
                int srcIndex = y * glyph.width + x2;
                int dstX = glyphX + x2;
                int dstY = glyphY + y;

                if (dstX >= 0 && dstX < result.width &&
                    dstY >= 0 && dstY < result.height) {
                    size_t dstIndex = static_cast<size_t>(dstY) * result.width + dstX;
                    result.data[dstIndex] = std::max(result.data[dstIndex], glyph.bitmap[srcIndex]);
                }
            }
        }

        x += glyph.advance;
    }

    return result;
}

ChineseFontRenderer::TextBitmap ChineseFontRenderer::renderText(const std::string& text, int padding) {
    return renderUTF8(text, padding);
}

void ChineseFontRenderer::getTextSize(const std::string& text, int& width, int& height) {
    TextBitmap bitmap = renderText(text, 0);
    width = bitmap.width;
    height = bitmap.height;
}

bool ChineseFontRenderer::saveAsPNM(const TextBitmap& bitmap, const std::string& filename) {
    if (bitmap.data.empty() || bitmap.width <= 0 || bitmap.height <= 0) {
        return false;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file << "P5\n";
    file << bitmap.width << " " << bitmap.height << "\n";
    file << "255\n";

    file.write(reinterpret_cast<const char*>(bitmap.data.data()), bitmap.data.size());

    file.close();
    return true;
}

void ChineseFontRenderer::setFontSize(int size) {
    if (face_ && size > 0) {
        fontSize_ = size;
        FT_Set_Pixel_Sizes(face_, 0, fontSize_);
    }
}

void ChineseFontRenderer::cleanup() {
    if (face_) {
        FT_Done_Face(face_);
        face_ = nullptr;
    }
    if (library_) {
        FT_Done_FreeType(library_);
        library_ = nullptr;
    }
    fontLoaded_ = false;
}
