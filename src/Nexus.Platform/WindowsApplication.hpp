#pragma once

#include <windows.h>

namespace nexus::platform
{
    class WindowsApplication final
    {
    public:
        int run(HINSTANCE instance, int show_command) const;
    };
}
