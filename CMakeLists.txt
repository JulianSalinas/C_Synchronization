cmake_minimum_required(VERSION 3.7)
project(C_Synchronization)

set(CMAKE_C_STANDARD 11)
set(SOURCE_FILES Sources/main.c Sources/Util/list.c Headers/Util/list.h Sources/Util/util.c Headers/Util/util.h Sources/Util/smemory.c Headers/Util/smemory.h Headers/main.h Sources/init.c Headers/init.h Sources/prod.c Headers/prod.h Sources/spy.c Headers/spy.h Sources/fin.c Headers/fin.h)

find_package( Threads REQUIRED )

# Inicializador de la memoria
add_executable(Init ${SOURCE_FILES})
target_link_libraries(Init Threads::Threads )
target_link_libraries(Init ${Boost_LIBRARIES} rt)

# Productor de procesos
add_executable(Prod ${SOURCE_FILES})
target_link_libraries(Prod Threads::Threads )
target_link_libraries(Prod ${Boost_LIBRARIES} rt)

# Espía para ver el estado de la memoria
add_executable(Spy ${SOURCE_FILES})
target_link_libraries(Spy Threads::Threads )
target_link_libraries(Spy ${Boost_LIBRARIES} rt)

# Proceso finalizador
add_executable(Fin ${SOURCE_FILES})
target_link_libraries(Fin Threads::Threads )
target_link_libraries(Fin ${Boost_LIBRARIES} rt)
