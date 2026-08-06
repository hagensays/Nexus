#include "Nexus.UI/MainWindow.hpp"

#include "Nexus.Core/AppMetadata.hpp"
#include "Nexus.Modules/Home/HomeModule.hpp"
#include "Nexus.Modules/Settings/SettingsModule.hpp"

#include <string>

namespace
{
    constexpr wchar_t window_class_name[] = L"NexusMainWindow";
    constexpr int home_button_id = 1001;
    constexpr int settings_button_id = 1002;
    constexpr int sidebar_width = 180;
    constexpr int outer_margin = 20;
}

namespace nexus::ui
{
    int MainWindow::run(HINSTANCE instance, int show_command)
    {
        WNDCLASSEXW window_class{};
        window_class.cbSize = sizeof(window_class);
        window_class.hInstance = instance;
        window_class.lpfnWndProc = &MainWindow::window_proc;
        window_class.lpszClassName = window_class_name;
        window_class.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        window_class.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

        if (RegisterClassExW(&window_class) == 0)
        {
            return 1;
        }

        const std::wstring window_title{nexus::core::app_name()};
        HWND window = CreateWindowExW(
            0,
            window_class_name,
            window_title.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            1000,
            680,
            nullptr,
            nullptr,
            instance,
            this);

        if (window == nullptr)
        {
            return 1;
        }

        ShowWindow(window, show_command);
        UpdateWindow(window);

        MSG message{};
        while (GetMessageW(&message, nullptr, 0, 0) > 0)
        {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }

        return static_cast<int>(message.wParam);
    }

    LRESULT CALLBACK MainWindow::window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
    {
        MainWindow* self = nullptr;

        if (message == WM_NCCREATE)
        {
            const auto* create = reinterpret_cast<CREATESTRUCTW*>(lparam);
            self = static_cast<MainWindow*>(create->lpCreateParams);
            SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        }
        else
        {
            self = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(window, GWLP_USERDATA));
        }

        return self != nullptr
            ? self->handle_message(window, message, wparam, lparam)
            : DefWindowProcW(window, message, wparam, lparam);
    }

    LRESULT MainWindow::handle_message(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
    {
        switch (message)
        {
        case WM_CREATE:
            create_controls(window);
            show_home();
            return 0;

        case WM_SIZE:
            layout_controls(window);
            return 0;

        case WM_COMMAND:
            switch (LOWORD(wparam))
            {
            case home_button_id:
                show_home();
                return 0;
            case settings_button_id:
                show_settings();
                return 0;
            default:
                break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            break;
        }

        return DefWindowProcW(window, message, wparam, lparam);
    }

    void MainWindow::create_controls(HWND window)
    {
        const HINSTANCE instance = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(window, GWLP_HINSTANCE));

        home_button_ = CreateWindowExW(0, L"BUTTON", L"Home", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 0, 0, 0, window, reinterpret_cast<HMENU>(home_button_id), instance, nullptr);
        settings_button_ = CreateWindowExW(0, L"BUTTON", L"Settings", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 0, 0, 0, window, reinterpret_cast<HMENU>(settings_button_id), instance, nullptr);
        title_label_ = CreateWindowExW(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE,
            0, 0, 0, 0, window, nullptr, instance, nullptr);
        description_label_ = CreateWindowExW(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE,
            0, 0, 0, 0, window, nullptr, instance, nullptr);

        const HFONT default_font = static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));
        for (HWND control : {home_button_, settings_button_, title_label_, description_label_})
        {
            SendMessageW(control, WM_SETFONT, reinterpret_cast<WPARAM>(default_font), TRUE);
        }

        layout_controls(window);
    }

    void MainWindow::layout_controls(HWND window) const
    {
        RECT client{};
        GetClientRect(window, &client);

        const int content_x = sidebar_width + outer_margin;
        const int content_width = (client.right - content_x - outer_margin) > 0
            ? client.right - content_x - outer_margin
            : 0;

        MoveWindow(home_button_, outer_margin, outer_margin, sidebar_width - (outer_margin * 2), 34, TRUE);
        MoveWindow(settings_button_, outer_margin, outer_margin + 44, sidebar_width - (outer_margin * 2), 34, TRUE);
        MoveWindow(title_label_, content_x, outer_margin + 4, content_width, 32, TRUE);
        MoveWindow(description_label_, content_x, outer_margin + 48, content_width, 64, TRUE);
    }

    void MainWindow::show_home()
    {
        const std::wstring title{nexus::modules::home::title()};
        const std::wstring description{nexus::modules::home::description()};
        set_content(title.c_str(), description.c_str());
    }

    void MainWindow::show_settings()
    {
        const std::wstring title{nexus::modules::settings::title()};
        const std::wstring description{nexus::modules::settings::description()};
        set_content(title.c_str(), description.c_str());
    }

    void MainWindow::set_content(const wchar_t* title, const wchar_t* description)
    {
        SetWindowTextW(title_label_, title);
        SetWindowTextW(description_label_, description);
    }
}
