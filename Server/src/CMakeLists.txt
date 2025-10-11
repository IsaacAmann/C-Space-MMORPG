cmake_minimum_required(VERSION 3.10)

project(C-Space-MMORPG-Server)

add_executable(C-Space-MMORPG-Server 
	main.c

)




add_subdirectory(PlayerConnectionManager)


target_include_directories(C-Space-MMORPG-Server PUBLIC ../include /usr/include/mysql /usr/include/mysql/mysql )
target_link_libraries(C-Space-MMORPG-Server PRIVATE PlayerConnectionManager)

target_link_libraries(C-Space-MMORPG-Server PRIVATE -lmariadb)

