cmake_minimum_required(VERSION 3.30)
project(waza)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.9.1/mingw_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(waza main.cpp
        pointQuadTree.cpp
        pointQuadTree.h)
target_link_libraries(waza
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# Deployment for Windows
if (WIN32)
    # Copy MinGW runtime DLLs
    set(MINGW_DLLS
            libgcc_s_seh-1.dll
            libstdc++-6.dll
            libwinpthread-1.dll
    )

    find_program(MINGW_PATH NAMES g++.exe PATHS "C:/Qt/Tools/mingw*" PATH_SUFFIXES bin)
    if(MINGW_PATH)
        get_filename_component(MINGW_BIN_DIR ${MINGW_PATH} DIRECTORY)
        foreach(DLL ${MINGW_DLLS})
            add_custom_command(TARGET waza POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${MINGW_BIN_DIR}/${DLL}"
                    $<TARGET_FILE_DIR:waza>
                    COMMENT "Copying MinGW DLL: ${DLL}"
            )
        endforeach()
    endif()

    # Use windeployqt for Qt dependencies
    find_program(WINDEPLOYQT_EXE windeployqt HINTS ${CMAKE_PREFIX_PATH}/bin)
    if(WINDEPLOYQT_EXE)
        add_custom_command(TARGET waza POST_BUILD
                COMMAND "${WINDEPLOYQT_EXE}"
                $<TARGET_FILE:waza>
                --no-compiler-runtime
                --no-system-d3d-compiler
                --no-opengl-sw
                COMMENT "Running windeployqt..."
        )
    endif()
endif()