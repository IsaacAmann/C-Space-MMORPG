
add_library(
	DatabaseService
	DatabaseConnectionManager.c
)

target_include_directories(DatabaseService PUBLIC ../../include /usr/include/mysql /usr/include/mysql/mysql )
