project(processor_dummy)

add_executable(${PROJECT_NAME}
    main.cxx
    msg_handler.cxx
    )

target_include_directories(${PROJECT_NAME} PRIVATE
    ${DUMMY_INCLUDE_DIR}
    ${MESSAGES_INCLUDE_DIR}
    )

target_link_libraries(${PROJECT_NAME} PRIVATE
    network
    )
