cmake_minimum_required(VERSION 3.10)
project(Minecraft_Village_Adv VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all .cpp files in src/
file(GLOB SOURCES "src/*.cpp")

# Add executable
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole")
add_executable(Minecraft_Village_Adv ${SOURCES})

if(WIN32)
    # Use vcpkg-style CONFIG mode packages
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_net CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)

    target_link_libraries(Minecraft_Village_Adv PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_net::SDL2_net
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
    )
else()
    # Assume Linux/macOS system packages
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_net REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)

    target_link_libraries(Minecraft_Village_Adv PRIVATE
        SDL2
        SDL2_image
        SDL2_net
        SDL2_ttf
        SDL2_mixer
    )

    target_include_directories(Minecraft_Village_Adv PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
    )
endif()

add_custom_command(TARGET Minecraft_Village_Adv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets" 
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)
