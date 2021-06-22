#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

compileAsC11()
set(theseTestsName hresult_to_string_int)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
)

set(${theseTestsName}_cpp_files
)

set(${theseTestsName}_h_files
)

build_test_artifacts(${theseTestsName} "tests/sf_c_util")
