#Copyright (C) Microsoft Corporation. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(sf_c_util_reals_c_files
    real_hresult_to_string.c
)

set(sf_c_util_reals_h_files
    real_hresult_to_string.h
    real_hresult_to_string_renames.h
)

#self include this folder
include_directories(${CMAKE_CURRENT_LIST_DIR})

add_library(sf_c_util_reals ${sf_c_util_reals_c_files} ${sf_c_util_reals_h_files})
target_link_libraries(sf_c_util_reals sf_c_util)
target_include_directories(sf_c_util_reals PUBLIC ${CMAKE_CURRENT_LIST_DIR})
