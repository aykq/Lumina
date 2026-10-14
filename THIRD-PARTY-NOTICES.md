# Third-party notices

Lumina itself is released under the [MIT License](LICENSE). The release binaries
(`LuminaCpp.exe`, `LuminaShell.dll`) statically link the open-source libraries below.
The full license text of each one is in [`third-party-licenses/`](third-party-licenses).

| Library | Version | License | Used for | Source |
| --- | --- | --- | --- | --- |
| libheif | 1.21.2 | LGPL-3.0-or-later | HEIC/HEIF decoding fallback | https://github.com/strukturag/libheif |
| libde265 | 1.0.16 | LGPL-3.0-or-later | HEVC decoding (used by libheif) | https://github.com/strukturag/libde265 |
| x265 | 4.1 | GPL-2.0-or-later | HEVC encoder, pulled in by libheif's default `hevc` feature | https://bitbucket.org/multicoreware/x265_git |
| libwebp | 1.6.0 | BSD-3-Clause | WebP decoding | https://chromium.googlesource.com/webm/libwebp |
| libjxl | 0.11.2 | BSD-3-Clause | JPEG XL encoding and decoding | https://github.com/libjxl/libjxl |
| libavif | 1.4.1 | BSD-2-Clause | AVIF encoding and decoding | https://github.com/AOMediaCodec/libavif |
| libaom | 3.13.1 | BSD-2-Clause | AV1 encoding and decoding (used by libavif) | https://aomedia.googlesource.com/aom |
| dav1d | 1.5.3 | BSD-2-Clause | AV1 decoding (used by libavif) | https://code.videolan.org/videolan/dav1d |
| libyuv | 1916 | BSD-3-Clause | Pixel format conversion | https://chromium.googlesource.com/libyuv/libyuv |
| libjpeg-turbo | 3.1.4.1 | IJG / BSD-3-Clause / Zlib | JPEG codec (dependency) | https://github.com/libjpeg-turbo/libjpeg-turbo |
| Highway | 1.3.0 | Apache-2.0 | SIMD helpers (used by libjxl) | https://github.com/google/highway |
| Brotli | 1.2.0 | MIT | Compression (used by libjxl) | https://github.com/google/brotli |
| Little CMS (lcms2) | 2.18 | MIT | ICC color profile handling | https://github.com/mm2/Little-CMS |

Versions are the ones pinned by the vcpkg baseline in `LuminaCpp/vcpkg.json`.

## LGPL libraries (libheif, libde265)

These libraries are linked statically. You can obtain their exact source from the links above
(the versions are in the table). Lumina's own source code is public at
https://github.com/aykq/Lumina, so you can rebuild `LuminaCpp.exe` and `LuminaShell.dll` against
a modified copy of either library by following the *Building from Source* section of the README.

## x265 (GPL-2.0-or-later)

x265 is included through libheif's default `hevc` feature. Lumina only decodes images and
never uses the encoder, but the library is statically linked into the release binaries. For that
reason the distributed binaries are subject to the terms of the GPL-2.0-or-later as well. The
complete corresponding source code is Lumina's public repository together with the x265 source
linked above. x265 is also available under a separate commercial license from MulticoreWare.

## Map data

Map tiles shown in the info panel are fetched from OpenStreetMap.
© [OpenStreetMap contributors](https://www.openstreetmap.org/copyright), available under the
[Open Database License (ODbL)](https://opendatacommons.org/licenses/odbl/).
