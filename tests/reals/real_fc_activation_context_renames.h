// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#define fc_activation_context_create                                    real_fc_activation_context_create
#define fc_activation_context_destroy                                   real_fc_activation_context_destroy
#define get_ContextId                                                   real_get_ContextId
#define get_CodePackageName                                             real_get_CodePackageName
#define get_CodePackageVersion                                          real_get_CodePackageVersion
#define get_WorkDirectory                                               real_get_WorkDirectory
#define get_LogDirectory                                                real_get_LogDirectory
#define get_TempDirectory                                               real_get_TempDirectory
#define get_ServiceTypes                                                real_get_ServiceTypes
#define get_ServiceGroupTypes                                           real_get_ServiceGroupTypes
#define get_ApplicationPrincipals                                       real_get_ApplicationPrincipals
#define get_ServiceEndpointResources                                    real_get_ServiceEndpointResources
#define GetServiceEndpointResource                                      real_GetServiceEndpointResource
#define GetCodePackageNames                                             real_GetCodePackageNames
#define GetConfigurationPackageNames                                    real_GetConfigurationPackageNames
#define GetDataPackageNames                                             real_GetDataPackageNames
#define GetCodePackage                                                  real_GetCodePackage
#define GetConfigurationPackage                                         real_GetConfigurationPackage
#define GetDataPackage                                                  real_GetDataPackage
#define RegisterCodePackageChangeHandler                                real_RegisterCodePackageChangeHandler
#define UnregisterCodePackageChangeHandler                              real_UnregisterCodePackageChangeHandler
#define RegisterConfigurationPackageChangeHandler                       real_RegisterConfigurationPackageChangeHandler
#define UnregisterConfigurationPackageChangeHandler                     real_UnregisterConfigurationPackageChangeHandler
#define RegisterDataPackageChangeHandler                                real_RegisterDataPackageChangeHandler
#define UnregisterDataPackageChangeHandler                              real_UnregisterDataPackageChangeHandler
#define IFabricCodePackageActivationContext_to_ARGC_ARGV                real_IFabricCodePackageActivationContext_to_ARGC_ARGV
