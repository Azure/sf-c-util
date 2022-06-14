// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "c_pal/gballoc_hl.h"
#include "c_pal/gballoc_hl_redirect.h"

#include "sf_c_util/configuration_wrapper.h"

#include "test_configuration_wrapper.h"

//DEFINE_CONFIGURATION_WRAPPER(my_config, L"default_config", L"MyConfigSectionName", MY_CONFIG_TEST_PARAMS);


typedef struct my_config_CONFIGURATION_TAG {
    IFabricCodePackageActivationContext* activation_context;
    const wchar_t* sf_config_name_string;
    const wchar_t* sf_parameters_section_name_string;
    uint64_t parameter_1;
    uint64_t parameter_2;
    uint32_t parameter_3;
    _Bool some_flag;
    thandle_rc_string string_option_in_thandle;
    char_ptr string_option;
    wchar_ptr wide_string_option;
    thandle_rc_string string_option_in_thandle_optional;
    char_ptr string_option_optional;
    wchar_ptr wide_string_option_optional;
    _Bool another_flag;
} my_config_CONFIGURATION;
;
typedef struct THANDLE_LL_TYPE_STRUCT_TYPE_my_config_CONFIGURATION_TAG {
    THANDLE_LL_MALLOC_FUNCTION_POINTER_T thandle_ll_malloc;
    THANDLE_LL_MALLOC_FLEX_FUNCTION_POINTER_T thandle_ll_malloc_flex;
    THANDLE_LL_FREE_FUNCTION_POINTER_T thandle_ll_free;
} THANDLE_LL_TYPE_STRUCT_TYPE_my_config_CONFIGURATION;
static const THANDLE_LL_TYPE_STRUCT_TYPE_my_config_CONFIGURATION
THANDLE_LL_TYPE_FROM_MACRO_my_config_CONFIGURATION = {
    ((void*)0), ((void*)0), ((void*)0) };
