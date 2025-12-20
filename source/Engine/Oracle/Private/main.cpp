#include "stdio.h"
#include "Engine/Oracle/Public/Widgets/Widgets.h"
#include "Engine/Oracle/Public/Render/OracleRenderer.h"
#include "Engine/Oracle/Public/Oracle/Oracle.h"
#include "projectInfo.h"

int main()
{

    OracleUIManager::Get()->Initialize();

    OcStyleHandle Style1 = OcMakeShared<OracleStyle>();
    Style1->m_background_color = LinearColour(0.082, 0.082, 0.082, 1);
    OcStyleHandle Style2 = OcMakeShared<OracleStyle>();
    Style2->m_background_color = LinearColour(0.141, 0.141, 0.141, 1);
    OcStyleHandle Style3 = OcMakeShared<OracleStyle>();
    Style3->m_resource = OracleRenderResource(RESOURCE_FILE(wizard/Texture/wizard0.png));

    OcPtr<OcWindow> window = OcMakeShared<OcWindow>();
    OracleUIManager::Get()->RegisterWindow(window);

    OcPtr<OcWindow> window2 = OcMakeShared<OcWindow>();
    OracleUIManager::Get()->RegisterWindow(window2);

    OcWidgetArgument canvas = OcNew(OcCanvas).Style(Style1) 
        + OcCanvas::Slot()
        .Anchor(EAnchorPreset::Stretch)
        .LeftTopRightBottom(200, 200, 300, 300)
        [
            OcNew(OcCanvas).Style(Style2)
                + OcCanvas::Slot()
                .Anchor(EAnchorPreset::Stretch)
                .LeftTopRightBottom(10, 10, 200, 200)
                [
                    OcNew(OcCanvas).Style(Style3)
                ]
        ];

    window->AddSubWidget(canvas.m_widget)
        .Anchor(EAnchorPreset::Stretch)
        .LeftTopRightBottom(0, 0, 0, 0);
        
    window2->AddSubWidget(canvas.m_widget)
        .Anchor(EAnchorPreset::Stretch)
        .LeftTopRightBottom(200, 200, 200, 200);

    while (1) 
    {
        OracleUIManager::Get()->Paint();
        OracleUIManager::Get()->PullEvent();
    };
    return 0;
}
