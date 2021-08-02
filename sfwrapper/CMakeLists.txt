#Copyright (C) Microsoft Corporation. All rights reserved.

set(sfwrapper_h_files
    inc/mycallback.h
    inc/servicefabricdox.h
    inc/ifabricapplicationmanagementclient10sync.h
    inc/ifabricclustermanagementclient10sync.h
    inc/ifabricfaultmanagementclientsync.h
    inc/ifabricqueryclient10sync.h
    inc/ifabricservicemanagementclient6sync.h
    inc/ifabrictestmanagementclient3sync.h
    inc/sf_macros.h

    inc/h_fabric_macro_generator.h
    inc/hfabricapplicationmanagementclient10.h
    inc/hfabricclustermanagementclient10.h
    inc/hfabricfaultmanagementclient.h
    inc/hfabricservicemanagementclient6.h
    inc/hfabrictestmanagementclient3.h
    inc/hfabricqueryclient10.h
)

set(sfwrapper_cpp_files
    src/ifabricapplicationmanagementclient10sync.cpp
    src/ifabricclustermanagementclient10sync.cpp
    src/ifabricfaultmanagementclientsync.cpp
    src/ifabricqueryclient10sync.cpp
    src/ifabricservicemanagementclient6sync.cpp
    src/ifabrictestmanagementclient3sync.cpp

    src/hfabricapplicationmanagementclient10.c
    src/hfabricclustermanagementclient10.c
    src/hfabricfaultmanagementclient.c
    src/hfabricservicemanagementclient6.c
    src/hfabrictestmanagementclient3.c
    src/hfabricqueryclient10.c
)


set(sfwrapper_c_files
    src/mycallback.c
    src/ifabricapplicationmanagementclient10sync.c
    src/ifabricclustermanagementclient10sync.c
    src/ifabricfaultmanagementclientsync.c
    src/ifabrictestmanagementclient3sync.c
    src/ifabricqueryclient10sync.c
    src/ifabricservicemanagementclient6sync.c
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

FILE(GLOB sf_wrapper_md_files "devdoc/*.md")
SOURCE_GROUP(devdoc FILES ${sf_wrapper_md_files})

add_library(sfwrapper ${sfwrapper_h_files} ${sfwrapper_c_files} ${sfwrapper_cpp_files} ${sf_wrapper_md_files})
target_link_libraries(sfwrapper c_util debug FabricClientD optimized FabricClient debug FabricUUIDD optimized FabricUUID debug FabricRuntimeD optimized FabricRuntime sf_c_util)
target_include_directories(sfwrapper PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)

add_subdirectory(tests)
