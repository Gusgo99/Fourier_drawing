function(copy_resources_to_binary_directory)
	# Copies resources files to the directory with where the executable goes
	file(GLOB_RECURSE RESOURCES "${PROJECT_SOURCE_DIR}/resources/*")
	file(
		COPY
		"${PROJECT_SOURCE_DIR}/resources/layout.xrc"
		DESTINATION
		"${CMAKE_BINARY_DIR}/resources")
endfunction()