#pragma once

#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>

// Direct2D ve DirectWrite kütüphanelerini otomatik linkle
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

// Renderer: Direct2D render target yönetimi
// Görev: pencereye GPU üzerinden çizim yapmak
class Renderer
{
public:
    explicit Renderer(HWND hwnd);
    ~Renderer();

    // Renderer kopyalanamaz (COM nesneleri paylaşılamaz)
    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;

    // Ana çizim fonksiyonu — WM_PAINT'te çağrılır
    void Render();

    // Pencere boyutlandığında render target'ı güncelle — WM_SIZE'da çağrılır
    void Resize(UINT width, UINT height);

private:
    // GPU cihazına bağlı kaynakları oluştur
    HRESULT CreateDeviceResources();
    // GPU cihazı kaybolunca kaynakları serbest bırak (D2DERR_RECREATE_TARGET)
    void    DiscardDeviceResources();

    HWND                   m_hwnd        = nullptr;
    ID2D1Factory*          m_factory     = nullptr;  // Direct2D fabrikası (cihazdan bağımsız)
    ID2D1HwndRenderTarget* m_renderTarget = nullptr; // HWND'e bağlı render target (GPU)
};
