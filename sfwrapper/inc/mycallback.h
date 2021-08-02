// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef MYCALLBACK_H
#define MYCALLBACK_H

#include "fabriccommon.h"

#ifdef __cplusplus
extern "C" {
#endif

    IFabricAsyncOperationCallback* IFabricAsyncOperationCallback_do_nothing_create(HANDLE ev); /*sort of a factory - the callback sets the event when it Invoke is called*/

#ifdef __cplusplus
}
#endif

#endif
