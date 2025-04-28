cmake_minimum_required(VERSION 3.31)
project(SFMLMonkeyTyper)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(SFML)

add_executable(
        SFMLMonkeyTyper main.cpp
)

target_link_libraries(SFMLMonkeyTyper
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)