#include "Engine/Oracle/Public/Render/OracleRenderWindow.h"
#include "Engine/RHI/Public/rpi/yjw_rpi.h"

using namespace rpi;

OracleRenderWindow::OracleRenderWindow(Window* window)
{
    m_rpi_window = RPICreateWindow(window->GetWindowHandle());
}

OracleRenderWindow::~OracleRenderWindow()
{
    RPIDestroyWindow(m_rpi_window);
}

rpi::RPIWindow OracleRenderWindow::GetRPIWindow()
{
    return m_rpi_window;
}