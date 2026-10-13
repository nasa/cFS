# Integration Guide

This document is for a project on an older version of cFS who wants to step up to this version. To use this guide, scroll down until you reach the first version that you do not have. In that section and each one above it, each entry represents an API change or other modification that may require updates in your project bundle.

Note that this document does *not* list all changes to cFS, only those that are expected to have a noticeable impact to existing code such as an API or behavioral changes. There is no guarantee that this document captures all changes that may affect the mission code, and full verification and validation is expected when updating versions.

## Format

- Description: What changed?
- Affects: Who is impacted by this change?
- Required modification: What do you need to change in your bundle?
- Common repo(s) affected: Where was the cFS change made?
- Commit: Which commit changed the code that required this?

## Versions

Versions here are listed from newest (at the top) to oldest (at the bottom). Start from your version and work your way to the top.

### Fornax

LAB apps table name change
 - The default CFE startup script now starts the LAB apps as `CI_LAB`, `TO_LAB`, and `SCH_LAB`, as opposed
   to `CI_LAB_APP`, `TO_LAB_APP`, and `SCH_LAB_APP`, respectively.
 - Additionally, the table names in the TO_LAB and SCH_LAB apps have been simplified.  CI_LAB does not have a table.
 - When combined with the application name, the "fully-qualified" table name now becomes:
    - For TO_LAB:  `TO_LAB.Subscriptions` instead of `TO_LAB_APP.TO_LAB_Subs`
    - For SCH_LAB: `SCH_LAB.Schedule` instead of `SCH_LAB_APP.ScheduleTable`
 - Affects: Those who use TO_LAB and SCH_LAB and have modified the table(s) by overriding the table file
 - Required modification: Change the table name in the `CFE_TBL_FILEDEF` macro in the overridden table file as above.  
   The fully-qualified table runtime name is the second parameter to the macro.
 - Common repo(s) affected: cFE, TO_LAB, SCH_LAB
 - cFE Commit: df6f7240b0e6265fdcb2ab9a182934c6baa7f5e8
 - TO_LAB Commit: 4121af6f1fa571325849ce809ae880635597b6cf
 - SCH_LAB Commit: 701d4600d3ef8b9011418e3f5e6a2311ea9fa3c0

CFE_SB_SUB_ENTRIES_PER_PKT renamed
 - Description: A macro `CFE_SB_SUB_ENTRIES_PER_PKT` had been incorrectly defined in cfe_sb_extern_typedefs.h.  This has been 
   moved to the CFE SB interface (mission-scope) configuration and is now called `CFE_MISSION_SB_SUB_ENTRIES_PER_PKT` in 
   accordance with the patterns used for all other interface configuration options
 - Affects: Those who use the `CFE_SB_SUB_ENTRIES_PER_PKT` macro in their mission code
 - Required modification: Rename any references from `CFE_SB_SUB_ENTRIES_PER_PKT` to `CFE_MISSION_SB_SUB_ENTRIES_PER_PKT`
 - Note this is related to internal telemetry from SB, which was only intended to be consumed by SBN.  It is not expected that this will affect any other external/user applications as this was not part of the TLM set normally published by SB.
 - Common repo(s) affected: cFE
 - Commit: b014007afae0d94b795daff16ca772e60f1c72d3

CFE Table Services handle typedef update
 - Description: The `CFE_TBL_Handle_t` typedef is changed from a simple int16 to an opaque 32-bit value derived from CFE_ResourceId_t
 - Affects: Those who perform operations on `CFE_TBL_Handle_t`
 - This change is only exposed by switching OMIT_DEPRECATED to ON.  If OMIT_DEPRECATED is OFF, then a 16 bit integer 
   type is used instead, which is backward compatible
 - Required modification: With the transition to the opaque 32 bit value, applications must transition to using the 
   inline functions for checking or comparing the values:
    - Use `CFE_TBL_HandleID_IsEqual()` instead of a numeric comparison using `==`
    - Use `CFE_TBL_HandleID_AsInt()` for use with logging/events or any other printf-style calls
    - Use `CFE_TBL_HandleID_IsDefined()` to check if a table handle is valid or not
 - Commit repo(s) affected: cFE
 - Commit: d16f1412f94e0e24b53e4f21b89e57efb80fff66

RTS Seconds to Wakeups
- Description: Relative Time Sequence (RTS) file times now represent the number of wakeups, rather than seconds
- Affects: Those who use the Stored Command RTS capability with a Scheduler wakeup different from 1 Hz
- Required modification: Regenerate all RTS files based on the wakeup frequency, rather than time in seconds
- Common repo(s) affected: SC
- Commit: 90ddac7c379690fdad05dee05368bdec70ae2912

