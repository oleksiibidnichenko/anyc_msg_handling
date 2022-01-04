project(network)

add_library(${PROJECT_NAME} STATIC
    tcp_acceptor.cxx
    tcp_server_connection.cxx
    )

target_include_directories(${PROJECT_NAME} PUBLIC
    ${MAIN_INCLUDE_DIR}
    ${NETWORK_INCLUDE_DIR}
    )

target_link_libraries(${PROJECT_NAME} PUBLIC
    Threads::Threads
    )
