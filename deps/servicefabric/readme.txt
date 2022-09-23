Copyright (C) Microsoft Corporation. All rights reserved.

The binary files in this folder have been produced on minint-gigel2.northamerica.corp.microsoft.com by anporumb on 22 sep 2022 from  WindowsFabric repo (https://msazure.visualstudio.com/One/_git/WindowsFabric) at SHA 4ab4f653cdfc764177cec769e746010b1faa6f4e tag: 9.0CU3_Windows

The CoreXT build was run in a window that had environment variable 

set cl=/ZH_SHA_256

This alleviates a binskim warning about not using /ZH:SHA_256

None of SF's official published artifacts produces proper libs/obj that are compiled with /ZH:SHA_256.



More information for the future readers:

Here's where SF's Official build pipeline is:

https://msazure.visualstudio.com/One/_build/results?buildId=59581088&view=artifacts&pathAsName=false&type=publishedArtifacts

