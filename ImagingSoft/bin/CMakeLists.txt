cmake_minimum_required(VERSION 2.8)

add_executable(app_In_3D_ComptonImager
In_3D_ComptonImager.cc
main.cc
)

target_link_libraries(app_In_3D_ComptonImager ${ROOT_LIBRARIES} ${libInSoft_DIR})
