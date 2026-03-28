Copyright (C) Microsoft Corporation. All rights reserved.

# Service Fabric Dependencies

## Current version

**Service Fabric 11.0.0** (built April 9, 2025, commit 6a93a5b74a3eecf0e3f9b4ebc604485aaeb15693)

There is a task to update to a more recent SF version:
https://msazure.visualstudio.com/One/_workitems/edit/37318440

## Source locations

- IDL files: copied from `WindowsFabric\src\prod\src\idl\public` or `WindowsFabric\src\prod\src\idl\internal`
- Headers: copied from `WindowsFabric\src\prod\src\prebuilt\sdk` or `WindowsFabric\src\prod\src\prebuilt\internal`
- Binary files (.lib): copied from `\\reddog\Builds\branches\git_windowsfabric_release_11_0\11.0.0`

[how_to](doc/how_to.md) contains more information on how to retrieve the paths above.

## Directory layout

```
lib/
  debug-amd64/     - x64 debug libs (FabricClient, FabricRuntime, FabricUUID, InternalFabricUUID)
  debug-arm64/     - ARM64 debug libs (same set)
  retail-amd64/    - x64 release libs (same set)
  retail-arm64/    - ARM64 release libs (same set)
inc/               - Public headers (fabricclient.h, fabricruntime.h, fabrictypes.h, etc.)
idl/               - IDL definitions
```

## Binary source paths on reddog

All from `\\reddog\Builds\branches\git_windowsfabric_release_11_0\11.0.0`:

| Directory | Reddog subfolder | Libraries |
|-----------|-----------------|-----------|
| debug-amd64 | `debug-amd64\<LibName>\<LibName>.lib` | FabricClient, FabricRuntime, FabricUUID, InternalFabricUUID |
| debug-arm64 | `debug-arm64\<LibName>\<LibName>.lib` | FabricClient, FabricRuntime, FabricUUID, InternalFabricUUID |
| retail-amd64 | `retail-amd64\<LibName>\<LibName>.lib` | FabricClient, FabricRuntime, FabricUUID, InternalFabricUUID |
| retail-arm64 | `retail-arm64\<LibName>\<LibName>.lib` | FabricClient, FabricRuntime, FabricUUID, InternalFabricUUID |

## How to build from source (should this ever be needed)

1. In an admin cmd.exe (not the Visual Studio developer prompt), type `init.full.cmd` in the WindowsFabric repo root
2. `build` for debug-amd64, or `build Release` for release-amd64

Build times observed on Intel Core i9-10900X @ 3.70GHz:
- Debug: ~40 minutes
- Release: ~49 minutes

