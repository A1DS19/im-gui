cmake_minimum_required(VERSION 3.31.6)
project(MyImGuiApp LANGUAGES CXX)

#------------------------------------------------------------------------------
#  Enable verbose output and C++20
#------------------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

include(FetchContent)
include_directories(${PROJECT_SOURCE_DIR}/include)

#------------------------------------------------------------------------------
#  Toggleable options
#------------------------------------------------------------------------------
option(ENABLE_FMT    "Fetch and build {fmt} library"      ON)
option(ENABLE_IMPLOT "Fetch and build ImPlot library"    ON)

#------------------------------------------------------------------------------
#  Fetch GLFW
#------------------------------------------------------------------------------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git   
  GIT_TAG        master
)
FetchContent_MakeAvailable(glfw)

#------------------------------------------------------------------------------
#  Fetch Glad
#------------------------------------------------------------------------------
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git   
  GIT_TAG        v0.1.34
)
FetchContent_MakeAvailable(glad)

#------------------------------------------------------------------------------
#  Find OpenGL
#------------------------------------------------------------------------------
find_package(OpenGL REQUIRED)

#------------------------------------------------------------------------------
#  Fetch Dear ImGui (docking branch)
#------------------------------------------------------------------------------
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git   
  GIT_TAG        docking
)
FetchContent_MakeAvailable(imgui)

file(GLOB IMGUI_SRC
  ${imgui_SOURCE_DIR}/*.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
  ${glad_SOURCE_DIR}/include
)
target_compile_definitions(imgui PUBLIC
  IMGUI_IMPL_OPENGL_LOADER_GLAD
)
target_link_libraries(imgui PUBLIC
  glfw
  glad
  OpenGL::GL
)

#------------------------------------------------------------------------------
#  Define main executable
#------------------------------------------------------------------------------
file(GLOB SOURCES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
add_executable(main ${SOURCES})

target_include_directories(main PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(main PRIVATE
  imgui
)

# Place the binary in bin/
set_target_properties(main PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

#------------------------------------------------------------------------------
#  Optional: generate .clang_complete
#------------------------------------------------------------------------------
add_custom_target(generate_clang_complete
  COMMAND python "${CMAKE_SOURCE_DIR}/scripts/cc_args.py"
          "${CMAKE_BINARY_DIR}/compile_commands.json"
          > "${CMAKE_SOURCE_DIR}/.clang_complete"
  DEPENDS "${CMAKE_BINARY_DIR}/compile_commands.json"
)

#------------------------------------------------------------------------------
#  Fetch & link {fmt}
#------------------------------------------------------------------------------
if (ENABLE_FMT)
  set(FMT_TEST OFF CACHE INTERNAL "Disable fmt tests")
  FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git   
    GIT_TAG        master
  )
  FetchContent_MakeAvailable(fmt)
  target_link_libraries(main PRIVATE fmt::fmt-header-only)
endif()

#------------------------------------------------------------------------------
#  Fetch, build & link ImPlot
#------------------------------------------------------------------------------
if (ENABLE_IMPLOT)
  FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git   
    GIT_TAG        v0.15
  )
  FetchContent_MakeAvailable(implot)

  add_library(implot STATIC
    ${implot_SOURCE_DIR}/implot.cpp
  )
  target_include_directories(implot PUBLIC
    ${implot_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
  )
  target_link_libraries(implot PUBLIC imgui)
  target_link_libraries(main PRIVATE implot)
endif()

#------------------------------------------------------------------------------
#  Installation rules (only project-defined targets)
#------------------------------------------------------------------------------
install(TARGETS main
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION include/MyImGuiApp
)
