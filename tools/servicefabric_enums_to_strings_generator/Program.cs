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
            var inputFileName =        @"..\..\..\..\deps\servicefabric\inc\fabrictypes.h";
            var outputHeaderFileName = @"..\..\..\..\inc\sf_c_util\servicefabric_enums_to_strings.h";
            var outputSourceFileName = @"..\..\..\..\src\servicefabric_enums_to_strings.c";

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
    }
}
