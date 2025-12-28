#pragma once
#include "Engine/Math/Public/Colour.h"
#include "Engine/Oracle/Public/Layout/Margin.h"
#include "Engine/Oracle/Public/Render/DrawElements/OracleDrawElement.h"

class OracleTextDrawElement : public OracleDrawElement
{
public:
    RegisteOracleType(OracleTextDrawElement);

    std::string m_text;
    LinearColour m_colour;
    std::string m_font_path;
    int m_size;

    void SetText(const std::string& text)
    {
        m_text = text;
    }

    void SetStyle(const OracleFontStyle* Style)
    {
        m_colour = Style->m_colour;
        m_font_path = Style->m_font_path;
        m_size = Style->m_size;
    }
};
