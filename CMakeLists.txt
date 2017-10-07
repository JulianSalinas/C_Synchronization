cmake_minimum_required(VERSION 3.7)
project(C_Synchronization)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES Sources/main.c Sources/Util/list.c Headers/Util/list.h Sources/Util/util.c Headers/Util/util.h Headers/main.h)
find_package( Threads REQUIRED )
add_executable(C_Synchronization ${SOURCE_FILES})
target_link_libraries( C_Synchronization Threads::Threads )