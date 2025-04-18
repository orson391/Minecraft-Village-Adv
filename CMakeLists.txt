cmake_minimum_required(VERSION 3.10)
project(Minecraft_Village_Adv)

set(CMAKE_CXX_STANDARD 17)

# Set path variables
set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SDL2_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/x64")

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIR})

# Link SDL2 libraries
link_directories(${SDL2_LIB_DIR})

# Add source files
add_executable(Minecraft_Village_Adv src/main.cpp)

# Link SDL2 libraries (add more if needed)
target_link_libraries(Minecraft_Village_Adv
    SDL2.lib
    SDL2main.lib
    SDL2_image.lib
    # SDL2_ttf.lib
    # SDL2_mixer.lib
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
