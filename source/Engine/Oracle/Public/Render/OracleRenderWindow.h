#pragma once
#include "Engine/Core/Public/Windows/Window.h"
#include "Engine/RHI/Public/rpi/yjw_rpi.h"

class OracleRenderWindow
{
public:
    OracleRenderWindow(Window* window);
    ~OracleRenderWindow();

    rpi::RPIWindow GetRPIWindow();
private:
    rpi::RPIWindow m_rpi_window;
};