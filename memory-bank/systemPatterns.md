# System Patterns

## System Architecture

PhotoViewer, MVVM (Model-View-ViewModel) mimarisi ile geliştirilmiş WinUI 3 uygulamasıdır.

```
Views (XAML + Code-behind)
    └── MainWindow.xaml / MainWindow.xaml.cs
            │
            ▼
ViewModels
    └── MainViewModel.cs (ObservableObject + RelayCommand)
            │
            ├── Services
            │   ├── MetadataService.cs
            │   └── ImageLoaderService.cs
            │
            └── Models
                └── ExifData.cs
```

## Key Technical Decisions

1. **Single-Instance Pattern**: App.xaml.cs içinde AppLifecycle API kullanılarak tek kopya çalışma garantisi
2. **WriteableBitmap Display**: Magick.NET ile işlenen görüntüler WriteableBitmap'e dönüştürülerek UI'da gösterilir
3. **Panel Animations**: Manuel easing fonksiyonları ile GridLength animasyonu (WinUI3'te XAML animasyonu sorunlu)
4. **Metadata Extraction**: MetadataExtractor ile EXIF okuma, Magick.NET ile çözünürlük alma

## Design Patterns in Use

- **MVVM**: CommunityToolkit.Mvvm ile [ObservableProperty] ve [RelayCommand] attribute'ları
- **Service Layer**: MetadataService ve ImageLoaderService ile iş mantığı ayrımı
- **Single Instance**: AppInstance.FindOrRegisterForKey ile tek kopya kontrolü

## Component Relationships

- **MainWindow**: Kullanıcı etkileşimlerini yönetir, ViewModel'e komutlar gönderir
- **MainViewModel**: Görüntü yükleme, metadata toplama, navigasyon mantığını yönetir
- **MetadataService**: EXIF verilerini MetadataExtractor ile çıkarır
- **ImageLoaderService**: Görüntüleri yükler (şu an minimal implementasyon)
- **ExifData**: Metadata bilgilerini taşıyan model sınıfı

## Critical Implementation Paths

1. **Fotoğraf Yükleme**: Command-line argüman -> App.CheckForFileActivation -> ViewModel.LoadPhotoAsync -> ImageLoaderService -> WriteableBitmap -> UI
2. **Metadata Okuma**: ViewModel.LoadPhotoAsync -> MetadataService.GetExifMetadata -> ExifData listesi -> UI bindings
3. **Dosya Aktivasyonu**: AppInstance.Activated event -> CheckForFileActivation -> ViewModel.LoadPhotoAsync
