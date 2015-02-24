
# enable/disable cmake debug messages related to this pile
set (REPORTBUILDER_DEBUG_MSG ON)

# make sure support code is present; no harm
# in including it twice; the user, however, should have used
# pileInclude() from pile_support.cmake module.
include(pile_support)

# initialize this module
macro    (reportbuilderInit
          ref_cnt_use_mode)

    # default name
    if (NOT REPORTBUILDER_INIT_NAME)
        set(REPORTBUILDER_INIT_NAME "ReportBuilder")
    endif ()

    # compose the list of headers and sources
    set(REPORTBUILDER_HEADERS
        "reportbuilder.h")
    set(REPORTBUILDER_SOURCES
        "reportbuilder.cc")
    set(REPORTBUILDER_QT_MODS
        "Core")

    pileSetSources(
        "${REPORTBUILDER_INIT_NAME}"
        "${REPORTBUILDER_HEADERS}"
        "${REPORTBUILDER_SOURCES}")

    pileSetCommon(
        "${REPORTBUILDER_INIT_NAME}"
        "0;0;1;d"
        "ON"
        "${ref_cnt_use_mode}"
        ""
        "category1"
        "tag1;tag2")

endmacro ()
