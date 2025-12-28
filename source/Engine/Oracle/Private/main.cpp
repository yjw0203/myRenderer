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
    OcFontStyleHandle FontStyle1 = OcMakeShared<OracleFontStyle>();
    FontStyle1->m_colour = LinearColour(0.75, 0.75, 0.75, 1);
    FontStyle1->m_font_path = "C:/Windows/Fonts/simhei.ttf";
    FontStyle1->m_size = 24;
    OcFontStyleHandle FontStyle2 = OcMakeShared<OracleFontStyle>();
    FontStyle2->m_colour = LinearColour(1, 0, 0, 1);
    FontStyle2->m_font_path = "C:/Windows/Fonts/simkai.ttf";
    FontStyle2->m_size = 32;
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
                .Anchor(EAnchorPreset::TopLeft)
                .LeftTopRightBottom(10, 10, 200, 200)
                [
                    OcNew(OcCanvas).Style(Style3)
                ]
                + OcCanvas::Slot()
                .Anchor(EAnchorPreset::TopLeft)
                .LeftTopRightBottom(10, 210, 200, 200)
                [
                    OcNew(OcLabel).Style(FontStyle1).Text("Hello World!\n你好，世界！")
                ]
                + OcCanvas::Slot()
                .Anchor(EAnchorPreset::TopLeft)
                .LeftTopRightBottom(10, 300, 200, 200)
                [
                    OcNew(OcLabel).Style(FontStyle2).Text("这是32大小楷书红色")
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
