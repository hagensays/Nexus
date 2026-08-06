#pragma once

#include <windows.h>

namespace nexus::ui
{
    class MainWindow final
    {
    public:
        int run(HINSTANCE instance, int show_command);

    private:
        static LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
        LRESULT handle_message(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

        void create_controls(HWND window);
        void layout_controls(HWND window) const;
        void show_home();
        void show_settings();
        void set_content(const wchar_t* title, const wchar_t* description);

        HWND home_button_{};
        HWND settings_button_{};
        HWND title_label_{};
        HWND description_label_{};
    };
}
