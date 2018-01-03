################################################################################
# External Projects                                                            #
################################################################################
if (NOT "$ENV{rcsw}" EQUAL "")
ExternalProject_Add(
project_rcsw
  SOURCE_DIR "$ENV{rcsw}"
  BINARY_DIR "$ENV{rcsw}/build"
  STEP_TARGETS build
  EXCLUDE_FROM_ALL TRUE
  )
ExternalProject_Get_Property(project_rcsw binary_dir)
ExternalProject_Get_Property(project_rcsw source_dir)
add_library(rcsw STATIC IMPORTED)
set_property(TARGET rcsw PROPERTY IMPORTED_LOCATION ${binary_dir}/lib/librcsw.a)
else()
  message("FATAL: rcsw environment variable not defined!")
  return()
endif()

find_package(Boost 1.58.0 COMPONENTS system filesystem thread)
set(Boost_USE_STATIC_LIBS OFF)

################################################################################
# Includes                                                                     #
################################################################################
include_directories(${source_dir}/include)

################################################################################
# Submodules                                                                   #
################################################################################
get_filename_component(target ${CMAKE_CURRENT_LIST_DIR} NAME)

list(APPEND ${root_target}_SUBDIRS src/er)
list(APPEND ${root_target}_SUBDIRS src/multithread)
list(APPEND ${root_target}_SUBDIRS src/utils)
list(APPEND ${root_target}_SUBDIRS src/patterns/state_machine)
list(APPEND ${root_target}_SUBDIRS src/control)
list(APPEND ${root_target}_SUBDIRS src/task_allocation)
list(APPEND ${root_target}_SUBDIRS src/common)

if (WITH_MPI)
  list(APPEND ${root_target}_SUBDIRS src/multiprocess)
endif()

################################################################################
# Libraries                                                                    #
################################################################################
set(${root_target}_HAS_RECURSIVE_DIRS YES)
add_library(${root_target}
  $<TARGET_OBJECTS:er>
  $<TARGET_OBJECTS:multithread>
  $<TARGET_OBJECTS:utils>
  $<TARGET_OBJECTS:state_machine>
  $<TARGET_OBJECTS:task_allocation>
  $<TARGET_OBJECTS:common>
  $<TARGET_OBJECTS:control>)
foreach(d ${${root_target}_SUBDIRS})
  add_subdirectory(${d})
endforeach()

target_link_libraries(${root_target}
  rcsw
  ${Boost_LIBRARIES}
  )
