cmake_minimum_required(VERSION 3.2)
set($TARGET bulk_async)

add_library(${TARGET} SHARED
    client_descriptor.h
    async.h async.cpp
    util.h util.cpp
    base/base_command.h
    base/base_publisher.h
    base/base_subscriber.h
    base/base_workers_keeper.h
    commands.h
    commands_queue.h
    commands_factory.h commands_factory.cpp
    command_handler.h command_handler.cpp
    subscriber.h subscriber.cpp
    subscriber_handler_creator.h subscriber_handler_creator.cpp
    session_manager.h session_manager.cpp)

set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${TARGET}
    PRIVATE ${Boost_INCLUDE_DIR}
)

# максимально строгие настройки компилятора
if (MSVC)
    target_compile_options(${TARGET} PRIVATE
        /W4
    )
else ()
    target_compile_options(${TARGET} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

target_link_libraries(${TARGET}
    ${Boost_LIBRARIES}
    Threads::Threads
)

install(TARGETS ${TARGET} LIBRARY DESTINATION lib)

# генерить будем deb пакет
set(CPACK_GENERATOR DEB)

# конфигурируем deb пакет
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_CONTACT oshuev.valery@yandex.ru)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-program-options,libboost-filesystem,libboost-regex,libboost-system") 

# подключем генератор пакетов
include(CPack)
