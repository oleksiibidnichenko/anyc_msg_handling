project(processor_cxx17)

add_executable(${PROJECT_NAME}
    ${MAIN_SOURCE_DIR}/common_msg_handler.cxx
    ${MAIN_SOURCE_DIR}/main.cxx
    msg_handler.cxx
    )

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CXX17_INCLUDE_DIR}
    ${MAIN_INCLUDE_DIR}
    ${MESSAGES_INCLUDE_DIR}
    )

target_link_libraries(${PROJECT_NAME} PRIVATE
    network
    )
