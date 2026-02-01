add_library(
	PlayerConnectionManager
	PlayerConnectionManager.c
	Client.c
)

#linking glib
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
target_include_directories(PlayerConnectionManager PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(PlayerConnectionManager INTERFACE ${GLIB_LDFLAGS})

target_include_directories(PlayerConnectionManager PUBLIC ../../include)

