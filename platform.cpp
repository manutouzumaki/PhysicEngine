#include "platform.h"
#include "input.h"

#include <Windowsx.h>
#include <glad/glad.h>
#include <GL/wglext.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h> 

static b32 g_running;

LRESULT CALLBACK WindowProc(HWND window, u32 msg, WPARAM w_param, LPARAM l_param)
{
    LRESULT result = {};
    switch (msg)
    {
    case WM_CLOSE:
    {
        g_running = false;
    }break;
    case WM_DESTROY:
    {
        g_running = false;
    }break;
    default:
    {
        result = DefWindowProcA(window, msg, w_param, l_param);
    }break;
    }
    return result;
}


Platform *platform_create(i32 x, i32 y, i32 width, i32 height, const char *name)
{
    Platform *platform = (Platform *)malloc(sizeof(Platform));
    memset(platform, 0, sizeof(Platform));

    HINSTANCE instance = GetModuleHandle(0);

    WNDCLASSEXA window_class = {};
    window_class.cbSize = sizeof(window_class);
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = WindowProc;
    window_class.hInstance = instance;
    window_class.hCursor = LoadCursor(0, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    window_class.lpszMenuName = 0;
    window_class.lpszClassName = name;

    RegisterClassExA(&window_class);

    RECT Rect = { x, y, width, height };
    AdjustWindowRect(&Rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, false);

    platform->window_width = width;
    platform->window_height = height;
    platform->running = &g_running;
    platform->window = CreateWindowA(name, name,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        Rect.right - Rect.left,
        Rect.bottom - Rect.top,
        0, 0, instance, 0);

    platform->device_context = GetDC(platform->window);

    PIXELFORMATDESCRIPTOR pixel_format = {};
    pixel_format.nSize = sizeof(pixel_format);
    pixel_format.nVersion = 1;
    pixel_format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixel_format.iPixelType = PFD_TYPE_RGBA;
    pixel_format.cColorBits = 32;
    pixel_format.cDepthBits = 24;
    pixel_format.cStencilBits = 8;
    pixel_format.iLayerType = PFD_MAIN_PLANE;

    i32 window_pixel_format = ChoosePixelFormat(platform->device_context, &pixel_format);
    SetPixelFormat(platform->device_context, window_pixel_format, &pixel_format);

    HGLRC tmp_opengl_context = wglCreateContext(platform->device_context);
    wglMakeCurrent(platform->device_context, tmp_opengl_context);

    i32 attributes[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (wglCreateContextAttribsARB)
    {
        HGLRC opengl_context = wglCreateContextAttribsARB(platform->device_context, 0, attributes);
        wglMakeCurrent(0, 0);
        wglDeleteContext(tmp_opengl_context);
        wglMakeCurrent(platform->device_context, opengl_context);
        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
            (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        if (wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT(1);
        }

        if (!gladLoadGL())
        {
            printf("Error: glad failed Initialize\n");
        }
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    platform->current_input = (Input *)malloc(sizeof(Input));
    memset(platform->current_input, 0, sizeof(Input));
    platform->last_input = (Input*)malloc(sizeof(Input));
    memset(platform->last_input, 0, sizeof(Input));

    LARGE_INTEGER frequency = { 0 };
    QueryPerformanceFrequency(&frequency);
    platform->inv_frequency = 1.0/(f64)frequency.QuadPart;

    ShowWindow(platform->window, true);

    LARGE_INTEGER last_counter = { 0 };
    QueryPerformanceCounter(&last_counter);
    platform->last_time = (f64)last_counter.QuadPart;

    srand((u32)time(0));

    g_running = true;
    return platform;
}

void platform_begin_frame(Platform* platform)
{
    LARGE_INTEGER current_counter = { 0 };
    QueryPerformanceCounter(&current_counter);
    platform->current_time = (f64)current_counter.QuadPart;
    platform->delta_time = (f32)((platform->current_time - platform->last_time) * platform->inv_frequency);

    for (i32 i = 0; i < ArrayCount(platform->current_input->keys); ++i)
    {
        platform->current_input->keys[i].was_down = false;
    }
    for (i32 i = 0; i < ArrayCount(platform->current_input->mouse_buttons); ++i)
    {
        platform->current_input->mouse_buttons[i].was_down = false;
    }

    MSG msg = {};
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        switch (msg.message)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            b8 was_down = ((msg.lParam & (1 << 30)) != 0);
            b8 is_down = ((msg.lParam & (1 << 31)) == 0);
            if (is_down != was_down)
            {
                DWORD VkCode = (DWORD)msg.wParam;
                platform->current_input->keys[VkCode].is_down = is_down;
            }
        }break;

        case WM_MOUSEMOVE:
        {
            platform->current_input->mouse_x = (i32)GET_X_LPARAM(msg.lParam);
            platform->current_input->mouse_y = (i32)GET_Y_LPARAM(msg.lParam);
        }break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        {
            platform->current_input->mouse_left.is_down = ((msg.wParam & MK_LBUTTON) != 0);
            platform->current_input->mouse_middle.is_down = ((msg.wParam & MK_MBUTTON) != 0);
            platform->current_input->mouse_right.is_down = ((msg.wParam & MK_RBUTTON) != 0);
        }break;
        default:
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }break;
        }
    }

    for (i32 i = 0; i < ArrayCount(platform->current_input->keys); ++i)
    {
        if (platform->last_input->keys[i].is_down)
        {
            platform->current_input->keys[i].was_down = true;
        }
    }
    for (i32 i = 0; i < ArrayCount(platform->current_input->mouse_buttons); ++i)
    {
        if (platform->last_input->mouse_buttons[i].is_down)
        {
            platform->current_input->mouse_buttons[i].was_down = true;
        }
    }

    glClearColor(0.8f, 0.8f, 0.9f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
}

void platform_end_frame(Platform* platform) {
    SwapBuffers(platform->device_context);
    *platform->last_input = *platform->current_input;
    platform->last_time = platform->current_time;
}

void platform_destroy(Platform *platform)
{
    ReleaseDC(platform->window, platform->device_context);
    free(platform->current_input);
    free(platform->last_input);
    free(platform);
}
