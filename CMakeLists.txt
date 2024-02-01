cmake_minimum_required(VERSION 3.27)
project(VoiceTranscriptionApp)
find_package(IntelSYCL REQUIRED)
find_package(MKL CONFIG REQUIRED)
cmake_policy(SET CMP0079 NEW)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsycl -fsycl-unnamed-lambda -mavx")

include_directories($ENV{DPCPP_HOME}/llvm/build/include)
include_directories($ENV{DPCPP_HOME}/llvm/build/include/sycl/)
include_directories($ENV{DPCPP_HOME}/llvm/build/lib)

#add_compile_options(-fsycl -fsycl-unnamed-lambda)
add_link_options(-fsycl -fsycl-unnamed-lambda  -mavx)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp
        src/AudioProcessingDevice.cpp
        src/AudioProcessingDevice.h
        src/SafeQueue.cpp
        src/SafeQueue.h)

# Add third party libraries.
add_subdirectory(libs)

# target_include_directories(${PROJECT_NAME} PUBLIC include)
#target_link_libraries(${PROJECT_NAME} Test)
target_compile_options(${PROJECT_NAME} PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>)
target_include_directories(${PROJECT_NAME} PUBLIC src $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(${PROJECT_NAME} $<LINK_ONLY:MKL::MKL>)


# Move rtaudiod.dll to the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/libs/rtaudio/rtaudiod.dll
        ${CMAKE_CURRENT_BINARY_DIR}/rtaudiod.dll)

# Download Models
if (MSVC)
    set(SCRIPT_EXTENSION cmd)
else ()
    set(SCRIPT_EXTENSION sh)
endif ()
add_custom_target(download_models ALL
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/libs/whisper/models/download-ggml-model.${SCRIPT_EXTENSION} base.en
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/whisper
        COMMENT "Downloading models"
        VERBATIM)

add_custom_target(create_model_dir ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/models
        COMMENT "Creating models directory"
        VERBATIM)

add_custom_target(copy_models ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/libs/whisper/models ${CMAKE_CURRENT_BINARY_DIR}/models
        COMMENT "Copying models"
        VERBATIM)
add_dependencies(copy_models create_model_dir download_models)
add_dependencies(${PROJECT_NAME} copy_models)