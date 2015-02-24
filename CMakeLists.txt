if (NOT REPORTBUILDER_BUILD_MODE)
    set (REPORTBUILDER_BUILD_MODE STATIC)
endif ()

include(pile_support)
pileInclude (ReportBuilder)
reportbuilderInit(${REPORTBUILDER_BUILD_MODE})
