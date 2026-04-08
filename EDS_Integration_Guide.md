# EDS Integration User Guide

## Table of Contents
- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [Formatting Notes](#formatting-notes)
- [Required Directory Structure](#required-directory-structure)
- [Required Files by Directory](#required-files-by-directory)
- [Integration Steps](#integration-steps)
- [EDS XML Configuration](#eds-xml-configuration)
- [Build Integration](#build-integration)
- [Updating System Configuration Files](#updating-system-configuration-files)
- [Testing and Validation](#testing-and-validation)
- [Troubleshooting](#troubleshooting)

## Introduction

The Electronic Data Sheet (EDS) is a CCSDS (Consultative Committee for Space Data Systems) standard that provides a structured way to define spacecraft command and telemetry interfaces. This guide walks through the integration process for EDS-enabled modules into flight software modules. When referencing the official CCSDS publication ([Spacecraft Onboard Interface Services - XML Specification for Electronic Data Sheets](https://ccsds.org/Pubs/876x0b1.pdf)), please remember that there are modules that have more detail than what is used in this implementation of EDS.

### Purpose
This guide helps developers integrate EDS packages into flight software modules, ensuring proper configuration and compatibility with the target system. EDS integration standardizes interface definitions and automates the generation of configuration files.

### Benefits of EDS Integration
- Standardized interface definitions
- Automated configuration file generation
- Reduced manual configuration errors
- Consistent naming conventions across applications and libraries
- Improved maintainability

## Prerequisites

Before beginning EDS integration, ensure you have:
- Basic understanding of CCSDS standards
- Familiarity with C/C++ development
- Access to the target flight software environment
- EDS toolchain installed and configured
- Understanding of your module's interface requirements (command/telemetry for applications, API functions for libraries)

## Formatting Notes

Throughout this user guide naming conventions for the module you're implementing EDS in varies to reflect the intent. Do not leave "Your Module", "ym", or "YM" in the final code. Replace it with whatever your module's name is
| Name        | Usage                                                                                                       | Example       |
|-------------|-------------------------------------------------------------------------------------------------------------|---------------|
| Your Module | The full title-case name of the EDS module, typically used when referring to the EDS module in explanations | Limit Checker |
| ym          | Lower-case abbreviation of the EDS module, typically used in file names                                     | lc            |
| YM          | Upper-case abbreaviation of the EDS module, typically used in source code                                   | LC            |

Additionally, any comment in the form of `/* === C code in xyz file for module YM === */` or `<!-- XML code in xyz file for module YM -->` are no be ignored. So are any comments on or about the example variables or values. These comments are here just to help get the context of where the examples are expected to be found or stand in as placeholders for your own actual, and no doubt very thorough, comments.

## Required Directory Structure

The following directories must be present in your module root:
```
[cFS repository directory]  # Clone of the cFS repository and the root of all paths in this user guide
├── (apps|libs)             # Home of all applications or libraries
    ├── ym/                 # Your specific submodule
        ├── config/         # Configuration files and interface definitions
        ├── eds/            # Houses the CCSDS Electronic Data Sheet package (.xml files)
        ├── fsw/            # Source units that comprise the flight software executing on the target
        │   ├── inc/        # Public/interface headers for the module
        │   └── src/        # Source files and private headers for the module
        └── tables/         # Example table definitions for the module (applications only - libraries do not use tables)
```

### Directory Descriptions
**- [cFS]/(apps|libs)/ym/config/** - Contains example configuration files for the module<br>
**- [cFS]/(apps|libs)/ym/eds/** - Houses the CCSDS Electronic Data Sheet package (.xml files)<br>
**- [cFS]/(apps|libs)/ym/fsw/** - Source units that comprise the flight software executing on the target<br>
**- [cFS]/(apps|libs)/ym/fsw/inc/** - Public/interface headers for the module<br>
**- [cFS]/(apps|libs)/ym/fsw/src/** - Source files and private headers for the module<br>
**- [cFS]/(apps|libs)/ym/tables/** - Example table definitions for the module (optional)<br>

## Required Files by Directory

**Note**: In the following file names, the root directory is also assumed to be `[cFS]/(apps|libs)/ym`

> **Important File Naming Convention**:
>
> The configuration files in the `config/` directory are created with a `default_` prefix (e.g., `default_ym_fcncode_values.h`), but the CMake build system automatically removes this prefix during the build process. This means:
> - **File creation**: You create `default_ym_fcncode_values.h`
> - **File usage**: In your `#include` statements, reference `ym_fcncode_values.h` (without the `default_` prefix)
> - **Build process**: CMake handles the mapping between the two
>
> This convention allows the build system to select between default configuration files and mission-specific overrides automatically.

### Understanding File Scopes

**Interface Scope Files**: Configuration that affects the public interface (visible to other applications)
- `xx_interface_cfg.h`
- `default_xx_msgdefs.h`
- `default_xx_msgstruct.h`

**Platform Scope Files**: Configuration that may differ per instance/platform but doesn't affect interfaces
- `xx_internal_cfg.h`

<h3 style="text-align: center;"><u>config/ Directory Files</u></h3>

### Core Configuration Files
*Required for all applications and libraries:*

- `default_ym_interface_cfg_values.h`
- `default_ym_internal_cfg_values.h`
- `default_ym_mission_cfg.h`
- `default_ym_perfids.h`
- `default_ym_platform_cfg.h`
- `eds_ym_interface_cfg_values.h`

### Command and Telemetry Interface Files
*Required for applications with CMD/TLM interfaces (not applicable to libraries):*

- `default_ym_fcncode_values.h`
- `default_ym_msg.h`
- `default_ym_msgdefs.h`
- `default_ym_msgid_values.h`
- `default_ym_msgids.h`
- `default_ym_msgstruct.h`
- `default_ym_topicid_values.h`
- `eds_ym_fcncode_values.h`
- `eds_ym_msgdefs.h`
- `eds_ym_msgstruct.h`
- `eds_ym_topicid_values.h`

### Table Interface Files
*Required for applications that use tables (not applicable to libraries):*

- `default_ym_tbl.h`
- `default_ym_tbldefs.h`
- `default_ym_tblstruct.h`
- `eds_ym_tbldefs.h`
- `eds_ym_tblstruct.h`

### Additional Files
*May be needed for specific component requirements (both applications and libraries):*

- `default_ym_extern_typedefs.h`
- `eds_ym_extern_typedefs.h`

**Important Notes:**
- Libraries typically do NOT have CMD/TLM interfaces and therefore will not need the message-related files
- Libraries typically do NOT use tables and therefore will not need the table-related files
- All applications need the core configuration files
- Libraries primarily need core configuration files and any custom data types

For explanations for what these files contain and what they do jump to [config header explanations](#step-6-populate-config-header-files-with-module-specific-definitions)

<div align="center">
  <h3><u>eds/ Directory Files</u></h3>
</div>

`- ym.xml` – The core EDS definition file containing all interface specifications

<div align="center" id="fsw-inc-directory-files">
  <h3><u>fsw/inc/ Directory Files</u></h3>
</div>

**Files Required for Both Applications and Libraries**<br>
`- ym_eventids.h` – Event ID definitions<br>
`- ym_interface_cfg.h` – Public interface definitions<br>
`- ym_internal_cfg.h` – Internal interface definitions<br>

**Application-Specific Files**<br>
`- ym_fcncodes.h` – Function code definitions (applications with CMD interface only)<br>
`- ym_topicids.h` – Topic ID definitions (applications with messaging interface only)<br>

**Library-Specific Files**<br>
`- ym_lib.h` – Public library API definitions

<div align="center" id="fsw-src-directory-files">
  <h3><u>fsw/src/ Directory Files</u></h3>
</div>

**Files Required for Both Applications and Libraries**<br>
`- ym_version.h` – Version information<br>
`- ym_internal.c` – (Optional) Utility functions<br>
`- ym_internal.h` – (Optional) Utility function headers<br>

**Application-Specific Files**<br>
`- ym_app.c` – Main application logic<br>
`- ym_app.h` – Application header<br>
`- ym_cmds.c` – Command processing implementation<br>
`- ym_cmds.h` – Command processing header<br>
`- ym_dispatch.c` – Message dispatch implementation<br>
`- ym_dispatch.h` – Message dispatch header<br>
`- ym_eds_dispatch.c` – EDS-specific dispatch logic<br>

**Library-Specific Files**<br>
`- ym_lib.c` – Library implementation

<div align="center">
  <h3><u>tables/ Directory Files</u></h3>
</div>

Contains table definition files specific to your module requirements.

## Integration Steps

### Step 1: Ensure all [required directories](#required-directory-structure) exist

These are the directories that are needed at a minimum. Create missing directories as needed.

### Step 2: Create the [required source files](#fsw-src-directory-files) within fsw/src/

**<u>For Applications:</u>**
```bash
touch ym_app.c
touch ym_app.h
touch ym_cmds.c
touch ym_cmds.h
touch ym_dispatch.c
touch ym_dispatch.h
touch ym_eds_dispatch.c
touch ym_internal.c
touch ym_internal.h
touch ym_version.h
```

**<u>For Libraries:</u>**
```bash
touch ym_internal.c
touch ym_internal.h
touch ym_version.h
```

### Step 2: Create the [required header files](#fsw-inc-directory-files) within fsw/inc/

**<u>For Applications:</u>**
``` bash
touch ym_eventids.h
touch ym_fcncodes.h
touch ym_interface_cfg.h
touch ym_internal_cfg.h
touch ym_topicids.h
```
**<u>For Libraries:</u>**
``` bash
touch ym_eventids.h
touch ym_interface_cfg.h
touch ym_internal_cfg.h
touch ym_lib.h
```

### Step 3: Create the [required header files](#config-directory-files) within config/

**<u>For Applications:</u>**
``` bash
# Core configuration files (required for all applications)
touch default_ym_interface_cfg_values.h
touch default_ym_internal_cfg_values.h
touch default_ym_mission_cfg.h
touch default_ym_perfids.h
touch default_ym_platform_cfg.h
touch eds_ym_interface_cfg_values.h

# Command/Telemetry interface files (applications with CMD/TLM interface)
touch default_ym_fcncode_values.h
touch default_ym_msg.h
touch default_ym_msgdefs.h
touch default_ym_msgid_values.h
touch default_ym_msgids.h
touch default_ym_msgstruct.h
touch default_ym_topicid_values.h
touch eds_ym_fcncode_values.h
touch eds_ym_msgdefs.h
touch eds_ym_msgstruct.h
touch eds_ym_topicid_values.h
```

**<u> For Libraries:</u>**
``` bash
# Core configuration files (required for all libraries)
touch default_ym_interface_cfg_values.h
touch default_ym_internal_cfg_values.h
touch default_ym_mission_cfg.h
touch default_ym_perfids.h
touch default_ym_platform_cfg.h
touch eds_ym_interface_cfg_values.h

# Additional files if needed
touch default_ym_extern_typedefs.h
touch eds_ym_extern_typedefs.h
```

**<u>Optional Files for Applications Using Tables</u>**

``` bash
touch default_ym_tbl.h
touch default_ym_tbldefs.h
touch default_ym_tblstruct.h
touch eds_ym_extern_typedefs.h
touch eds_ym_tbldefs.h
```

### Step 4: Create the skeleton for each of the created files

* License Header
    * Copy the standard license block from other .h files in the project. This is typically the multi-line comment at the very top of the file that includes copyright, licensing info, and version identifiers
* Include Guards
    * Use traditional `#ifndef #define #endif` guards that match the file name in all caps with underscores. This prevents double-inclusion of headers
    * Example:
        ``` C
        #ifndef DEFAULT_YM_FCNCODE_VALUES_H
        #define DEFAULT_YM_FCNCODE_VALUES_H

        /* header content */

        #endif /* DEFAULT_YM_FCNCODE_VALUES_H */
        ```


### Step 5: Populate fsw/inc/ header files with module-specific definitions

**Note**: All modules should include their doxygen markups for user documentation
#### `ym_eventids.h` (Applications and Libraries)
* Defines CFE EVS Event IDs for the module
* How to find what goes here
    * Macros the typically end in `_EID`
* Include brief descriptions and documentation for each event

#### `ym_fcncodes.h` (Applications Only)
* Contains function codes (also called Command Codes) for the module's CMD interface
* **Note**: Libraries do not have command interfaces and therefore do not need this file
* How to find what goes here
    * Macros that typically end in `_CC`
        * Example: `#define YM_NOOP_CC 1`

#### `ym_interface_cfg.h` (Applications and Libraries)
* Defines public interface configuration values used across the module
    * Command lengths, message sizes, and externally visible parameters
* This file should only contain macros. Any structs should go in default_ym_extern_typedefs.h
* Make sure you rename all instances of the macro in use

1. Define macros
    * Macro naming convention:
        ``` C
        /**
         * Long macro comment that totally
         * makes the purpose of this macro
         * much clearer and less confusing
         */
        #define YM_INTERFACE_EXAMPLE [value]
        ```
2. Add `#include "ym_interface_cfg.h"` into [default_ym\_msgdefs.h](#default_ym_msgdefsh-applications-only) and [default_ym_msgstruct.h](#default_ym_msgstructh-applications-only)
3. Depending on whether you have tables, add `#include "ym_interface_cfg.h"` to [default_ym\_tbldefs.h](#default_ym_tbldefsh-applications-with-tables-only) and [default_ym_tblstruct.h](#default_ym_tblstructh-applications-with-tables-only)

#### `ym_internal_cfg.h` (Applications and Libraries)
* Software configuration values that do not affect interfaces, and may be different per instance/platform
    * Includes pipe depth, pipe name, number of tables, platform-specific error codes, valid table value ranges, table file name, SB timeout, memory size specifications
* This file should only contain macros
    * Non-macros that seem like they should go here should instead be in [ym_extern_typedefs.h](#default_ym_extern_typedefsh-applications-and-libraries)

1. Define macros
    * Macro naming convention:
        ``` C
        /**
         * Another long comment for a macro
         * that is so in-depth and perfectly
         * worded that you now understand
         * that value more than the original
         * developer
         */
        #define YM_INTERNAL_EXAMPLE [value]
        ```
2. Add `#include "ym_internal_cfg.h"` from any file you stole a macro from

#### `ym_topicids.h` (Applications Only)
* Defines topic IDs used in the messaging system
* **Note**: Libraries do not use software bus messaging and therefore do not need this file
* Make sure you rename all instances of the macro in use
* There are 3 required topic IDs shown here as an example:
``` C
    /* Command topic IDs */
    #define YM_MISSION_CMD_TOPICID             YM_MISSION_TIDVAL(CMD)
    #define DEFAULT_YM_MISSION_CMD_TOPICID     0xA4

    #define YM_MISSION_SEND_HK_TOPICID         YM_MISSION_TIDVAL(SEND_HK)
    #define DEFAULT_YM_MISSION_SEND_HK_TOPICID 0xA5

    /* Telemetry topic IDs */
    #define YM_MISSION_HK_TLM_TOPICID          YM_MISSION_TIDVAL(HK_TLM)
    #define DEFAULT_YM_MISSION_HK_TLM_TOPICID  0xA7
```

* **Topic ID Default Value Calculation:** <br>
    * Extract the lower 8 bits from the corresponding Message ID in `default_ym_msgids.h`
        * YM_CMD_MID = 0x18A4     → DEFAULT_YM_MISSION_CMD_TOPICID = 0xA4
        * YM_SEND_HK_MID = 0x18A5 → DEFAULT_YM_MISSION_SEND_HK_TOPICID = 0xA5
        * YM_HK_TLM_MID = 0x08A7  → DEFAULT_YM_MISSION_HK_TLM_TOPICID = 0xA7

#### `ym_lib.h` (Libraries Only)
* Defines the public API for the library
* **Note**: Applications do not need this file
* Contains function prototypes, public data structures, and constants needed by library users

### Step 6: Populate config/ Header Files with Module-Specific Definitions

__**Required files:**__
#### `default_ym_fcncode_values.h` (Applications Only)
* Defines default function code values used across the interface

1. Create a macro to shorthand values in the function code enumerator
    ``` C
    #define YM_CCVAL(x) YM_FunctionCode_##x
    ```
2. Create an enumerator that holds the command code value for each command found in [ym_fcncodes.h](#ym_fcncodesh-applications-only)
    ``` C
    /* === C code in ym_fcncodes.h for module YM === */

    /**
     * Wow this comment is so long.
     * It explains the purpose of
     * the macro almost too well
     */
    #define YM_NOOP_CC  (0u)

    /**
     * This comment isn't as good
     * as the one above but it at
     * least makes sense
     */
    #define YM_RESET_CC (1u)

    /* Other function codes here */
    ```

    will become

    ```C
    /* === C code in default_ym_fcncode_values.h for module YM === */

    enum YM_FunctionCode
    {
        YM_FunctionCode_NOOP  = 0,
        YM_FunctionCode_RESET = 1,
        /* Other function codes here */
    };
    ```
3. Include default_ym_fcncode_values.h in [ym_fcncodes.h](#ym_fcncodesh-applications-only)
    ``` C
    /* === C code in ym_fcncodes.h for module YM === */

    /* ======== */
    /* Includes */
    /* ======== */

    #include "ym_fcncode_values.h"
    ```
4. Use the shorthand macro in [ym_fcncodes.h](#ym_fcncodesh-applications-only)
    ``` C
    /* === C code in ym_fcncodes.h for module YM === */

    /**
     * Pretend this is a long comment
     * Woooooow, so many words. Thanks
     */
    #define YM_NOOP_CC  YM_CCVAL(NOOP)

    /**
     * This comment is only pretending
     * to be long but it's a phony
     */
    #define YM_RESET_CC YM_CCVAL(RESET)

    /* Other function codes here */
    ```
* The final version of default_ym_fcncode_values.h should look something like this:
    ``` C
    #ifndef DEFAULT_YM_FCNCODE_VALUES_H
    #define DEFAULT_YM_FCNCODE_VALUES_H

    /* ====== */
    /* Macros */
    /* ====== */

    #define YM_CCVAL(x) YM_FunctionCode_##x

    /* ======== */
    /* Typedefs */
    /* ======== */

    enum YM_FunctionCode
    {
        YM_FunctionCode_NOOP  = 0,
        YM_FunctionCode_RESET = 1,
        /* Other function codes */
    };

    #endif /* DEFAULT_YM_FCNCODE_VALUES_H */
    ```

#### `default_ym_interface_cfg_values.h` (Applications and Libraries)
* Defines interface configuration values that can be instantiated by name
* Make sure you rename all instances of the macro in use
    * Don't use the version of the macro with the word "DEFAULT_" in it, just the non-default one

1. Create a macro to resolve interface configuration values by name
    ``` C
    #define YM_INTERFACE_CFGVAL(x) DEFAULT_YM_INTERFACE_##x
    ```
2. Include ym_interface_cfg_values.h in [ym_interface_cfg.h](#ym_interface_cfgh-applications-and-libraries)
    ``` C
    #include "ym_interface_cfg_values.h"
    ```
3. Re-define each macro with `YM_INTERFACE_CFGVAL`, adding the default value under the modified macro
    ``` C
    /* === C code in ym_interface_cfg.h for module YM === */

    /**
     * Long macro comment that totally
     * makes the purpose of this macro
     * much clearer and less confusing
     */
    #define YM_INTERFACE_EXAMPLE         YM_INTERFACE_CFGVAL(EXAMPLE)
    #define DEFAULT_YM_INTERFACE_EXAMPLE [value]
    ```
* The final version of default_ym_interface_cfg_values.h should look something like this:
    ``` C
    #ifndef DEFAULT_YM_INTERFACE_CFG_VALUES_H
    #define DEFAULT_YM_INTERFACE_CFG_VALUES_H

    /* ====== */
    /* Macros */
    /* ====== */

    #define YM_INTERFACE_CFGVAL(x) DEFAULT_YM_##x

    #endif /* DEFAULT_YM_INTERFACE_CFG_VALUES_H */
    ```

#### `default_ym_internal_cfg_values.h` (Applications and Libraries)
* Defines internal configuration values that can be instantiated by name
* Make sure you rename all instances of the macro in use
    * Don't use the version of the macro with the word "DEFAULT_" in it, just the non-default one

1. Create a macro to resolve internal config values by name
    ``` C
    #define YM_INTERNAL_CFGVAL(x) DEFAULT_YM_INTERNAL_##x
    ```
2. Include ym_internal_cfg_values.h in [ym_internal_cfg.h](#ym_internal_cfgh-applications-and-libraries)
    ``` C
    #include "ym_internal_cfg_values.h"
    ```
3. Re-define each macro with `YM_INTERNAL_CFGVAL`, adding the default value under the modified macro
    ``` C
    /* === C code in ym_internal_cfg.h for module YM === */

    /**
     * I mean, I don't know what to
     * put here. Just act like this
     * is pithy and cool
     */
    #define YM_INTERNAL_EXAMPLE         YM_INTERNAL_CFGVAL(EXAMPLE)
    #define DEFAULT_YM_INTERNAL_EXAMPLE [value]
    ```
* The final version of default_ym_internal_cfg_values.h should like something like this:
    ``` C
    #ifndef DEFAULT_YM_INTERNAL_CFG_VALUES_H
    #define DEFAULT_YM_INTERNAL_CFG_VALUES_H

    /* ====== */
    /* Macros */
    /* ====== */

    #define YM_INTERNAL_CFGVAL(x) DEFAULT_YM_INTERNAL_##x

    #endif /* DEFAULT_YM_INTERNAL_CFG_VALUES_H */
    ```

#### `default_ym_mission_cfg.h` (Applications and Libraries)
* Combination of the mission scope configuration files. This file is created and populated for backwards compatibility

1. Include the interface configuration file
    ``` C
    #include "ym_interface_cfg.h
    ```
2. (Optional) Include the global configuration file
    ``` C
    #include "ym_global_cfg.h
    ```
* The final version of default_ym_mission_cfg.h should look something like this:
    ``` C
    #ifndef DEFAULT_YM_MISSION_CFG_H
    #define DEFAULT_YM_MISSION_CFG_H

    #include "ym_interface_cfg.h"
    /* Uncomment below if you have a global_cfg.h file */
    /* #include "ym_global_cfg.h" */

    #endif /* DEFAULT_YM_MISSION_CFG_H */
    ```

#### `default_ym_msg.h` (Applications Only)
* Combination of the message interface files and all their dependencies

1. Include the message definitions file
    ``` C
    #include "ym_msgdefs.h"
    ```
2. Include the message structures file
    ``` C
    #include "ym_msgstruct.h"
    ```

#### `default_ym_msgdefs.h` (Applications Only)
* Definitions for the command and telemetry message payloads of the module
* No macros should go in here
* How to find what goes here
    * Any struct used directly inside packets
    * Any struct with "Payload_t" at the end of its name
* The final version of default_ym_msgdefs.h should look something like:
    ``` C
    #ifndef DEFAULT_YM_MSGDEFS_H
    #define DEFAULT_YM_MSGDEFS_H

    /* ======== */
    /* Includes */
    /* ======== */

    #include "common_types.h"
    #include "cfe.h"
    /* Any other dependencies */

    /* ======== */
    /* Typedefs */
    /* ======== */

    /**
        *  \brief Example Command Payload
        */
    typedef struct
    {
        size_t ExampleSize;
        uint8  ExampleInt;
        char   ExampleStr[CFE_MISSION_MAX_PATH_LEN];
    } YM_ExampleCmd_Payload_t;

    /**
        *  \brief Housekeeping Packet Payload Structure
        */
    typedef struct
    {
        uint8  CmdCounter;
        uint8  ErrCounter;
        uint8  ExampleInt;
        size_t ExampleSize;
    } YM_HkTlm_Payload_t;

    /* Other payloads */

    #endif /* DEFAULT_YM_MSGDEFS_H */
    ```

#### `default_ym_msgids.h` (Applications Only)
* CFE Software Bus Message ID definitions for CMD/TLM interface(s) of the module
* How to find what goes here
    * Any value you pass into `CFE_SB_ValueToMsgId`
        * This function is typically called in conjunction with `CFE_SB_Suscribe` and `CFE_MSG_Init`
    * Message IDs are used as the value in case statements for the switch-case in your module's `AppPipe` function
    * Message ID macros typically end with "_MID"
    * The command, send HK, and telemetry message IDs are required for modules
* The final version of default_ym_msgids.h should look something like:
    ``` C
    #ifndef DEFAULT_YM_MSGIDS_H
    #define DEFAULT_YM_MSGIDS_H

    /* ======== */
    /* Includes */
    /* ======== */

    #include "cfe_core_api_base_msgids.h"

    /* ====== */
    /* Macros */
    /* ====== */

    #define YM_CMD_MID     0x0 /* Dummy value, yours might be different */
    #define YM_SEND_HK_MID 0x1 /* Dummy value, yours might be different */

    #define YM_HK_TLM_MID  0x2 /* Dummy value, yours might be different */

    #endif /* DEFAULT_YM_MSGIDS_H */
    ```

#### `default_ym_msgid_values.h` (Applications Only)
* Defines message ID values that can be instantiated by name
* Make sure you rename all instances of the macro in use

1. Include the topic ID definitions
    ``` C
    /* === C code in default_ym_msgid_values.h for module YM === */

    /* ======== */
    /* Includes */
    /* ======== */

    #include "ym_topicids.h"
    ```
2. Create a macro to resolve message ID values for commands and telemetry by name
    ``` C
    /* === C code in default_ym_msgid_values.h for module YM === */

    #define YM_CMD_PLATFORM_MIDVAL(x) CFE_PLATFORM_CMD_TOPICID_TO_MIDV(YM_MISSION_##x##_TOPICID)
    #define YM_TLM_PLATFORM_MIDVAL(x) CFE_PLATFORM_TLM_TOPICID_TO_MIDV(YM_MISSION_##x##_TOPICID)
    ```
3. Include default_ym_msgid_values.h in [default_ym_msgids.h](#default_ym_msgidsh-applications-only)
    ``` C
    /* === C code in default_ym_msgids.h for module YM === */

    #include "ym_msgid_values.h"
    ```
4. Define the message IDs with the newly created macro. The 3 required message IDs are used below as examples
    ``` C
    /* === C code in default_ym_msgids.h for module YM === */

    #define YM_CMD_MID     YM_CMD_PLATFORM_MIDVAL(CMD)
    #define YM_SEND_HK_MID YM_CMD_PLATFORM_MIDVAL(SEND_HK)

    #define YM_HK_TLM_MID  YM_TLM_PLATFORM_MIDVAL(HK_TLM)
    ```

#### `default_ym_msgstruct.h` (Applications Only)
* Structures that define the command and telemetry message interfaces of the module. The structs defined in this file combine a payload (from [default_ym_msgdefs.h](#default_ym_msgdefsh-applications-only)) with a header (and possibly a trailer).
* How to find what goes here
    * Any struct that has:
        * A member of data type `CFE_MSG_CommandHeader_t`
        * A member of data type `CFE_MSG_TelemetryHeader_t`
        * A name ending with "Cmd_t" or "HkPacket_t"/"HkTlm_t"
    * The No-Op command, Reset Counters command, Send HK command, and HK packet are required
* Every command needs its own struct
* The final version of default_ym_msgstruct.h should look something like:
    ``` C
    #ifndef DEFAULT_YM_MSGSTRUCT_H
    #define DEFAULT_YM_MSGSTRUCT_H

    /* ======== */
    /* Includes */
    /* ======== */

    #include "ym_msgdefs.h"
    #include "cfe.h"

    /* ======== */
    /* Typedefs */
    /* ======== */

    typedef struct
    {
        CFE_MSG_CommandHeader_t CommandHeader;
    } YM_NoopCmd_t;

    typedef struct
    {
        CFE_MSG_CommandHeader_t CommandHeader;
    } YM_ResetCountersCmd_t;

    typedef struct
    {
        CFE_MSG_CommandHeader_t CommandHeader;
        YM_ExampleCmd_Payload_t Payload;
    } YM_ExampleCmd_t;

    typedef struct
    {
        CFE_MSG_CommandHeader_t CommandHeader;
    } YM_SendHkCmd_t;

    typedef struct
    {
        CFE_MSG_TelemetryHeader_t TelemetryHeader;
        YM_HkTlm_Payload_t Payload;
    } YM_HkTlm_t;

    #endif /* DEFAULT_YM_MSGSTRUCT_H */
    ```

#### `default_ym_perfids.h` (Applications and Libraries)
* CFE ES performance monitor IDs for the module, with descriptions/documentation
* **For Applications**: Performance IDs for main processing loop, command processing, telemetry generation
* **For Libraries**: Performance IDs for major API functions or processing algorithms
* How to find what goes here:
    * You most likely have an "_perfids.h" file. Just move that file into config with the proper name
    * With regex enabled, search for `#define .*PERF_?ID` in your module
    * Any value passed into the `CFE_ES_PerfLogEntry` or `CFE_ES_PerfLogExit` function is a performance monitor ID
* The final version of default_ym_perfids.h should look something like:
    ``` C
    #ifndef DEFAULT_YM_PERFIDS_H
    #define DEFAULT_YM_PERFIDS_H

    #define YM_APPMAIN_PERF_ID 30 /* Dummy values, yours can be different */
    #define YM_EXAMPLE_PERF_ID 31 /* Dummy values, yours can be different */

    #endif /* DEFAULT_MM_PERFIDS_H */
    ```

#### `default_ym_platform_cfg.h` (Applications and Libraries)
* Combination of mission_cfg.h, internal_cfg.h and any other dependencies

1. Include the mission configuration file
    ``` C
    #include "ym_mission_cfg.h"
    ```
2. Include the internal configuration file
    ``` C
    #include "ym_internal_cfg.h"
    ```

#### `default_ym_topicid_values.h` (Applications Only)
* Defines topic ID values that can be instantiated by name

1. Create a macro to resolve topic ID values by name
    ``` C
    /* === C code in default_ym_topicid_values.h for module YM === */

    #define YM_MISSION_TIDVAL(x) DEFAULT_YM_MISSION_##x##_TOPICID
    ```
2. Include default_ym_topicid_values.h in [ym_topicids.h](#ym_topicidsh-applications-only)
    ``` C
    /* === C code in ym_topicids.h for module YM === */

    #include "ym_topicid_values.h"
    ```
3. Define the topic IDs with the newly created macro. The 3 required message IDs are used below as examples
    ``` C
    #define YM_CMD_MID     YM_CMD_PLATFORM_MIDVAL(CMD)
    #define YM_SEND_HK_MID YM_CMD_PLATFORM_MIDVAL(SEND_HK)

    #define YM_HK_TLM_MID  YM_TLM_PLATFORM_MIDVAL(HK_TLM)
    ```

#### `eds_ym_fcncode_values.h` (Applications Only)
* When using EDS, map function codes to their EDS-defined values instead of their default values
* **Note**: Libraries do not have command interfaces and therefore do not need this file

1. Include EDS command codes
    ``` C
    #include "ym_eds_cc.h"
    ```
2. Map the command codes to an EDS generate command code
    ``` C
    #define YM_CCVAL(x) EDS_CONTAINER_YM_##x##_CC
    ```
* The final version of eds_ym_fcncode_values.h should look something like this:
    ``` C
    #ifndef EDS_YM_FCNCODE_VALUES_H
    #define EDS_YM_FCNCODE_VALUES_H

    /* ======== */
    /* Includes */
    /* ======== */

    #include "ym_eds_cc.h"

    /* ====== */
    /* Macros */
    /* ====== */

    #define YM_CCVAL(x) EDS_CONTAINER_YM_##x##_CC

    #endif /* EDS_YM_FCNCODE_VALUES_H */
    ```

#### `eds_ym_interface_cfg_values.h` (Applications and Libraries)
* When using EDS, map interface configuration to their EDS-defined values instead of their default values
* **For Applications**: EDS-generated command/telemetry interface parameters
* **For Libraries**: EDS-generated API interface parameters
* The way EDS interface configuration generation is handled means there isn't an "_INTERFACE\_" in the macro definition

1. Include EDS headers
    ``` C
    /* === C code in eds_ym_interface_cfg_values.h for module YM === */

    #include "cfe_mission_eds_designparameters.h"
    #include "ym_eds_defines.h"
    ```
2. Create a macro to resolve EDS interface configuration values by name
    ``` C
    /* === C code in eds_ym_interface_cfg_values.h for module YM === */

    #define YM_INTERFACE_CFGVAL(x) EdsParam_YM_##x
    ```
3. The final version of eds_ym_interface_cfg_values.h should look something like:
    ``` C
    #ifndef EDS_YM_INTERFACE_CFG_VALUES_H
    #define EDS_YM_INTERFACE_CFG_VALUES_H

    #include "cfe_mission_eds_designparameters.h"

    #define YM_INTERFACE_CFGVAL(x) EdsParam_YM_##x

    #endif /* EDS_YM_INTERFACE_CFG_VALUES_H */
    ```

#### `eds_ym_msgdefs.h` (Applications Only)
* When using EDS, map the message payloads to their EDS-defined values instead of their default values

1. Include the typedefs created by EDS from the XML file
    ``` C
    /* === C code in eds_ym_msgdefs.h for module YM === */

    #include "ym_eds_typedefs.h"
    ```
2. Include the function codes for payloads
    ``` C
    /* === C code in eds_ym_msgdefs.h for module YM === */

    #include "ym_fcncodes.h"
    ```
3. The final version of eds_ym_msgdefs.h should look something like:
    ``` C
    #ifndef EDS_YM_MSGDEFS_H
    #define EDS_YM_MSGDEFS_H

    /* ======== */
    /* Includes */
    /* ======== */

    #include "ym_eds_typedefs.h"
    #include "ym_fcncodes.h"

    #endif /* EDS_YM_MSGDEFS_H */
    ```

#### `eds_ym_msgstruct.h` (Applications Only)
* When using EDS, map message structs to their EDS-defined values instead of their default values

1. Include the EDS-defined typedefs generated from the XML
    ``` C
    /* === C code in eds_ym_msgstruct.h for module YM === */

    #include "ym_eds_typedefs.h"
    ```
2. The final version of eds_ym_msgstruct.h should look something like this:
    ``` C
    #ifndef EDS_YM_MSGSTRUCT_H
    #define EDS_YM_MSGSTRUCT_H

    /* ======== */
    /* Includes */
    /* ======== */

    #include "ym_eds_typedefs.h"

    #endif /* EDS_YM_MSGSTRUCT_H */
    ```

#### `eds_ym_topicid_values.h` (Applications Only)
* When using EDS, map function codes to their EDS-defined values instead of their default values by defining topic ID values that can be instantiated by name

1. Include EDS design parameters
    ``` C
    /* === C code in eds_tm_topicid_values.h for module YM === */

    #include "cfe_mission_eds_designparameters.h"
    ```
2. Define a macro to instantiate EDS topic IDs by name
    ``` C
    /* === C code in eds_ym_topicid_values.h for module YM === */

    #define YM_MISSION_TIDVAL(x) EdsParam_CFE_MISSION_YM_##x##_TOPICID
    ```
3. The final version of eds_ym_topicid_values.h should look something like this:
    ``` C
    #ifndef EDS_YM_TOPICID_VALUES_H
    #define EDS_YM_TOPICID_VALUES_H

    #include "cfe_mission_eds_designparameters.h"

    #define YM_MISSION_TIDVAL(x) EdsParam_CFE_MISSION_YM_##x##_TOPICID

    #endif /* EDS_YM_TOPICID_VALUES_H */
    ```

__**Optional files:**__
#### `default_ym_tbldefs.h` (Applications with Tables Only)
* Definitions for the table file payload(s) of the module
* How to find what goes here
    * Data type of the array that constitutes a table
* No macros should go in this file, put them in [default_ym_tbldefs.h](#default_ym_tbldefsh-applications-with-tables-only) instead

1. In ym/fsw/tables/*tbl.c, find where the datatype of this instance of the table is declared
    * In this example, you're interested in YM_ExampleTable_t:
        ``` C
        YM_ExampleTable_t ExampleTable = {
            "MemberString 1",
            {0, 0, "Entry 1"},
            {0, 0, "Entry 2"},
            {0, 0, "Entry 3"},
        }
        ```
2. Find the definition of the table entry member
    * Could have "Entry_t" in the data type name
    * Could have "Entry" in or as the name of the variable
    * Could be an array of size X, where X is a macro with some variant of "TABLE_SIZE"
    * In this example, you're interested in YM_ExampleEntry_t:
        ``` C
        typedef struct
        {
            YM_ExampleEntry_t Entry[YM_INTERFACE_EXAMPLE_TABLE_SIZE];
        } YM_ExampleTable_t;
        ```
3. Put _that_ typedef in default_ym_tbldefs.h
4. Repeat for any other unique tables
5. Include dependencies

#### `default_ym_tblstruct.h` (Applications with Tables Only)
* Structures that define the table file interface(s) of the module. The structs defined in this file combine a payload (from [default_ym_tbldefs.h](#default_ym_tbldefsh-applications-with-tables-only)) with a header (and possibly a trailer).
* How to find what goes here
    * Datatypes of tables
    * All table related macros

1. In ym/fsw/tables/*tbl.c, find where the datatype of this instance of the table is declared
    * In this example, you're interested in YM_ExampleTable_t:
        ``` C
        YM_ExampleTable_t ExampleTable = {
            "MemberString 1",
            {0, 0, "Entry 1"},
            {0, 0, "Entry 2"},
            {0, 0, "Entry 3"},
        }
        ```
2. Put that typedef in default_ym_tblstruct.h
3. Repeat for any other unique tables
4. Include dependencies

#### `default_ym_tbl.h` (Applications with Tables Only)
* Required if default_tm_tbldefs.h and [default_ym_tbldefs.h](#default_ym_tbldefsh-applications-with-tables-only) are used

1. Include table payloads
    ``` C
    /* === C code in default_ym_tbl.h for module YM === */

    #include "ym_tbldefs.h"
    ```
2. Include table interfaces
    ``` C
    /* === C code in default_ym_tbl.h for module YM === */

    #include "ym_tblstruct.h"
    ```

#### `default_ym_extern_typedefs.h` (Applications and Libraries)
* Macros and data types shared between command or telemetry messages, data files, and API functions
* **For Applications**: Custom types used in command/telemetry payloads and table entries
* **For Libraries**: Custom types used in API functions and shared data structures
* How to find what goes here
    * typedef declarations (struct, enums, etc.) used as member of payloads or entries in [default_ym_tbldefs.h](#default_ym_tbldefsh-applications-with-tables-only) and [default_ym_msgdefs.h](#default_ym_msgdefsh-applications-only)
    * Any structs inside diagnostic packets (typically held in YM_AppData)
* Example
    ``` C
    /* === C code in default_ym_tbldefs.h for module YM === */

    typedef struct
    {
        uint16             ExampleInt;
        YM_ExampleStruct_t ExampleStruct[YM_NUM_EXAMPLE_STRUCTS]; /* This struct should go in default_ym_extern_typedefs.h */
    } YM_ExampleTableEntry_t;
    ```

    ``` C
    /* === C code in default_ym_msgdefs.h for module YM === */

    typedef struct
    {
        uint8            ExampleInt;
        YM_ExampleEnum_t ExampleEnum; /* This enum should go in default_ym_extern_typedefs.h */
    } YM_ExampleCmd_Payload_t;
    ```

    ``` C
    /* === C code in default_ym_extern_typedefs.h for module YM === */

    /* ====== */
    /* Macros */
    /* ====== */

    #define YM_NUM_EXAMPLE_STRUCTS (5u)

    /* ======== */
    /* Typedefs */
    /* ======== */

    typedef struct
    {
        uint8  Example1;
        uint16 Example2;
    } YM_ExampleStruct_t;

    typedef enum
    {
        YM_TYPE1 = 0,
        YM_TYPE2 = 1,
        YM_TYPE3 = 2,
    } YM_ExampleEnum_t;
    ```

#### `eds_ym_tbldefs.h` (Applications with Tables Only)
* When using EDS, map table payload definitions to their EDS-defined values instead of their default values

* Include the typedefs created by EDS from the XML file
    ``` C
    /* === C code in eds_ym_tbldefs.h for module YM === */

    /* ======== */
    /* Includes */
    /* ======== */

    #include "ym_eds_typedefs.h"
    ```

#### `eds_ym_extern_typedefs.h` (Applications and Libraries - As Needed)
* When using EDS, map macros and data types shared between command or telemetry messages, data files, and API functions to their EDS-defined values instead of their default values
* **For Applications**: EDS-generated versions of custom command/telemetry types
* **For Libraries**: EDS-generated versions of custom API types

1. Include common header files
    ``` C
    /* === C code in eds_ym_extern_typdefs.h for module YM === */

    #include "common_types.h"
    #include "cfe_resourceid_typedefs.h"
    ```
2. Include the typedefs created by EDS from the XML file
    ``` C
    /* === C code in eds_ym_extern_typdefs.h for module YM === */

    #include "ym_eds_typedefs.h"
    ```

### Step 7: Populate Dispatch Files

* Non-EDS dispatch files
    1. Add the license header to ym_dispatch.h and ym_dispatch.c. See [Step 4](#step-4-create-the-skeleton-for-each-of-the-created-files)
    2. Add include guards to ym_dispatch.h. See [Step 4](#step-4-create-the-skeleton-for-each-of-the-created-files)
    3. Move the command length verification function prototype and definition to ym_dispatch.h and ym_dispatch.c respectively
        * How to find:
            * Search for "VerifyCmdLength"
            * Search for "CFE_MSG_GetSize"
            * Look in fsw/src/ym_app.h
            * Look in fsw/src/ym_cmds.h
            * Sometimes this function is put into a file named something like ym*_utils.c/h
    4. Move the ground command processing function prototype and definition to ym_dispatch.h and ym_dispatch.c respectively
        * How to find:
            * Search for "AppPipe"
            * Search for "ProcessGroundCommand"
            * Search for usages of the message IDs in [default_ym_msgids.h](#default_ym_msgidsh-applications-only)
            * Search for "switch (CFE_SB_MsgIdToValue"
    5. Move the task pipe function prototype and definition to ym_dispatch.h and ym_dispatch.c respectively
        * How to find:
            * Sometimes it's part of the ground processing function and hasn't been prototyped yet
                * If this is the case the prototype should be `void YM_TaskPipe(const CFE_SB_Buffer_t* BufPtr);`. Make sure you add documentation
            * Search for "TaskPipe"
            * Search for the Send HK message ID from [default_ym_msgids.h](#default_ym_msgidsh-applications-only)
    * The final version of ym_dispatch.h should look something like the following code snippet. Pay special attention to the prototype function names and the const arguments
        ``` C
        /* === C code in ym_dispatch.h for module YM === */

        #ifndef YM_DISPATCH_H
        #define YM_DISPATCH_H

        /* ======== */
        /* Includes */
        /* ======== */

        #include "cfe.h"

        /* =================== */
        /* Function Prototypes */
        /* =================== */

        /**
         * \brief This routine will check if the actual length of a software bus
         *        command message matches the expected length
         * \param[in] MsgPtr         Pointer to the command to be verified
         * \param[in] ExpectedLength The expected length of the message
         *                           based upon the command code
         * \return Command length validity
         * \retval true:  The length of the provided message and the expected
         *                length match
         * \retval false: The length of the provided message differs from the
         *                expected length
         */
        bool YM_VerifyCmdLength(const CFE_MSG_Message_t* MsgPtr, size_t ExpectedLength);

        /**
         * \brief Using the command IDs this function calls the appropriate
         *        routine to handle the command
         * \param[in] BufPtr Pointer to Software Bus buffer
         */
        void YM_ProcessGroundCommd(const CFE_SB_Buffer_t* BufPtr);

        /**
         * \brief Route a message/packet to the command processing function or to
         *        the housekeeping request function
         * \param[in] BufPtr SB buffer pointer to be routed
         */
        void YM_TaskPipe(const CFE_SB_Buffer_t* BufPtr);

        #endif /* YM_DISPATCH_H */
        ```

    * The final version of ym_dispatch.c should look something like:
        ``` C
        /* === C code in ym_dispatch.c for module YM === */

        /* ======== */
        /* Includes */
        /* ======== */

        #include "ym_dispatch.h"
        #include "ym_app.h"
        #include "ym_cmds.h"
        #include "ym_eventids.h"
        #include "ym_msg.h"
        #include "ym_msgids"
        #include "ym_fcncodes.h"

        #include "cfe.h"

        /* ==================== */
        /* Function Definitions */
        /* ==================== */

        bool YM_VerifyCmdLength(const CFE_MSG_Message_t* MsgPtr, size_t ExpectedLength)
        {
            /* Code here */
        }

        void YM_ProcessGroundCommd(const CFE_SB_Buffer_t* BufPtr)
        {
            /* Code here */
        }

        void YM_TaskPipe(const CFE_SB_Buffer_t* BufPtr)
        {
            /* Code here */
        }
        ```

* EDS dispatch file
1. Add the license header to ym_eds_dispatch.c. See [Step 4](#step-4-create-the-skeleton-for-each-of-the-created-files)
2. Include likely dependencies. While these dependencies might be enough for you, you might end up needed to add others
    ``` C
    /* === C code in ym_eds_dispatch.c for module YM === */

    /* ======== */
    /* Includes */
    /* ======== */

    #include "ym_app.h"
    #include "ym_dispatch.h"
    #include "ym_cmds.h"
    #include "ym_eventids.h"
    #include "ym_msgids.h"
    #include "ym_msg.h"

    #include "ym_eds_dispatcher.h"
    #include "ym_eds_dictionary.h"
    ```
3. Create a lookup table for MM command codes by referencing ym_cmds.h
    ``` C
    /* === C code in ym_cmds.h for module YM === */

    CFE_Status_t YM_NoopCmd(const CFE_SB_Buffer_t* BufPtr);
    CFE_Status_t YM_ResetCountersCmd(const CFE_SB_Buffer_t* BufPtr);
    CFE_Status_t YM_SendHkCmd(const CFE_SB_Buffer_t* BufPtr);
    ```

    ``` C
    /* === C code in ym_eds_dispatch.c for module YM === */

    /* ======= */
    /* Globals */
    /* ======= */

    /**
     * \brief Lookup table for YM command codes
     */
    /* clang-format off */
    static const EdsDispatchTable_EdsComponent_YM_Application_CFE_SB_Telecommand_t YM_TC_DISPATCH_TABLE =
    {
        .CMD =
        {
            .NoopCmd_indication          = YM_NoopCmd,
            .ResetCountersCmd_indication = YM_ResetCountersCmd,
        },
        .SEND_HK =
        {
            .indication = YM_SendHkCmd
        }
    };
    /* clang-format on */
    ```
4. Create the EDS version of the task pipe function, this can be copy-pasted
    ``` C
    /* === C code in ym_eds_dispatch.c === */

    /* ==================== */
    /* Function Definitions */
    /* ==================== */

    void YM_TaskPipe(const CFE_SB_Buffer_t* BufPtr)
    {
        CFE_Status_t      Status;
        CFE_SB_MsgId_t    MsgId;
        CFE_MSG_Size_t    MsgSize;
        CFE_MSG_FcnCode_t MsgFc;

        Status = EdsDispatch_EdsComponent_YM_Application_Telecommand(BufPtr, &YM_TC_DISPATCH_TABLE);

        if (Status != CFE_SUCCESS)
        {
            CFE_MSG_GetMsgId(&BufPtr->Msg, &MsgId);
            CFE_MSG_GetSize(&BufPtr->Msg, &MsgSize);
            CFE_MSG_GetFcnCode(&BufPtr->Msg, &MsgFc);
            ++YM_AppData.HkTlm.Payload.ErrCounter;

            if (Status == CFE_STATUS_UNKNOWN_MSG_ID)
            {
                CFE_EVS_SendEvent(YM_MID_ERR_EID, CFE_EVS_EventType_ERROR,
                                    "YM: invalid command packet,MID = 0x%x", (unsigned int)CFE_SB_MsgIdToValue(MsgId));
            }
            else if (Status == CFE_STATUS_WRONG_MSG_LENGTH)
            {
                CFE_EVS_SendEvent(YM_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                                    "YM: Invalid Msg length: ID = 0x%X, CC = %u, Len = %u",
                                    (unsigned int)CFE_SB_MsgIdToValue(MsgId), (unsigned int)MsgFc, (unsigned int)MsgSize);
            }
            else
            {
                CFE_EVS_SendEvent(YM_CC_ERR_EID, CFE_EVS_EventType_ERROR,
                                    "YM: Invalid ground command code: CC = %d", (int)MsgFc);
            }
        }
    }
    ```

### Step 8: Standardize Files
*When integrating EDS into your module, the following code modifications are typically required to ensure compatibility:*

**<u>Unit Test Updates**</u><br>
Existing unit tests may require updates to accommodate the new EDS-based interface definitions and message structures.

**<u>Macro Standardization**</u><br>
Replace OS-specific macros with CFE/mission-standard macros to ensure consistency across the flight software framework.

**<u>Command Structure Requirements**</u><br>
Each command must be defined with its own dedicated structure to properly interface with the EDS message definitions.

**<u>Enumeration Naming Convention**</u><br>
All enumeration types must end with `Enum_t` suffix to match the naming convention used by the EDS code generator.

**<u>Telemetry Header Naming Convention**</u><br>
Telemetry packet structures must use `TelemetryHeader` as the name for the telemetry header member to align with EDS-generated code expectations.

**<u>Message Pointer Usage**</u><br>
When accessing message pointer (`CFE_MSG_Message_t*`), expecially telemetry packet message pointers, use the `CFE_MSG_PTR()` macro in conjunction with the `TelemetryHeader` member of the packet structure to ensure proper type casting and memory access.

Example:

Given the following definitions
``` C
/* === C code in ym_msgstructs.h === */

typedef struct
{
    CFE_MSG_TelemetryHeader_t TelemetryHeader; /** \brief Telemetry Header */
    YM_HkTlm_Payload_t        Payload;         /** \brief Telemetry Payload */
} YM_HkTlm_t;
```

and

``` C
/* === C code in ym_app.h === */

typedef struct
{
    /* Other stuff */
    YM_HkTlm_t HkTlm;
    /* Other stuff */
} YM_AppData_t;
```

this:
``` C
/* === C code in ym_cmds.c === */

CFE_SB_TimeStampMsg((CFE_MSG_Message_t*) &(YM_AppData.HkTlm));
CFE_SB_TransmitMsg((CFE_MSG_Message_t*) &(YM_AppData.HkTlm), true);
```

will become:
``` C
/* === C code in ym_cmds.c === */

CFE_SB_TimeStampMsg(CFE_MSG_PTR(YM_AppData.HkTlm.TelemetryHeader));
CFE_SB_TransmitMsg(CFE_MSG_PTR(YM_AppData.HkTlm.TelemetryHeader), true);
```

**<u>Command Organization**</u><br>
* All command processing functions must be implemented in `ym_cmds.c` and declared in `ym_cmds.h` to maintain clear separation of concerns between message dispatching and command execution.
* All command function names are expected to mirror the names of the command structs. For example:
    ``` C
    /* === C code in default_ym_msgstruct.h for module YM === */

    typedef struct
    {
        CFE_MSG_CommandHeader_t CommandHeader;
        YM_ExampleCmd_Payload_t Payload;
    } YM_ExampleCmd_t;
    ```

    The important part of the struct name is ExampleCmd. That should mean that your example command function should have this signature:

    ``` C
    /* === C code in ym_cmds.h for module YM === */

    CFE_Status_t YM_ExampleCmd(const YM_ExampleCmd_t* Msg);
    ```
* All command processing functions should return a CFE_Status_t-type status
    * So long as there is nothing else to do, the command processing function should return CFE_SUCCESS, even if the command processing function rejected the message
    * A non-CFE_SUCCESS response is only if the calling function is expecting to do something atypical, such as increment the error counter.
    * The nominal case for application dispatchers (like ym_dispatch.c) is to ignore the return code


<b>These standardizations ensure seamless integration between manually written code and EDS-generated interface definitions.</b>


## EDS XML Configuration
**Important**: The following EDS XML configuration instructions apply primarily to **applications** that have command/telemetry interfaces. **Libraries** typically have much simpler EDS definitions that focus on:
- Custom data types used in API functions
- Public interface parameters
- Shared data structures

Libraries generally do **not** need:
- Command containers
- Telemetry containers
- Topic ID mappings

For libraries, focus on the DataTypeSet section and skip most of the ComponentSet requirements.

**Reference Files for EDS XML**: When creating your EDS XML definitions, you can reference existing types and assets from:
- `base_types.xml` - Contains fundamental data types (uint8, uint16, strings, etc.)
- `config.xml` - Contains mission-specific configuration parameters and constants
- See [Update EDS Configuration Files](#3-update-eds-configuration-files) for details on config.xml setup

* All members of the XML should be found in the config directory. If you wander outside those files, you're probably doing it wrong
* Start by creating the general XML Headers `<PackageFile>` and `<Package>`
    *   Verbatim,
        ``` XML
        <?xml version="1.0" encoding="utf-8"?>
        <!--
            GSC-18128-1, "Core Flight Executive Version 6.7"
            LEW-19710-1, "CCSDS electronic data sheet implementation"

            Copyright (c) 2006-2019 United States Government as represented by
            the Administrator of the National Aeronautics and Space Administration.
            All Rights Reserved.

            Licensed under the Apache License, Version 2.0 (the "License");
            you may not use this file except in compliance with the License.
            You may obtain a copy of the License at

            http://www.apache.org/licenses/LICENSE-2.0

            Unless required by applicable law or agreed to in writing, software
            distributed under the License is distributed on an "AS IS" BASIS,
            WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
            See the License for the specific language governing permissions and
            limitations under the License.

            This document adheres to the Electronic Data Sheet (EDS) XML schema
            as prescribed in CCSDS book 876.0.

            Purpose:
            This describes all interface objects for the YM module
        -->
        <PackageFile xmlns="http://www.ccsds.org/schema/sois/seds">
            <Package name="YM" shortDescription="Your Module application">
            </Package>
        </PackageFile>
        ```
* A `<Package>` should have 2 sub-elements, `<DataTypeSet>` and `<ComponentSet>`
    *   ``` XML
        <Package>
            <DataTypeSet>
                <!-- See below -->
            </DataTypeSet>

            <ComponentSet>
                <!-- See below -->
            </ComponentSet>
        </Package>
        ```
    * The `<DataTypeSet>` contains the internal definitions of the module
        * Custom Data Types
            * Strings
                * How to find `StringDataType`s
                    * Using regex, search the module for `^\s*(const|static)?\s*char[*]?.*[[]?.*$`
                        * This will match all character arrays/pointers
                            * Exclude unit tests
                            * Exclude source files (.c files)
                            * Any remaining strings that aren't `CFE_MISSION_*`-sized are strings you'll need to create
                        * Be careful to not create strings that already exist in `base_types.xml` or `config.xml` (see [Update EDS Configuration Files](#3-update-eds-configuration-files))
                *   ``` XML
                    <StringDataType name="[StringName]" length="${YM/[Length]}"/>
                    ```
                    *   | Value        | Description |
                        | ------------ | ----------- |
                        | [StringName] | Name of the variable from the source code |
                        | [Length]     | Value of the macro from the source code, also defined in [base_types.xml](#3-update-eds-configuration-files) or [config.xml](#3-update-eds-configuration-files) |
                    *   Example
                        ``` C
                        /* === C code for module YM === */
                        char  YM_Example1[YM_STR_LEN];
                        char* YM_Example2;
                        ```

                        becomes

                        ``` XML
                        <!-- XML code in ym.xml for module YM -->
                        <StringDataType name="Example1" length="${YM/STR_LEN}"/>
                        <StringDataType name="Example2" length="${YM/MAX_STR_LEN}"/>
                        ```
            * Enums
                * How to find `EnumeratedDataType`s
                    * Search your module for "typedef enum"
                    * Search your module for "_Enum_t"
                *   ``` XML
                    <EnumeratedDataType name="[EnumDataTypeName]" shortDescription="[EnumDataTypeDesc]">
                        <EnumerationList>
                            <Enumeration label="[EnumerationLabel]" value="[EnumerationVal]" shortDescription="[EnumerationDesc]"/>
                            <Enumeration label="[EnumerationLabel]" value="[EnumerationVal]">
                                <LongDescription>
                                    [EnumerationDesc]
                                </LongDescription>
                            </Enumeration>
                        </EnumerationList>
                        <IntegerDataEncoding sizeInBits="[EnumerationBits]" encoding="[EnumerationSign]"/>
                    </EnumeratedDataType>
                    ```
                    *   | Value              | Description |
                        | ------------------ | ----------- |
                        | [EnumDataTypeName] | The name of the enum from the source code |
                        | [EnumDataTypeDesc] | Comment from the source code describing the purpose of the enum |
                        | [EnumerationLabel] | Name of an enum member from the source code |
                        | [EnumerationVal]   | Value of [EnumerationLabel] |
                        | [EnumerationDesc]  | Comment from the source code describing the purpose of the enum |
                        | [EnumerationBits]  | The number of bits (in multiples of 8) that covers the largest enumeration value |
                        | [EnumerationSign]  | Almost always `unsigned`, but could be `signed` if appropriate |
                    *   Example
                        ``` C
                        /* === C code for module YM === */
                        /** \brief Example Enum */
                        typedef enum
                        {
                            YM_A = 1, /** \brief Value of A for YM */
                            /*
                            * Some complicated multi-line
                            * explanation of the purpose of
                            * the value of YM_B
                            */
                            YM_B = 2,
                            YM_C = 3, /* Value of C for YM */
                            YM_D,
                        } YM_ExampleEnum_t;
                        ```

                        becomes

                        ``` XML
                        <!-- XML code in ym.xml for module YM -->
                        <EnumeratedDataType name="ExampleEnum" shortDescription="Example Enum">
                            <EnumerationList>
                                <Enumeration label="A" value="1" shortDescription="Value of A for YM"/>
                                <Enumeration label="B" value="2">
                                    <LongDescription>
                                        Some complicated multi-line
                                        explanation of the purpose of
                                        the value of YM_B
                                    </LongDescription>
                                </Enumeration>
                                <Enumeration label="C" value=3 shortDescription="Value of C for YM"/>
                                <Enumeration label="D" value=4 shortDescription="Value of D for YM"/>
                            </EnumerationList>
                            <IntegerDataEncoding sizeInBits="8" encoding="unsigned"/>
                        </EnumeratedDataType>
                        ```
            * Non-character Arrays
                *   ``` XML
                    <ArrayDataType name="[ArrayName]" dataTypeRef="[ArrayDataType]" shortDescription="[ArrayDesc]">
                        <DimensionList>
                            <Dimension size="${YM/[ArrayDimension]}"/>
                        </DimensionList>
                    </ArrayDataType>
                    ```
                    *   | Value            | Description |
                        | ---------------- | ----------- |
                        | [ArrayName]      | Variable name of the array from the source code |
                        | [ArrayDataType]  | The name of a previously defined container or common type |
                        | [ArrayDesc]      | Comment from the source code describing the purpose of the array |
                        | [ArrayDimension] | Size of the array from the source code and defined in `config.xml` (see [Update EDS Configuration Files](#3-update-eds-configuration-files)) |
                    *   Example
                        ``` C
                        /* === C code for module YM === */
                        uint8 YM_ExampleArray[YM_MAX_ARRAY_SIZE]; /* Just an example */
                        ```

                        becomes

                        ``` XML
                        <!-- XML code in ym.xml for module YM -->
                        <ArrayDataType name="ExampleArray" dataTypeRef="BASE_TYPES/uint8" shortDescription="Just an example">
                            <DimensionList>
                                <Dimension size="${YM/MAX_ARRAY_SIZE}"/>
                            </DimensionList>
                        </ArrayDataType>
                        ```
            * Structs
                * Most instances of `typedef struct` declared in config/
                * Excludes structs in:
                    * [default_ym_msgdefs.h](#default_ym_msgdefsh-applications-only)
                    * [default_ym_msgstruct.h](#default_ym_msgstructh-applications-only)
                    * [default_ym_tbldefs.h](#default_ym_tbldefsh-applications-with-tables-only)
                    * [default_ym_tbldefs.h](#default_ym_tbldefsh-applications-with-tables-only)

        * Payloads
            * Defined in [default_ym_msgdefs.h](#default_ym_msgdefsh-applications-only)
            *   ``` C
                /* === C code in default_ym_msgdefs.h for module YM === */
                typedef struct
                {
                    uint32 ValU32;                 /* 32 bit unsigned integer value */
                    int16  ValI16;                 /* 16 bit signed integer value */
                } YM_Example_Payload_t;
                ```

                becomes

                ``` XML
                <!-- XML code in ym.xml for module YM -->
                <ContainerDataType name="Example_Payload" shortDescription="Example payload">
                    <EntryList>
                        <Entry name="ValU32" type="BASE_TYPES/uint32" shortDescription="32 bit unsigned integer value"/>
                        <Entry name="ValI16" type="BASE_TYPES/int16" shortDescription="16 bit signed integer value"/>
                    </EntryList>
                </ContainerDataType>
                ```
        * Basic command container
            * The base data type for all command containers
            * In the source code this is sometimes called `YM_NoArgsCmd_t`
            *   ``` XML
                <!-- XML code in ym.xml for module YM -->
                <ContainerDataType name="CommandBase" baseType="CFE_HDR/CommandHeader"/>
                ```
        * No-Op Command container
            * ``` XML
                <!-- XML code in ym.xml for module YM -->
                <ContainerDataType name="NoopCmd" baseType="CommandBase" shortDescription="No-Op command packet structure"/>
                ```
        * Reset Counters Command container
            * ``` XML
                <!-- XML code in ym.xml for module YM -->
                <ContainerDataType name="ResetCountersCmd" baseType="CommandBase" shortDescription="Reset Counters command packet structure"/>
                ```
        * Send Housekeeping Command container
            *   ``` XML
                <!-- XML code in ym.xml for module YM -->
                <ContainerDataType name="SendHkCmd" baseType="CFE_HDR/CommandHeader" shortDescription="Housekeeping Request command packet structure"/>
                ```
        * Housekeeping telemetry container
            *   ``` C
                /* === C code in default_ym_msgdefs.h for module YM === */

                /**
                     *  \brief Housekeeping Packet Payload Structure
                    */
                typedef struct
                {
                    uint8  CommandCounter;      /** \brief Command Counter */
                    uint8  CommandErrorCounter; /** \brief Command Error Counter */
                    /* [others] */
                } YM_HkTlm_Payload_t;
                ```

                ``` XML
                <!-- XML code in ym.xml for module YM -->
                <ContainerDataType name="HkTlm_Payload" shortDescription="YM Housekeeping Packet Payload Structure">
                    <EntryList>
                        <Entry name="CommandCounter" type="BASE_TYPES/uint8"/>
                        <Entry name="CommandErrorCounter" type="BASE_TYPES/uint8"/>
                        <!-- [others] -->
                    </EntryList>
                </ContainerDataType>

                ...

                <ContainerDataType name="HkTlm" baseType="CFE_HDR/TelemetryHeader">
                    <EntryList>
                        <Entry type="HkTlm_Payload" name="Payload"/>
                    </EntryList>
                </ContainerDataType>
                ```
                * The HkTlm_Payload container should be defined before using it in the HkTlm container
        * Commands
            * The structs to be added to the EDS XML file are found in [default_ym_msgdefs.h](#default_ym_msgdefsh-applications-only)
            * The function code values are found in [default_ym_fcncode_values.h](#default_ym_fcncode_valuesh-applications-only)
            *   ``` C
                /* === C code in default_ym_msgdefs.h for module YM === */

                /**
                  *  \brief Example command
                  */
                typedef struct
                {
                    uint32 CommandParam; /** \brief Example command parameter */
                } YM_ExampleCmd_Payload_t;
                ```

                ``` C
                /* === C code in default_ym_fcncode_values.h for module YM === */
                enum YM_FunctionCode
                {
                    YM_FunctionCode_EXAMPLE = 0,
                    ...
                }
                ```

                ``` XML
                <!-- XML code in ym.xml for module YM -->
                <ContainerDataType name="ExampleCmd_Payload" shortDescription="YM example command">
                    <EntryList>
                        <Entry name="CommandParam" type="BASE_TYPES/uint32"/>
                    </EntryList>
                </ContainerDataType>

                ...

                <ContainerDataType name="ExampleCmd" baseType="CommandBase">
                    <ConstraintSet>
                        <ValueConstraint entry="Sec.FunctionCode" value="0"/>
                    </ConstraintSet>

                    <EntryList>
                        <Entry type="ExampleCmd_Payload" name="Payload"/>
                    </EntryList>
                </ContainerDataType>
                ```
        * Table container
            *   ``` C
                /* === C code in default_ym_tbldefs.h for module YM === */

                #define YM_NUM_MEMBERS (42u)

                ...

                /*
                ** Example Table structure
                */
                typedef struct
                {
                    uint16 Int1;
                    uint16 Int2;
                } YM_ExampleTableMember_t;
                ```

                ``` C
                /* === C code in default_ym_tblstruct.h for module YM === */

                typedef struct
                {
                    YM_ExampleTableMember_t Members[YM_NUM_MEMBERS];
                } YM_ExampleTable_t;
                ```

                ``` XML
                <!-- XML code in ym.xml for module YM -->
                <ContainerDataType name="ExampleTableMember" shortDescription="Member of the example table">
                    <EntryList>
                        <Entry name="Int1" type="BASE_TYPES/uint16" shortDescription="Int1"/>
                        <Entry name="Int2" type="BASE_TYPES/uint16" shortDescription="Int2"/>
                    </EntryList>
                </ContainerDataType>

                ...

                <ArrayDataType name="Members" dataTypeRef="ExampleTableMember" shortDescription="Array of members of the example table">
                    <DimensionList>
                        <Dimension size="${YM/NUM_MEMBERS}"/>
                    </DimensionList>
                </ArrayDataType>

                ...

                <ContainerDataType name="ExampleTable" shortDescription="Example table">
                    <EntryList>
                        <Entry name="ExampleTableMembers" type="Members" shortDescription="Table members"/>
                    </EntryList>
                </ContainerDataType>
                ```
                *   [default_ym_tbldefs.h](#default_ym_tbldefsh-applications-with-tables-only)'s `YM_ExampleTableMember_t` is turned in ym.xml's `ExampleTableMember`

                    ym.xlm's `ExampleTableMember` is used as the `dataTypeRef` in ym.xml's `ArrayDataType`, `Members`

                    ym.xml's `Members` was then used as the `type` in the `ContainerDataType` that defines the table `ExampleTable`

                    ym.xml's `ExampleTable` thus, is a converted version of [default_ym_tbldefs.h](#default_ym_tbldefsh-applications-with-tables-only)'s `YM_ExampleTable_t`

    * `<ComponentSet>`
        * Required block that contains the interface definitions for the module
        *   ``` XML
            <ComponentSet>
                <Component name="Application">
                    <!-- See below -->
                </Component>
            </ComponentSet>
            ```
        * `<Component>`
            * Required block that outlines the interfaces of the module
            * ``` XML
                <Component name="Application">
                    <RequiredInterfaceSet>
                        <!-- See below -->
                    </RequiredInterfaceSet>
                    <Implementation>
                        <!-- See below -->
                    </Implementation>
                </Component>
                ```
            * `<RequiredInterfaceSet>`
                * Required block that contains all the assets used by other modules or software
                *   ``` XML
                    <RequiredInterfaceSet>
                        <Interface name="A" shortDescription="B" type="C">
                            <!-- See below -->
                        </Interface>
                        <Interface name="D" shortDescription="E" type="F">
                            <!-- See below -->
                        </Interface>

                        <!-- Any additional interfaces -->
                    </RequiredInterfaceSet>
                    ```
                * `<Interface>`
                    * Required block that defines a specific interface. Interfaces are tables, a generic command interface, a Send HK command interface, HK telemetry interface, and any additional diagnostic packets (these are typically packets issed by command)
                        * Each table file that the app uses is its own interface. However many tables the app uses, there should be that many interfaces
                            * Example: HS has the 3 typical interfaces (CMD, HK_TLM, and SEND_HK), a wake up interface, plus 4 different tables. Therefore, there should be 8 total interfaces, 4 for CFE_SB and 4 for CFE_TBL
                    *   ``` XML
                        <Interface name="[InterfaceName]" shortDescription="[InterfaceDescription]" type="[InterfaceType]">
                            <GenericTypeMapSet>
                                <GenericTypeMap name="[GenericTypeMapName]" type="[GenericTypeMapType]"/>
                            </GenericTypeMapSet>
                        </Interface>
                        ```
                        *   | Value                  | Description |
                            | ---------------------- | ----------- |
                            | [InterfaceName]        | A common sense name for the interface. Tables should have the table's variable name as defined in the source code (typically found in ym_tbl.c). Diagnostic packets should have the variable name of the packet as defined in the source code (typically found in `YM_AppData`) |
                            | [InterfaceDescription] | A quick description of what the interface is for |
                            | [InterfaceType]        | `CFE_TBL/Table` for tables, `CFE_SB/Telemetry` for telemetry packets, or `CFE_SB/Telecommand` for commands |
                            | [GenericTypeMapName]   | Can be whatever you want. If you don't feel like thinking all that much use `TableDataType` for tables, `TelemetryDataType` for packets, and `TelecommandDataType` for commands |
                            | [GenericTypeMapType]   | Must be a type defined by a ContainerDataType block in the DataTypeSet block |
                        * Example:
                        ``` C
                        /* === C code in default_ym_tbldefs.h for module YM === */

                        /*
                        ** Example Table structure
                        */
                        typedef struct
                        {
                            uint16 Int1;
                            uint16 Int2;
                        } YM_ExampleTable_t;
                        ```

                        ``` C
                        /* === C code in ym_tbl.c for module YM === */

                        /** \brief Just an example table that holds
                         *         two dummy values
                            */
                        YM_ExampleTable_t ExampleTable = {1, 2};
                        ```

                        ``` XML
                        <!-- XML code in ym.xml for module YM -->

                        <DataTypeSet>
                            <!-- Other data type definitions -->

                            <ContainerDataType name="ExampleTable" shortDescription="Example ExampleTable structure">
                                <EntryList>
                                    <Entry name="Int1" type="BASE_TYPES/uint16"/>
                                    <Entry name="Int2" type="BASE_TYPES/uint16"/>
                                </EntryList>
                            </ContainerDataType>

                            <!-- Other data type definitions -->
                        </DataTypeSet>

                        <ComponentSet>
                            <Component name="Application">
                                <RequiredInterfaceSet>
                                    <!-- Other interfaces -->

                                    <Interface name="ExampleTable" shortDescription="Just an example table that holds two dummy values" type="CFE_TBL/Table">
                                        <GenericTypeMapSet>
                                            <GenericTypeMap name="TableDataType" type="ExampleTable"/>
                                        </GenericTypeMapSet>
                                    </Interface>

                                    <!-- Other interfaces -->
                                </RequiredInterfaceSet>
                        </ComponentSet>
                        ```
                        * There are 3 required `<Interface>`s
                            *   The generic command interface
                                *   ``` XML
                                    <Interface name="CMD" shortDescription="Software bus telecommand interface" type="CFE_SB/Telecommand">
                                        <GenericTypeMapSet>
                                            <GenericTypeMap name="TelecommandDataType" type="CommandBase"/>
                                        </GenericTypeMapSet>
                                    </Interface>
                                    ```
                            * The Send Housekeeping Telemetry command interface
                                *   ``` XML
                                    <Interface name="SEND_HK" shortDescription="Send telemetry command interface" type="CFE_SB/Telecommand">
                                        <GenericTypeMapSet>
                                            <GenericTypeMap name="TelecommandDataType" type="SendHkCmd"/>
                                        </GenericTypeMapSet>
                                    </Interface>
                                    ```
                            * The Housekeeping Telemetry interface
                                *   ``` XML
                                    <Interface name="HK_TLM" shortDescription="Software bus housekeeping telemetry interface" type="CFE_SB/Telemetry">
                                        <GenericTypeMapSet>
                                            <GenericTypeMap name="TelemetryDataType" type="HkTlm"/>
                                        </GenericTypeMapSet>
                                    </Interface>
                                    ```
            * `<Implementation>`
                * Assigns topic IDs to interfaces
                *   ``` XML
                    <Implementation>
                        <VariableSet>
                            <!-- See below -->
                        </VariableSet>

                        <ParameterMapSet>
                            <!-- See below -->
                        </ParameterMapSet>
                    </Implementation>
                    ```
                * `<VariableSet>`
                    * Defines the topic IDs found in [ym_topicids.h](#ym_topicidsh) for module YM
                    *   ``` XML
                        <VariableSet>
                            <Variable type="A" readOnly="true" name="B" initialValue="C"/> <!-- See below -->
                            <Variable type="D" readOnly="true" name="E" initialValue="F"/> <!-- See below -->
                            <!-- Other Variable blocks -->
                        </VariableSet>
                        ```
                    * `<Variable>`
                        * For applications, 3 `<Variable>` containers are needed: `CMD`, `SEND_HK`, `HK_TLM`. See the example for those containers
                        *   ``` XML
                            <Variable type="[VariableType]" readOnly="true" name="[VariableName]" initialValue="[VariableInitialValue]"/>
                            ```
                            *   | Value                  | Description |
                                | ---------------------- | ----------- |
                                | [VariableType]         | Typically BASE_TYPES/uint16 |
                                | [VariableName]         | Take the topic ID from [ym_topicids.h](#ym_topicidsh-applications-only), remove the module name, remove "MISSION", covert to title case |
                                | [VariableInitialValue] | Topic IDs defined in `cfe-topicids.xml` (see [Update EDS Configuration Files](#update-eds-configuration-files)) which get its values from [ym_topicids.h](#ym_topicidsh-applications-only) |
                        *   Example:
                            ``` C
                            /* === C code in ym_topicids.h for module YM === */

                            /* Command topic IDs */
                            #define YM_MISSION_CMD_TOPICID             YM_MISSION_TIDVAL(CMD)
                            #define DEFAULT_YM_MISSIONE_CMD_TOPICID    0x42

                            #define YM_MISSION_SEND_HK_TOPICID         YM_MISSION_TIDVAL(SEND_HK)
                            #define DEFAULT_YM_MISSION_SEND_HK_TOPICID 0x43

                            /* Other command topic ID definitions here! */

                            /* Telemetry topic IDs */
                            #define YM_MISSION_HK_TLM_TOPICID          YM_MISSION_TIDVAL(HK_TLM)
                            #define DEFAULT_YM_MISSION_HK_TLM_TOPICID  0x44

                            /* Other telemetry topic IDs definitions here! */
                            ```

                            ``` XML
                            <!-- XML code in ym.xml for module YM -->

                            <VariableSet>
                                <Variable type="BASE_TYPES/uint16" readOnly="true" name="CmdTopicId" initialValue="${CFE_MISSION/YM_CMD_TOPICID}"/>
                                <Variable type="BASE_TYPES/uint16" readOnly="true" name="SendHkTopicId" initialValue="${CFE_MISSION/YM_SEND_HK_TOPICID}"/>
                                <Variable type="BASE_TYPES/uint16" readOnly="true" name="HkTlmTopicId" initialValue="${CFE_MISSION/YM_HK_TLM_TOPICID}"/>
                                <!-- Other Variable blocks -->
                            </VariableSet>
                            ```
                * `<ParameterMapSet>`
                    * Assigns values from Variable blocks to the interfaces
                    *   ``` XML
                        <ParameterMapSet>
                            <!-- Other ParameterMap blocks -->
                            <ParameterMap interface="[ParameterMapInterface]" parameter="TopicId" variableRef="[ParameterMapVariableRef]"/>
                            <!-- Other ParameterMap blocks -->
                        </ParameterMapSet>
                        ```
                        *   | Value                     | Description |
                            | ------------------------- | ----------- |
                            | [ParameterMapInterface]   | The `name` field for an `<Interface>` block in the `<RequiredInterfaceSet>` block |
                            | [ParameterMapVariableRef] | The `name` field for a `<Variable>` block in the `<VariableSet>` block |
                    *   Example:
                        ``` XML
                        <ParameterMapSet>
                            <ParameterMap interface="CMD" parameter="TopicId" variableRef="CmdTopicId"/>
                            <ParameterMap interface="SEND_HK" parameter="TopicId" variableRef="SendHkTopicId"/>
                            <ParameterMap interface="HK_TLM" parameter="TopicId" variableRef="HkTlmTopicId"/>
                        </ParameterMapSet>
                        ```

## Build Integration

### 1. Configure EDS-Specific Build Options within `CMakeLists.txt` (Applications Only)
To conditionally include different source files based on whether EDS is enabled, add the following code to your `CMakeLists.txt` file:

Check if EDS is enabled and include the appropriate dispatch file

``` cmake
# ==== Context, don't add ====
set(APP_SRC_FILES
    # App source files
    )
# ==== Context, dont' add ====

if (CFE_EDS_ENABLED)
  list(APPEND APP_SRC_FILES
    fsw/src/ym_eds_dispatch.c
  )
else()
  list(APPEND APP_SRC_FILES
    fsw/src/ym_dispatch.c
  )
endif()

# ==== Context, don't add ====
add_cfe_app(ym ${APP_SRC_FILES})
# ==== Context, dont' add ====
```

### 2. Creating `arch_build.cmake` for Platform-Specific Setup (Applications and Libraries)

As part of the build integration, create an `arch_build.cmake` file to handle platform-specific build setup. This file is evaluated during the "prepare" stage and sets up prerequisites for the build, such as generating header files.

Create the file in your module directory with content like this:

``` cmake
###########################################################
#
# YM platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the CI configuration
set(YM_PLATFORM_CONFIG_FILE_LIST
  ym_internal_cfg_values.h
  ym_platform_cfg.h
  ym_msgids.h
  ym_msgid_values.h
)

generate_configfile_set(${YM_PLATFORM_CONFIG_FILE_LIST})
```

### 3. Verify `mission_build.cmake` Configuration (Applications and Libraries)

Ensure that your `mission_build.cmake` file contains the appropriate mission-level configuration files:

``` cmake
###########################################################
#
# YM mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# Add stand alone documentation
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/docs/dox_src ${MISSION_BINARY_DIR}/docs/ym-usersguide)

# The list of header files that control the YM configuration
set(YM_MISSION_CONFIG_FILE_LIST
    ym_fcncode_values.h
    ym_interface_cfg_values.h
    ym_mission_cfg.h
    ym_msgdefs.h
    ym_msg.h
    ym_msgstruct.h
    ym_perfids.h
    ym_topicid_values.h
)

generate_configfile_set(${YM_MISSION_CONFIG_FILE_LIST})
```
## Updating System Configuration Files
Several system configuration files must be updated to properly integrate your EDS-enabled module:

### 1. Update sample_defs/targets.cmake
Add your module to the list of modules being built for your target.
```
SET(cpu1_APPLIST ci_lab to_lab sch_lab ym)
```

### 2. Update cfe/cmake/sample_defs/cpu1_cfe_es_startup.scr
Add an entry to load your module during system startup
``` C
CFE_APP, ym, YM_AppMain, YM, 50, 16384, 0x0, 0;
```

### 3. Update EDS Configuration Files
- `sample_defs/eds/config.xml`

    Defines module specific configuration parameters. These are the macros you defined in [ym_interface_cfg.h](#ym_interface_cfgh-applications-and-libraries)

    Example:
    ``` C
    /* === C code in ym_interface_cfg.h for module YM === */
    #define YM_[ConfigName] [ConfigValue]
    ```

    is defined as,

    ``` XML
    <!-- XML code for module YM in config.xml -->
    <Package name="YM">
        <Define name="[ConfigName]" value="[ConfigValue]"/>
        <!-- Any other Define statements -->
    </Package>
    ```

- `sample_defs/eds/cfe-topicids.xml`

    Register both your module's command and telemetry topics in the CFE topic ID registry. These are the macros defined in [ym_topicids.h](#ym_topicidsh-applications-only)

    Example:
    ``` C
    /* === C code in ym_topicids.h for module YM === */

    #define YM_MISSION_CMD_TOPICID              YM_MISSION_TIDVAL(CMD)
    #define DEFAULT_YM_MISSION_CMD_TOPICID      0x456

    #define YM_MISSION_SEND_HK_TOPICID          YM_MISSION_TIDVAL(SEND_HK)
    #define DEFAULT_YM_MISSION_SEND_HK_TOPICID  0x789

    #define YM_MISSION_HK_TLM_TOPICID           YM_MISSION_TIDVAL(HK_TLM)
    #define DEFAULT_YM_MISSION_HK_TLM_TOPICID   0xABC

    #define YM_MISSION_DIAG_TLM_TOPICID         YM_MISSION_TIDVAL(DIAG_TLM)
    #define DEFAULT_YM_MISSION_DIAG_TLM_TOPICID 0xDEF
    ```

    ``` XML
    <!-- XML code in cfe-topicids.xml -->

    <!-- Your Module (YM) Command Topics -->
    <Define name="YM_CMD_TOPICID"       value="${CFE_MISSION/TELECOMMAND_BASE_TOPICID} + ###"/>
    <Define name="YM_SEND_HK_TOPICID"   value="${CFE_MISSION/TELECOMMAND_BASE_TOPICID} + ###"/>

    <!-- Your Module (YM) Telemetry Topics -->
    <Define name="YM_HK_TLM_TOPICID"    value="${CFE_MISSION/TELEMETRY_BASE_TOPICID} + ###"/>
    <Define name="YM_DIAG_TLM_TOPICID"  value="${CFE_MISSION/TELEMETRY_BASE_TOPICID} + ###"/>
    ```

## Testing and Validation

After integrating your EDS-enabled module, thorough testing is crucial to ensure proper operation. Follow these steps for effective validation:

### Validate EDS Integration

In order to verify that EDS was integrated properly, one must build the source code in the following combinations without issue:
    1. Unit tests enabled, EDS enabled
        *   ``` shell
            make distclean
            make SIMULATION=native ENABLE_UNIT_TESTS=true CFE_EDS_ENABLED=true prep
            make
            make test
            ```
    2. Unit tests enabled, EDS disabled
        *   ``` shell
            make distclean
            make SIMULATION=native ENABLE_UNIT_TESTS=true prep
            make
            make test
            ```
    3. Unit tests disabled, EDS enabled
        *   ``` shell
            make distclean
            make SIMULATION=native CFE_EDS_ENABLED=true prep
            make
            make test
            ```
    4. Unit tests disabled, EDS disabled
        *   ``` shell
            make distclean
            make SIMULATION=native prep
            make
            make test
            ```

### Unit Testing

1. **Command Processing Tests**
   - Verify that each command defined in your EDS XML is properly processed
   - Test boundary conditions and error handling for each command
   - Ensure command counters are properly incremented

2. **Telemetry Verification**
   - Confirm all telemetry packets are properly formatted according to EDS definitions
   - Verify that telemetry data is updated at the expected frequency
   - Test housekeeping telemetry comprehensively

### Integration Testing
1. **End-to-End Testing**
   - Send commands from ground systems using the EDS-generated command dictionary
   - Verify telemetry can be properly decoded using the EDS-generated telemetry dictionary
   - Test all nominal operations and error recovery scenarios

2. **Performance Testing**
   - Monitor CPU and memory usage to ensure they remain within budget
   - Verify timing requirements are met under load conditions

### Validation Tools
- **cFS EDS Tools**: Use built-in tools like `eds2cfgfile` for configuration validation
- **CTF (cFS Test Framework)**: If available, create automated tests using the cFS Test Framework
- **GDS (Ground Data System)**: Use a compatible ground system to verify end-to-end operations

## Troubleshooting

Common issues when integrating EDS-enabled modules and their solutions:

### Build Issues

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| Missing EDS-generated header files | Incorrect path in `mission_build.cmake` or `arch_build.cmake` | Verify file paths and ensure `generate_configfile_set()` is called with the correct file list |
| Linker errors with undefined EDS symbols | Missing EDS library linkage | Add `CFE_EDS_ENABLED_BUILD` flag and ensure proper library linking in `CMakeLists.txt` |
| `ym_eds_dispatch.c` compilation errors | Incompatible EDS definitions | Verify EDS XML syntax and ensure it matches the implementation in the dispatch file |

### Runtime Issues

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| Commands not being processed | Incorrect topic IDs | Verify topic IDs in [cfe-topicids.xml](#3-update-eds-configuration-files) match those used in the module |
| "Unknown command code" errors | Mismatch between EDS definitions and code | Ensure function codes in EDS XML match those in the implementation |
| Module fails to start | Missing or incorrect entries in startup script | Check `cfe_es_startup.scr` for correct module name and entry point |
| Telemetry not appearing | Incorrect telemetry message format | Verify EDS telemetry definitions match the structure being sent |

### EDS XML Issues

| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| EDS validation errors | Syntax or semantic errors in XML | Use the EDS validator tool: `eds_tool verify ym.xml` |
| Namespace conflicts | Duplicate definitions across modules | Ensure unique namespace usage in your EDS XML |
| Incompatible data types | Using types not supported by EDS | Review CCSDS EDS specification for supported types |

### Debugging Tips

1. **Enable Verbose Logging**
   - Add debug messages in key processing points
   - Set `CFE_EVS_DEFAULT_TYPE_FLAG` to include debug events

2. **Use Message Analyzer**
   - Capture and decode software bus messages to verify format
   - Compare against EDS definitions

3. **Check Generated Files**
   - Examine generated header files to ensure they match expectations
   - Look for discrepancies between default and EDS-generated versions

4. **Common Resolution Steps**
   - Rebuild the entire system with `make dist clean`
   - Verify EDS XML file is properly included in the build
   - Ensure module is properly registering for message reception
