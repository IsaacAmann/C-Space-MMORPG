cmake_minimum_required(VERSION 3.10)

project(C-Space-MMORPG-Server)

add_executable(C-Space-MMORPG-Server 
	main.c

)




add_subdirectory(PlayerConnectionManager)
add_subdirectory(DatabaseService)
add_subdirectory(GameManagement)



target_link_libraries(C-Space-MMORPG-Server PRIVATE PlayerConnectionManager)
target_link_libraries(C-Space-MMORPG-Server PRIVATE DatabaseService)
target_link_libraries(C-Space-MMORPG-Server PRIVATE GameManagement)

target_link_libraries(C-Space-MMORPG-Server PRIVATE -lmariadb)

