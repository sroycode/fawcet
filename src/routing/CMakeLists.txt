include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(FWCT_ROUTING_SOURCES
	OsrmHelper.cc
)
set(FWCT_ROUTING_LIB_DEPS
	${GLOG_LIBRARIES}
	${LibOSRM_LIBRARIES}
	${LibOSRM_DEPENDENT_LIBRARIES}
	fwct_geohash
)

add_library(fwct_routing STATIC ${FWCT_ROUTING_SOURCES})

add_executable(test_osrm ${FWCT_ROUTING_SOURCES} OsrmTest.cc)
target_link_libraries(test_osrm ${FWCT_ROUTING_LIB_DEPS})
