Copyright (C) Microsoft Corporation. All rights reserved.

The binary files in this folder have been produced on minint-gigel2.northamerica.corp.microsoft.com by anporumb on 09 Aug 2022 from  WindowsFabric repo (https://msazure.visualstudio.com/One/_git/WindowsFabric) at SHA b71bfc491d35968dd4046509ba8c1ec48b386e2e tag: 9.1CU5_Windows

The CoreXT build was run in a window that had environment variable 

set cl=/ZH_SHA_256

This alleviates a binskim warning about not using /ZH:SHA_256 (not tested for version 9.1CU5_Windows)

None of SF's official published artifacts produces proper libs/obj that are compiled with /ZH:SHA_256.

How to build:
1) in an admin cmd.exe (not the Visual Studio developer prompt), type "init.cmd" when in WindowsFabric repo root
2) type "build" to get the debug-amd64 target
or
2) type "build Release" to get the release-amd64 target

More information for the future readers:

Here's where SF's Official build pipeline is:

https://msazure.visualstudio.com/One/_build/results?buildId=59581088&view=artifacts&pathAsName=false&type=publishedArtifacts

