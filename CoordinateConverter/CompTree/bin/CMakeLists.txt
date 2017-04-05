cmake_minimum_required(VERSION 2.8)

add_executable(app_CompTreeCoordinateConverter
CompTreeCoordinateConverter.cc
main.cc
)

target_link_libraries(app_CompTreeCoordinateConverter ${ROOT_LIBRARIES} ${libInSoft_DIR})
