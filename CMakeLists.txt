# CMakeList.txt : CMake project for AutoCoverLetterFiller, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (App VERSION 0.9)


add_library(imgui
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/backends/imgui_impl_opengl3.cpp
    extern/imgui/backends/imgui_impl_glfw.cpp)

add_library(srcfiles
    src/AutoCoverLetterFiller.cpp
    src/AutoCoverLetterFiller.h
    src/companyData.cpp
    src/companyData.h
    src/docxAdjuster.cpp
    src/docxAdjuster.h
)

add_subdirectory(extern/DuckX)
add_subdirectory(extern/ImGuiFileDialog)
add_subdirectory(extern/glfw)

# Add source to this project's executable.
add_executable (App "src/main.cpp")


#if (CMAKE_VERSION VERSION_GREATER 3.12)
 # set_property(Target App PROPERTY CXX_STANDARD 20)
#endif()

# Linking external libraries
target_link_libraries(App
    duckx 
    ImGuiFileDialog
    imgui
    glfw
    opengl32
    srcfiles
)

target_include_directories(imgui PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/include 
    opengl32
)

target_include_directories(App PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)



