// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

typedef struct FABRIC_STRING_LIST_RESULT_TAG* real_FABRIC_STRING_LIST_RESULT_HANDLE;

/*com_wrapper.h derives the name of every symbol it generates from the wrapped handle type, so renaming the type renames all of them*/
#define FABRIC_STRING_LIST_RESULT_HANDLE real_FABRIC_STRING_LIST_RESULT_HANDLE
