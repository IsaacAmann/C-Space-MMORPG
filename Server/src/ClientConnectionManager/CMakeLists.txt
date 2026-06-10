add_library(
	ClientConnectionManager
	ClientConnectionManager.c
	Client.c
)

#linking glib
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
target_include_directories(ClientConnectionManager PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(ClientConnectionManager INTERFACE ${GLIB_LDFLAGS})

#linking libuuid
target_link_libraries(ClientConnectionManager PRIVATE uuid)

#linking shared include folder
target_include_directories(ClientConnectionManager PUBLIC ../../../SharedInclude)

target_include_directories(ClientConnectionManager PUBLIC ../../include)

