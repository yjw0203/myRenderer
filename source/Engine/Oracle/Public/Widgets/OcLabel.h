#pragma once
#include "Engine/Oracle/Public/Widgets/OcPanel.h"

class OcLabel : public OcWidget
{
public:
    virtual void OnComputeDesiredSize(float ScaleMultiplier) override;
    virtual void OnPaint(OcacleDrawElementList& DrawElements, const OracleLayout& Layout) override;

    struct Argument : OcWidgetArgument
    {
        Argument& Style(OcFontStyleHandle& style) { ((OcLabel*)m_widget.get())->m_font_style = style; return *this; }
        Argument& Text(const std::string& text) { ((OcLabel*)m_widget.get())->m_text = text; return *this; }
    };

private:
    std::string m_text;
    OcFontStyleHandle m_font_style{};
};
