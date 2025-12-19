#pragma once
#include "Engine/Utils/Public/DesignPatterns/Singleton.h"
#include "Engine/Oracle/Public/Utils/Utils.h"

class OracleRenderer;
class OcWindow;
class OracleUI
{
public:
    void Initialize();
    void Destory();
    void PullEvent();
    void Paint();
    void RegisterWindow(OcPtr<OcWindow> window);
    void UnregisterWindow(OcPtr<OcWindow> window);
private:
    OcArray<OcPtr<OcWindow>> m_windows;

    OracleRenderer* m_renderer{};
};

typedef Singleton<OracleUI> OracleUIManager;