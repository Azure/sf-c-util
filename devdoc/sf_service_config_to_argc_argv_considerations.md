sf_service_config_to_argc_argv_design
===

References:

[StartServiceA](https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-startservicea)

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

Recommendation at the moment is to go with B.1 and once a proper "argification/unargification" of `IFabricCodePackageActivationContext` is reached re-evaluate B.2
