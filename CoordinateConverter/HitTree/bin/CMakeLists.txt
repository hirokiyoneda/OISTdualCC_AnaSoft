cmake_minimum_required(VERSION 2.8)

add_executable(app_HitTreeCoordinateConverter
HitTreeCoordinateConverter.cc
main.cc
)

target_link_libraries(app_HitTreeCoordinateConverter ${ROOT_LIBRARIES} ${libInSoft_DIR})
