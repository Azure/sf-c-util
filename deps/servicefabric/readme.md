Copyright (C) Microsoft Corporation. All rights reserved.

# Service Fabric Dependencies

## Current version

**Service Fabric 11.4.268-internal** (SF 11 CU4, runtime 11.4.205.1, currently deployed on EBS production clusters).

## Source

The Service Fabric native libs and headers come from NuGet, restored at CMake configure time by [`CMakeLists.txt`](CMakeLists.txt) according to [`packages.config`](packages.config). One package per architecture, both pinned to the same version:

| Architecture | Package id |
|---|---|
| x64   | `ServiceFabric.NativeLibsHeaders.retail` |
| ARM64 | `ServiceFabric.NativeLibsHeaders.retail.win-arm64` |

Restore goes through `Azure-MessagingStore-Feed`, which has the SF team's `ServiceFabric` feed configured as an upstream source, so the packages are pulled transparently without needing a second feed entry in `nuget.config`.

- Our feed: `https://msazure.pkgs.visualstudio.com/One/_packaging/Azure-MessagingStore-Feed/nuget/v3/index.json`
- Upstream (SF team): `https://msazure.pkgs.visualstudio.com/One/_packaging/ServiceFabric/nuget/v3/index.json`

To update, bump `SF_NUGET_PKG_VERSION` in `CMakeLists.txt` and the two `version=` attributes in `packages.config` to a value that exists for both packages on the SF feed.

## NuGet package layout and what we use

Each per-arch package has the same shape (~33 MB). Relevant subset:

```
content/native/prebuilt/sdk/      <-- public SF SDK headers (4 .h)
content/native/prebuilt/internal/ <-- internal SF headers   (6 .h, incl. fabricruntime_.h)
lib/native/                       <-- 12 .lib files
```

Out of the 12 libs in `lib/native/`, EBS links exactly 4 (declared as IMPORTED targets in `CMakeLists.txt`):

| CMake target          | NuGet path                              | Purpose |
|-----------------------|-----------------------------------------|---|
| `FabricClient`        | `lib/native/FabricClient.lib`           | Import lib for `FabricClient.dll` (loaded at runtime from the host's installed SF) |
| `FabricRuntime`       | `lib/native/FabricRuntime.lib`          | Import lib for `FabricRuntime.dll` (same) |
| `FabricUUID`          | `lib/native/FabricUUID.lib`             | Static GUID/IID table for SF's public COM interfaces |
| `FabricUUIDInternal`  | `lib/native/InternalFabricUUID.lib`     | Static GUID/IID table for SF's internal COM interfaces (we need `IID_IFabricInternalReplicator` for queue-saturation throttling) |

The other 8 libs in `lib/native/` (`Common.lib`, `Replication.lib`, `FabricCommon.lib`, `FabricImageStore.lib`, `FabricResources.lib`, `FabricServiceCommunication.lib`, `FabricZip.lib`, `ReliableCollectionRuntime.lib`) and all the headers under `content/native/Common/`, `shared/`, `ServiceModel/` are present in the package but never referenced from EBS. They're essentially dead bytes on disk in the package cache; they cost nothing at compile or link time.

The packages are off-label for us: the nuspec calls them "Service Fabric libs and header files for generating JNI in Service Fabric Java repository". They're the only published SF-team package that contains `InternalFabricUUID.lib`, which is why we picked them over the smaller `ServiceFabric.NativeSDK[.win-arm64]`.

### Why `NativeLibsHeaders.retail` and not `ServiceFabric.NativeSDK`

The SF feed publishes two parallel package families that both contain Fabric headers and libs. The cleaner-looking option was `ServiceFabric.NativeSDK[.win-arm64]` (~310 KB), which mirrors the official SDK MSI layout: just the 4 public SDK headers, the 4 public IDLs, and 3 of the libs (`FabricClient.lib`, `FabricRuntime.lib`, `FabricUUID.lib`).

We can't use it because it deliberately excludes `InternalFabricUUID.lib`. That lib carries the GUID/IID table for SF's *internal* COM interfaces (the ones declared in `FabricRuntime_.h`, `FabricClient_.h`, etc. - note the trailing underscore), which the SF team treats as off-the-public-supportability-contract. EBS has one production code path - `bs2sf_replica_get_write_queue_state` in `block_storage_2_sf` - that has to QI from `IFabricReplicator` to `IFabricInternalReplicator` to read the replication queue saturation percentage that drives the write throttler's back-pressure FSM. There is no public equivalent of `IFabricInternalReplicator::GetReplicatorStatus` that returns synchronously from inside the replica's own process.

So we pick the bigger `NativeLibsHeaders.retail` family because it's the only one of the two that ships `InternalFabricUUID.lib` and the matching internal headers (`fabricruntime_.h` etc., placed under `content/native/prebuilt/internal/`). The downside is the size (~33 MB per arch instead of ~310 KB) and that it's an off-label "JNI" package rather than the SF team's intended C++ SDK package - so it has no formal supportability contract. If the SF team ever publishes a properly-named native SDK NuGet that includes the internal libs, that would be the natural place to migrate to.

## Why no debug binaries

The SF team only publishes retail flavors to NuGet. The `ServiceFabric.NativeLibsHeaders.debug` package was abandoned in 2024 and an ARM64 debug equivalent never existed. Debug-flavored libs are still produced internally and end up on the reddog drop (see [how_to.md](doc/how_to.md) for the manual pull procedure, kept around just in case), but they're not on any feed.

This is fine for EBS:
- `FabricClient.lib` and `FabricRuntime.lib` are tiny import libs (~4 KB and ~12 KB on x64). Their content is byte-identical between debug and retail builds.
- `FabricUUID.lib` and `InternalFabricUUID.lib` are static GUID/IID tables generated by MIDL. The IIDs themselves are part of the SF binary contract and don't differ between debug and retail.

So all EBS configurations (Debug, RelWithDebInfo, Release) link the retail libs. The CMake target naming is also collapsed accordingly: there's only `FabricClient`, not `FabricClient` + `FabricClientD`. Consumers should link the four targets unconditionally rather than using the legacy `debug FabricXD optimized FabricX` idiom.

## Runtime DLLs

Not in the NuGet package and not in this directory. `FabricClient.dll`, `FabricRuntime.dll`, `FabricCommon.dll` etc. are loaded from the host's installed Service Fabric runtime at process start. The pipeline templates that operate the cluster (in the EBS repo under `build/templates/setup_sf_runtime.yml` and friends) assume the agents already have the SF runtime installed.

