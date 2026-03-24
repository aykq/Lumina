# Tech Context

## Technologies Used

| Teknoloji | Versiyon | Kullanım Alanı |
|-----------|----------|----------------|
| .NET | 8.0 | Runtime |
| Windows App SDK | 1.8.260209005 | WinUI 3 UI Framework |
| CommunityToolkit.Mvvm | 8.4.0 | MVVM pattern implementasyonu |
| **Windows.Graphics.Imaging (WIC)** | OS codec | Birincil decode (hızlı) |
| **Magick.NET-Q8-AnyCPU** | 14.11.0 | **Fallback** decode (WIC başarısız olunca; JXL, bazı RAW vb.) |
| MetadataExtractor | 2.9.0 | EXIF metadata okuma |
| WinUI 3 | - | Kullanıcı arayüzü |

**Akış:** Önce WIC → başarısızsa ImageMagick (piksel decode arka planda; `WriteableBitmap` UI iş parçacığında).

## Development Setup

### Gereksinimler
- Visual Studio 2022 veya .NET 8 SDK
- Windows 10/11
- Windows App SDK workload

### Derleme
```bash
dotnet build
dotnet run
```

### Platform Hedefleri
- x86, x64, ARM64

### Target Framework
- net8.0-windows10.0.19041.0
- Minimum: Windows 10 1809 (build 17763)

## Technical Constraints

- WinUI 3'te GridLength animasyonu XAML Storyboard ile çalışmıyor, manuel easing kullanılıyor
- Fallback Magick decode CPU ağırdır; yalnızca WIC yetmediğinde devreye girer
- MetadataExtractor + çözünürlük için `BitmapDecoder` (WIC); codec yoksa çözünürlük satırı boş kalabilir
- Single-instance için AppLifecycle API kullanılıyor

## Dependencies

```
CommunityToolkit.Mvvm
Magick.NET-Q8-AnyCPU
MetadataExtractor
Microsoft.WindowsAppSDK
```

## Tool Usage Patterns

- **Git**: Versiyon kontrolü
- **Visual Studio**: IDE
- **dotnet CLI**: Build ve test
- **NuGet**: Paket yönetimi
