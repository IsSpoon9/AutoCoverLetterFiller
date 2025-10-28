# CMakeLists.txt : CMake project for AutoCoverLetterFiller
cmake_minimum_required(VERSION 3.8)

project(AutoCoverLetterFiller VERSION 1.0)

# --- ImGui Library ---
add_library(imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_glfw.cpp
)
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/extern          # allows <imgui/imgui.h>
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/include
)

# --- External Dependencies ---
add_subdirectory(extern/DuckX)
add_subdirectory(extern/ImGuiFileDialog)
add_subdirectory(extern/glfw)

# Ensure ImGuiFileDialog has access to ImGui includes
target_include_directories(ImGuiFileDialog PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/extern
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends
)

# --- Main Executable ---
add_executable(AutoCoverLetterFiller
    src/gui.cpp
    src/companyData.cpp
    src/docxAdjuster.cpp
    src/main.cpp
)

# --- Linking ---
target_link_libraries(AutoCoverLetterFiller
    PRIVATE
        duckx
        ImGuiFileDialog
        imgui
        glfw
        opengl32
)

# Your app’s own source
target_include_directories(AutoCoverLetterFiller PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/extern
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/DuckX/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/DuckX/thirdparty/pugixml
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/DuckX/thirdparty/zip
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/include
)

# --- Executable Properties ---

if (WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(AutoCoverLetterFiller PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()
endif()

# Set C++ standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AutoCoverLetterFiller PROPERTY CXX_STANDARD 20)
endif()