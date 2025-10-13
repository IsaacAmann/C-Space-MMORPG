
add_subdirectory(Simulation)

add_library(
	GameManagement
	SectorManager.c
)

target_link_libraries(GameManagement PRIVATE Simulation)

target_include_directories(DatabaseService PUBLIC ../../include)
