
add_library(
	Simulation
	SectorSimulation.c
)

#linking glib
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
target_include_directories(Simulation PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(Simulation INTERFACE ${GLIB_LDFLAGS})

target_include_directories(Simulation PUBLIC ../../../include)
