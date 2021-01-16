cmake_minimum_required(VERSION 3.19)
project(IconWrangler)

set(CMAKE_CXX_STANDARD 17)
set(IW_CXX_FLAGS /W4 /DUNICODE /D_UNICODE)

find_package(GTest CONFIG REQUIRED)

add_executable(IconWrangler
        IconWrangler/main.cpp
        IconWrangler/ArgumentsTest.cpp IconWrangler/Arguments.cpp IconWrangler/Arguments.h
        IconWrangler/DesktopTest.cpp IconWrangler/Desktop.cpp IconWrangler/Desktop.h
)
target_compile_options(IconWrangler PRIVATE ${IW_CXX_FLAGS})
target_link_libraries(IconWrangler PRIVATE GTest::gtest)
set_target_properties(IconWrangler PROPERTIES OUTPUT_NAME iw)