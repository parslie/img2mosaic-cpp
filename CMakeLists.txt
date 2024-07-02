# CMakeList.txt : CMake project for img2mosaic, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project ("img2mosaic")

# Add source to this project's executable.
add_executable (img2mosaic "src/main.cpp" "src/arguments.cpp" "src/commands/analysis.cpp" "src/commands/generation.cpp" "src/data/image.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET img2mosaic PROPERTY CXX_STANDARD 20)
endif()

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(img2mosaic PRIVATE CLI11::CLI11)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(img2mosaic PRIVATE nlohmann_json::nlohmann_json)

find_package(OpenCV CONFIG REQUIRED)
target_link_libraries(img2mosaic PRIVATE ${OpenCV_LIBS})
