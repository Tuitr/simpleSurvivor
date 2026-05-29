cmake_minimum_required(VERSION 3.22)

project(PixelDungeonSurvivor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the SFML copy inside this project, not a system installation.
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML/lib/cmake/SFML")
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System)

add_executable(PixelDungeonSurvivor
    main.cpp
    src/Button.cpp
    src/DungeonMap.cpp
    src/Enemy.cpp
    src/Game.cpp
    src/HealthPack.cpp
    src/Player.cpp
    src/Projectile.cpp
    src/RecordManager.cpp
    src/Skeleton.cpp
    src/SpeedBooster.cpp
    src/TextureAtlas.cpp
    src/WaveManager.cpp
    src/WeaponBooster.cpp
    src/Weapon.cpp
)

target_include_directories(PixelDungeonSurvivor PRIVATE include)

target_compile_definitions(PixelDungeonSurvivor PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(PixelDungeonSurvivor PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

add_custom_command(TARGET PixelDungeonSurvivor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/SFML/bin"
        "$<TARGET_FILE_DIR:PixelDungeonSurvivor>"
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:PixelDungeonSurvivor>/save"
    COMMENT "Copying SFML 3 DLL files next to the executable"
)

if (MINGW)
    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)

    foreach(MINGW_DLL libstdc++-6.dll libgcc_s_seh-1.dll libwinpthread-1.dll)
        if (EXISTS "${MINGW_BIN_DIR}/${MINGW_DLL}")
            add_custom_command(TARGET PixelDungeonSurvivor POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${MINGW_BIN_DIR}/${MINGW_DLL}"
                    "$<TARGET_FILE_DIR:PixelDungeonSurvivor>"
                COMMENT "Copying MinGW runtime DLL: ${MINGW_DLL}"
            )
        endif()
    endforeach()
endif()
