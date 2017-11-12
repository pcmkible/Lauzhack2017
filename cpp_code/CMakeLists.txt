cmake_minimum_required (VERSION 2.6)
project (CreditSuisse)

add_definitions("-std=c++14 -o prog")
add_definitions("-lboost_system")
add_definitions("-lboost_filesystem")
add_definitions("-DBOOST_NO_CXX11_SCOPED_ENUMS")
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 COMPONENTS filesystem system) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
endif()

#-------------------------------------------------------------------------------
IF (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
	add_definitions("-Wno-tautological-compare")

ENDIF()

#-------------------------------------------------------------------------------
IF (APPLE)
	set (CMAKE_MODULE_LINKER_FLAGS "-Wl,-flat_namespace -Wl,-undefined -Wl,suppress")
ENDIF (APPLE)

#-------------------------------------------------------------------------------
#IF (POLICY CMP0043)
#	cmake_policy(SET CMP0043 OLD)
#ENDIF()
#
#-------------------------------------------------------------------------------
# Control CMAKE_BUILD_TYPE, default: Debug
#-------------------------------------------------------------------------------
#if (NOT CMAKE_BUILD_TYPE)
#	set (CMAKE_BUILD_TYPE "Debug" CACHE string "Build type" FORCE)
#endif (NOT CMAKE_BUILD_TYPE)
message (STATUS "Build type: " ${CMAKE_BUILD_TYPE})

#-------------------------------------------------------------------------------
# Add library tools (tools)
#-------------------------------------------------------------------------------
#aux_source_directory(tools TOOLS_FILES)
#add_library (tools STATIC ${TOOLS_FILES})
#
#list (INSERT LIBS 0 tools)

#-------------------------------------------------------------------------------
# Add library tools (models)
#-------------------------------------------------------------------------------

#aux_source_directory(models MODELS_FILES)
#add_library (models STATIC ${MODELS_FILES})

#list (INSERT LIBS 0 models)

#-------------------------------------------------------------------------------
# Add library tools (methods)
#-------------------------------------------------------------------------------

#aux_source_directory(methods METHODS_FILES)
#add_library (methods STATIC ${METHODS_FILES})

#list (INSERT LIBS 0 methods)

#-------------------------------------------------------------------------------
# Add library tools (methods)
#-------------------------------------------------------------------------------

#aux_source_directory(smc SMC_FILES)
#add_library (smc STATIC ${SMC_FILES})

#list (INSERT LIBS 0 smc)

#-------------------------------------------------------------------------------
# Add the executable 'main'
#-------------------------------------------------------------------------------
add_executable (main main.cpp)
target_link_libraries (main ${Boost_LIBRARIES}  ${LIBS} -lpthread)

#-------------------------------------------------------------------------------
# Add the executable 'main'
#-------------------------------------------------------------------------------
add_executable (spike_in_incoming_transactions spike_in_incoming_transactions.cpp)
target_link_libraries (spike_in_incoming_transactions ${Boost_LIBRARIES}  ${LIBS} -lpthread)
#-------------------------------------------------------------------------------
# Add the executable 'main'
#-------------------------------------------------------------------------------
add_executable (spike_in_outgoing_transactions spike_in_outgoing_transactions.cpp)
target_link_libraries (spike_in_outgoing_transactions ${Boost_LIBRARIES}  ${LIBS} -lpthread)
