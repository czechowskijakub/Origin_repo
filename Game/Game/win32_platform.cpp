#include <Windows.h>
#include "utility.cpp"

globalVariable bool running = true;


struct RenderState {
    int height, width;
    void* memory;

    BITMAPINFO bitmapInfo;
};

globalVariable RenderState renderState;

#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
    case WM_CLOSE:
    case WM_DESTROY: {
        running = false;
    } break;

    case WM_SIZE: {
        RECT rect;
        GetClientRect(hwnd, &rect);
        renderState.width = rect.right - rect.left;
        renderState.height = rect.bottom - rect.top;

        int size = renderState.width * renderState.height * sizeof(unsigned int);

        if (renderState.memory) VirtualFree(renderState.memory, 0, MEM_RELEASE);
        renderState.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
        renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
        renderState.bitmapInfo.bmiHeader.biHeight = renderState.height; 
        renderState.bitmapInfo.bmiHeader.biPlanes = 1;
        renderState.bitmapInfo.bmiHeader.biBitCount = 32;
        renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;
    } break;

    case WM_PAINT: {
        PAINTSTRUCT paint;
        HDC hdc = BeginPaint(hwnd, &paint);

        StretchDIBits(hdc,
            0, 0, renderState.width, renderState.height,
            0, 0, renderState.width, renderState.height,
            renderState.memory, &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

        EndPaint(hwnd, &paint);
    } break;

    default: {
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = L"Game Window Class";
    window_class.lpfnWndProc = window_callback;

    RegisterClass(&window_class);

    HWND window = CreateWindow(window_class.lpszClassName, L"GAME", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080, 0, 0, hInstance, 0);
    HDC hdc = GetDC(window);

    Input input = {};

    float deltaTime = 0.016666f;
    LARGE_INTEGER frameBeginTime;
    QueryPerformanceCounter(&frameBeginTime);

    float performanceFrequency; {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performanceFrequency = (float)perf.QuadPart;
    }

    while (running) {

        MSG message;
        
        for (int i = 0; i < BUTTON_COUNT; i++) {
            input.buttons[i].changed = false;
        }
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

            switch (message.message) {
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    u32 vkCode = (u32)message.wParam;
                    bool isDown = ((message.lParam & (1 << 31)) == 0);

// defining macro for every button press

#define processButton(b, vk)\
case vk: {\
    input.buttons[b].isDown = isDown;\
    input.buttons[b].changed = true;\
} break;
                    switch (vkCode) {
                        processButton(BUTTON_UP, VK_UP);
                        processButton(BUTTON_DOWN, VK_DOWN);
                        processButton(BUTTON_LEFT, VK_LEFT);
                        processButton(BUTTON_RIGHT, VK_RIGHT);
                    }
                } break;
                
                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

     
        simulateGame(&input);
        
        StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

        LARGE_INTEGER frameEndTime;
        QueryPerformanceCounter(&frameEndTime);
        deltaTime = (float)(frameEndTime.QuadPart - frameBeginTime.QuadPart) / performanceFrequency;
        frameBeginTime = frameEndTime;
    }

}
