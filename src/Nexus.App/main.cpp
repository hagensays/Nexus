#include "Nexus.Platform/WindowsApplication.hpp"

#include <windows.h>

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show_command)
{
    return nexus::platform::WindowsApplication{}.run(instance, show_command);
}
