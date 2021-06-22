#Copyright (C) Microsoft Corporation. All rights reserved.

# unit tests
if(${run_unittests})
    build_test_folder(configuration_reader_ut)
    build_test_folder(fabric_async_op_cb_ut)
    build_test_folder(fabric_op_completed_sync_ctx_ut)
    build_test_folder(fabric_string_result_ut)
    build_test_folder(fabric_async_op_wrapper_ut)
    build_test_folder(fabric_async_op_sync_wrapper_ut)
endif()
