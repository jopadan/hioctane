cmake_minimum_required(VERSION 3.0)
project(hioctane LANGUAGES C)

set(PROJECT_VERSION 1.0)
set(PROJECT_DESCRIPTION "Hi-Octane C port")

include(GNUInstallDirs)
include(FindPkgConfig)

set(SOURCE_FILES
	src/res/map_entry.c
	src/res/entity.c
	src/res/block.c
	src/res/column.c
	src/res/level_file.c
	src/res/model_file.c
	src/system_types.c
	src/log.c
)

set(INCLUDE_FILES
	src/system_types.h
	src/res/entity.h
	src/res/map_entry.h
	src/res/block.h
	src/res/column.h
	src/res/level_file.h
	src/res/model_file.h
	src/log.h
)

set(EXAMPLE_SOURCE_FILES
	src/main.c
)

pkg_search_module(ZIP REQUIRED libzip)
pkg_search_module(CRC REQUIRED libcrc)

add_executable( ${PROJECT_NAME} ${EXAMPLE_SOURCE_FILES} ${SOURCE_FILES}
	${INCLUDE_FILES} ${CRC_INCLUDE} ${ZIP_INCLUDE})

target_compile_options(${PROJECT_NAME} PUBLIC ${CRC_CFLAGS} ${ZIP_CFLAGS}
	-D_GNU_SOURCE)
target_include_directories(${PROJECT_NAME} PUBLIC ${CRC_INCLUDEDIR} ${ZIP_INCLUDEDIR}
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/lib${PROJECT_NAME}>
			)
target_link_libraries( ${PROJECT_NAME} PUBLIC ${CRC_LIBRARIES}
	${ZIP_LIBRARIES} -lm )



set(install_prefix "${CMAKE_INSTALL_PREFIX}")
set(install_libdir "${CMAKE_INSTALL_LIBDIR}")
set(install_includedir "${CMAKE_INSTALL_INCLUDEDIR}")

configure_file(lib${PROJECT_NAME}.pc.in ${PROJECT_BINARY_DIR}/lib${PROJECT_NAME}.pc @ONLY)

set(ENV{PKG_CONFIG_PATH} "${PROJECT_BINARY_DIR}:$ENV{PKG_CONFIG_PATH}")

install(TARGETS hioctane EXPORT ${PROJECT_NAME}Config
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	PUBLIC_HEADER DESTINATION
	${CMAKE_INSTALL_INCLUDEDIR}/lib${PROJECT_NAME}
	)

install(FILES ${INCLUDE_FILES} DESTINATION
	${CMAKE_INSTALL_INCLUDEDIR}/lib${PROJECT_NAME}/)

install(FILES lib${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/)
