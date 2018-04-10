include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(FWCT_GEOHASH_SOURCES
	GeoHashHelper.cc
)

set(FWCT_GEOHASH_LIB_DEPS
	${GLOG_LIBRARIES}
)


add_library(fwct_geohash STATIC ${FWCT_GEOHASH_SOURCES})

add_executable(test_geohash ${FWCT_GEOHASH_SOURCES} GeoHashTest.cc)
target_link_libraries(test_geohash ${FWCT_GEOHASH_LIB_DEPS})
