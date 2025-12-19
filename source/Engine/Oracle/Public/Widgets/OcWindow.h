#pragma once
#include "Engine/Core/Public/Windows/Window.h"
#include "Engine/Oracle/Public/Widgets/OcWidget.h"
#include "Engine/Oracle/Public/Widgets/OcCanvas.h"
#include "Engine/Oracle/Public/Render/OracleRenderer.h"

class OcWindow : public OcCanvas
{
public:
    OcWindow();
    virtual ~OcWindow();

    virtual void OnComputeDesiredSize(float ScaleMultiplier) override;
    virtual void OnLayoutChildren(const OracleLayout& Layout, OracleLayoutedWidgets& LayoutedWidgets) override;
    virtual void OnPaint(OcacleDrawElementList& DrawElements, const OracleLayout& Layout) override;

    OracleRenderWindow* GetRenderWindow();
    Window* GetWindow();
    Vector2 GetSize();

private:
    Window* m_window = nullptr;

    OracleRenderWindow* m_render_window = nullptr;
};
