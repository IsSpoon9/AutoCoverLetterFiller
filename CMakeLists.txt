# CMakeList.txt : CMake project for AutoCoverLetterFiller, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AutoCoverLetterFiller")

add_subdirectory(extern/DuckX)

# Add source to this project's executable.
add_executable (AutoCoverLetterFiller "src/AutoCoverLetterFiller.cpp" "src/AutoCoverLetterFiller.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AutoCoverLetterFiller PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(AutoCoverLetterFiller duckx)
target_include_directories(AutoCoverLetterFiller PRIVATE extern/DuckX/include)
target_include_directories(AutoCoverLetterFiller PRIVATE extern/DuckX/thirdparty/pugixml)
target_include_directories(AutoCoverLetterFiller PRIVATE extern/DuckX/thirdparty/zip)
