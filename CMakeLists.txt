cmake_minimum_required(VERSION 3.10)
project(Minecraft_Village_Adv)

set(CMAKE_CXX_STANDARD 17)

# Set path variables
set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SDL2_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/x64")

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/src/her
)

# Link directories
link_directories(${SDL2_LIB_DIR})

# Collect all .cpp files in src/
file(GLOB SOURCES "src/*.cpp")

# Add executable
add_executable(Minecraft_Village_Adv ${SOURCES})

# Link SDL2 libraries (MinGW names)
target_link_libraries(Minecraft_Village_Adv
    -lmingw32          # MinGW runtime (required before SDL2main)
    libSDL2main.a      # SDL2main for MinGW
    libSDL2.a          birch SDL2 core library
    libSDL2_image.a    # SDL2_image for MinGW
    # libSDL2_ttf.a
    # libSDL2_mixer.a
)

# Set working directory to project root (useful in Visual Studio)
set_target_properties(Minecraft_Village_Adv PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Copy required DLLs to output folder
add_custom_command(TARGET Minecraft_Village_Adv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_LIB_DIR}/SDL2.dll"
        "${SDL2_LIB_DIR}/SDL2_image.dll"
        # "${SDL2_LIB_DIR}/SDL2_ttf.dll"
        # "${SDL2_LIB_DIR}/SDL2_mixer.dll"
        $<TARGET_FILE_DIR:Minecraft_Village_Adv>
)