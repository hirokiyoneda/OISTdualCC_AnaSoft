cmake_minimum_required(VERSION 2.8)

add_executable(app_ExtractCoincidenceCompTree
ExtractCoincidenceCompTree.cc
main.cc
)

target_link_libraries(app_ExtractCoincidenceCompTree ${ROOT_LIBRARIES} ${libInSoft_DIR})
