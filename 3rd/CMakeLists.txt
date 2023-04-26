#add_subdirectory(DuckX)
if (WIN32)
    add_subdirectory(vcpkg)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE
        STRING
        "Vcpkg toolchain file"
    )
else()
    add_subdirectory(libzippp)
endif()

add_subdirectory(pugixml)
