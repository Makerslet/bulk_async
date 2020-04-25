cmake_minimum_required(VERSION 3.2)

set(TARGET_BIN bulk_async_test)
set(TARGET_SRC
    subscriber_mock.h
    bulk_mt_test.cpp)

set(TESTED_SRC
    ../src/command_handler.h ../src/command_handler.cpp
    ../src/commands_factory.h ../src/commands_factory.cpp)
message("TESTED_SRC: ${TESTED_SRC}")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_executable(${TARGET_BIN} ${TARGET_SRC} ${TESTED_SRC})

# включаем 17 стандарт
set_target_properties(${TARGET_BIN} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${TARGET_BIN}
    PRIVATE "${PROJECT_SOURCE_DIR}/src"
)

target_link_libraries(${TARGET_BIN}
    CONAN_PKG::gtest
    Threads::Threads
)

# максимально строгие настройки компилятора
if (MSVC)
    target_compile_options(${TARGET_BIN} PRIVATE
        /W4
    )
else ()
    target_compile_options(${TARGET_BIN} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

add_test(${TARGET_BIN} bin/${TARGET_BIN})
