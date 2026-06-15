cmake_minimum_required(VERSION 3.10)

project(C-Space-MMORPG-Server)

add_executable(C-Space-MMORPG-Server 
	main.c

)

set (CMAKE_BUILD_TYPE Debug)


add_subdirectory(ClientConnectionManager)
add_subdirectory(DatabaseService)
add_subdirectory(AuthenticationService)


target_link_libraries(C-Space-MMORPG-Server PRIVATE ClientConnectionManager)
target_link_libraries(C-Space-MMORPG-Server PRIVATE DatabaseService)
target_link_libraries(C-Space-MMORPG-Server PRIVATE AuthenticationService)

target_link_libraries(C-Space-MMORPG-Server PRIVATE -lmariadb)

target_include_directories(C-Space-MMORPG-Server PRIVATE ${GLIB_INCLUDE_DIRS})
