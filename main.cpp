#include <Windows.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "Particle.hpp"

#define PARTICLE_SIZE 40
#define PARTICLE_SPACING 2
#define AMOUNT_PARTICLES 100

const char ClassName[] = "MyWindowClass";

static BITMAPINFO BitmapInfo;
static void *BitmapMemory;
static HBITMAP BitmapHandle;
static HDC DeviceContext;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
std::vector<Particle> draw_circles(HWND hwnd, HDC hdc, int amount_circles);
void RepaintBackground(HWND hwnd, HDC hdc);
void ResizeDIBSection(int width, int height);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    WNDCLASS WindowClass = {};

    WindowClass.lpfnWndProc = WindowProc; //pointer to the function that handles the window logic
    WindowClass.hInstance = hInstance; //Window Instance
    WindowClass.lpszClassName = ClassName;

    RegisterClass(&WindowClass);

    HWND WindowHandle = CreateWindowEx(0, ClassName, "Fluid Simulator", WS_OVERLAPPEDWINDOW, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);
    if(WindowHandle == NULL) {
        printf("No windowhandle created, exiting");
    }

    ShowWindow(WindowHandle, nCmdShow);

    bool message_loop = false;
    MSG msg = {};

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
    ParticleManager particleManager;

    switch (uMsg) {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC DeviceContext = BeginPaint(hwnd, &ps);

            RepaintBackground(hwnd, DeviceContext);
            particleManager.particles = draw_circles(hwnd, DeviceContext, AMOUNT_PARTICLES);

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_SIZE:
        {
            RECT ClientRect;
            GetClientRect(hwnd, &ClientRect);
            ResizeDIBSection(ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

std::vector<Particle> draw_circles(HWND hwnd, HDC hdc, int amount_circles) {
    int particlesPerRow = (int)sqrt(amount_circles);
    int particlesPerCol = (amount_circles - 1) / particlesPerRow + 1;
    float spacing = PARTICLE_SIZE * 2 + PARTICLE_SPACING;

    HBRUSH hbr = CreateSolidBrush(RGB(3, 144, 254));
    SelectObject(hdc,hbr);

    std::vector<Particle> particles;

    //521860 microseconds
    float initial_x = ((-particlesPerRow / 2) + 0.5f) * spacing + GetSystemMetrics(SM_CXSCREEN) / 2;
    float initial_y = ((-particlesPerCol / 2) + 0.5f) * spacing + GetSystemMetrics(SM_CYSCREEN) / 2; 

    float current_y = initial_y;
    for(int col = 0; col < particlesPerCol; col++) {
        float current_x = initial_x;
        for(int row = 0; row < particlesPerRow; row++) {
            Particle particle;
            particle.x = current_x;
            particle.y = current_y;
            particle.bounce_multiplier = 0.1;

            particles.push_back(particle);

            Ellipse(hdc, current_x - PARTICLE_SIZE / 2, current_y - PARTICLE_SIZE / 2, current_x + PARTICLE_SIZE / 2, current_y + PARTICLE_SIZE / 2);
        
            current_x += spacing;
        }
        current_y += spacing;
    }

    DeleteObject(hbr);

    return particles;
}

void RepaintBackground(HWND hwnd, HDC DeviceContext) {
    RECT rc;
    GetClientRect(hwnd, &rc);

    HBRUSH hcolor = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(DeviceContext, &rc, hcolor);

    DeleteObject(hcolor);
}

void ResizeDIBSection(int width, int height) {
    if(BitmapHandle) {
        DeleteObject(BitmapHandle);
    } 
    
    if(!DeviceContext)5I6KL.TR, {
        DeviceContext = CreateCompatibleDC(0);
    }

    BITMAPINFO  BitmapInfo = {};
    BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitmapInfo.bmiHeader.biWidth = width;
    BitmapInfo.bmiHeader.biHeight = height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    BitmapHandle = CreateDIBSection(DeviceContext, &BitmapInfo, DIB_RGB_COLORS, &BitmapMemory, 0, 0);

}