#pragma once
#include "Engine/Math/Public/Colour.h"
#include "Engine/Oracle/Public/Layout/Margin.h"
#include "Engine/Oracle/Public/Render/DrawElements/OracleDrawElement.h"

class OracleBoxDrawElement : public OracleDrawElement, public OracleBackgroundColourAttribute, public OracleImageAttribute
{
public:
    Margin m_margin;

    void SetStyle(const OracleStyle* Style)
    {
        OracleBackgroundColourAttribute::SetStyle(Style);
        OracleImageAttribute::SetStyle(Style);

        m_margin = Style->m_margin;
    }
};
