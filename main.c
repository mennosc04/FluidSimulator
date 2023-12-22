#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "particle.h"

/**
 * Rendering
 * ResizeDIBsection() (Device Independent Bitmap) in WM_SIZE (maybe use aspect ratio)
 * 
*/

void ResizeDIBSection(int width, int height);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static Particle *particles;
static HDC BitmapDeviceContext;
static void *BitmapMemory;
static BITMAPINFO BitmapInfo;
static HBITMAP BitmapHandle;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    WNDCLASS WindowClass = {0};

    WindowClass.lpfnWndProc = WindowProc; 
    WindowClass.hInstance = hInstance;
    WindowClass.lpszClassName = "MyWindowClass";

    RegisterClass(&WindowClass);

    HWND WindowHandle = CreateWindowEx(0, WindowClass.lpszClassName, "Fluid Simulator", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);
    if(WindowHandle == NULL) {
        printf("No windowhandle created, exiting");
    }

    bool message_loop = false;
    MSG msg = {0};

    while(!message_loop) {
        WaitMessage();
        while(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
            if(GetMessage(&msg, NULL, 0, 0) == 0) {
                message_loop = true;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY: {
                PostQuitMessage(0);
            } break;
        case WM_NCCREATE: {
                particles = (Particle *)malloc(sizeof(Particle) * AMOUNT_PARTICLES);
                particles_create(particles);
            } break;
        case WM_PAINT: {
                draw_particles(hwnd, particles);
            } break;
        case WM_SIZE: {
                RECT ClientRect;
                GetClientRect(hwnd, &ClientRect);
                int width = ClientRect.right - ClientRect.left;
                int height = ClientRect.bottom - ClientRect.top;
                ResizeDIBSection(width, height);
            } break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 1;
}

void ResizeDIBSection(int width, int height) {
    if(BitmapHandle) {
        DeleteObject(BitmapHandle);
    }

    if(!BitmapDeviceContext) {
        BitmapDeviceContext = CreateCompatibleDC(0);
    }

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = width;
    BitmapInfo.bmiHeader.biHeight = height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    BitmapHandle = CreateDIBSection(BitmapDeviceContext, &BitmapInfo, DIB_RGB_COLORS, &BitmapMemory, 0, 0);
}