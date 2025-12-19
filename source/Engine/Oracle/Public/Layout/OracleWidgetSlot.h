#pragma once
#include "Engine/Oracle/Public/Utils/Utils.h"

class OcWidget;
using OcWidgetHandle = OcPtr<OcWidget>;
class OracleWidgetSlot
{
public:
    OracleWidgetSlot(OcWidget* parent_widget);
    virtual ~OracleWidgetSlot();
    void SetSlotWidget(OcWidgetHandle widget);
    OcWidgetHandle GetSlotWidget();
    void RemoveSlot(OcWidgetHandle widget);
    bool IsBounding();
    OcWidget* GetBounding();

    void SetOffset(Vector2 offset);
    Vector2 GetOffset();

    void SetSize(Vector2 size);
    Vector2 GetSize();

    void SetSizeToContent(bool enable);
    bool IsSizeToContent();

private:
    OcWidgetHandle m_widget;

    OcWidget* m_parent_widget{};

    Vector2 m_offset{};
    Vector2 m_size{ 10,10 };

    bool m_size_to_content{false};
};

using OracleWidgetSlotHandle = OcPtr<OracleWidgetSlot>;
using OracleWidgetSlotWeakHandle = OcWeakPtr<OracleWidgetSlot>;