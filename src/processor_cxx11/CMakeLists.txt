project(processor_cxx11)

set(SOURCES main.cxx
        ../basic_async_model/basic_server.cxx
        )

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${MAIN_INCLUDE_DIR}
    ${MESSAGES_INCLUDE_DIR}
    )

target_link_libraries(${PROJECT_NAME} PRIVATE
    Threads::Threads
    )
