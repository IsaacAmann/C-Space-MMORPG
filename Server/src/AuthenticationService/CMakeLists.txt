

add_library(
	AuthenticationService
	PlayerAuthentication.c
)

#link SSL
find_package(OpenSSL REQUIRED)
target_link_libraries(AuthenticationService PRIVATE OpenSSL::SSL OpenSSL::Crypto)

target_include_directories(AuthenticationService PUBLIC ../../include)

#linking shared include folder
target_include_directories(AuthenticationService PUBLIC ../../../SharedInclude)
