#include "Engine/Oracle/Public/Oracle/Oracle.h"
#include "Engine/Oracle/Public/Widgets/OcWindow.h"
#include "Engine/Oracle/Public/Render/OracleRenderer.h"

void OracleUI::Initialize()
{
    OracleRenderer::StaticInitialize();
}

void OracleUI::Destory()
{
}

void OracleUI::PullEvent()
{
    for (auto& wd : m_windows)
    {
        if (wd.get())
        {
            wd->GetWindow()->PoolEvents();
            wd->PullEvent();
        }
    }
}

void OracleUI::Paint()
{
    for (auto& wd : m_windows)
    {
        if (wd.get())
        {
            wd->ComputeDesiredSize(1.0f);
            wd->Render();
        }
    }
}

void OracleUI::RegisterWindow(OcPtr<OcWindow> window)
{
    for (auto& wd : m_windows)
    {
        if (wd.get() == window.get())
        {
            return;
        }
    }
    m_windows.push_back(window);
}

void OracleUI::UnregisterWindow(OcPtr<OcWindow> window)
{
    for (auto it = m_windows.begin(); it != m_windows.end(); it++)
    {
        if ((*it).get() == window.get())
        {
            m_windows.erase(it);
            return;
        }
    }
}