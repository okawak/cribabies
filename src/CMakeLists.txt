# source files
set(SOURCES
    libbbcaenvme.c
    libbabies.c
    bbpid.c
    # modules
    modules/v7XX.c
    modules/v1190.c
    modules/madc32.c
    modules/sis3820.c
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