Copyright (C) Microsoft Corporation. All rights reserved.

The .idls have been copied from `WindowsFabric\src\prod\src\idl\public` or from `WindowsFabric\src\prod\src\idl\internal` folders.

The .h have been copies from `WindowsFabric\src\prod\src\prebuilt\sdk` or from `WindowsFabric\src\prod\src\prebuilt\internal` folders.

The binary files (.lib) in this folder have been copied from \\reddog\Builds\branches\git_windowsfabric_release_11_0\11.0.0

[how_to](doc/how_to.md) contains more information on how to retrieve the path above.

Debug files:

"\\reddog\Builds\branches\git_windowsfabric_release_11_0\11.0.0\debug-amd64\InternalFabricUUID\InternalFabricUUID.lib"

"\\reddog\Builds\branches\git_windowsfabric_release_11_0\11.0.0\debug-amd64\FabricUUID\FabricUUID.lib"

"\\reddog\Builds\branches\git_windowsfabric_release_11_0\11.0.0\debug-amd64\FabricRuntime\FabricRuntime.lib"

"\\reddog\Builds\branches\git_windowsfabric_release_11_0\11.0.0\debug-amd64\FabricClient\FabricClient.lib"

Release files:

"\\reddog\Builds\branches\git_windowsfabric_release_11_0\11.0.0\retail-amd64\InternalFabricUUID\InternalFabricUUID.lib"

"\\reddog\Builds\branches\git_windowsfabric_release_11_0\11.0.0\retail-amd64\FabricUUID\FabricUUID.lib"

"\\reddog\Builds\branches\git_windowsfabric_release_11_0\11.0.0\retail-amd64\FabricRuntime\FabricRuntime.lib"

"\\reddog\Builds\branches\git_windowsfabric_release_11_0\11.0.0\retail-amd64\FabricClient\FabricClient.lib"


How to build (should this ever be needed):
1) in an admin cmd.exe (not the Visual Studio developer prompt), type "init.full.cmd" when in WindowsFabric repo root
2) type "build" to get the debug-amd64 target
or
2) type "build Release" to get the release-amd64 target

More information for the future readers that might lose access to reddog or just want to build locally:

Debug build took: Time Elapsed 00:39:44.46

Release build took: 00:48:42.33

CPU used was: Intel(R) Core(TM) i9-10900X CPU @ 3.70GHz

