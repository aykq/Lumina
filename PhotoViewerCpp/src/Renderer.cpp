#include "Renderer.h"

Renderer::Renderer(HWND hwnd) : m_hwnd(hwnd)
{
    // D2D1Factory: Direct2D'nin giriş noktası.
    // SINGLE_THREADED: render işlemleri tek iş parçacığından yapılacak (UI thread).
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
}

Renderer::~Renderer()
{
    DiscardDeviceResources();

    if (m_factory)
    {
        m_factory->Release();
        m_factory = nullptr;
    }
}

HRESULT Renderer::CreateDeviceResources()
{
    // Render target zaten varsa tekrar oluşturma
    if (m_renderTarget) return S_OK;

    RECT rc;
    GetClientRect(m_hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    // HwndRenderTarget: doğrudan Win32 HWND üzerine GPU ile çizer.
    // Zoom/pan transform'ları bu render target üzerinde uygulanacak.
    return m_factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size),
        &m_renderTarget
    );
}

void Renderer::DiscardDeviceResources()
{
    if (m_renderTarget)
    {
        m_renderTarget->Release();
        m_renderTarget = nullptr;
    }
}

void Renderer::Render()
{
    if (FAILED(CreateDeviceResources())) return;

    m_renderTarget->BeginDraw();

    // Arka plan: koyu gri (#1E1E1E — VS Code dark teması ile aynı ton)
    m_renderTarget->Clear(D2D1::ColorF(0.118f, 0.118f, 0.118f));

    // --- Sonraki phase'lerde buraya eklenecek ---
    // Phase 1: görüntü çizimi (DrawBitmap)
    // Phase 2: zoom/pan transform
    // Phase 5: EXIF paneli (DirectWrite metin)

    HRESULT hr = m_renderTarget->EndDraw();

    if (hr == D2DERR_RECREATE_TARGET)
    {
        // GPU cihazı kayboldu (örn. ekran kartı sürücüsü sıfırlandı).
        // Kaynakları serbest bırak — bir sonraki Render() çağrısında yeniden oluşturulur.
        DiscardDeviceResources();
    }
}

void Renderer::Resize(UINT width, UINT height)
{
    if (m_renderTarget)
    {
        // Render target'ı yeni pencere boyutuna göre güncelle.
        // ID2D1Bitmap'ler geçersiz olmaz, sadece viewport boyutu değişir.
        m_renderTarget->Resize(D2D1::SizeU(width, height));
    }
}
