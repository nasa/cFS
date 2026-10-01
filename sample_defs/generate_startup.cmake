function (generate_cfs_startup_script CFS_INSTALL_DIR)
    set (STARTUP_FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/${CFS_INSTALL_DIR}/cfe_es_startup.scr")

    # These apps should always be present
    # Startup script fields:
    # 1. Object Type      -- CFE_APP for an Application, or CFE_LIB for a library.
    # 2. Path/Filename    -- This is a cFE Virtual filename, not a vxWorks device/pathname
    # 3. Entry Point      -- This is the "main" function for Apps.
    # 4. CFE Name         -- The cFE name for the APP or Library
    # 5. Priority         -- This is the Priority of the App, not used for Library
    # 6. Stack Size       -- This is the Stack size for the App, not used for the Library
    # 7. Load Address     -- This is the Optional Load Address for the App or Library. Currently
    #                        not implemented so keep it at 0x0.
    # 8. Exception Action -- This is the Action the cFE should take if the App has an exception.
    #                        0        = Just restart the Application
    #                        Non-Zero = Do a cFE Processor Reset
    file (WRITE ${STARTUP_FILE}
        "CFE_LIB, cfe_assert,  CFE_Assert_LibInit, ASSERT_LIB,    0,   0,     0x0, 0;\n"
        "CFE_LIB, sample_lib,  SAMPLE_LIB_Init,    SAMPLE_LIB,    0,   0,     0x0, 0;\n"
        "CFE_APP, sch_lab,     SCH_LAB_AppMain,    SCH_LAB,      35,   32768, 0x0, 0;\n"
        "CFE_APP, ci_lab,      CI_LAB_AppMain,     CI_LAB,       40,   32768, 0x0, 0;\n"
        "CFE_APP, to_lab,      TO_LAB_AppMain,     TO_LAB,       45,   32768, 0x0, 0;\n"
        "CFE_APP, sample_app,  SAMPLE_APP_Main,    SAMPLE_APP,   50,   32768, 0x0, 0;\n"
    )

    # the rest of the apps can vary by config
    set(CFS_APP_STARTUP_ORDER lc cf ds fm hk hs mm sc md cs sbn)
    foreach(APP ${CFS_APP_STARTUP_ORDER})
        list(FIND ARGN ${APP} SHOULD_START)
        if (SHOULD_START GREATER -1)
            string(TOUPPER "${APP}" APP_UPPER)
            file (APPEND ${STARTUP_FILE}
                "CFE_APP, ${APP},  ${APP_UPPER}_AppMain, ${APP_UPPER}, 70,  131072, 0x0, 0;\n"
            )
        endif()
    endforeach()

endfunction(generate_cfs_startup_script)
