#Copyright (c) Microsoft. All rights reserved.

compileAsC11()
set(theseTestsName h_fabric_macro_generator_ut)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
hfabriczzzz.c
)

set(${theseTestsName}_h_files
../../inc/h_fabric_macro_generator.h
ifabriczzzz_sync.h
ifabriczzzz.h

hfabriczzzz.h
)

build_test_artifacts(${theseTestsName} "tests/sf_wrapper" ADDITIONAL_LIBS sfwrapper c_pal c_pal_reals c_util_reals)
