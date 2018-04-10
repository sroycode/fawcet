include_directories(${CMAKE_CURRENT_BINARY_DIR})

# fawcet_main

set(FWCT_FMAIN_LIB_DEPS
	${FWCT_LIB_DEPS}
	fwct_geohash
	fwct_routing
)

add_executable(fawcet_main
	Fawcet.cc
)
target_link_libraries(fawcet_main ${FWCT_FMAIN_LIB_DEPS})

set(TOOL_TARGETS ${TOOL_TARGETS} fawcet_main)

# all targets

install(TARGETS ${TOOL_TARGETS}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

