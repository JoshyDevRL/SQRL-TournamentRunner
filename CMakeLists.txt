cmake_minimum_required(VERSION 3.8)
project(SQRL_TournamentRunner LANGUAGES CXX)

add_subdirectory(RocketSim)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE JSON "JSON/*.hpp")
add_executable(SQRL_TournamentRunner ${SOURCES} ${HEADERS} ${JSON})

target_include_directories(SQRL_TournamentRunner PRIVATE RocketSim/src RocketSim/libsrc)

if (UNIX)
    target_link_libraries(SQRL_TournamentRunner RocketSim pthread)
else (UNIX)
    target_link_libraries(SQRL_TournamentRunner RocketSim)
endif (UNIX)

target_compile_features(SQRL_TournamentRunner PRIVATE cxx_std_20)

add_custom_command(TARGET SQRL_TournamentRunner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/Release
)