cmake_minimum_required(VERSION 3.14)

# Dynamically name the project after the folder
get_filename_component(PROJECT_NAME \${CMAKE_SOURCE_DIR} NAME)
project(\${PROJECT_NAME} LANGUAGES CXX)

# Enable compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use modern C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable verbose makefile
set(CMAKE_VERBOSE_MAKEFILE ON)

# Include local headers
include_directories(\${CMAKE_SOURCE_DIR}/include)

# Gather all source files
file(GLOB SOURCES CONFIGURE_DEPENDS \${CMAKE_SOURCE_DIR}/src/*.cpp)

# Define the executable
add_executable(main \${SOURCES})

# Set output directory
set_target_properties(main PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY \${CMAKE_SOURCE_DIR}/bin
)

# Optional: link libraries from lib/
# link_directories(\${CMAKE_SOURCE_DIR}/lib)
# target_link_libraries(main PRIVATE pdcurses)

# Optional: custom target to generate .clang_complete
add_custom_target(
  generate_clang_complete
  COMMAND python \${CMAKE_SOURCE_DIR}/scripts/cc_args.py \${CMAKE_BINARY_DIR}/compile_commands.json > \${CMAKE_SOURCE_DIR}/.clang_complete
  DEPENDS \${CMAKE_BINARY_DIR}/compile_commands.json
)
