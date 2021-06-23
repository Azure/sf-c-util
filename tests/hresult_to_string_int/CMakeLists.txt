#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

compileAsC11()
set(theseTestsName hresult_to_string_int)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS sf_c_util)
