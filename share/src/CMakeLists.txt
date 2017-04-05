cmake_minimum_required(VERSION 2.8)

add_library(InSoft
    STATIC
    CompTreeMaker.cc
    CompTreeReader.cc
    EventTreeMaker.cc
    EventTreeReader.cc
    HitTreeMaker.cc
    HitTreeReader.cc
    TFileMaker.cc
    TFileReader.cc
)

target_link_libraries(InSoft  ${ROOT_LIBRARIES})

#install(TARGETS InSoftSO LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/lib)
