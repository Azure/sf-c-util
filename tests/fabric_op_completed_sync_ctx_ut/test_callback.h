// Copyright (C) Microsoft Corporation. All rights reserved.

#ifndef TEST_CALLBACK_H
#define TEST_CALLBACK_H


#include "fabriccommon.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TEST_CALLBACK_TAG* TEST_CALLBACK_HANDLE;

TEST_CALLBACK_HANDLE test_callback_create(void);
void test_callback_destroy(TEST_CALLBACK_HANDLE test_callback);
void test_callback_Invoke(TEST_CALLBACK_HANDLE test_callback, IFabricAsyncOperationContext* context);

#ifdef __cplusplus
}
#endif

#endif /* TEST_CALLBACK_H */
