#Copyright (C) Microsoft Corporation. All rights reserved.

add_library(FabricClient STATIC IMPORTED GLOBAL)
set_target_properties(FabricClient PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/retail-amd64/FabricClient.lib)
set_target_properties(FabricClient PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_library(FabricRuntime STATIC IMPORTED GLOBAL)
set_target_properties(FabricRuntime PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/retail-amd64/FabricRuntime.lib)
set_target_properties(FabricRuntime PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_library(FabricUUID STATIC IMPORTED GLOBAL)
set_target_properties(FabricUUID PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/retail-amd64/FabricUUID.lib)
set_target_properties(FabricUUID PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_library(FabricUUIDInternal STATIC IMPORTED GLOBAL)
set_target_properties(FabricUUIDInternal PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/retail-amd64/InternalFabricUUID.lib)
set_target_properties(FabricUUIDInternal PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_library(FabricClientD STATIC IMPORTED GLOBAL)
set_target_properties(FabricClientD PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug-amd64/FabricClient.lib)
set_target_properties(FabricClientD PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_library(FabricRuntimeD STATIC IMPORTED GLOBAL)
set_target_properties(FabricRuntimeD PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug-amd64/FabricRuntime.lib)
set_target_properties(FabricRuntimeD PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_library(FabricUUIDD STATIC IMPORTED GLOBAL)
set_target_properties(FabricUUIDD PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug-amd64/FabricUUID.lib)
set_target_properties(FabricUUIDD PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_library(FabricUUIDInternalD STATIC IMPORTED GLOBAL)
set_target_properties(FabricUUIDInternalD PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug-amd64/InternalFabricUUID.lib)
set_target_properties(FabricUUIDInternalD PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc)

