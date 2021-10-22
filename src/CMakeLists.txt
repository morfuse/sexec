cmake_minimum_required(VERSION 3.1)

project(sexec)

find_package(morfuse)

file(GLOB_RECURSE src "*.cpp")

add_executable(mfuse_exec ${src})

target_include_directories(mfuse_exec PRIVATE "../thirdparty/tclap/include" ${morfuse_INCLUDE_DIR})
target_link_libraries(mfuse_exec PRIVATE mfuse::morfuse)
target_compile_features(mfuse_exec PUBLIC cxx_std_17)

install(TARGETS mfuse_exec
	DESTINATION ${CMAKE_INSTALL_PREFIX}
	EXPORT mfuse_exec_targets
)

install(
	EXPORT mfuse_exec_targets
	DESTINATION .
	NAMESPACE mfuse::
)
