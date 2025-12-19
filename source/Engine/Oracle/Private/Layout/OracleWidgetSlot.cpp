#include "Engine/Oracle/Public/Layout/OracleWidgetSlot.h"

OracleWidgetSlot::OracleWidgetSlot(OcWidget* parent_widget)
{
    m_parent_widget = parent_widget;
}

OracleWidgetSlot::~OracleWidgetSlot()
{

}

void OracleWidgetSlot::SetSlotWidget(OcWidgetHandle widget)
{
    m_widget = widget;
}

OcWidgetHandle OracleWidgetSlot::GetSlotWidget()
{
    return m_widget;
}

bool OracleWidgetSlot::IsBounding()
{
    return m_widget.get() != nullptr;
}

OcWidget* OracleWidgetSlot::GetBounding()
{
    return m_widget.get();
}

void OracleWidgetSlot::RemoveSlot(OcWidgetHandle widget)
{
    m_widget = nullptr;
}

void OracleWidgetSlot::SetOffset(Vector2 offset)
{
    m_offset = offset;
}

Vector2 OracleWidgetSlot::GetOffset()
{
    return m_offset;
}

void OracleWidgetSlot::SetSize(Vector2 size)
{
    m_size = size;
}

Vector2 OracleWidgetSlot::GetSize()
{
    return m_size;
}

void OracleWidgetSlot::SetSizeToContent(bool enable)
{
    m_size_to_content = enable;
}

bool OracleWidgetSlot::IsSizeToContent()
{
    return m_size_to_content;
}