
add_subdirectory(Simulation)

add_library(
	GameManagement
	SectorManager.c
)

target_link_libraries(GameManagement PRIVATE Simulation)
target_link_libraries(GameManagement PRIVATE -lbox2d)
target_link_libraries(GameManagement PRIVATE -lm)



target_include_directories(DatabaseService PUBLIC ../../include)