typedef struct my_config_CONFIGURATION_WRAPPER_TAG {
    char name[32];
    volatile int32_t refCount;
    THANDLE_LL_FREE_FUNCTION_POINTER_T free_function;
    void (*dispose)(my_config_CONFIGURATION*);
    my_config_CONFIGURATION data;
} my_config_CONFIGURATION_WRAPPER;
;
static my_config_CONFIGURATION*
THANDLE_LL_MALLOC_WITH_MALLOC_FUNCTIONS_my_config_CONFIGURATION(
    void (*dispose)(my_config_CONFIGURATION*),
    THANDLE_LL_MALLOC_FUNCTION_POINTER_T malloc_function,
    THANDLE_LL_FREE_FUNCTION_POINTER_T free_function) {
    my_config_CONFIGURATION* result;
    THANDLE_LL_MALLOC_FUNCTION_POINTER_T malloc_function_used;
    THANDLE_LL_FREE_FUNCTION_POINTER_T free_function_used;
    if (malloc_function != ((void*)0)) {
        malloc_function_used = malloc_function;
        free_function_used = free_function;
    }
    else if (THANDLE_LL_TYPE_FROM_MACRO_my_config_CONFIGURATION
        .thandle_ll_malloc != ((void*)0)) {
        malloc_function_used =
            THANDLE_LL_TYPE_FROM_MACRO_my_config_CONFIGURATION.thandle_ll_malloc;
        free_function_used =
            THANDLE_LL_TYPE_FROM_MACRO_my_config_CONFIGURATION.thandle_ll_free;
    }
    else {
        malloc_function_used = gballoc_hl_malloc;
        free_function_used = gballoc_hl_free;
    }
    my_config_CONFIGURATION_WRAPPER* handle_impl =
        (my_config_CONFIGURATION_WRAPPER*)malloc_function_used(
            sizeof(my_config_CONFIGURATION_WRAPPER));
    if (handle_impl == ((void*)0)) {
        do {
            {
                (void)(0 &&
                    printf(
                        "error in "
                        "malloc_function_used=%p(sizeof(THANDLE_WRAPPER_TYPE_NAME("
                        "my_config_CONFIGURATION"
                        "))=%zu)",
                        malloc_function_used,
                        sizeof(my_config_CONFIGURATION_WRAPPER)));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(
                            AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                            "configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "error in "
                            "malloc_function_used=%p(sizeof(THANDLE_WRAPPER_TYPE_NAME("
                            "my_config_CONFIGURATION"
                            "))=%zu)",
                            malloc_function_used, sizeof(my_config_CONFIGURATION_WRAPPER));
                    }
                }
            };
        } while ((void)0, 0);
        result = ((void*)0);
    }
    else {
        (void)snprintf(handle_impl->name, 32, "%s", "my_config_CONFIGURATION");
        ;
        handle_impl->dispose = dispose;
        handle_impl->free_function = free_function_used;
        (void)interlocked_exchange(&handle_impl->refCount, 1);
        result = &(handle_impl->data);
    }
    return result;
}
static my_config_CONFIGURATION* THANDLE_MALLOC_my_config_CONFIGURATION(
    void (*dispose)(my_config_CONFIGURATION*)) {
    return THANDLE_LL_MALLOC_WITH_MALLOC_FUNCTIONS_my_config_CONFIGURATION(
        dispose, ((void*)0), ((void*)0));
}
static my_config_CONFIGURATION*
THANDLE_LL_MALLOC_WITH_EXTRA_SIZE_MACRO_WITH_MALLOC_FUNCTIONS_my_config_CONFIGURATION(
    void (*dispose)(my_config_CONFIGURATION*), size_t extra_size,
    THANDLE_LL_MALLOC_FLEX_FUNCTION_POINTER_T malloc_flex_function,
    THANDLE_LL_FREE_FUNCTION_POINTER_T free_function) {
    my_config_CONFIGURATION* result;
    THANDLE_LL_MALLOC_FLEX_FUNCTION_POINTER_T malloc_flex_function_used;
    THANDLE_LL_FREE_FUNCTION_POINTER_T free_function_used;
    if (malloc_flex_function != ((void*)0)) {
        malloc_flex_function_used = malloc_flex_function;
        free_function_used = free_function;
    }
    else if (THANDLE_LL_TYPE_FROM_MACRO_my_config_CONFIGURATION
        .thandle_ll_malloc_flex != ((void*)0)) {
        malloc_flex_function_used =
            THANDLE_LL_TYPE_FROM_MACRO_my_config_CONFIGURATION
            .thandle_ll_malloc_flex;
        free_function_used =
            THANDLE_LL_TYPE_FROM_MACRO_my_config_CONFIGURATION.thandle_ll_free;
    }
    else {
        malloc_flex_function_used = gballoc_hl_malloc_flex;
        free_function_used = gballoc_hl_free;
    }
    my_config_CONFIGURATION_WRAPPER* handle_impl = malloc_flex_function_used(
        sizeof(my_config_CONFIGURATION_WRAPPER), extra_size, 1);
    if (handle_impl == ((void*)0)) {
        do {
            {
                (void)(0 && printf("error in "
                    "malloc_flex_function_used=%p(sizeof(THANDLE_"
                    "WRAPPER_TYPE_NAME("
                    "my_config_CONFIGURATION"
                    "))=%zu)",
                    malloc_flex_function_used,
                    sizeof(my_config_CONFIGURATION_WRAPPER)));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(
                            AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                            "configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "error in "
                            "malloc_flex_function_used=%p(sizeof(THANDLE_WRAPPER_TYPE_NAME("
                            "my_config_CONFIGURATION"
                            "))=%zu)",
                            malloc_flex_function_used,
                            sizeof(my_config_CONFIGURATION_WRAPPER));
                    }
                }
            };
        } while ((void)0, 0);
        result = ((void*)0);
    }
    else {
        (void)snprintf(handle_impl->name, 32, "%s", "my_config_CONFIGURATION");
        ;
        handle_impl->dispose = dispose;
        handle_impl->free_function = free_function_used;
        (void)interlocked_exchange(&handle_impl->refCount, 1);
        result = &(handle_impl->data);
    }
    return result;
}
static my_config_CONFIGURATION* my_config_CONFIGURATION_MALLOC_WITH_EXTRA_SIZE(
    void (*dispose)(my_config_CONFIGURATION*), size_t extra_size) {
    return THANDLE_LL_MALLOC_WITH_EXTRA_SIZE_MACRO_WITH_MALLOC_FUNCTIONS_my_config_CONFIGURATION(
        dispose, extra_size, ((void*)0), ((void*)0));
}
static my_config_CONFIGURATION*
THANDLE_CREATE_FROM_CONTENT_FLEX_WITH_MALLOC_FUNCTIONS_my_config_CONFIGURATION(
    const my_config_CONFIGURATION* source,
    void (*dispose)(my_config_CONFIGURATION*),
    int (*copy)(my_config_CONFIGURATION* destination,
        const my_config_CONFIGURATION* source),
    size_t(*get_sizeof)(const my_config_CONFIGURATION* source),
    THANDLE_LL_MALLOC_FLEX_FUNCTION_POINTER_T malloc_flex_function,
    THANDLE_LL_FREE_FUNCTION_POINTER_T free_function) {
    my_config_CONFIGURATION* result;
    if ((source == ((void*)0)) || (get_sizeof == ((void*)0))) {
        do {
            {
                (void)(0 &&
                    printf("invalid arguments const T* source=%p, "
                        "void(*dispose)(T*)=%p, int(*copy)(T* destination, const "
                        "T* source)=%p, size_t(*get_sizeof)(const T* source)=%p, "
                        "THANDLE_LL_MALLOC_FLEX_FUNCTION_POINTER_T "
                        "malloc_flex_function=%p, "
                        "THANDLE_LL_FREE_FUNCTION_POINTER_T free_function=%p",
                        source, dispose, copy, get_sizeof, malloc_flex_function,
                        free_function));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(
                            AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                            "configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "invalid arguments const T* source=%p, void(*dispose)(T*)=%p, "
                            "int(*copy)(T* destination, const T* source)=%p, "
                            "size_t(*get_sizeof)(const T* source)=%p, "
                            "THANDLE_LL_MALLOC_FLEX_FUNCTION_POINTER_T "
                            "malloc_flex_function=%p, THANDLE_LL_FREE_FUNCTION_POINTER_T "
                            "free_function=%p",
                            source, dispose, copy, get_sizeof, malloc_flex_function,
                            free_function);
                    }
                }
            };
        } while ((void)0, 0);
        result = ((void*)0);
    }
    else {
        THANDLE_LL_MALLOC_FLEX_FUNCTION_POINTER_T malloc_flex_function_used;
        THANDLE_LL_FREE_FUNCTION_POINTER_T free_function_used;
        if (malloc_flex_function != ((void*)0)) {
            malloc_flex_function_used = malloc_flex_function;
            free_function_used = free_function;
        }
        else if (THANDLE_LL_TYPE_FROM_MACRO_my_config_CONFIGURATION
            .thandle_ll_malloc_flex != ((void*)0)) {
            malloc_flex_function_used =
                THANDLE_LL_TYPE_FROM_MACRO_my_config_CONFIGURATION
                .thandle_ll_malloc_flex;
            free_function_used =
                THANDLE_LL_TYPE_FROM_MACRO_my_config_CONFIGURATION.thandle_ll_free;
        }
        else {
            malloc_flex_function_used = gballoc_hl_malloc_flex;
            free_function_used = gballoc_hl_free;
        }
        size_t sizeof_source = get_sizeof(source);
        my_config_CONFIGURATION_WRAPPER* handle_impl =
            (my_config_CONFIGURATION_WRAPPER*)malloc_flex_function_used(
                sizeof(my_config_CONFIGURATION_WRAPPER) -
                sizeof(my_config_CONFIGURATION),
                1, sizeof_source);
        if (handle_impl == ((void*)0)) {
            do {
                {
                    (void)(0 &&
                        printf(
                            "error in "
                            "malloc_flex_function_used=%p(sizeof(THANDLE_WRAPPER_TYPE_"
                            "NAME(T))=%zu - sizeof(T)=%zu, 1, sizeof_source=%zu)",
                            malloc_flex_function_used,
                            sizeof(my_config_CONFIGURATION_WRAPPER),
                            sizeof(my_config_CONFIGURATION), sizeof_source));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(
                                AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                                "configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "error in "
                                "malloc_flex_function_used=%p(sizeof(THANDLE_WRAPPER_TYPE_"
                                "NAME(T))=%zu - sizeof(T)=%zu, 1, sizeof_source=%zu)",
                                malloc_flex_function_used,
                                sizeof(my_config_CONFIGURATION_WRAPPER),
                                sizeof(my_config_CONFIGURATION), sizeof_source);
                        }
                    }
                };
            } while ((void)0, 0);
            result = ((void*)0);
        }
        else {
            if (copy == ((void*)0)) {
                (void)snprintf(handle_impl->name, 32, "%s", "my_config_CONFIGURATION");
                ;
                (void)memcpy(&(handle_impl->data), source, sizeof_source);
                handle_impl->dispose = dispose;
                handle_impl->free_function = free_function_used;
                (void)interlocked_exchange(&handle_impl->refCount, 1);
                result = &(handle_impl->data);
            }
            else {
                if (copy(&handle_impl->data, source) != 0) {
                    do {
                        {
                            (void)(0 &&
                                printf("failure in copy(&handle_impl->data=%p, source=%p)",
                                    &handle_impl->data, source));
                            {
                                LOGGER_LOG logger_function = xlogging_get_log_function();
                                if (logger_function != ((void*)0)) {
                                    logger_function(
                                        AZ_LOG_ERROR,
                                        "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                                        "ut\\test_configuration_wrapper.c",
                                        __FUNCDNAME__, 11, 0x01,
                                        "failure in copy(&handle_impl->data=%p, source=%p)",
                                        &handle_impl->data, source);
                                }
                            }
                        };
                    } while ((void)0, 0);
                    free_function_used(handle_impl);
                    result = ((void*)0);
                }
                else {
                    handle_impl->dispose = dispose;
                    handle_impl->free_function = free_function_used;
                    (void)interlocked_exchange(&handle_impl->refCount, 1);
                    result = &(handle_impl->data);
                }
            }
        }
    }
    return result;
}
static my_config_CONFIGURATION*
my_config_CONFIGURATION_CREATE_FROM_CONTENT_FLEX(
    const my_config_CONFIGURATION* source,
    void (*dispose)(my_config_CONFIGURATION*),
    int (*copy)(my_config_CONFIGURATION* destination,
        const my_config_CONFIGURATION* source),
    size_t(*get_sizeof)(const my_config_CONFIGURATION* source)) {
    return THANDLE_CREATE_FROM_CONTENT_FLEX_WITH_MALLOC_FUNCTIONS_my_config_CONFIGURATION(
        source, dispose, copy, get_sizeof, ((void*)0), ((void*)0));
}
static size_t
my_config_CONFIGURATION_GET_SIZE_OF(const my_config_CONFIGURATION* t) {
    return sizeof(*t);
}
static my_config_CONFIGURATION* my_config_CONFIGURATION_CREATE_FROM_CONTENT(
    const my_config_CONFIGURATION* source,
    void (*dispose)(my_config_CONFIGURATION*),
    int (*copy)(my_config_CONFIGURATION* destination,
        const my_config_CONFIGURATION* source)) {
    return my_config_CONFIGURATION_CREATE_FROM_CONTENT_FLEX(
        source, dispose, copy, my_config_CONFIGURATION_GET_SIZE_OF);
}
static void my_config_CONFIGURATION_FREE(my_config_CONFIGURATION* t) {
    if (t == ((void*)0)) {
        do {
            {
                (void)(0 && printf("invalid arg "
                    "my_config_CONFIGURATION"
                    "* t=%p",
                    t));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "invalid arg "
                            "my_config_CONFIGURATION"
                            "* t=%p",
                            t);
                    }
                }
            };
        } while ((void)0, 0);
    }
    else {
        my_config_CONFIGURATION_WRAPPER* handle_impl =
            ((my_config_CONFIGURATION_WRAPPER
                *)((PCHAR)(t)-
                    (ULONG_PTR)(&((my_config_CONFIGURATION_WRAPPER*)0)->data)));
        handle_impl->free_function(handle_impl);
    }
}
static void
my_config_CONFIGURATION_DEC_REF(CONST_P2_CONST_my_config_CONFIGURATION t) {
    my_config_CONFIGURATION_WRAPPER* handle_impl =
        ((my_config_CONFIGURATION_WRAPPER
            *)((PCHAR)(t)-
                (ULONG_PTR)(&((my_config_CONFIGURATION_WRAPPER*)0)->data)));
    if (interlocked_decrement(&handle_impl->refCount) == 0) {
        if (handle_impl->dispose != ((void*)0)) {
            handle_impl->dispose(&handle_impl->data);
        }
        handle_impl->free_function(handle_impl);
    }
}
static void
my_config_CONFIGURATION_INC_REF(CONST_P2_CONST_my_config_CONFIGURATION t) {
    my_config_CONFIGURATION_WRAPPER* handle_impl =
        ((my_config_CONFIGURATION_WRAPPER
            *)((PCHAR)(t)-
                (ULONG_PTR)(&((my_config_CONFIGURATION_WRAPPER*)0)->data)));
    (void)interlocked_increment(&handle_impl->refCount);
}
void my_config_CONFIGURATION_ASSIGN(CONST_P2_CONST_my_config_CONFIGURATION* t1,
    CONST_P2_CONST_my_config_CONFIGURATION t2) {
    if (t1 == ((void*)0)) {
        do {
            {
                (void)(0 && printf("invalid argument THANDLE("
                    "my_config_CONFIGURATION"
                    ") * t1=%p, THANDLE("
                    "my_config_CONFIGURATION"
                    ") t2=%p",
                    t1, t2));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "invalid argument THANDLE("
                            "my_config_CONFIGURATION"
                            ") * t1=%p, THANDLE("
                            "my_config_CONFIGURATION"
                            ") t2=%p",
                            t1, t2);
                    }
                }
            };
        } while ((void)0, 0);
    }
    else {
        if (*t1 == ((void*)0)) {
            if (t2 == ((void*)0)) {
            }
            else {
                my_config_CONFIGURATION_INC_REF(t2);
                *(my_config_CONFIGURATION const**)t1 = t2;
            }
        }
        else {
            if (t2 == ((void*)0)) {
                my_config_CONFIGURATION_DEC_REF(*t1);
                *(my_config_CONFIGURATION const**)t1 = t2;
            }
            else {
                my_config_CONFIGURATION_INC_REF(t2);
                my_config_CONFIGURATION_DEC_REF(*t1);
                *(my_config_CONFIGURATION const**)t1 = t2;
            }
        }
    }
}
void my_config_CONFIGURATION_INITIALIZE(
    CONST_P2_CONST_my_config_CONFIGURATION* lvalue,
    CONST_P2_CONST_my_config_CONFIGURATION rvalue) {
    if (lvalue == ((void*)0)) {
        do {
            {
                (void)(0 && printf("invalid argument THANDLE("
                    "my_config_CONFIGURATION"
                    ") * lvalue=%p, THANDLE("
                    "my_config_CONFIGURATION"
                    ") rvalue=%p",
                    lvalue, rvalue));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "invalid argument THANDLE("
                            "my_config_CONFIGURATION"
                            ") * lvalue=%p, THANDLE("
                            "my_config_CONFIGURATION"
                            ") rvalue=%p",
                            lvalue, rvalue);
                    }
                }
            };
        } while ((void)0, 0);
    }
    else {
        if (rvalue == ((void*)0)) {
        }
        else {
            my_config_CONFIGURATION_INC_REF(rvalue);
        }
        *(my_config_CONFIGURATION const**)lvalue = rvalue;
    }
}
static my_config_CONFIGURATION*
my_config_CONFIGURATION_GET_T(CONST_P2_CONST_my_config_CONFIGURATION t) {
    return (my_config_CONFIGURATION*)t;
}
const my_config_CONFIGURATION_WRAPPER* const
my_config_CONFIGURATION_INSPECT(CONST_P2_CONST_my_config_CONFIGURATION t) {
    return ((my_config_CONFIGURATION_WRAPPER
        *)((PCHAR)(t)-
            (ULONG_PTR)(&((my_config_CONFIGURATION_WRAPPER*)0)->data)));
}
void my_config_CONFIGURATION_MOVE(CONST_P2_CONST_my_config_CONFIGURATION* t1,
    CONST_P2_CONST_my_config_CONFIGURATION* t2) {
    if ((t1 == ((void*)0)) || (t2 == ((void*)0))) {
        do {
            {
                (void)(0 && printf("invalid argument THANDLE("
                    "my_config_CONFIGURATION"
                    ") * t1=%p, THANDLE("
                    "my_config_CONFIGURATION"
                    ") t2=%p",
                    t1, t2));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "invalid argument THANDLE("
                            "my_config_CONFIGURATION"
                            ") * t1=%p, THANDLE("
                            "my_config_CONFIGURATION"
                            ") t2=%p",
                            t1, t2);
                    }
                }
            };
        } while ((void)0, 0);
    }
    else {
        if (*t1 == ((void*)0)) {
            if (*t2 == ((void*)0)) {
            }
            else {
                *(my_config_CONFIGURATION const**)t1 = *t2;
                *(my_config_CONFIGURATION const**)t2 = ((void*)0);
            }
        }
        else {
            if (*t2 == ((void*)0)) {
                my_config_CONFIGURATION_DEC_REF(*t1);
                *(my_config_CONFIGURATION const**)t1 = ((void*)0);
            }
            else {
                my_config_CONFIGURATION_DEC_REF(*t1);
                *(my_config_CONFIGURATION const**)t1 = *t2;
                *(my_config_CONFIGURATION const**)t2 = ((void*)0);
            }
        }
    }
}
void my_config_CONFIGURATION_INITIALIZE_MOVE(
    CONST_P2_CONST_my_config_CONFIGURATION* t1,
    CONST_P2_CONST_my_config_CONFIGURATION* t2) {
    if ((t1 == ((void*)0)) || (t2 == ((void*)0))) {
        do {
            {
                (void)(0 && printf("invalid argument THANDLE("
                    "my_config_CONFIGURATION"
                    ") * t1=%p, THANDLE("
                    "my_config_CONFIGURATION"
                    ") t2=%p",
                    t1, t2));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "invalid argument THANDLE("
                            "my_config_CONFIGURATION"
                            ") * t1=%p, THANDLE("
                            "my_config_CONFIGURATION"
                            ") t2=%p",
                            t1, t2);
                    }
                }
            };
        } while ((void)0, 0);
    }
    else {
        if (*t2 == ((void*)0)) {
            *(my_config_CONFIGURATION const**)t1 = ((void*)0);
        }
        else {
            *(my_config_CONFIGURATION const**)t1 = *t2;
            *(my_config_CONFIGURATION const**)t2 = ((void*)0);
        }
    }
};
static void
my_config_CONFIGURATION_cleanup_fields(my_config_CONFIGURATION* handle) {
    if (handle->string_option_in_thandle != ((void*)0)) {
        RC_STRING_ASSIGN(&handle->string_option_in_thandle, ((void*)0));
    }
    if (handle->string_option != ((void*)0)) {
        gballoc_hl_free(handle->string_option);
        handle->string_option = ((void*)0);
    }
    if (handle->wide_string_option != ((void*)0)) {
        gballoc_hl_free(handle->wide_string_option);
        handle->wide_string_option = ((void*)0);
    }
    if (handle->string_option_in_thandle_optional != ((void*)0)) {
        RC_STRING_ASSIGN(&handle->string_option_in_thandle_optional, ((void*)0));
    }
    if (handle->string_option_optional != ((void*)0)) {
        gballoc_hl_free(handle->string_option_optional);
        handle->string_option_optional = ((void*)0);
    }
    if (handle->wide_string_option_optional != ((void*)0)) {
        gballoc_hl_free(handle->wide_string_option_optional);
        handle->wide_string_option_optional = ((void*)0);
    }
}
static void my_config_CONFIGURATION_dispose(my_config_CONFIGURATION* handle) {
    my_config_CONFIGURATION_cleanup_fields(handle);
    (void)handle->activation_context->lpVtbl->Release(handle->activation_context);
}
static int my_config_read_all_config_values(my_config_CONFIGURATION* handle) {
    int result;
    _Bool error_occurred = 0;
    if (!error_occurred) {
        if (configuration_reader_get_uint64_t(
            handle->activation_context, handle->sf_config_name_string,
            handle->sf_parameters_section_name_string, L"Parameter1",
            &handle->parameter_1) != 0) {
            do {
                {
                    (void)(0 && printf("configuration_reader_get_uint64_t"
                        "(\"%ls\", \"%ls\", \"%ls\") failed",
                        handle->sf_config_name_string,
                        handle->sf_parameters_section_name_string,
                        L"Parameter1"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "configuration_reader_get_uint64_t"
                                "(\"%ls\", \"%ls\", \"%ls\") failed",
                                handle->sf_config_name_string,
                                handle->sf_parameters_section_name_string,
                                L"Parameter1");
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else if (handle->parameter_1 == 0xffffffffffffffffui64) {
            do {
                {
                    (void)(0 && printf("Invalid %ls=%"
                        "llu",
                        L"Parameter1", handle->parameter_1));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "Invalid %ls=%"
                                "llu",
                                L"Parameter1", handle->parameter_1);
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else {
            do {
                {
                    (void)(0 && printf("Config loaded: %ls = %"
                        "llu",
                        L"Parameter1", handle->parameter_1));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_VERBOSE,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "Config loaded: %ls = %"
                                "llu",
                                L"Parameter1", handle->parameter_1);
                        }
                    }
                };
            } while ((void)0, 0);
        }
    }
    if (!error_occurred) {
        if (configuration_reader_get_uint64_t(
            handle->activation_context, handle->sf_config_name_string,
            handle->sf_parameters_section_name_string, L"Parameter2",
            &handle->parameter_2) != 0) {
            do {
                {
                    (void)(0 && printf("configuration_reader_get_uint64_t"
                        "(\"%ls\", \"%ls\", \"%ls\") failed",
                        handle->sf_config_name_string,
                        handle->sf_parameters_section_name_string,
                        L"Parameter2"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "configuration_reader_get_uint64_t"
                                "(\"%ls\", \"%ls\", \"%ls\") failed",
                                handle->sf_config_name_string,
                                handle->sf_parameters_section_name_string,
                                L"Parameter2");
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else if (handle->parameter_2 == 0xffffffffffffffffui64) {
            do {
                {
                    (void)(0 && printf("Invalid %ls=%"
                        "llu",
                        L"Parameter2", handle->parameter_2));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "Invalid %ls=%"
                                "llu",
                                L"Parameter2", handle->parameter_2);
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else {
            do {
                {
                    (void)(0 && printf("Config loaded: %ls = %"
                        "llu",
                        L"Parameter2", handle->parameter_2));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_VERBOSE,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "Config loaded: %ls = %"
                                "llu",
                                L"Parameter2", handle->parameter_2);
                        }
                    }
                };
            } while ((void)0, 0);
        }
    }
    if (!error_occurred) {
        if (configuration_reader_get_uint32_t(
            handle->activation_context, handle->sf_config_name_string,
            handle->sf_parameters_section_name_string,
            L"Parameter3WithLongerName", &handle->parameter_3) != 0) {
            do {
                {
                    (void)(0 && printf("configuration_reader_get_uint32_t"
                        "(\"%ls\", \"%ls\", \"%ls\") failed",
                        handle->sf_config_name_string,
                        handle->sf_parameters_section_name_string,
                        L"Parameter3WithLongerName"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "configuration_reader_get_uint32_t"
                                "(\"%ls\", \"%ls\", \"%ls\") failed",
                                handle->sf_config_name_string,
                                handle->sf_parameters_section_name_string,
                                L"Parameter3WithLongerName");
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else if (handle->parameter_3 == 0xffffffffui32) {
            do {
                {
                    (void)(0 && printf("Invalid %ls=%"
                        "u",
                        L"Parameter3WithLongerName", handle->parameter_3));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "Invalid %ls=%"
                                "u",
                                L"Parameter3WithLongerName", handle->parameter_3);
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else {
            do {
                {
                    (void)(0 && printf("Config loaded: %ls = %"
                        "u",
                        L"Parameter3WithLongerName", handle->parameter_3));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_VERBOSE,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "Config loaded: %ls = %"
                                "u",
                                L"Parameter3WithLongerName", handle->parameter_3);
                        }
                    }
                };
            } while ((void)0, 0);
        }
    }
    handle->some_flag = 0;
    if (!error_occurred) {
        if (configuration_reader_get_bool(handle->activation_context,
            handle->sf_config_name_string,
            handle->sf_parameters_section_name_string,
            L"SomeFlag", &handle->some_flag) != 0) {
            do {
                {
                    (void)(0 && printf("configuration_reader_get_bool (\"%ls\", \"%ls\", "
                        "\"%ls\") failed",
                        handle->sf_config_name_string,
                        handle->sf_parameters_section_name_string,
                        L"SomeFlag"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "configuration_reader_get_bool (\"%ls\", "
                                "\"%ls\", \"%ls\") failed",
                                handle->sf_config_name_string,
                                handle->sf_parameters_section_name_string,
                                L"SomeFlag");
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else {
            do {
                {
                    (void)(0 &&
                        printf("Config loaded: %ls = %"
                            "s (%d)",
                            L"SomeFlag",
                            &("false"
                                "\0"
                                "true"[((!!(handle->some_flag)) * sizeof("false"))]),
                            (handle->some_flag)));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(
                                AZ_LOG_VERBOSE,
                                "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                                "configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "Config loaded: %ls = %"
                                "s (%d)",
                                L"SomeFlag",
                                &("false"
                                    "\0"
                                    "true"[((!!(handle->some_flag)) * sizeof("false"))]),
                                (handle->some_flag));
                        }
                    }
                };
            } while ((void)0, 0);
        }
    }
    RC_STRING_INITIALIZE(&handle->string_option_in_thandle, ((void*)0));
    if (!error_occurred) {
        if (configuration_reader_get_thandle_rc_string(
            handle->activation_context, handle->sf_config_name_string,
            handle->sf_parameters_section_name_string, L"StringOptionThandle",
            &handle->string_option_in_thandle) != 0) {
            do {
                {
                    (void)(0 && printf("configuration_reader_get_thandle_rc_string(\"%"
                        "ls\", \"%ls\", \"%ls\") failed",
                        handle->sf_config_name_string,
                        handle->sf_parameters_section_name_string,
                        L"StringOptionThandle"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "configuration_reader_get_thandle_rc_string(\"%"
                                "ls\", \"%ls\", \"%ls\") failed",
                                handle->sf_config_name_string,
                                handle->sf_parameters_section_name_string,
                                L"StringOptionThandle");
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else {
            if (handle->string_option_in_thandle != ((void*)0) &&
                handle->string_option_in_thandle->string[0] == '\0') {
                RC_STRING_ASSIGN(&handle->string_option_in_thandle, ((void*)0));
            }
            if (handle->string_option_in_thandle == ((void*)0)) {
                do {
                    {
                        (void)(0 &&
                            printf("Invalid %ls=%"
                                "s",
                                L"StringOptionThandle",
                                (((handle->string_option_in_thandle) == ((void*)0))
                                    ? "NULL"
                                    : (handle->string_option_in_thandle)->string)));
                        {
                            LOGGER_LOG logger_function = xlogging_get_log_function();
                            if (logger_function != ((void*)0)) {
                                logger_function(
                                    AZ_LOG_ERROR,
                                    "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                                    "configuration_wrapper.c",
                                    __FUNCDNAME__, 11, 0x01,
                                    "Invalid %ls=%"
                                    "s",
                                    L"StringOptionThandle",
                                    (((handle->string_option_in_thandle) == ((void*)0))
                                        ? "NULL"
                                        : (handle->string_option_in_thandle)->string));
                            }
                        }
                    };
                } while ((void)0, 0);
                error_occurred = 1;
            }
            else {
                do {
                    {
                        (void)(0 &&
                            printf("Config loaded: %ls = %"
                                "s",
                                L"StringOptionThandle",
                                (((handle->string_option_in_thandle) == ((void*)0))
                                    ? "NULL"
                                    : (handle->string_option_in_thandle)->string)));
                        {
                            LOGGER_LOG logger_function = xlogging_get_log_function();
                            if (logger_function != ((void*)0)) {
                                logger_function(
                                    AZ_LOG_VERBOSE,
                                    "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                                    "configuration_wrapper.c",
                                    __FUNCDNAME__, 11, 0x01,
                                    "Config loaded: %ls = %"
                                    "s",
                                    L"StringOptionThandle",
                                    (((handle->string_option_in_thandle) == ((void*)0))
                                        ? "NULL"
                                        : (handle->string_option_in_thandle)->string));
                            }
                        }
                    };
                } while ((void)0, 0);
            }
        }
    }
    handle->string_option = ((void*)0);
    if (!error_occurred) {
        if (configuration_reader_get_char_string(
            handle->activation_context, handle->sf_config_name_string,
            handle->sf_parameters_section_name_string, L"MyString",
            &handle->string_option) != 0) {
            do {
                {
                    (void)(0 && printf("configuration_reader_get_char_string"
                        "(\"%ls\", \"%ls\", \"%ls\") failed",
                        handle->sf_config_name_string,
                        handle->sf_parameters_section_name_string,
                        L"MyString"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "configuration_reader_get_char_string"
                                "(\"%ls\", \"%ls\", \"%ls\") failed",
                                handle->sf_config_name_string,
                                handle->sf_parameters_section_name_string,
                                L"MyString");
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else {
            if (handle->string_option != ((void*)0) &&
                handle->string_option[0] == '\0') {
                gballoc_hl_free(handle->string_option);
                handle->string_option = ((void*)0);
            }
            if (handle->string_option == ((void*)0)) {
                do {
                    {
                        (void)(0 && printf("Invalid %ls=%"
                            "s",
                            L"MyString",
                            (((handle->string_option) != ((void*)0))
                                ? (handle->string_option)
                                : "NULL")));
                        {
                            LOGGER_LOG logger_function = xlogging_get_log_function();
                            if (logger_function != ((void*)0)) {
                                logger_function(AZ_LOG_ERROR,
                                    "C:\\src\\sf-c-util\\tests\\configuration_"
                                    "wrapper_ut\\test_configuration_wrapper.c",
                                    __FUNCDNAME__, 11, 0x01,
                                    "Invalid %ls=%"
                                    "s",
                                    L"MyString",
                                    (((handle->string_option) != ((void*)0))
                                        ? (handle->string_option)
                                        : "NULL"));
                            }
                        }
                    };
                } while ((void)0, 0);
                error_occurred = 1;
            }
            else {
                do {
                    {
                        (void)(0 && printf("Config loaded: %ls = %"
                            "s",
                            L"MyString",
                            (((handle->string_option) != ((void*)0))
                                ? (handle->string_option)
                                : "NULL")));
                        {
                            LOGGER_LOG logger_function = xlogging_get_log_function();
                            if (logger_function != ((void*)0)) {
                                logger_function(AZ_LOG_VERBOSE,
                                    "C:\\src\\sf-c-util\\tests\\configuration_"
                                    "wrapper_ut\\test_configuration_wrapper.c",
                                    __FUNCDNAME__, 11, 0x01,
                                    "Config loaded: %ls = %"
                                    "s",
                                    L"MyString",
                                    (((handle->string_option) != ((void*)0))
                                        ? (handle->string_option)
                                        : "NULL"));
                            }
                        }
                    };
                } while ((void)0, 0);
            }
        }
    }
    handle->wide_string_option = ((void*)0);
    if (!error_occurred) {
        if (configuration_reader_get_wchar_string(
            handle->activation_context, handle->sf_config_name_string,
            handle->sf_parameters_section_name_string, L"MyWideString",
            &handle->wide_string_option) != 0) {
            do {
                {
                    (void)(0 && printf("configuration_reader_get_wchar_string"
                        "(\"%ls\", \"%ls\", \"%ls\") failed",
                        handle->sf_config_name_string,
                        handle->sf_parameters_section_name_string,
                        L"MyWideString"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "configuration_reader_get_wchar_string"
                                "(\"%ls\", \"%ls\", \"%ls\") failed",
                                handle->sf_config_name_string,
                                handle->sf_parameters_section_name_string,
                                L"MyWideString");
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else {
            if (handle->wide_string_option != ((void*)0) &&
                handle->wide_string_option[0] == L'\0') {
                gballoc_hl_free(handle->wide_string_option);
                handle->wide_string_option = ((void*)0);
            }
            if (handle->wide_string_option == ((void*)0)) {
                do {
                    {
                        (void)(0 && printf("Invalid %ls=%"
                            "ls",
                            L"MyWideString",
                            (((handle->wide_string_option) != ((void*)0))
                                ? (handle->wide_string_option)
                                : L"NULL")));
                        {
                            LOGGER_LOG logger_function = xlogging_get_log_function();
                            if (logger_function != ((void*)0)) {
                                logger_function(AZ_LOG_ERROR,
                                    "C:\\src\\sf-c-util\\tests\\configuration_"
                                    "wrapper_ut\\test_configuration_wrapper.c",
                                    __FUNCDNAME__, 11, 0x01,
                                    "Invalid %ls=%"
                                    "ls",
                                    L"MyWideString",
                                    (((handle->wide_string_option) != ((void*)0))
                                        ? (handle->wide_string_option)
                                        : L"NULL"));
                            }
                        }
                    };
                } while ((void)0, 0);
                error_occurred = 1;
            }
            else {
                do {
                    {
                        (void)(0 && printf("Config loaded: %ls = %"
                            "ls",
                            L"MyWideString",
                            (((handle->wide_string_option) != ((void*)0))
                                ? (handle->wide_string_option)
                                : L"NULL")));
                        {
                            LOGGER_LOG logger_function = xlogging_get_log_function();
                            if (logger_function != ((void*)0)) {
                                logger_function(AZ_LOG_VERBOSE,
                                    "C:\\src\\sf-c-util\\tests\\configuration_"
                                    "wrapper_ut\\test_configuration_wrapper.c",
                                    __FUNCDNAME__, 11, 0x01,
                                    "Config loaded: %ls = %"
                                    "ls",
                                    L"MyWideString",
                                    (((handle->wide_string_option) != ((void*)0))
                                        ? (handle->wide_string_option)
                                        : L"NULL"));
                            }
                        }
                    };
                } while ((void)0, 0);
            }
        }
    }
    handle->another_flag = 0;
    if (!error_occurred) {
        if (configuration_reader_get_bool(
            handle->activation_context, handle->sf_config_name_string,
            handle->sf_parameters_section_name_string, L"AnotherFlag",
            &handle->another_flag) != 0) {
            do {
                {
                    (void)(0 && printf("configuration_reader_get_bool (\"%ls\", \"%ls\", "
                        "\"%ls\") failed",
                        handle->sf_config_name_string,
                        handle->sf_parameters_section_name_string,
                        L"AnotherFlag"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "configuration_reader_get_bool (\"%ls\", "
                                "\"%ls\", \"%ls\") failed",
                                handle->sf_config_name_string,
                                handle->sf_parameters_section_name_string,
                                L"AnotherFlag");
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else {
            do {
                {
                    (void)(0 &&
                        printf(
                            "Config loaded: %ls = %"
                            "s (%d)",
                            L"AnotherFlag",
                            &("false"
                                "\0"
                                "true"[((!!(handle->another_flag)) * sizeof("false"))]),
                            (handle->another_flag)));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(
                                AZ_LOG_VERBOSE,
                                "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                                "configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "Config loaded: %ls = %"
                                "s (%d)",
                                L"AnotherFlag",
                                &("false"
                                    "\0"
                                    "true"[((!!(handle->another_flag)) * sizeof("false"))]),
                                (handle->another_flag));
                        }
                    }
                };
            } while ((void)0, 0);
        }
    };
    RC_STRING_INITIALIZE(&handle->string_option_in_thandle_optional, ((void*)0));
    if (!error_occurred) {
        if (configuration_reader_get_thandle_rc_string(
            handle->activation_context, handle->sf_config_name_string,
            handle->sf_parameters_section_name_string,
            L"OptionalStringOptionThandle",
            &handle->string_option_in_thandle_optional) != 0) {
            do {
                {
                    (void)(0 && printf("configuration_reader_get_thandle_rc_string(\"%"
                        "ls\", \"%ls\", \"%ls\") failed",
                        handle->sf_config_name_string,
                        handle->sf_parameters_section_name_string,
                        L"OptionalStringOptionThandle"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "configuration_reader_get_thandle_rc_string(\"%"
                                "ls\", \"%ls\", \"%ls\") failed",
                                handle->sf_config_name_string,
                                handle->sf_parameters_section_name_string,
                                L"OptionalStringOptionThandle");
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else {
            if (handle->string_option_in_thandle_optional != ((void*)0) &&
                handle->string_option_in_thandle_optional->string[0] == '\0') {
                RC_STRING_ASSIGN(&handle->string_option_in_thandle_optional,
                    ((void*)0));
            }
            {
                do {
                    {
                        (void)(0 &&
                            printf("Config loaded: %ls = %"
                                "s",
                                L"OptionalStringOptionThandle",
                                (((handle->string_option_in_thandle_optional) ==
                                    ((void*)0))
                                    ? "NULL"
                                    : (handle->string_option_in_thandle_optional)
                                    ->string)));
                        {
                            LOGGER_LOG logger_function = xlogging_get_log_function();
                            if (logger_function != ((void*)0)) {
                                logger_function(
                                    AZ_LOG_VERBOSE,
                                    "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                                    "configuration_wrapper.c",
                                    __FUNCDNAME__, 11, 0x01,
                                    "Config loaded: %ls = %"
                                    "s",
                                    L"OptionalStringOptionThandle",
                                    (((handle->string_option_in_thandle_optional) ==
                                        ((void*)0))
                                        ? "NULL"
                                        : (handle->string_option_in_thandle_optional)
                                        ->string));
                            }
                        }
                    };
                } while ((void)0, 0);
            }
        }
    }
    handle->string_option_optional = ((void*)0);
    if (!error_occurred) {
        if (configuration_reader_get_char_string(
            handle->activation_context, handle->sf_config_name_string,
            handle->sf_parameters_section_name_string, L"OptionalStringOption",
            &handle->string_option_optional) != 0) {
            do {
                {
                    (void)(0 && printf("configuration_reader_get_char_string"
                        "(\"%ls\", \"%ls\", \"%ls\") failed",
                        handle->sf_config_name_string,
                        handle->sf_parameters_section_name_string,
                        L"OptionalStringOption"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "configuration_reader_get_char_string"
                                "(\"%ls\", \"%ls\", \"%ls\") failed",
                                handle->sf_config_name_string,
                                handle->sf_parameters_section_name_string,
                                L"OptionalStringOption");
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else {
            if (handle->string_option_optional != ((void*)0) &&
                handle->string_option_optional[0] == '\0') {
                gballoc_hl_free(handle->string_option_optional);
                handle->string_option_optional = ((void*)0);
            }
            {
                do {
                    {
                        (void)(0 &&
                            printf("Config loaded: %ls = %"
                                "s",
                                L"OptionalStringOption",
                                (((handle->string_option_optional) != ((void*)0))
                                    ? (handle->string_option_optional)
                                    : "NULL")));
                        {
                            LOGGER_LOG logger_function = xlogging_get_log_function();
                            if (logger_function != ((void*)0)) {
                                logger_function(
                                    AZ_LOG_VERBOSE,
                                    "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                                    "configuration_wrapper.c",
                                    __FUNCDNAME__, 11, 0x01,
                                    "Config loaded: %ls = %"
                                    "s",
                                    L"OptionalStringOption",
                                    (((handle->string_option_optional) != ((void*)0))
                                        ? (handle->string_option_optional)
                                        : "NULL"));
                            }
                        }
                    };
                } while ((void)0, 0);
            }
        }
    }
    handle->wide_string_option_optional = ((void*)0);
    if (!error_occurred) {
        if (configuration_reader_get_wchar_string(
            handle->activation_context, handle->sf_config_name_string,
            handle->sf_parameters_section_name_string,
            L"OptionalWideStringOption",
            &handle->wide_string_option_optional) != 0) {
            do {
                {
                    (void)(0 && printf("configuration_reader_get_wchar_string"
                        "(\"%ls\", \"%ls\", \"%ls\") failed",
                        handle->sf_config_name_string,
                        handle->sf_parameters_section_name_string,
                        L"OptionalWideStringOption"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "configuration_reader_get_wchar_string"
                                "(\"%ls\", \"%ls\", \"%ls\") failed",
                                handle->sf_config_name_string,
                                handle->sf_parameters_section_name_string,
                                L"OptionalWideStringOption");
                        }
                    }
                };
            } while ((void)0, 0);
            error_occurred = 1;
        }
        else {
            if (handle->wide_string_option_optional != ((void*)0) &&
                handle->wide_string_option_optional[0] == L'\0') {
                gballoc_hl_free(handle->wide_string_option_optional);
                handle->wide_string_option_optional = ((void*)0);
            }
            {
                do {
                    {
                        (void)(0 && printf("Config loaded: %ls = %"
                            "ls",
                            L"OptionalWideStringOption",
                            (((handle->wide_string_option_optional) !=
                                ((void*)0))
                                ? (handle->wide_string_option_optional)
                                : L"NULL")));
                        {
                            LOGGER_LOG logger_function = xlogging_get_log_function();
                            if (logger_function != ((void*)0)) {
                                logger_function(
                                    AZ_LOG_VERBOSE,
                                    "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                                    "configuration_wrapper.c",
                                    __FUNCDNAME__, 11, 0x01,
                                    "Config loaded: %ls = %"
                                    "ls",
                                    L"OptionalWideStringOption",
                                    (((handle->wide_string_option_optional) != ((void*)0))
                                        ? (handle->wide_string_option_optional)
                                        : L"NULL"));
                            }
                        }
                    };
                } while ((void)0, 0);
            }
        }
    };
    if (error_occurred) {
        my_config_CONFIGURATION_cleanup_fields(handle);
        result = 11;
    }
    else {
        result = 0;
    }
    return result;
}
CONST_P2_CONST_my_config_CONFIGURATION my_config_configuration_create(
    IFabricCodePackageActivationContext* activation_context) {
    CONST_P2_CONST_my_config_CONFIGURATION result = ((void*)0);
    if (activation_context == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: IFabricCodePackageActivationContext* "
                    "activation_context = %p",
                    activation_context));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(
                            AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_ut\\test_"
                            "configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: IFabricCodePackageActivationContext* "
                            "activation_context = %p",
                            activation_context);
                    }
                }
            };
        } while ((void)0, 0);
    }
    else {
        CONST_P2_CONST_my_config_CONFIGURATION temp_config =
            THANDLE_MALLOC_my_config_CONFIGURATION(my_config_CONFIGURATION_dispose);
        if (temp_config == ((void*)0)) {
            do {
                {
                    (void)(0 && printf("THANDLE_MALLOC("
                        "my_config_CONFIGURATION"
                        ") failed"));
                    {
                        LOGGER_LOG logger_function = xlogging_get_log_function();
                        if (logger_function != ((void*)0)) {
                            logger_function(AZ_LOG_ERROR,
                                "C:\\src\\sf-c-util\\tests\\configuration_"
                                "wrapper_ut\\test_configuration_wrapper.c",
                                __FUNCDNAME__, 11, 0x01,
                                "THANDLE_MALLOC("
                                "my_config_CONFIGURATION"
                                ") failed");
                        }
                    }
                };
            } while ((void)0, 0);
        }
        else {
            my_config_CONFIGURATION* temp_config_obj =
                my_config_CONFIGURATION_GET_T(temp_config);
            temp_config_obj->activation_context = activation_context;
            (void)temp_config_obj->activation_context->lpVtbl->AddRef(
                temp_config_obj->activation_context);
            temp_config_obj->sf_config_name_string = L"default_config";
            temp_config_obj->sf_parameters_section_name_string =
                L"MyConfigSectionName";
            if (my_config_read_all_config_values(temp_config_obj) != 0) {
            }
            else {
                my_config_CONFIGURATION_INITIALIZE_MOVE(&result, &temp_config);
                goto all_ok;
            }
            (void)temp_config_obj->activation_context->lpVtbl->Release(
                temp_config_obj->activation_context);
            my_config_CONFIGURATION_FREE(temp_config_obj);
        }
    }
all_ok:
    return result;
}
uint64_t my_config_configuration_get_parameter_1(
    CONST_P2_CONST_my_config_CONFIGURATION handle) {
    uint64_t result;
    if (handle == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: THANDLE("
                    "my_config_CONFIGURATION"
                    ") handle = %p",
                    handle));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: THANDLE("
                            "my_config_CONFIGURATION"
                            ") handle = %p",
                            handle);
                    }
                }
            };
        } while ((void)0, 0);
        result = 0xffffffffffffffffui64;
    }
    else {
        result = handle->parameter_1;
    }
    return result;
}
uint64_t my_config_configuration_get_parameter_2(
    CONST_P2_CONST_my_config_CONFIGURATION handle) {
    uint64_t result;
    if (handle == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: THANDLE("
                    "my_config_CONFIGURATION"
                    ") handle = %p",
                    handle));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: THANDLE("
                            "my_config_CONFIGURATION"
                            ") handle = %p",
                            handle);
                    }
                }
            };
        } while ((void)0, 0);
        result = 0xffffffffffffffffui64;
    }
    else {
        result = handle->parameter_2;
    }
    return result;
}
uint32_t my_config_configuration_get_parameter_3(
    CONST_P2_CONST_my_config_CONFIGURATION handle) {
    uint32_t result;
    if (handle == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: THANDLE("
                    "my_config_CONFIGURATION"
                    ") handle = %p",
                    handle));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: THANDLE("
                            "my_config_CONFIGURATION"
                            ") handle = %p",
                            handle);
                    }
                }
            };
        } while ((void)0, 0);
        result = 0xffffffffui32;
    }
    else {
        result = handle->parameter_3;
    }
    return result;
}
_Bool my_config_configuration_get_some_flag(
    CONST_P2_CONST_my_config_CONFIGURATION handle) {
    _Bool result;
    if (handle == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: THANDLE("
                    "my_config_CONFIGURATION"
                    ") handle = %p",
                    handle));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: THANDLE("
                            "my_config_CONFIGURATION"
                            ") handle = %p",
                            handle);
                    }
                }
            };
        } while ((void)0, 0);
        result = 0;
    }
    else {
        result = handle->some_flag;
    }
    return result;
}
CONST_P2_CONST_RC_STRING my_config_configuration_get_string_option_in_thandle(
    CONST_P2_CONST_my_config_CONFIGURATION handle) {
    CONST_P2_CONST_RC_STRING result = ((void*)0);
    if (handle == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: THANDLE("
                    "my_config_CONFIGURATION"
                    ") handle = %p",
                    handle));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: THANDLE("
                            "my_config_CONFIGURATION"
                            ") handle = %p",
                            handle);
                    }
                }
            };
        } while ((void)0, 0);
        RC_STRING_INITIALIZE(&result, ((void*)0));
    }
    else {
        RC_STRING_INITIALIZE(&result, handle->string_option_in_thandle);
    }
    return result;
}
const char* my_config_configuration_get_string_option(
    CONST_P2_CONST_my_config_CONFIGURATION handle) {
    const char* result;
    if (handle == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: THANDLE("
                    "my_config_CONFIGURATION"
                    ") handle = %p",
                    handle));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: THANDLE("
                            "my_config_CONFIGURATION"
                            ") handle = %p",
                            handle);
                    }
                }
            };
        } while ((void)0, 0);
        result = ((void*)0);
    }
    else {
        result = handle->string_option;
    }
    return result;
}
const wchar_t* my_config_configuration_get_wide_string_option(
    CONST_P2_CONST_my_config_CONFIGURATION handle) {
    const wchar_t* result;
    if (handle == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: THANDLE("
                    "my_config_CONFIGURATION"
                    ") handle = %p",
                    handle));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: THANDLE("
                            "my_config_CONFIGURATION"
                            ") handle = %p",
                            handle);
                    }
                }
            };
        } while ((void)0, 0);
        result = ((void*)0);
    }
    else {
        result = handle->wide_string_option;
    }
    return result;
}
CONST_P2_CONST_RC_STRING
my_config_configuration_get_string_option_in_thandle_optional(
    CONST_P2_CONST_my_config_CONFIGURATION handle) {
    CONST_P2_CONST_RC_STRING result = ((void*)0);
    if (handle == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: THANDLE("
                    "my_config_CONFIGURATION"
                    ") handle = %p",
                    handle));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: THANDLE("
                            "my_config_CONFIGURATION"
                            ") handle = %p",
                            handle);
                    }
                }
            };
        } while ((void)0, 0);
        RC_STRING_INITIALIZE(&result, ((void*)0));
    }
    else {
        RC_STRING_INITIALIZE(&result, handle->string_option_in_thandle_optional);
    }
    return result;
}
const char* my_config_configuration_get_string_option_optional(
    CONST_P2_CONST_my_config_CONFIGURATION handle) {
    const char* result;
    if (handle == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: THANDLE("
                    "my_config_CONFIGURATION"
                    ") handle = %p",
                    handle));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: THANDLE("
                            "my_config_CONFIGURATION"
                            ") handle = %p",
                            handle);
                    }
                }
            };
        } while ((void)0, 0);
        result = ((void*)0);
    }
    else {
        result = handle->string_option_optional;
    }
    return result;
}
const wchar_t* my_config_configuration_get_wide_string_option_optional(
    CONST_P2_CONST_my_config_CONFIGURATION handle) {
    const wchar_t* result;
    if (handle == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: THANDLE("
                    "my_config_CONFIGURATION"
                    ") handle = %p",
                    handle));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: THANDLE("
                            "my_config_CONFIGURATION"
                            ") handle = %p",
                            handle);
                    }
                }
            };
        } while ((void)0, 0);
        result = ((void*)0);
    }
    else {
        result = handle->wide_string_option_optional;
    }
    return result;
}
_Bool my_config_configuration_get_another_flag(
    CONST_P2_CONST_my_config_CONFIGURATION handle) {
    _Bool result;
    if (handle == ((void*)0)) {
        do {
            {
                (void)(0 && printf("Invalid args: THANDLE("
                    "my_config_CONFIGURATION"
                    ") handle = %p",
                    handle));
                {
                    LOGGER_LOG logger_function = xlogging_get_log_function();
                    if (logger_function != ((void*)0)) {
                        logger_function(AZ_LOG_ERROR,
                            "C:\\src\\sf-c-util\\tests\\configuration_wrapper_"
                            "ut\\test_configuration_wrapper.c",
                            __FUNCDNAME__, 11, 0x01,
                            "Invalid args: THANDLE("
                            "my_config_CONFIGURATION"
                            ") handle = %p",
                            handle);
                    }
                }
            };
        } while ((void)0, 0);
        result = 0;
    }
    else {
        result = handle->another_flag;
    }
    return result;
};
