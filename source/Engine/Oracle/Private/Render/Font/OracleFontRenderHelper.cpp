#include "Engine/Oracle/Public/Render/Font/OracleFontRenderHelper.h"
#include <codecvt>
#include <locale>
#include <iostream>


std::u32string utf8ToUtf32(const std::string& utf8) {
    try {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        return converter.from_bytes(utf8);
    }
    catch (const std::range_error&) {
        return std::u32string();
    }
}

FT_Library OracleFontLibrary::s_library = nullptr;

void OracleFontLibrary::StaticInitialize()
{
    FT_Error error = FT_Init_FreeType(&s_library);
    if (error) {
        std::cout << "Failed to initialize FreeType library: " << error << std::endl;
    }
}

void OracleFontLibrary::StaticCleanup()
{
    if (s_library)
    {
        FT_Done_FreeType(s_library);
        s_library = nullptr;
    }
}

OracleFontLibrary::OracleFontLibrary(const std::string& font_path)
{
    FT_Error error = FT_New_Face(s_library, font_path.c_str(), 0, &m_face);
    if (error) {
        std::cerr << "Failed to load font: " << font_path
            << " (Error: " << error << ")" << std::endl;
        m_loaded = false;
        return;
    }

    m_loaded = true;
}

OracleFontLibrary::~OracleFontLibrary()
{
    if (m_face) {
        FT_Done_Face(m_face);
        m_face = nullptr;
    }
}

bool OracleFontLibrary::SetFontSize(int font_size)
{
    FT_Error error = FT_Set_Pixel_Sizes(m_face, 0, font_size);
    if (error) {
        std::cerr << "Failed to set font size: " << font_size << std::endl;
        return false;
    }
    return true;
}

OracleFontLibrary::GlyphInfo OracleFontLibrary::LoadGlyph(uint32_t unicode)
{
    OracleFontLibrary::GlyphInfo info{};
    if (!m_face) return info;

    FT_UInt glyph_index = FT_Get_Char_Index(m_face, unicode);
    if (glyph_index == 0) {
        return info;
    }

    FT_Error error = FT_Load_Glyph(m_face, glyph_index, FT_LOAD_RENDER);
    if (error) {
        return info;
    }

    FT_GlyphSlot glyph = m_face->glyph;
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

OracleFontRenderHelper::OracleFontRenderHelper()
{
    OracleFontLibrary::StaticInitialize();
    int max_glyph = 1000; // to be remove
    m_glyph_info_buffer = rpi::RPICreateUploadBuffer(max_glyph * sizeof(OracleFontRenderGlyphInfo));
    m_font_texture = rpi::RPICreateDefaultTexture2D(m_font_texture_width, m_font_texture_height, rpi::RPIFormat::R8_unorm);
}

OracleFontRenderHelper::~OracleFontRenderHelper()
{
    m_font_texture.Release();
    m_glyph_info_buffer.Release();
    UnloadAllFontLibrarys();
    OracleFontLibrary::StaticCleanup();
}

OracleFontLibrary* OracleFontRenderHelper::GetOrLoadFontLibrary(const std::string& font_path)
{
    if (m_font_librarys.count(font_path))
    {
        return m_font_librarys[font_path];
    }
    return m_font_librarys[font_path] = new OracleFontLibrary(font_path);
}

void OracleFontRenderHelper::UnloadAllFontLibrarys()
{
    for (auto itr : m_font_librarys)
    {
        if (itr.second)
        {
            delete itr.second;
        }
    }
    m_font_librarys.clear();
}

void OracleFontRenderHelper::UploadGlyphToTexture(const std::vector<uint8_t>& bitmap, int width, int height, Vector2& out_uv_offset, Vector2& out_uv_size)
{
    m_current_arrange_x += 1;
    if (m_current_arrange_x + width + 1 >= m_font_texture_width)
    {
        m_current_arrange_y += 1 + m_current_line_height;
        m_current_arrange_x = 0;
        m_current_line_height = 0;
    }

    int offset[2] = { m_current_arrange_x , m_current_arrange_y };
    int extend[2] = { width, height };
    m_font_texture.GetTexture()->Update((void*)bitmap.data(), (int)bitmap.size()
        , rhi::RHIOffset3D{ m_current_arrange_x ,m_current_arrange_y ,0 }, rhi::RHIExtent3D{ width ,height,1 });

    out_uv_offset.x = (float)m_current_arrange_x / m_font_texture_width;
    out_uv_offset.y = (float)m_current_arrange_y / m_font_texture_height;
    out_uv_size.x = (float)width / m_font_texture_width;
    out_uv_size.y = (float)height / m_font_texture_height;

    m_current_arrange_x += width;
    m_current_line_height = std::max(m_current_line_height, height);
}

void OracleFontRenderHelper::RegisterChar(OracleGlyphRenderInfo& out_glyph_info, uint32_t unicode, int size, const std::string& font_path)
{
    std::tuple<uint32_t, int, std::string> glyph_id = std::tuple<uint32_t, int, std::string>(unicode, size, font_path);
    if (m_glyph_infos.count(glyph_id))
    {
        out_glyph_info = m_glyph_infos[glyph_id];
        return ;
    }

    OracleFontLibrary* library = GetOrLoadFontLibrary(font_path);
    library->SetFontSize(size);
    OracleFontLibrary::GlyphInfo info = library->LoadGlyph(unicode);

    OracleGlyphRenderInfo render_info{};
    render_info.unicode = unicode;
    render_info.width = info.width;
    render_info.height = info.height;
    render_info.advance = info.advance;
    render_info.bearingX = info.bearingX;
    render_info.bearingY = info.bearingY;
    if (info.bitmap.size() > 0)
    {
        UploadGlyphToTexture(info.bitmap, info.width, info.height, render_info.uv_offset, render_info.uv_size);
    }
    out_glyph_info = m_glyph_infos[glyph_id] = render_info;
}

void OracleFontRenderHelper::RegisterText(std::vector<OracleGlyphRenderInfo>& out_glyph_infos, const std::string& text, int size, const std::string& font_path)
{
    std::u32string utf32 = utf8ToUtf32(text);
    
    OracleFontLibrary* library = GetOrLoadFontLibrary(font_path);
    library->SetFontSize(size);

    for (char32_t ch : utf32) 
    {
        OracleGlyphRenderInfo info{};
        RegisterChar(info, ch, size, font_path);
        out_glyph_infos.push_back(info);
    }
}

rpi::RPIBuffer OracleFontRenderHelper::GetGlyphInfoBuffer()
{
    return m_glyph_info_buffer;
}

rpi::RPITexture OracleFontRenderHelper::GetFontTexture()
{
    return m_font_texture;
}