#pragma once
#include "Engine/Oracle/Public/Layout/Margin.h"
#include "Engine/Math/Public/Colour.h"
#include "Engine/Oracle/Public/Render/OracleRenderResource.h"
#include "Engine/Oracle/Public/Utils/Utils.h"

class OracleStyle
{
public:

    Margin m_margin;

    LinearColour m_background_color;

    OracleRenderResource m_resource;
};

class OracleFontStyle
{
public:
    LinearColour m_colour = LinearColour(1, 0, 0, 1);
    std::string m_font_path = "C:/Windows/Fonts/simhei.ttf";
    int m_size = 12;
};

using OcStyleHandle = OcPtr<OracleStyle>;
using OcFontStyleHandle = OcPtr<OracleFontStyle>;