// Copyright (c) Microsoft. All rights reserved.


#include "windows.h"
#include "fabricruntime.h"

#define UuidToStringA mocked_UuidToStringA
#define RpcStringFreeA mocked_RpcStringFreeA
#define FabricGetNodeContext mocked_FabricGetNodeContext

RPC_STATUS mocked_UuidToStringA(
    const UUID* Uuid,
    RPC_CSTR* StringUuid);

RPC_STATUS mocked_RpcStringFreeA(
    RPC_CSTR* String);

HRESULT mocked_FabricGetNodeContext(
    void** nodeContext);

#include "../../src/sf_config_to_strings.c"
