#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "particle.h"

void ResizeDIBSection(HWND hwnd, int width, int height);
void DisplayBufferOnScreen(HDC DeviceContext, RECT *WindowRect, int x, int y, int width, int height);
void RenderParticles(int XOffset, int YOffset);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

 static Particle *particles;
static void *BitmapMemory;
static BITMAPINFO BitmapInfo;
static int BitmapWidth;
static int BitmapHeight;
static int BytesPerPixel = 4;

static HBITMAP Bitmap;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    WNDCLASS WindowClass = {0};

    WindowClass.lpfnWndProc = WindowProc; 
    WindowClass.hInstance = hInstance;
    WindowClass.lpszClassName = "MyWindowClass";

    RegisterClass(&WindowClass);

    HWND WindowHandle = CreateWindowEx(0, WindowClass.lpszClassName, "Fluid Simulator", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);
    if(WindowHandle == NULL) {
        printf("No windowhandle created,exiting");
    }

    bool running = true;
    int XOffset = 0;
    int YOffset = 0;
    while(running) {
        MSG msg;
        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_QUIT) {
                running = false;
                break;
            } 
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        RenderParticles(0, 0);
        HDC DeviceContext = GetDC(WindowHandle);
        RECT ClientRect;
        GetClientRect(WindowHandle, &ClientRect);

        int WindowWidth = ClientRect.right - ClientRect.left;
        int WindowHeight = ClientRect.bottom - ClientRect.top;

        DisplayBufferOnScreen(DeviceContext, &ClientRect, 0, 0, WindowWidth, WindowHeight);
        ReleaseDC(WindowHandle, DeviceContext);
        XOffset += 1;
        YOffset += 1;
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
                PAINTSTRUCT ps;
                HDC DeviceContext = BeginPaint(hwnd, &ps);
                int x = ps.rcPaint.left;
                int y = ps.rcPaint.top;
                int width = ps.rcPaint.right - ps.rcPaint.left;
                int height = ps.rcPaint.bottom - ps.rcPaint.top;

                RECT ClientRect;
                GetClientRect(hwnd, &ClientRect);
                DisplayBufferOnScreen(DeviceContext, &ClientRect, x, y, width, height);

                EndPaint(hwnd, &ps);
            } break;
        case WM_SIZE: {
                RECT ClientRect;
                GetClientRect(hwnd, &ClientRect);
                int width = ClientRect.right - ClientRect.left;
                int height = ClientRect.bottom - ClientRect.top;
                ResizeDIBSection(hwnd, width, height);
            } break;
        case WM_CLOSE: {
            PostQuitMessage(0);
        } break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 1;
}

void ResizeDIBSection(HWND hwnd, int width, int height) {
    /* if(Bitmap) {
        DeleteObject(Bitmap);
    }

    BITMAPINFO BitmapInfo = {};
    BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitmapInfo.bmiHeader.biWidth = width;
    BitmapInfo.bmiHeader.biHeight = height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    Bitmap = CreateDIBSection(0, &BitmapInfo, DIB_RGB_COLORS, 0, 0, 0);
    HDC DeviceContext = GetDC(hwnd);
    HDC memDC = CreateCompatibleDC(DeviceContext);
    HGDIOBJ oldBitmap = SelectObject(memDC, Bitmap);
    SetDCBrushColor(memDC, RGB(255,0,0));
    Ellipse(memDC, 50 - 10, 50 - 10, 50 + 10, 50 + 10);
    SelectObject(emmDC, oldBitmap);
    DeleteDC(memDC);
    
    ReleaseDC(hwnd, DeviceContext); */

    if(BitmapMemory) {
        VirtualFree(BitmapMemory, 0, MEM_RELEASE);
    }

    BitmapWidth = width;
    BitmapHeight = height;

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = BitmapWidth;
    BitmapInfo.bmiHeader.biHeight = -BitmapHeight;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = BitmapWidth * BitmapHeight * BytesPerPixel;
    BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

void DisplayBufferOnScreen(HDC DeviceContext, RECT *WindowRect, int x, int y, int width, int height) {
    int WindowWidth = WindowRect->right - WindowRect->left;
    int WindowHeight = WindowRect->bottom - WindowRect->top;
    StretchDIBits(DeviceContext, 
                    /* x, y, width, height, 
                    x, y, width, height,  */
                    0, 0, BitmapWidth, BitmapHeight,
                    0, 0, WindowWidth, WindowHeight,
                    BitmapMemory, 
                    &BitmapInfo, 
                    DIB_RGB_COLORS, SRCCOPY);
}

void RenderParticles(int XOffset, int YOffset) {
    int pitch = BitmapWidth * BytesPerPixel;
    uint8_t *row = (uint8_t *)BitmapMemory;
    for(int i = 0; i < BitmapHeight; i++) {
        uint8_t *pixel = (uint8_t *)row;
        for(int j = 0; j < BitmapWidth; j++) {
            *pixel = 0;
             ++pixel;

            *pixel = (uint8_t)(j + XOffset);
            ++pixel;

            *pixel = (uint8_t)(i + YOffset);
            ++pixel;

            *pixel = 0;
            ++pixel;
        }
        row += pitch;
    }
    
}