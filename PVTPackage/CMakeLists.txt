# set IDE filters
set(PROJECT_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/source)
set(PROJECT_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/source)

file(GLOB_RECURSE PROJECT_SRC ${PROJECT_SRC_DIR}/*.cpp)
file(GLOB_RECURSE PROJECT_HDR ${PROJECT_SRC_DIR}/*.hpp)

foreach(src IN LISTS PROJECT_HDR PROJECT_SRC)
    file(RELATIVE_PATH rel_scr ${CMAKE_CURRENT_LIST_DIR} ${src})
	get_filename_component(fname "${rel_scr}" NAME)
	string(REPLACE "src/" "" src_grp "${rel_scr}")
	string(REPLACE "${fname}" "" src_grp "${src_grp}")
	string(REPLACE "/" "\\" src_grp "${src_grp}")
	source_group("${src_grp}" FILES "${rel_scr}")
endforeach()

# Expose includes
set(PVTPackage_INCLUDE_DIRS ${PROJECT_INC_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(PVTPackage_target PVTPackage)

set(PVTPackage_headers ${PROJECT_HDR})

set(PVTPackage_sources ${PROJECT_SRC})

set(PVTPackage_dependencies "")

message("adding PVTPackage library")
blt_add_library(NAME                  ${PVTPackage_target}
                SOURCES               ${PVTPackage_sources}
                HEADERS               ${PVTPackage_headers}
                HEADERS_OUTPUT_SUBDIR PVTPackage
                DEPENDS_ON            ${PVTPackage_dependencies})

target_include_directories(${PVTPackage_target} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${PROJECT_SRC_DIR})

	