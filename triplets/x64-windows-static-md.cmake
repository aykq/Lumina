set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

# MSVC 14.51 miscompiles libde265 init_scan_orders at /O2 (crashes at startup)
if(PORT STREQUAL "libde265")
    set(VCPKG_CXX_FLAGS_RELEASE "/O1")
    set(VCPKG_C_FLAGS_RELEASE "/O1")
endif()
