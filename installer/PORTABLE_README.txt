Lumina — Portable Version
=========================

This archive contains the portable version of Lumina.
No installer is required, but the shell thumbnail handler must be
registered manually for Explorer thumbnails to work.

Files
-----
  LuminaCpp.exe     — Main application
  LuminaShell.dll   — Shell thumbnail handler (HEIC/HEIF, JXL, AVIF, WebP)

Quick Start
-----------
1. Extract both files to any folder (e.g. C:\Tools\Lumina\)
2. Run LuminaCpp.exe directly — no installation needed

Enabling Explorer Thumbnails (optional)
-----------------------------------------
To see thumbnails for HEIC, JXL, AVIF, and WebP files in Explorer,
register the shell extension with administrator privileges:

  1. Open PowerShell or Command Prompt as Administrator
  2. Run:
       regsvr32 "C:\path\to\LuminaShell.dll"
  3. Replace the path with the actual location of LuminaShell.dll

To unregister:
       regsvr32 /u "C:\path\to\LuminaShell.dll"

Setting Lumina as Default App
-------------------------------
1. Right-click any image file in Explorer
2. Choose "Open with" > "Choose another app"
3. Select LuminaCpp.exe and tick "Always use this app"

Or go to: Settings > Apps > Default apps > Choose defaults by file type

Notes
-----
- Lumina saves window position and preferences to the registry
  under HKEY_CURRENT_USER\Software\Lumina
- To fully remove these settings after uninstalling:
  reg delete "HKCU\Software\Lumina" /f

GitHub: https://github.com/aykq/Lumina
