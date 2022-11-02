#Copyright (C) Microsoft Corporation. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

add_subdirectory(reals)

# unit tests
if(${run_unittests})
    build_test_folder(configuration_reader_ut)
    build_test_folder(fabric_async_op_cb_ut)
    build_test_folder(fabric_op_completed_sync_ctx_ut)
    build_test_folder(fabric_string_result_ut)
    build_test_folder(fabric_async_op_wrapper_ut)
    build_test_folder(fabric_async_op_sync_wrapper_ut)
    build_test_folder(hresult_to_string_ut)
    build_test_folder(sf_service_config_ut)
    build_test_folder(sf_c_util_reals_ut)
endif()

# int tests
if(${run_int_tests})
    build_test_folder(hresult_to_string_int)
    build_test_folder(fc_parameter_argc_argv_int)
    build_test_folder(fc_parameter_list_argc_argv_int)
    build_test_folder(common_argc_argv_int)
    build_test_folder(fc_section_argc_argv_int)
    build_test_folder(fc_section_list_argc_argv_int)
    build_test_folder(fc_package_argc_argv_int)
    build_test_folder(fc_activation_context_int)
    build_test_folder(fc_activation_context_and_sf_config_int)
endif()
