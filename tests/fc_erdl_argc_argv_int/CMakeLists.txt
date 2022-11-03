#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

#note: "fc_erd_argc_argv" comes from "fabric configuration endpoint resource description" and was shortened because of the build system who is unhappy with such a long filename

compileAsC11()
set(theseTestsName fc_erdl_argc_argv_int)

set(${theseTestsName}_test_files
${theseTestsName}.c
)

set(${theseTestsName}_c_files
)

set(${theseTestsName}_h_files
)

include_directories($<TARGET_PROPERTY:FabricClient,INTERFACE_INCLUDE_DIRECTORIES>)

build_test_artifacts(${theseTestsName} "tests/sf_c_util" ADDITIONAL_LIBS c_util debug FabricUUIDD optimized FabricUUID c_pal_reals sf_c_util)
