cmake_minimum_required(VERSION 3.16)
project(GameProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ищем SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Указываем исходники
file(GLOB SOURCES src/*.cpp)
include_directories(include)

# Собираем исполняемый файл
add_executable(game ${SOURCES})

# Линкуем SFML
target_link_libraries(game sfml-graphics sfml-window sfml-system)
