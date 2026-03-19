using System.Collections.Generic;
using System.IO;
using System.Linq;
using MetadataExtractor;
using MetadataExtractor.Formats.Exif;
using PhotoViewer.Models;
using ImageMagick;

namespace PhotoViewer.Services
{
    public class MetadataService
    {
        public List<ExifData> GetExifMetadata(string filePath)
        {
            var metadataList = new List<ExifData>();
            var fileInfo = new FileInfo(filePath);

            try
            {
                var directories = ImageMetadataReader.ReadMetadata(filePath);

                // Kamera marka-model
                var ifd0 = directories.OfType<ExifIfd0Directory>().FirstOrDefault();
                var cameraMake = ifd0?.GetDescription(ExifDirectoryBase.TagMake);
                var cameraModel = ifd0?.GetDescription(ExifDirectoryBase.TagModel);
                var camera = string.Join(" ", new[] { cameraMake, cameraModel }.Where(s => !string.IsNullOrWhiteSpace(s)));
                if (!string.IsNullOrWhiteSpace(camera))
                    metadataList.Add(new ExifData { Label = "Camera", Value = camera });

                // Çözünürlük - Magick.NET ile daha güvenilir
                try
                {
                    using (var img = new MagickImage(filePath))
                    {
                        metadataList.Add(new ExifData { Label = "Resolution", Value = $"{img.Width} x {img.Height}" });
                    }
                }
                catch
                {
                    // fallback: try directories
                }

                // Dosya boyutu
                metadataList.Add(new ExifData { Label = "File Size", Value = FormatFileSize(fileInfo.Length) });

                // Koordinatlar ve GPS Altitude
                var gps = directories.OfType<GpsDirectory>().FirstOrDefault();
                var lat = gps?.GetDescription(GpsDirectory.TagLatitude);
                var lon = gps?.GetDescription(GpsDirectory.TagLongitude);
                if (!string.IsNullOrWhiteSpace(lat) && !string.IsNullOrWhiteSpace(lon))
                    metadataList.Add(new ExifData { Label = "Coordinates", Value = $"{lat}, {lon}" });

                var altitude = gps?.GetDescription(GpsDirectory.TagAltitude);
                if (!string.IsNullOrWhiteSpace(altitude))
                    metadataList.Add(new ExifData { Label = "GPS Altitude", Value = altitude });

                // Fotoğraf formatı
                metadataList.Add(new ExifData { Label = "Format", Value = fileInfo.Extension.Trim('.').ToUpperInvariant() });
            }
            catch
            {
                metadataList.Add(new ExifData { Label = "Hata", Value = "Metadata okunamadı." });
            }

            return metadataList;
        }

        private string FormatFileSize(long bytes)
        {
            if (bytes < 1024) return $"{bytes} B";
            double kb = bytes / 1024.0;
            if (kb < 1024) return $"{kb:F1} KB";
            double mb = kb / 1024.0;
            return $"{mb:F2} MB";
        }
    }
}
