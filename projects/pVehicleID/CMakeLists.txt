#==============================================================================
# pVehicleID CMake configuration file
#
# Created 8-2011 by Dave Billin
#==============================================================================

#===============================
# Project Settings
#===============================
project(pVehicleID)

set(PROJECT_AUTHOR "Brandt Pedrow")
set(PROJECT_EMAIL "pedr5603@vandals.uidaho.edu")

# Set project version info
set( VERSION_MAJOR 1 )
set( VERSION_MINOR 0 )
GetSvnWorkingCopyRevision( ${PROJECT_SOURCE_DIR} VERSION_PATCH )
set( VERSION_TUPLE "${VERSION_MAJOR}.${VERSION_MINOR}${VERSION_PATCH}" )


# Set up convenience variables for project subdirectories
set( SRC_DIR ${PROJECT_SOURCE_DIR}/src )
set( INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include )
set( BUILD_DIR ${PROJECT_SOURCE_DIR}/build )
set( DOCS_DIR ${PROJECT_SOURCE_DIR}/docs )
set( TEST_DIR ${PROJECT_SOURCE_DIR}/test )
if ( NOT DEFINED MANFILE_OUTPUT_DIRECTORY )
    set( MANFILE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/docs} )
endif()

# Fill in fields of templated files
configure_file(${BUILD_DIR}/main.cpp.in
               ${SRC_DIR}/main.cpp)
configure_file(${BUILD_DIR}/AppVersion.h.in
               ${INCLUDE_DIR}/AppVersion.h )
configure_file( "${BUILD_DIR}/manfile.in"
                "${MANFILE_OUTPUT_DIRECTORY}/${PROJECT_NAME}.1" )


#===============================
# Source Files
#===============================

include_directories( ${MOOS_INCLUDE_DIRECTORIES}
                     ${INCLUDE_DIR} )

set( SOURCE_FILES
     ${SRC_DIR}/pVehicleID.cpp
     ${SRC_DIR}/pVehicleIDMain.cpp
)



#============================
# Specify the project target
#============================

# Create an executable file
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )
set_target_properties( ${PROJECT_NAME} PROPERTIES 
                       VERSION ${VERSION_TUPLE} )
                       
# Indicate how to link
target_link_libraries( ${PROJECT_NAME}
					   ${MOOS_LIBRARIES}
                       ${PLATFORM_LIBRARIES}
					 )

# Install Targets
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)