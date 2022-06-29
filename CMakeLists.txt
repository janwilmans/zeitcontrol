cmake_minimum_required(VERSION 3.17)

project(
    zeitcontrol
    DESCRIPTION "zeitcontrol library"
    HOMEPAGE_URL "https://github.com/janwilmans/zeitcontrol"
    LANGUAGES C CXX
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
include(CTest)

add_library(zeitcontrol
    src/serialport.cpp
    src/zeitcontrol.cpp
)

target_link_libraries(zeitcontrol PRIVATE pthread)

target_include_directories(zeitcontrol PUBLIC
   fmt-8.1.1/include
   include/
) 

install(TARGETS zeitcontrol RUNTIME DESTINATION bin)

add_executable(zeit
    test/main.cpp
)

target_link_libraries(zeit PRIVATE zeitcontrol)

add_test(NAME zeit COMMAND zeit)

set(CPACK_PACKAGE_CONTACT "janwilmans@gmail.com")
set(CPACK_PACKAGE_FILE_NAME "libzeitcontrol1")
set(CPACK_PACKAGE_VERSION "1.0")

set(CPACK_DEBIAN_PACKAGE_SECTION "misc")
set(CPACK_GENERATOR "DEB")
#set(CPACK_DEB_COMPONENT_INSTALL ON)

install(DIRECTORY include/ DESTINATION include
  FILES_MATCHING 
  PATTERN "*"
  )

# this include needs to be after all install() rules
include(CPack)
