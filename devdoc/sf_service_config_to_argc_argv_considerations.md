sf_service_config_to_argc_argv_design
===

References:

[StartServiceA](https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-startservicea)

## Considerations

In the context of another project (shall not be named), the application parameters that are passed to the SF application need to be made known to a Windows Service.

Windows Services receive their parameters on the command line when they are started by a call to [StartServiceA].

There are several ways this problem can be approached:

A) Assume there's a way to get the .xml that SF uses to store the initialization data for the service:

  1) In the SF service: get the `.xml` that initializes the application. Pass the path to the `.xml` on the command line, leave the Windows Service "deal with it" and understand what's inside.

  2) In the SF service: get the `.xml` that initializes the application. Pass the path to the `.xml` on the command line, steal the SF code that parses it, and provide to the application the parsed structured data, contained in the `IFabricCodePackageActivationContext*` as parsed by the stolen SF code.

  Note 1: at the moment of writing this document it is rather unknown if the assumption "get the .xml... " works. Several interfaces (`IFabricConfigurationPackage`, `IFabricCodePackage` and `IFabricDataPackage`) that contain "get_Path()" have been called and these were the results:

```
Process = 26824 Info: there were detected 1 configurationPackages
Process = 26824 Info: allConfigurationPackageNames[0]=default_config
Process = 26824 Info: allConfigurationPackageNames[0].get_Path()=C:\SfDevCluster\Data\_App\_Node_3\block_storage_2_sf_type_App126\BlockStorageSFPkg.default_config.1.3.14.62738391
Process = 26824 Info: there were detected 0 dataPackages
Process = 26824 Info: there were detected 1 codePackages
Process = 26824 Info: allCodePackageNames[0]=code
Process = 26824 Info: allCodePackageNames[0].get_Path()=C:\SfDevCluster\Data\_App\_Node_3\block_storage_2_sf_type_App126\BlockStorageSFPkg.code.1.3.14.62738391
```

  Note 2: none of the strings returned by `get_Path()` contain an .xml file, they are really folders.

  Note 3: `C:\SfDevCluster` will contain some .xml that are prepared by SF (such as "C:\SFDevCluster\Data\_App\_Node_2\block_storage_2_sf_type_App126\BlockStorageSFPkg.Package.1.0.xml") which do seem to contain the parameters of the application. 

  Note 4: the above .xml is not returned by any APIs... and it is not accompanied by a .xsd that would validate it... :(


B) If the .xml cannot be obtained/parsed from SF, then the data passed in the `IFabricCodePackageActivationContext` needs to be exported by code to some intermediate format. Then the intermediate format needs to be imported in the Windows Service. Several options on how to proceed here too:

  1) export the data to a string of `argc/argv` typical command line arguments that are passed to [StartServiceA].

    a) valid concern here is the size of the data to be passed. Windows command line is limited to 8191 characters. A brief inspection of the current state of affairs lists about 2.7K of characters that are passed... likely the command line will contain some "decorations"... it is unknown at the moment how much those decorations would add to the size of the command line.

  2) export the data to a file (using same format as `argc/argv`). Then pass this file on the command line of the Windows Service.

    a) this alleviates the concern raised in B.1.a, however, it does create a new file that needs to be managed (created/deleted/scrubbed/updated etc).

  3) have the data available from the SF service to the Windows Service on some IPC mechanism (pipes, shared memory sections etc) and pass on the command line only the needed arguments to find the data in the Windows Service.

    a) this still requires some level of serialization/deserialization of data, unless the `IFabricCodePackageActivationContext` itself is made available. 

      i) if `IFabricCodePackageActivationContext` would be made available "as is" (through COM) then life would be awesome. However, we've failed exporting COM interfaces from a SF service - this is why we have a Windows Service that hosts COM.

    b) if `IFabricCodePackageActivationContext` would serialized/deserialized then some other IPC would carry the serialization/deserialization. This serialization/deserialization would only need to be done once (serialized once, transported over IPC once, deserialized once). As far as performance goes, this is not a concern.

Recommendation at the moment is to go with B.1 and once a proper "argification/unargification" of `IFabricCodePackageActivationContext` is reached re-evaluate B.2 / B.3.b

## Design

```
                                                    SF Service                                        Windows Service                                                        `sf_service_config`
                                           +===========================+                        +=========================+                                                  +=================+
IFabricCodePackageActivationContext* ----> |        argificator        | ----> argc/argv -----> |      unargificator      | ----> IFabricCodePackageActivationContext* ----> | existing magic  | ----> THANDLE(SF_CONFIGURATION)
                                           +===========================+                        +=========================+                                                  +=================+
```

## Argification

Analysis of the usage of `IFabricCodePackageActivationContext/IFabricConfigurationPackage` in `sf_service_config` leads to the conclusion that only several of its rather numerous methods are used. Specifically, the following are used in `sf_service_config`:

`IFabricCodePackageActivationContext`: `GetConfigurationPackage` + IUnknown
`IFabricConfigurationPackage`: `GetValue` + IUnknown

`sf_service_config` does not provide the values from `IFabricCodePackageActivationContext`'s `GetServiceEndpointResource`, but there's a reasonable expectation that those will also be used later.

(note: would be nice if `sf_service_config` did that, to be addressed with https://msazure.visualstudio.com/One/_workitems/edit/16024277).

In conclusion the argificator/unargificator will need to export the following:
1) all Configuration Packages
2) all ServiceEndpointResources

This is a rough example of the proposal of the export format :

--configurationPackageName configuration_package_name_1 --sectionName section_name_1 --parameter name_1 value_1 --parameter name_2 value_2 [...] --sectionName section_name_2 [...] --configurationPackage configuration_package_name_2 [...] --serviceEndpointResource service_endpoint_resource_name_1 --protocol tcp --port 4242 --type whatever --certificateName certificate_name_1

This is a rough grammar example of the export format:

export_format
  :   (configurationPackage)*
      (serviceEndPointResource)*

configurationPackage
  :   --configurationPackageName "string" 
      (section)*

section
  :   --sectionName "string"
      (parameter)*

parameter
  :   "string" "string"

serviceEndPointResource
  :   --serviceEndpointResource "string" --Protocol "string" --Type "string" --Port "string" --CertificateName "string"

