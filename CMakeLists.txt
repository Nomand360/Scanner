cmake_minimum_required(VERSION 3.28)
project(Scanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file( GLOB_RECURSE sources ${SOURCE_DIR}/*.cpp)
file( GLOB_RECURSE headers ${SOURCE_DIR}/*.h)
file (GLOB_RECURSE UI ${SOURCE_DIR}/*.ui)


find_library(Tess NAMES -ltesseract )
find_library(Lept NAMES -llept)
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

include_directories(${Leptonica_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})


# Add the executable
add_executable(${PROJECT_NAME}
        ${sources}
        ${headers}
        ${UI}
)
# Link libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} tesseract)
target_link_libraries(${PROJECT_NAME} lept)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui)

