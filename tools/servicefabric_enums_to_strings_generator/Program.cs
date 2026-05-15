// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Text.RegularExpressions;

namespace servicefabric_enums_to_strings_generator
{
    class Program
    {
        static void writeHeaderHeader(StreamWriter outputHeaderFile)
        {
            outputHeaderFile.Write(
@"// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/*THIS FILE IS GENERATED, DO NOT EDIT BY HAND!!!*/
/*generator is called ""servicefabric_enums_to_strings_generator""*/

#ifndef SERVICEFABRIC_ENUMS_TO_STRINGS_H
#define SERVICEFABRIC_ENUMS_TO_STRINGS_H

#include ""macro_utils/macro_utils.h""
#include ""fabrictypes.h""

#ifdef __cplusplus
extern ""C""
{
#endif
");
        }

        static void writeHeaderFooter(StreamWriter outputHeaderFile)
        {
            outputHeaderFile.Write(

@"
#ifdef __cplusplus
}
#endif
#endif
"
);
        }

        static void writeSourceHeader(StreamWriter outputHeaderFile)
        {
            outputHeaderFile.Write(
@"// Copyright (C) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/*THIS FILE IS GENERATED, DO NOT EDIT BY HAND!!!*/
/*generator is called ""servicefabric_enums_to_strings_generator""*/

#include <stddef.h>                          // for NULL, size_t

#include ""macro_utils/macro_utils.h""
#include ""fabrictypes.h""

#include ""sf_c_util/servicefabric_enums_to_strings.h""

typedef struct SF_ENUM_AND_STRING_TAG
{
    int value;
    const char* valueAsString;
} SF_ENUM_AND_STRING;

");
        }

        static void Main(string[] args)
        {
            // Input: path to fabrictypes.h. With the NuGet migration the file
            // no longer lives at a fixed relative path inside the repo; the
            // caller passes its absolute or relative path explicitly. A typical
            // value (after a CMake configure of sf-c-util) is something like
            //   <cmake-build-dir>\sf_packages\ServiceFabric.NativeLibsHeaders.retail.<ver>\content\native\prebuilt\sdk\fabrictypes.h
            // or, for an x64 ebs build:
            //   D:\r\ebs_ut\sf_packages\ServiceFabric.NativeLibsHeaders.retail.<ver>\content\native\prebuilt\sdk\fabrictypes.h
            // Set the path under Project Properties -> Debug -> Application
            // arguments in Visual Studio so F5 keeps working.
            if (args.Length < 1)
            {
                Console.Error.WriteLine(
                    "usage: servicefabric_enums_to_strings_generator <fabrictypes.h> [<output-header>] [<output-source>]\n" +
                    "\n" +
                    "  <fabrictypes.h>  Absolute or relative path to the public Service Fabric\n" +
                    "                   types header. Find it under your CMake build dir at\n" +
                    "                   sf_packages\\ServiceFabric.NativeLibsHeaders.retail.<ver>\\\n" +
                    "                   content\\native\\prebuilt\\sdk\\fabrictypes.h after the\n" +
                    "                   NuGet restore step has run.\n" +
                    "  <output-header>  Optional path for the generated header. Defaults to\n" +
                    "                   <repo>\\inc\\sf_c_util\\servicefabric_enums_to_strings.h.\n" +
                    "  <output-source>  Optional path for the generated source. Defaults to\n" +
                    "                   <repo>\\src\\servicefabric_enums_to_strings.c.");
                Environment.Exit(2);
                return;
            }

            string inputFileName = args[0];
            if (!File.Exists(inputFileName))
            {
                Console.Error.WriteLine($"Input file not found: {inputFileName}");
                Environment.Exit(2);
                return;
            }

            // Default output locations are computed from the exe's own
            // location (walk up bin\<Config>\<framework>\ to the project dir,
            // then up two more to the repo root) so the tool does not depend
            // on cwd. Override via args[1] and args[2] if needed.
            string repoRoot = ResolveRepoRoot();
            string outputHeaderFileName = args.Length >= 2
                ? args[1]
                : Path.Combine(repoRoot, "inc", "sf_c_util", "servicefabric_enums_to_strings.h");
            string outputSourceFileName = args.Length >= 3
                ? args[2]
                : Path.Combine(repoRoot, "src", "servicefabric_enums_to_strings.c");

            Console.WriteLine($"Input:  {Path.GetFullPath(inputFileName)}");
            Console.WriteLine($"Header: {Path.GetFullPath(outputHeaderFileName)}");
            Console.WriteLine($"Source: {Path.GetFullPath(outputSourceFileName)}");

            var fileContent = File.ReadAllText(inputFileName);

            var outputHeaderFile = File.CreateText(outputHeaderFileName);
            var outputSourceFile = File.CreateText(outputSourceFileName);

            writeHeaderHeader(outputHeaderFile);
            writeSourceHeader(outputSourceFile);

            /*look for all enums*/
            var pattern = @"enum ([A-Z_a-z0-9]*)\r?\n\s*{\r?\n(([A-Z0-9a-z_\s]*=([A-Z_a-z0-9,(+) \t]*)\r?\n)*)\s*}\s*[A-Z_a-z0-9]*;";
            Regex regex = new Regex(pattern);
            var allEnum = regex.Matches(fileContent);
            foreach (Match someEnum in allEnum)
            {
                /*enumIdentifier has the same meaning as in C standard*/
                Group enumIdentifier = someEnum.Groups[1];

                /*declare its stringify function in header*/
                outputHeaderFile.WriteLine($"MU_DECLARE_ENUM_STRINGS({enumIdentifier});");

                /*enumeratorList has the same meaning as in C standard*/
                string enumeratorList = someEnum.Groups[2].Value;
                outputSourceFile.WriteLine($"static const SF_ENUM_AND_STRING {enumIdentifier}_ValuesAndStrings[] ={{"); /*by convention, because SF defines "too big" enumerations that are not handled by macro_utils.h*/

                /*enumerator has the same meaning as in C standard*/
                foreach (var enumerator in Regex.Split(enumeratorList, "\r?\n"))
                {
                    var enumerationConstant = Regex.Match(enumerator, @"\s*([^\s]*)").Groups[1].Value;
                    if (enumerationConstant.Length > 0) /*skip empty lines*/
                    {
                        outputSourceFile.WriteLine($"{{ {enumerationConstant} , \"{enumerationConstant}\" }},");
                    }
                    /*copy paste from macro_utils.h*/
                }
                outputSourceFile.WriteLine(@"};");

                outputSourceFile.WriteLine(
$@"const char* MU_C3(MU_,{enumIdentifier},_ToString)({enumIdentifier} value)
{{
    for(size_t i=0;i<sizeof(MU_C2({enumIdentifier}, _ValuesAndStrings))/sizeof(MU_C2({enumIdentifier}, _ValuesAndStrings)[0]);i++)
    {{
        if(MU_C2({enumIdentifier}, _ValuesAndStrings)[i].value == (int)value)
        {{
            return MU_C2({enumIdentifier}, _ValuesAndStrings)[i].valueAsString;
        }}
    }}
    return ""UNKNOWN"";
}}
");

            }

            writeHeaderFooter(outputHeaderFile);
            outputHeaderFile.Close();
            outputSourceFile.Close();
        }

        // The exe lives at <repo>\tools\servicefabric_enums_to_strings_generator\bin\<Config>\
        // (Debug or Release). Walk up four directories to get to <repo>. Falls
        // back to the exe's own directory if the layout has been customized.
        static string ResolveRepoRoot()
        {
            string exePath = System.Reflection.Assembly.GetExecutingAssembly().Location;
            DirectoryInfo dir = new DirectoryInfo(Path.GetDirectoryName(exePath));
            for (int i = 0; i < 4 && dir != null; i++)
            {
                dir = dir.Parent;
            }
            if (dir == null)
            {
                return Path.GetDirectoryName(exePath);
            }
            return dir.FullName;
        }
    }
}
