file(GLOB_RECURSE SRCS ./*.cpp)
file(GLOB_RECURSE HDRS ./*.h)

add_executable(raytracer raytracer.cpp ${SRCS} ${HDRS})

target_link_libraries(raytracer lvandeve)
target_link_libraries(raytracer nlohmann)
target_link_libraries(raytracer ${OpenCV_LIBS})
