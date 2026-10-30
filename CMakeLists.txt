cmake_minimum_required(VERSION 3.16)
project(Minesweeper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Minden futtathato a build/bin mappaba keruljon, az assets melle
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# SFML 2.6 automatikus letoltese es forditasa
include(FetchContent)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SFML)

# Grafikus jatek
add_executable(Minesweeper src/main.cpp src/Textbox.h)
target_link_libraries(Minesweeper PRIVATE sfml-graphics)
if(WIN32)
    # Ne nyisson konzolablakot a jatek mellett
    set_target_properties(Minesweeper PROPERTIES WIN32_EXECUTABLE ON)
    target_link_libraries(Minesweeper PRIVATE sfml-main)
endif()

# Konzolos valtozat
add_executable(minesweeper_console console/main.cpp)

# MinGW-nel a futtatokonyvtarakat is statikusan linkeljuk,
# hogy a kesz exe-nek ne kelljen libstdc++-6.dll / libgcc_s_seh-1.dll
if(MINGW)
    target_link_options(Minesweeper PRIVATE -static-libgcc -static-libstdc++ -static)
    target_link_options(minesweeper_console PRIVATE -static-libgcc -static-libstdc++ -static)
endif()

# Assets masolasa a futtathato melle
add_custom_command(TARGET Minesweeper POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Minesweeper>
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/console/pelda.palya $<TARGET_FILE_DIR:Minesweeper>
)

# Telepites / csomagolas: csak a jatek es az assets
# (cmake --install build --component game, igy az SFML sajat fajljai kimaradnak)
install(TARGETS Minesweeper minesweeper_console DESTINATION . COMPONENT game)
install(DIRECTORY assets/ DESTINATION . COMPONENT game)
install(FILES console/pelda.palya DESTINATION . COMPONENT game)
