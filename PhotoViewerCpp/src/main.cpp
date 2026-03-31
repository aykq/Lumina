#include <windows.h>
#include <shellapi.h>   // CommandLineToArgvW
#include <string>
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
    _In_     LPSTR,       // lpCmdLine — ANSI, Unicode için GetCommandLineW kullanıyoruz
    _In_     int       nCmdShow)
{
    // COM başlat — WIC (IWICImagingFactory) için gerekli
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    // Komut satırı argümanlarını Unicode olarak oku
    // Explorer'dan çift tıklamada: argv[1] = tam dosya yolu (boşluklu yollar dahil)
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    std::wstring filePath;
    if (argc >= 2) filePath = argv[1];
    LocalFree(argv);  // CommandLineToArgvW heap allocate eder, biz serbest bırakıyoruz

    // Pencere sınıfını kaydet
    // CS_DBLCLKS: çift tıklama mesajlarını etkinleştirir (Phase 2: zoom reset)
    // CS_HREDRAW | CS_VREDRAW: pencere boyutlandığında yeniden çiz
    WNDCLASSEX wc    = {};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr; // Direct2D çiziyor, GDI arka planı gereksiz
    wc.lpszClassName = L"PhotoViewerWindow";
    RegisterClassEx(&wc);

    // Pencere başlığını belirle: dosya adı varsa "dosya.jpg — PhotoViewer"
    std::wstring title = L"PhotoViewer";
    if (!filePath.empty())
    {
        // Yol ayırıcıdan sonraki kısım = dosya adı (\ veya / her ikisini destekle)
        auto pos = filePath.find_last_of(L"\\/");
        std::wstring fileName = (pos != std::wstring::npos) ? filePath.substr(pos + 1) : filePath;
        title = fileName + L" \u2014 PhotoViewer";  // \u2014 = em dash (—)
    }

    // Pencereyi oluştur (WM_CREATE tetiklenir → g_renderer hazır olur)
    HWND hwnd = CreateWindowEx(
        0,                      // Genişletilmiş stil yok
        L"PhotoViewerWindow",   // Pencere sınıfı adı (yukarıda kayıtlı)
        title.c_str(),          // Başlık: "dosya.jpg — PhotoViewer" veya "PhotoViewer"
        WS_OVERLAPPEDWINDOW,    // Standart başlık + min/max/kapat butonları
        CW_USEDEFAULT, CW_USEDEFAULT, // Başlangıç konumu (OS seçer)
        1280, 800,              // Başlangıç boyutu
        nullptr,                // Üst pencere yok
        nullptr,                // Menü yok
        hInstance,
        nullptr                 // WM_CREATE'e geçirilecek ek veri yok
    );

    if (!hwnd)
    {
        CoUninitialize();
        return -1;
    }

    // Dosya yolunu yükle — CreateWindowEx içinde WM_CREATE tetiklendiğinden
    // g_renderer bu noktada hazır
    if (!filePath.empty() && g_renderer)
        g_renderer->LoadImage(filePath);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd); // İlk WM_PAINT'i hemen gönder

    // Mesaj döngüsü: uygulama kapatılana kadar çalışır
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg); // Klavye mesajlarını WM_CHAR'a çevirir
        DispatchMessage(&msg);  // WndProc'a yönlendirir
    }

    CoUninitialize();
    return static_cast<int>(msg.wParam);
}
