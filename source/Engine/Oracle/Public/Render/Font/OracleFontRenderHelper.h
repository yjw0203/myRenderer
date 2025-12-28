#pragma once
#include <string>
#include "Engine/RHI/Public/rpi/yjw_rpi.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Engine/Math/Public/Math.h"
#include <map>

class OracleFontLibrary
{
public:
    struct GlyphInfo 
    {
        uint32_t unicode = 0;
        int width = 0;
        int height = 0;
        int bearingX = 0;
        int bearingY = 0;
        int advance = 0;
        std::vector<uint8_t> bitmap;
    };

    static void StaticInitialize();
    static void StaticCleanup();
    static FT_Library s_library;

    OracleFontLibrary(const std::string& font_path);
    ~OracleFontLibrary();
    bool SetFontSize(int font_size);
    GlyphInfo LoadGlyph(uint32_t unicode);

private:
    FT_Face m_face = nullptr;
    bool m_loaded = false;
};

struct OracleFontRenderGlyphInfo
{
    Vector2 uv_offset;
    Vector2 uv_size;
};

struct OracleGlyphRenderInfo
{
    uint32_t unicode;
    Vector2 uv_offset;
    Vector2 uv_size;
    int width = 0;
    int height = 0;
    int bearingX = 0;
    int bearingY = 0;
    int advance = 0;
};

class OracleFontRenderHelper
{
public:
    OracleFontRenderHelper();
    ~OracleFontRenderHelper();
    void RegisterChar(OracleGlyphRenderInfo& out_glyph_info, uint32_t unicode, int size, const std::string& font_path = "C:/Windows/Fonts/simhei.ttf");
    void RegisterText(std::vector<OracleGlyphRenderInfo>& out_glyph_infos, const std::string& text, int size, const std::string& font_path = "C:/Windows/Fonts/simhei.ttf");
    rpi::RPIBuffer GetGlyphInfoBuffer();
    rpi::RPITexture GetFontTexture();
private:
    OracleFontLibrary* GetOrLoadFontLibrary(const std::string& font_path);
    void UnloadAllFontLibrarys();

    void UploadGlyphToTexture(const std::vector<uint8_t>& bitmap, int width, int height, Vector2& out_uv_offset, Vector2& out_uv_size);
private:
    std::map<std::string, OracleFontLibrary*> m_font_librarys;

    std::vector<OracleFontRenderGlyphInfo> m_glyph_info;
    rpi::RPIBuffer m_glyph_info_buffer;

    std::map<std::tuple<uint32_t, int, std::string>, OracleGlyphRenderInfo> m_glyph_infos;

    const int m_font_texture_width = 2048;
    const int m_font_texture_height = 2048;
    rpi::RPITexture m_font_texture;
    int m_current_arrange_x = 0;
    int m_current_arrange_y = 0;
    int m_current_line_height = 0;
};