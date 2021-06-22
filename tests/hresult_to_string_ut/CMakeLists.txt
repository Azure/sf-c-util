#Copyright (c) Microsoft. All rights reserved.

compileAsC11()
set(theseTestsName hresult_to_string_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
hresult_to_string_mocked.c
)

set(${theseTestsName}_h_files
../../inc/hresult_to_string.h
)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS c_util sfwrapper c_pal_reals)

if("${building}" STREQUAL "exe")
#   /ignore:4217 is: "warning LNK4217: symbol 'symbol' defined in 'filename_1.obj' is imported by 'filename_2.obj' in function 'function'"
    set_target_properties(${theseTestsName}_exe PROPERTIES LINK_FLAGS "/ignore:4217")
endif()

if("${building}" STREQUAL "dll")
#   /ignore:4217 is: "warning LNK4217: symbol 'symbol' defined in 'filename_1.obj' is imported by 'filename_2.obj' in function 'function'"
    set_target_properties(${theseTestsName}_dll PROPERTIES LINK_FLAGS "/ignore:4217")
endif()