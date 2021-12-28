project(processor_cxx11)

set(SOURCES main.cxx
        ../basic_async_model/basic_server.cxx
        )

add_executable(${PROJECT_NAME} ${SOURCES})
