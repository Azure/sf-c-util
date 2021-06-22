// Copyright (c) Microsoft. All rights reserved.

#include <stdlib.h>


#include "c_logging/xlogging.h"

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "test_callback.h"

typedef struct TEST_CALLBACK_TAG
{
    int dummy;
} TEST_CALLBACK;

TEST_CALLBACK_HANDLE test_callback_create(void)
{
    TEST_CALLBACK_HANDLE result = malloc(sizeof(TEST_CALLBACK));
    if (result == NULL)
    {
        LogError("malloc failed");
    }
    else
    {
        // all OK
    }

    return result;
}

void test_callback_destroy(TEST_CALLBACK_HANDLE test_callback)
{
    if (test_callback == NULL)
    {
        LogError("Invalid arguments: TEST_CALLBACK_HANDLE test_callback=%p", test_callback);
    }
    else
    {
        free(test_callback);
    }
}

void test_callback_Invoke(TEST_CALLBACK_HANDLE test_callback, IFabricAsyncOperationContext* context)
{
    (void)test_callback;
    (void)context;
}
