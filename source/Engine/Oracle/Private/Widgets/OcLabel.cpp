#include "Engine/Oracle/Public/Widgets/OcLabel.h"

void OcLabel::OnComputeDesiredSize(float ScaleMultiplier)
{

}

void OcLabel::OnPaint(OcacleDrawElementList& DrawElements, const OracleLayout& Layout)
{
    OracleDrawElement::MakeText(DrawElements, m_text, m_font_style.get() ? *m_font_style.get() : OracleFontStyle(), Layout);
}