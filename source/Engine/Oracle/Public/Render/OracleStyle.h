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

using OcStyleHandle = OcPtr<OracleStyle>;