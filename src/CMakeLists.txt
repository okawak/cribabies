# source files
set(SOURCES
    libbbcaenvme.c
    libbabies.c
    bi-pid.c
    # modules
    modules/v7XX.c
    modules/v1190.c
    modules/madc32.c
    modules/sis3820.c
)

# header files
set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/libbbcaenvme.h
    ${PROJECT_SOURCE_DIR}/include/libbabies.h
    # modules
    ${PROJECT_SOURCE_DIR}/include/modules/v7XX.h
    ${PROJECT_SOURCE_DIR}/include/modules/v1190.h
    ${PROJECT_SOURCE_DIR}/include/modules/madc32.h
    ${PROJECT_SOURCE_DIR}/include/modules/sis3820.h
)

# library
add_library(cribvme SHARED ${SOURCES})
target_include_directories(cribvme
    PUBLIC
      $<INSTALL_INTERFACE:include/cribvme>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_include_directories(cribvme
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(cribvme -lCAENVME)
install(TARGETS cribvme
    LIBRARY DESTINATION lib
)

# cmdvme
add_executable(cmdvme cmdvme.c)
target_include_directories(cmdvme PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(cmdvme cribvme)

install(TARGETS cmdvme
    RUNTIME DESTINATION bin
)

# cribabies
add_executable(cribabies cribabies.c)
target_include_directories(cribabies PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(cribabies cribvme)

install(TARGETS cribabies
    RUNTIME DESTINATION bin
)