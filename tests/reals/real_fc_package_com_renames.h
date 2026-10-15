// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

typedef struct FC_PACKAGE_TAG* real_FC_PACKAGE_HANDLE;

/*com_wrapper.h derives the name of every symbol it generates from the wrapped handle type, so renaming the type renames all of them*/
#define FC_PACKAGE_HANDLE real_FC_PACKAGE_HANDLE
