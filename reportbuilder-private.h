/**
 * @file reportbuilder-private.h
 * @brief Declarations for ReportBuilder class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_REPORTBUILDER_PRIVATE_H_INCLUDE
#define GUARD_REPORTBUILDER_PRIVATE_H_INCLUDE

#include <reportbuilder/reportbuilder-config.h>

#if 0
#    define REPORTBUILDER_DEBUGM printf
#else
#    define REPORTBUILDER_DEBUGM black_hole
#endif

#if 0
#    define REPORTBUILDER_TRACE_ENTRY printf("REPORTBUILDER ENTRY %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define REPORTBUILDER_TRACE_ENTRY
#endif

#if 0
#    define REPORTBUILDER_TRACE_EXIT printf("REPORTBUILDER EXIT %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define REPORTBUILDER_TRACE_EXIT
#endif


static inline void black_hole (...)
{}

#endif // GUARD_REPORTBUILDER_PRIVATE_H_INCLUDE
