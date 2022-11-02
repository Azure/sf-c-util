#Copyright (C) Microsoft Corporation. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(sf_c_util_reals_c_files
    real_hresult_to_string.c
    real_fc_parameter_argc_argv.c
    real_fc_parameter_argc_argv.c
    real_fc_parameter_list_argc_argv.c
    real_fc_section_argc_argv.c
    real_fc_section_list_argc_argv.c
    real_common_argc_argv.c
    real_fc_package.c
    real_fc_activation_context.c
    real_fabric_string_result.c
)

set(sf_c_util_reals_h_files
    real_hresult_to_string.h
    real_hresult_to_string_renames.h

    real_fc_parameter_argc_argv.h
    real_fc_parameter_argc_argv_renames.h

    real_fc_parameter_list_argc_argv.h
    real_fc_parameter_list_argc_argv_renames.h

    real_fc_section_argc_argv.h
    real_fc_section_argc_argv_renames.h

    real_fc_section_list_argc_argv.h
    real_fc_section_list_argc_argv_renames.h

    real_common_argc_argv.h
    real_common_argc_argv_renames.h

    real_fc_package.h
    real_fc_package_renames.h
    
    real_fc_activation_context.h
    real_fc_activation_context_renames.h

    real_fabric_string_result.h
    real_fabric_string_result_renames.h
)

#self include this folder
include_directories(${CMAKE_CURRENT_LIST_DIR})

add_library(sf_c_util_reals ${sf_c_util_reals_c_files} ${sf_c_util_reals_h_files})
target_link_libraries(sf_c_util_reals c_pal_reals c_util_reals com_wrapper debug FabricUUIDD optimized FabricUUID)
target_include_directories(sf_c_util_reals PUBLIC ${CMAKE_CURRENT_LIST_DIR})
