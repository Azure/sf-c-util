// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

typedef struct FC_ACTIVATION_CONTEXT_TAG* real_FC_ACTIVATION_CONTEXT_HANDLE;

/*com_wrapper.h derives the name of every symbol it generates from the wrapped handle type, so renaming the type renames all of them*/
#define FC_ACTIVATION_CONTEXT_HANDLE real_FC_ACTIVATION_CONTEXT_HANDLE
