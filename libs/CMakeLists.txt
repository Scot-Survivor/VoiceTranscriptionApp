set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
set( SDL_LIBC ON CACHE BOOL "" FORCE )
set( SDL_TEST OFF CACHE BOOL "" FORCE )
add_subdirectory(SDL)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2-static)

# GLEW
add_subdirectory(glew EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} libglew_static)

# ImGUI stuff
file(GLOB IMGUI_STANDARD_SRC_FILES ${CMAKE_SOURCE_DIR}/libs/imgui/*.cpp ${CMAKE_SOURCE_DIR}/libs/imgui/*.h)
message("Imgui Files: ${IMGUI_STANDARD_SRC_FILES}")
add_library(imgui STATIC
        ${IMGUI_STANDARD_SRC_FILES}
        ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_sdl2.cpp
        ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_sdl2.h
        ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl2.cpp
        ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl2.h
        )

target_include_directories(imgui PUBLIC
                            ${CMAKE_SOURCE_DIR}/libs/imgui
                            ${CMAKE_SOURCE_DIR}/libs/imgui/backends
                            ${CMAKE_SOURCE_DIR}/libs/SDL/include
        )
target_link_libraries(${PROJECT_NAME} imgui)
find_package(OpenVINO)
set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
set ( WHISPER_BUILD_TESTS OFF CACHE BOOL "" FORCE )
set ( WHISPER_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )
set( WHISPER_SDL2 ON CACHE BOOL "" FORCE )
if (OpenVINO_FOUND)
    set ( WHISPER_OPENVINO OFF CACHE BOOL "" FORCE )  # Disable this for now
    message("OpenVINO Found, Using Whisper OpenVINO")
    set(WHISPER_TARGET "whisper")
    file(GLOB WHISPER_H_FILES ${CMAKE_SOURCE_DIR}/libs/whisper/*.h ${CMAKE_SOURCE_DIR}/libs/whisper/openvino/*.h)
else()
    set ( WHISPER_OPENVINO OFF CACHE BOOL "" FORCE )
    set(WHISPER_TARGET "whisper")
    file(GLOB WHISPER_H_FILES ${CMAKE_SOURCE_DIR}/libs/whisper/*.h)
endif()
add_subdirectory(whisper)

message("Whisper Files: ${WHISPER_H_FILES}")

target_link_libraries(${PROJECT_NAME} ${WHISPER_TARGET})

include_directories(${WHISPER_H_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/whisper)

# RT Audio
set (RTAUDIO_TARGETNAME_UNINSTALL "rtaudio_uninstall" CACHE STRING "" FORCE)
add_subdirectory(rtaudio)
target_link_libraries(${PROJECT_NAME} rtaudio)