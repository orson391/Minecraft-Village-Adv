cmake_minimum_required(VERSION 3.10)
project(Minecraft_Village_Adv VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set path variables
set(VCPKG_BIN_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug")
set(VCPKG_BIN_RELEASE "${CMAKE_SOURCE_DIR}/bin/release")
set(VCPKG_LIB_DEBUG "${CMAKE_SOURCE_DIR}/lib/debug")
set(VCPKG_LIB_RELEASE "${CMAKE_SOURCE_DIR}/lib/release")

set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")


# Include directories
include_directories(${SDL2_INCLUDE_DIR})

# Collect all .cpp files in src/
file(GLOB SOURCES "src/*.cpp")

# Add executable
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole")
add_executable(Minecraft_Village_Adv ${SOURCES})

# Function to Link SDL2 libraries (MinGW names)
function(link_sdl2_library target lib_base)
    target_link_libraries(${target} PRIVATE
        $<$<CONFIG:Debug>:${VCPKG_LIB_DEBUG}/lib${lib_base}d.dll.a>
        $<$<CONFIG:Release>:${VCPKG_LIB_RELEASE}/lib${lib_base}.dll.a>
    )
endfunction()

#Macro Copy required DLLs to output folder
macro(copy_sdl2_dll target lib_base)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${VCPKG_BIN_DEBUG}/${lib_base}d.dll>
            $<$<CONFIG:Release>:${VCPKG_BIN_RELEASE}/${lib_base}.dll>
            $<TARGET_FILE_DIR:${target}>
    )
endmacro()

#add all the libraries needed here
set(SDL_LIBS SDL2 SDL2_image SDL2_net SDL2_ttf SDL2_mixer)


foreach(lib ${SDL_LIBS})
    link_sdl2_library(Minecraft_Village_Adv ${lib})
    copy_sdl2_dll(Minecraft_Village_Adv ${lib})
endforeach()

#add other dependencies and assets
add_custom_command(TARGET Minecraft_Village_Adv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${VCPKG_BIN_DEBUG}/libbrotlicommon.dll>
            $<$<CONFIG:Release>:${VCPKG_BIN_RELEASE}/libbrotlicommon.dll>
            $<TARGET_FILE_DIR:Minecraft_Village_Adv>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${VCPKG_BIN_DEBUG}/libbrotlidec.dll>
            $<$<CONFIG:Release>:${VCPKG_BIN_RELEASE}/libbrotlidec.dll>
            $<TARGET_FILE_DIR:Minecraft_Village_Adv>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${VCPKG_BIN_DEBUG}/libbrotlienc.dll>
            $<$<CONFIG:Release>:${VCPKG_BIN_RELEASE}/libbrotlienc.dll>
            $<TARGET_FILE_DIR:Minecraft_Village_Adv>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${VCPKG_BIN_DEBUG}/libbz2d.dll>
            $<$<CONFIG:Release>:${VCPKG_BIN_RELEASE}/libbz2.dll>
            $<TARGET_FILE_DIR:Minecraft_Village_Adv>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${VCPKG_BIN_DEBUG}/libfreetyped.dll>
            $<$<CONFIG:Release>:${VCPKG_BIN_RELEASE}/libfreetype.dll>
            $<TARGET_FILE_DIR:Minecraft_Village_Adv>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${VCPKG_BIN_DEBUG}/libpng16d.dll>
            $<$<CONFIG:Release>:${VCPKG_BIN_RELEASE}/libpng16.dll>
            $<TARGET_FILE_DIR:Minecraft_Village_Adv>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${VCPKG_BIN_DEBUG}/libzlibd1.dll>
            $<$<CONFIG:Release>:${VCPKG_BIN_RELEASE}/libzlib1.dll>
            $<TARGET_FILE_DIR:Minecraft_Village_Adv>
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Minecraft_Village_Adv>/assets    
)
