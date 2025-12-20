#pragma once
#include "Engine/Oracle/Public/Utils/Utils.h"
#include "Engine/Oracle/Public/Layout/Anchor.h"
#include "Engine/Oracle/Public/Layout/OracleLayout.h"

class OcWidget;
using OcWidgetHandle = OcPtr<OcWidget>;

struct OcWidgetArgument
{
    OcWidgetHandle m_widget;
};

class OcSlot
{
public:
    void SetSlotWidget(OcWidgetHandle widget);
    OcWidgetHandle GetSlotWidget();
    void RemoveSlot(OcWidgetHandle widget);
    bool IsBounding();
    virtual Vector2 ActualPosition(const OracleLayout& Layout) = 0;
    virtual Vector2 ActualSize(const OracleLayout& Layout) = 0;

private:
    OcWidgetHandle m_widget{};
};

using OcSlotHandle = OcPtr<OcSlot>;
using OcSlotWeakHandle = OcWeakPtr<OcSlot>;

struct SlotArgumentBase
{
    OcSlotHandle slot;
};

class OcAnchorSlot : public OcSlot
{
public:
    virtual Vector2 ActualPosition(const OracleLayout& Layout) override;
    virtual Vector2 ActualSize(const OracleLayout& Layout) override;

    OcAnchorSlot& LeftTopRightBottom(Vector4 size);
    OcAnchorSlot& LeftTopRightBottom(float left, float top, float right, float bottom);
    OcAnchorSlot& Anchor(FAnchor anchor);

    const FAnchor& Anchor() const;

    struct Argument : SlotArgumentBase
    {
        Argument& LeftTopRightBottom(Vector4 size) { ((OcAnchorSlot*)slot.get())->LeftTopRightBottom(size); return *this; }
        Argument& LeftTopRightBottom(float left, float top, float right, float bottom) { ((OcAnchorSlot*)slot.get())->LeftTopRightBottom(left, top, right, bottom); return *this; }
        Argument& Anchor(FAnchor anchor) { ((OcAnchorSlot*)slot.get())->Anchor(anchor); return *this; }
        Argument& operator [] (OcWidgetArgument arg) { slot.get()->SetSlotWidget(arg.m_widget); return *this; }
    };

private:
    FAnchor m_anchor;
    Vector2 m_left_top;
    Vector2 m_right_bottom;
};
using OcAnchorSlotHandle = OcPtr<OcAnchorSlot>;