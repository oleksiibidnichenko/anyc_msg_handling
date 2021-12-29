project(processor_cxx17)

set(SOURCES main.cxx)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
    ${MAIN_INCLUDE_DIR}
    ${MESSAGES_INCLUDE_DIR}
    )
