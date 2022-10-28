#include <inttypes.h>

#include "c_logging/xlogging.h"

#include "sf_c_util/sf_service_config_to_argc_argv.h"

int IFabricCodePackageActivationContext_to_argc_argv(IFabricCodePackageActivationContext* iFabricCodePackageActivationContext, int* argc, char*** argv)
{
    int result;
    if (
        (iFabricCodePackageActivationContext == NULL) ||
        (argc == NULL) ||
        (argv == NULL)
        )
    {
        LogError("invalid argument IFabricConfigurationPackage* iFabricConfigurationPackage=%p, int* argc=%p, char*** argv=%p",
            iFabricCodePackageActivationContext, argc, argv);
        result = MU_FAILURE;
    }
    else
    {
        {
            IFabricStringListResult* allConfigurationPackageNames;
            iFabricCodePackageActivationContext->lpVtbl->GetConfigurationPackageNames(iFabricCodePackageActivationContext, &allConfigurationPackageNames);

            ULONG allConfigurationPackageNames_Count;
            const wchar_t** names;
            allConfigurationPackageNames->lpVtbl->GetStrings(allConfigurationPackageNames, &allConfigurationPackageNames_Count, &names);
            LogInfo("there were detected %" PRIu32 " configurationPackages", allConfigurationPackageNames_Count);
            for (uint32_t i = 0; i < allConfigurationPackageNames_Count; i++)
            {
                LogInfo("allConfigurationPackageNames[%" PRIu32 "]=%ls", i, (*names) + i);
                IFabricConfigurationPackage* configPackage;
                iFabricCodePackageActivationContext->lpVtbl->GetConfigurationPackage(iFabricCodePackageActivationContext, (*names) + i, &configPackage);
                const wchar_t* path = configPackage->lpVtbl->get_Path(configPackage);
                LogInfo("allConfigurationPackageNames[%" PRIu32 "].get_Path()=%ls", i, path);
            }
        }

        {
            IFabricStringListResult* allDataPackageNames;
            iFabricCodePackageActivationContext->lpVtbl->GetDataPackageNames(iFabricCodePackageActivationContext, &allDataPackageNames);

            ULONG allDataPackageNames_Count;
            const wchar_t** names;
            allDataPackageNames->lpVtbl->GetStrings(allDataPackageNames, &allDataPackageNames_Count, &names);
            LogInfo("there were detected %" PRIu32 " dataPackages", allDataPackageNames_Count);
            for (uint32_t i = 0; i < allDataPackageNames_Count; i++)
            {
                LogInfo("allDataPackageNames[%" PRIu32 "]=%ls", i, (*names) + i);
                IFabricDataPackage* dataPackage;
                iFabricCodePackageActivationContext->lpVtbl->GetDataPackage(iFabricCodePackageActivationContext, (*names) + i, &dataPackage);
                const wchar_t* path = dataPackage->lpVtbl->get_Path(dataPackage);
                LogInfo("allDataPackageNames[%" PRIu32 "].get_Path()=%ls", i, path);
            }
        }

        {
            IFabricStringListResult* allConfigurationPackageNames;
            iFabricCodePackageActivationContext->lpVtbl->GetCodePackageNames(iFabricCodePackageActivationContext, &allConfigurationPackageNames);

            ULONG allConfigurationPackageNames_Count;
            const wchar_t** names;
            allConfigurationPackageNames->lpVtbl->GetStrings(allConfigurationPackageNames, &allConfigurationPackageNames_Count, &names);
            LogInfo("there were detected %" PRIu32 " codePackages", allConfigurationPackageNames_Count);
            for (uint32_t i = 0; i < allConfigurationPackageNames_Count; i++)
            {
                LogInfo("allCodePackageNames[%" PRIu32 "]=%ls", i, (*names) + i);
                IFabricCodePackage* configPackage;
                iFabricCodePackageActivationContext->lpVtbl->GetCodePackage(iFabricCodePackageActivationContext, (*names) + i, &configPackage);
                const wchar_t* path = configPackage->lpVtbl->get_Path(configPackage);
                LogInfo("allCodePackageNames[%" PRIu32 "].get_Path()=%ls", i, path);
            }
        }



#if 0
        const FABRIC_CONFIGURATION_SETTINGS* fabric_configuration_settings = iFabricCodePackageActivationContext->lpVtbl->get_Settings(iFabricCodePackageActivationContext);
        if (fabric_configuration_settings == NULL)
        {
            LogError("failure in iFabricConfigurationPackage->lpVtbl->get_Settings(iFabricConfigurationPackage);");
            result = MU_FAILURE;
        }
        else
        {
            LogInfo("fabric_configuration_settings->Count=%" PRIu32 "", fabric_configuration_settings->Sections->Count);
            for (uint32_t i = 0; i < fabric_configuration_settings->Sections->Count; i++)
            {
                const FABRIC_CONFIGURATION_SECTION* section = fabric_configuration_settings->Sections->Items + i;
                LogInfo("section->Name = %ls, Parameters_Count=%" PRIu32 "", section->Name, section->Parameters->Count);
                const FABRIC_CONFIGURATION_PARAMETER_LIST* parameters = section->Parameters;
                for (uint32_t j = 0; j < parameters->Count; j++)
                {
                    const FABRIC_CONFIGURATION_PARAMETER* parameter = parameters->Items + j;
                    LogInfo("    Parameters[%" PRIu32 "]= { .Name=%ls, .Value=%ls }", j, parameter->Name, parameter->Value);
                }
            }

        }
#endif
        result = 0;
    }
    return result;
}