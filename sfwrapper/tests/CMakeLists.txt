#Copyright (C) Microsoft Corporation. All rights reserved.


if(${run_unittests})
    # unit tests
    build_test_folder(h_fabric_macro_generator_ut)
endif()

if(${run_int_tests})
    # integration tests
    
endif()

if(${run_perf_tests})
    # perf tests
endif()
