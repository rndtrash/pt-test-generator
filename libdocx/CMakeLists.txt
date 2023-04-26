cmake_minimum_required(VERSION 3.5)

project(libdocx VERSION 0.1 LANGUAGES CXX)

set(LIBDOCX_SOURCES
	src/docx.cpp
        src/document.cpp
        src/content_types.cpp
)

set(LIBDOCX_HEADERS
	include/libdocx/libdocx.h
        include/libdocx/document.h
        include/libdocx/document_node.h
        include/libdocx/content_types.h
)

add_library(libdocx
	${LIBDOCX_SOURCES}
        ${LIBDOCX_HEADERS}
)

target_compile_features(libdocx PRIVATE cxx_std_20) # Required for std::unordered_map<>.contains()

target_include_directories(libdocx PUBLIC include)

target_link_libraries(libdocx
    pugixml
    libzippp
)

set_target_properties(libdocx PROPERTIES VERSION ${PROJECT_VERSION})

include(GNUInstallDirs)
install(TARGETS libdocx
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# TODO: public headers
