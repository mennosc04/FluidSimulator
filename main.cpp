#include <Windows.h>
#include <math.h>
#include <vector>
#include <iostream>

#define PARTICLE_SIZE 40
#define PARTICLE_SPACING 2
#define AMOUNT_PARTICLES 100

const char ClassName[] = "MyWindowClass";

class Particle {
    public:
        int x, y, bounce_multiplier;

};

class ParticleManager { 
    public:
        std::vector<Particle> particles;

        void particles_print() {
            for(Particle p : this->particles) {
                std::cout << "{x: " << p.x << ", y: " << p.y << ", bounce multiplier: " << p.bounce_multiplier << "}" << std::endl;
            }
        }
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
std::vector<Particle> draw_circles(HWND hwnd, int amount_circles);
void RepaintBackground(HWND hwnd);

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
            PostQuitMessage(0);
            return 1;
        case WM_PAINT:
            RepaintBackground(hwnd);
            particleManager.particles = draw_circles(hwnd, AMOUNT_PARTICLES);
            particleManager.particles_print();

            return 1;
        case WM_SIZE:
            InvalidateRect(hwnd, NULL, TRUE);
            return 1;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

std::vector<Particle> draw_circles(HWND hwnd, int amount_circles) {
    int particlesPerRow = (int)sqrt(amount_circles);
    int particlesPerCol = (amount_circles - 1) / particlesPerRow + 1;
    float spacing = PARTICLE_SIZE * 2 + PARTICLE_SPACING;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

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
    //EndPaint(hdc, ps);

    return particles;
}

void RepaintBackground(HWND hwnd) {
    HDC dc = GetDC(hwnd);

    RECT rc;
    GetClientRect(hwnd, &rc);

    HBRUSH hcolor = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(dc, &rc, hcolor);

    ReleaseDC(hwnd, dc);
    DeleteObject(hcolor);
}