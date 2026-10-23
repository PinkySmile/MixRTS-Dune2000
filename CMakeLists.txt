cmake_minimum_required(VERSION 4.1)
project(MixRTS_D2K)

set(CMAKE_CXX_STANDARD 20)

add_library(
	d2k_ui MODULE
	ui/src/D2KCameo.cpp
	ui/src/D2KCameo.hpp
	ui/src/D2KUI.cpp
	ui/src/D2KUI.hpp
	ui/src/main.cpp
)
target_link_libraries(
	d2k_ui
	MixRTS_Core
)

add_library(
	d2k_support_powers MODULE
	src/main.cpp
)
target_link_libraries(
	d2k_support_powers
	MixRTS_Core
)

set(D2K_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Scanning ${D2K_SOURCE_DIR}")
file(GLOB_RECURSE D2K_ASSET_FILES CONFIGURE_DEPENDS
	"${D2K_SOURCE_DIR}/*.png"
	"${D2K_SOURCE_DIR}/*.ogg"
	"${D2K_SOURCE_DIR}/*.json"
	"${D2K_SOURCE_DIR}/*.dat"
	"${D2K_SOURCE_DIR}/*.gui"
	"${D2K_SOURCE_DIR}/*.BMP"
)

set(COPIED_FILES "")
message(STATUS "Install in ${MOD_DEST_PATH}")
foreach(D2K_ASSET_PATH ${D2K_ASSET_FILES})
	get_filename_component(FILE_NAME ${D2K_ASSET_PATH} NAME)
	cmake_path(RELATIVE_PATH D2K_ASSET_PATH BASE_DIRECTORY ${D2K_SOURCE_DIR} OUTPUT_VARIABLE D2K_ASSET_PATH_REL)

	add_custom_command(
		OUTPUT "${MOD_DEST_PATH}/${D2K_ASSET_PATH_REL}"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${D2K_ASSET_PATH}" "${MOD_DEST_PATH}/${D2K_ASSET_PATH_REL}"
		DEPENDS "${D2K_ASSET_PATH}"
		COMMENT "Copy D2K:${D2K_ASSET_PATH_REL}"
	)
	list(APPEND COPIED_FILES "${MOD_DEST_PATH}/${D2K_ASSET_PATH_REL}")
endforeach()

# Target that drives the build dependencies
add_custom_target(d2k_assets ALL DEPENDS ${COPIED_FILES})
add_dependencies(d2k_ui d2k_assets)
add_dependencies(d2k_support_powers d2k_assets)

add_custom_command(
	OUTPUT "${MOD_DEST_PATH}/ui/ui.ui"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:d2k_ui> "${MOD_DEST_PATH}/ui/ui.ui"
	DEPENDS $<TARGET_FILE:d2k_ui>
	COMMENT "Copy D2K:ui/ui.ui"
)
add_custom_command(
	OUTPUT "${MOD_DEST_PATH}/support.dl"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:d2k_support_powers> "${MOD_DEST_PATH}/support.dl"
	DEPENDS $<TARGET_FILE:d2k_support_powers>
	COMMENT "Copy D2K:support.dl"
)

add_custom_target(dune2000 ALL DEPENDS "${MOD_DEST_PATH}/ui/ui.ui" "${MOD_DEST_PATH}/support.dl")
add_dependencies(dune2000 d2k_ui)
add_dependencies(dune2000 d2k_support_powers)

if (NOT NO_TEST)
	add_library(
		d2k_ui_test MODULE
		ui/src/D2KCameo.cpp
		ui/src/D2KCameo.hpp
		ui/src/D2KUI.cpp
		ui/src/D2KUI.hpp
		ui/src/main.cpp
	)
	target_link_libraries(
		d2k_ui_test
		MixRTS_Core_Mock
	)

	add_library(
		d2k_support_powers_test MODULE
		src/main.cpp
	)
	target_link_libraries(
		d2k_support_powers_test
		MixRTS_Core_Mock
	)

	set(COPIED_FILES "")
	message(STATUS "Install in ${TEST_MOD_DEST_PATH}")
	foreach(D2K_ASSET_PATH ${D2K_ASSET_FILES})
		get_filename_component(FILE_NAME ${D2K_ASSET_PATH} NAME)
		cmake_path(RELATIVE_PATH D2K_ASSET_PATH BASE_DIRECTORY ${D2K_SOURCE_DIR} OUTPUT_VARIABLE D2K_ASSET_PATH_REL)

		add_custom_command(
			OUTPUT "${TEST_MOD_DEST_PATH}/${D2K_ASSET_PATH_REL}"
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${D2K_ASSET_PATH}" "${TEST_MOD_DEST_PATH}/${D2K_ASSET_PATH_REL}"
			DEPENDS "${D2K_ASSET_PATH}"
			COMMENT "Copy TEST_D2K:${D2K_ASSET_PATH_REL}"
		)
		list(APPEND COPIED_FILES "${TEST_MOD_DEST_PATH}/${D2K_ASSET_PATH_REL}")
	endforeach()

	# Target that drives the build dependencies
	add_custom_target(d2k_assets_test ALL DEPENDS ${COPIED_FILES})
	add_dependencies(d2k_ui_test d2k_assets_test)
	add_dependencies(d2k_support_powers_test d2k_assets_test)

	add_custom_command(
		OUTPUT "${TEST_MOD_DEST_PATH}/ui/ui.ui"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:d2k_ui> "${TEST_MOD_DEST_PATH}/ui/ui.ui"
		DEPENDS $<TARGET_FILE:d2k_ui>
		COMMENT "Copy TEST_D2K:ui/ui.ui"
	)
	add_custom_command(
		OUTPUT "${TEST_MOD_DEST_PATH}/support.dl"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:d2k_support_powers> "${TEST_MOD_DEST_PATH}/support.dl"
		DEPENDS $<TARGET_FILE:d2k_support_powers>
		COMMENT "Copy TEST_D2K:support.dl"
	)

	add_custom_target(
		dune2000_test ALL DEPENDS
		"${TEST_MOD_DEST_PATH}/ui/ui.ui"
		"${TEST_MOD_DEST_PATH}/support.dl"
	)
	add_dependencies(dune2000_test d2k_ui_test d2k_support_powers_test)
endif ()
