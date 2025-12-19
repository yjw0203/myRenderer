#include "stdio.h"
#include "Engine/Oracle/Public/Widgets/Widgets.h"
#include "Engine/Oracle/Public/Render/OracleRenderer.h"
#include "Engine/Oracle/Public/Oracle/Oracle.h"
#include "projectInfo.h"

int main()
{

    OracleUIManager::Get()->Initialize();

    OcStyleHandle Style1 = OcMakeShared<OracleStyle>();
    Style1->m_background_color = LinearColour(1, 0, 0, 1);
    OcStyleHandle Style2 = OcMakeShared<OracleStyle>();
    Style2->m_background_color = LinearColour(0, 1, 0, 1);
    OcStyleHandle Style3 = OcMakeShared<OracleStyle>();
    Style3->m_resource = OracleRenderResource(RESOURCE_FILE(wizard/Texture/wizard0.png));

    OcPtr<OcWindow> window = OcMakeShared<OcWindow>();
    OracleUIManager::Get()->RegisterWindow(window);
    window->m_style = Style1;

    OcPtr<OcCanvas> canvas = OcMakeShared<OcCanvas>();
    canvas->m_style = Style2;

    OcPtr<OcCanvas> canvas1 = OcMakeShared<OcCanvas>();
    canvas1->m_style = Style3;

    {
        OracleWidgetSlotHandle slot = window->AddSubWidget(canvas);
        slot->SetOffset(Vector2(200, 200));
        slot->SetSize(Vector2(300, 300));
    }
    
    {
        OracleWidgetSlotHandle slot = canvas->AddSubWidget(canvas1);
        slot->SetOffset(Vector2(10, 10));
        slot->SetSize(Vector2(200, 200));
    }

    while (1) 
    {
        OracleUIManager::Get()->Paint();
        OracleUIManager::Get()->PullEvent();
    };
    return 0;
}
