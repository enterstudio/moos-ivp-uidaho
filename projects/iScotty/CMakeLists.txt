#==============================================================================
# iScotty CMake configuration file
#
# Created 8-2011 by Dave Billin
#==============================================================================

#===============================
# Project Settings
#===============================
project( iScotty )

set( PROJECT_AUTHOR "Dave Billin" )
set( PROJECT_EMAIL "david.billin@vandals.uidaho.edu" )

# Set project version info
set( VERSION_MAJOR 1 )
set( VERSION_MINOR 0 )
set( VERSION_PATCH 0 )
set( VERSION_TUPLE "${VERSION_MAJOR}.${VERSION_MINOR}${VERSION_PATCH}" )

# Set up convenience variables for project subdirectories
set( src_dir ${PROJECT_SOURCE_DIR}/src )
set( include_dir ${PROJECT_SOURCE_DIR}/include )
set( config_dir ${PROJECT_SOURCE_DIR}/config )
set( docs_dir ${PROJECT_SOURCE_DIR}/docs )
set( test_dir ${PROJECT_SOURCE_DIR}/test )
if ( NOT DEFINED MANFILE_OUTPUT_DIRECTORY )
    set( MANFILE_OUTPUT_DIRECTORY ${docs_dir} )
endif()


# Generate files from templates
set( generated_config_h  "${CMAKE_CURRENT_BINARY_DIR}/config.h" )
set( generated_manfile_1 "${MANFILE_OUTPUT_DIRECTORY}/${PROJECT_NAME}.1" )
set( generated_files
     ${generated_config_h}
     ${generated_manfile_1} 
   )
configure_file(${config_dir}/config.h.in   "${generated_config_h}" )
configure_file( "${config_dir}/manfile.in" "${generated_manfile_1}" )


#===============================
# Source Files
#===============================

include_directories( ${include_dir}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${LIBBUNNYSOCK_INCLUDE_DIRS}
                     ${LIBYELLOWSUBUTILS_INCLUDE_DIRS}
                     ${MOOS_INCLUDE_DIRS}
                     ${IVP_INCLUDE_DIRS}
                   )

# Create a list of source files
set( SOURCE_FILES
     ${src_dir}/ActuatorPID.cpp
     ${src_dir}/ScalarPID.cpp
     ${src_dir}/iScotty.cpp
     ${src_dir}/main.cpp
     ${src_dir}/PitchController.cpp
     ${src_dir}/ScottyModule.cpp
   )

#============================
# Specify the project target
#============================

# Create an executable file
add_executable( iScotty
                ${SOURCE_FILES}
                ${generated_files}
              )

# Indicate how to link
target_link_libraries( iScotty
                       BunnySock
                       ${MOOS_LIBRARIES}
                       ${PLATFORM_LIBRARIES}
                     )

set_target_properties( iScotty PROPERTIES 
                       VERSION ${VERSION_TUPLE} )

# Install Targets
install(TARGETS iScotty
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
