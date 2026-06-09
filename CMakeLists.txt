cmake_minimum_required(VERSION 3.26)

project(oddor_shifter_hid_driver)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVEDEV REQUIRED libevdev)
pkg_check_modules(HIDAPI REQUIRED hidapi-libusb)

add_executable(${CMAKE_PROJECT_NAME} main.c)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${LIBEVEDEV_INCLUDE_DIRS} ${HIDAPI_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIBEVEDEV_LIBRARIES} ${HIDAPI_LIBRARIES})
