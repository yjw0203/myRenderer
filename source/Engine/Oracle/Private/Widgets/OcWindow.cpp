#include "Engine/Oracle/Public/Widgets/OcWindow.h"

OcWindow::OcWindow()
{
    m_window = new Window();
    m_render_window = new OracleRenderWindow(m_window);

    m_renderer = new OracleRenderer();
    m_renderer->Initialize();
}

OcWindow::~OcWindow()
{
    delete m_render_window;
    delete m_window;
    delete m_renderer;
}

void OcWindow::OnComputeDesiredSize(float ScaleMultiplier)
{
    SetDesiredSize(GetSize());
    OcCanvas::OnComputeDesiredSize(ScaleMultiplier);
}

void OcWindow::OnLayoutChildren(const OracleLayout& Layout, OracleLayoutedWidgets& LayoutedWidgets)
{
    OracleLayout layout = OracleLayout(Vector2(0, 0), GetSize(), 1.0f);;
    OcCanvas::OnLayoutChildren(layout, LayoutedWidgets);
}

void OcWindow::OnPaint(OcacleDrawElementList& DrawElements, const OracleLayout& Layout)
{
    OracleLayout layout = OracleLayout(Vector2(0, 0), GetSize(), 1.0f);;
    OcCanvas::OnPaint(DrawElements, layout);
}

OracleRenderWindow* OcWindow::GetRenderWindow()
{
    return m_render_window;
}

Window* OcWindow::GetWindow()
{
    return m_window;
}

Vector2 OcWindow::GetSize()
{
    return Vector2(1200, 1200);
}

void OcWindow::Render()
{
    OcacleDrawElementList DrawElementList;
    DrawElementList.SetResolution(GetSize());
    Paint(DrawElementList, OracleLayout());
    m_renderer->Render(GetRenderWindow(), DrawElementList);
}
