// Copyright (C) Microsoft Corporation. All rights reserved.

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/fc_package.h"


struct FC_PACKAGE_TAG
{
    int a;
};

FC_PACKAGE_HANDLE fc_create(int a)
{
    return malloc(a + sizeof(struct FC_PACKAGE_TAG));
}

void fc_package_destroy(FC_PACKAGE_HANDLE fc_package_handle)
{
    free(fc_package_handle);
}

const FABRIC_CONFIGURATION_PACKAGE_DESCRIPTION* get_Description(FC_PACKAGE_HANDLE fc_package_handle)
{
    (void)fc_package_handle;
    return NULL;
}

