#include <windows.h>
#include "Renderer.h"

// Global renderer — pencere mesajlarından erişilebilmesi için
// (Sonraki phase'lerde bu App sınıfına taşınacak)
static Renderer* g_renderer = nullptr;

// WndProc: Win32 pencere mesaj işleyicisi
// Her klavye/fare/sistem olayı buraya gelir
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        // Pencere oluşturuldu — renderer'ı başlat
        g_renderer = new Renderer(hwnd);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        if (g_renderer) g_renderer->Render();
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_SIZE:
    {
        // Pencere boyutlandı — render target'ı güncelle
        UINT width  = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        if (g_renderer) g_renderer->Resize(width, height);
        return 0;
    }

    case WM_KEYDOWN:
        // Phase 3'te: VK_LEFT / VK_RIGHT navigasyon buraya eklenecek
        if (wParam == VK_ESCAPE)
            DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        delete g_renderer;
        g_renderer = nullptr;
        PostQuitMessage(0); // Mesaj döngüsünü sonlandır
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE,   // hPrevInstance — artık kullanılmıyor
    _In_     LPSTR     lpCmdLine,
    _In_     int       nCmdShow)
{
    // Phase 1'de lpCmdLine buradan okunacak (dosya yolu)
    (void)lpCmdLine;

    // Pencere sınıfını kaydet
    // CS_DBLCLKS: çift tıklama mesajlarını etkinleştirir (Phase 2: zoom reset)
    // CS_HREDRAW | CS_VREDRAW: pencere boyutlandığında yeniden çiz
    WNDCLASSEX wc   = {};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr; // Direct2D çiziyor, GDI arka planı gereksiz
    wc.lpszClassName = L"PhotoViewerWindow";
    RegisterClassEx(&wc);

    // Pencereyi oluştur
    HWND hwnd = CreateWindowEx(
        0,                      // Genişletilmiş stil yok
        L"PhotoViewerWindow",   // Pencere sınıfı adı (yukarıda kayıtlı)
        L"PhotoViewer",         // Başlık çubuğu metni
        WS_OVERLAPPEDWINDOW,    // Standart başlık + min/max/kapat butonları
        CW_USEDEFAULT, CW_USEDEFAULT, // Başlangıç konumu (OS seçer)
        1280, 800,              // Başlangıç boyutu
        nullptr,                // Üst pencere yok
        nullptr,                // Menü yok
        hInstance,
        nullptr                 // WM_CREATE'e geçirilecek ek veri yok
    );

    if (!hwnd) return -1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd); // İlk WM_PAINT'i hemen gönder

    // Mesaj döngüsü: uygulama kapatılana kadar çalışır
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg); // Klavye mesajlarını WM_CHAR'a çevirir
        DispatchMessage(&msg);  // WndProc'a yönlendirir
    }

    return static_cast<int>(msg.wParam);
}
