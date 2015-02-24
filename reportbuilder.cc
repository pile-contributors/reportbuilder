/**
 * @file reportbuilder.cc
 * @brief Definitions for ReportBuilder class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "reportbuilder.h"
#include "reportbuilder-private.h"

/**
 * @class ReportBuilder
 *
 * Detailed description.
 */


#if DEBUG_OFF
#define RBLD_DEBUG DBG_PMESSAGE
#else
#define RBLD_DEBUG black_hole
#endif

#if DEBUG_OFF
#define RBLD_TRACE_ENTRY DBG_TRACE_ENTRY
#else
#define RBLD_TRACE_ENTRY
#endif

#if DEBUG_OFF
#define RBLD_TRACE_EXIT DBG_TRACE_EXIT
#else
#define RBLD_TRACE_EXIT
#endif

#ifdef _MSC_VER
#   define __func__ __FUNCTION__
#endif

/**
 * @def STRINGIFY
 * @brief Convert symbol to string at compile time.
 * @internal
 */
#ifndef STRINGIFY
#   define STRINGIFY(s) STRINGIFY_HELPER(s)
#   define STRINGIFY_HELPER(s) #s
#endif


/**
 * @def DBG_ASSERT
 * @brief Only in debug builds - check a condition.
 * @internal
 */
#ifdef REPORTBUILDER_DEBUG
#  define DBG_ASSERT(a) if (!(a)) {\
        printf("\nDBG_ASSERT Failed: " STRINGIFY(a) "\n"); \
        printf("%s[%d]: %s\n\n",__FILE__, __LINE__, __func__); \
        }
#else
#  define DBG_ASSERT(a)
#endif


/* ------------------------------------------------------------------------- */
ReportBuilder::ReportBuilder (const QString & s_file) :
    f_out_(s_file),
    out_(NULL),
    foreground_(),
    background_(),
    font_name_(),
    font_size_(),
    table_buffer(),
    table_cols_(0),
    flags_(F_NONE),
    b_status_(true),
    s_stream_error_()
{
    RBLD_TRACE_ENTRY;
    if (f_out_.open (QIODevice::WriteOnly | QIODevice::Text)) {
        out_ = new QTextStream (&f_out_);
        b_status_ = f_out_.isOpen () &&
                f_out_.isWritable () &&
                out_->status () == QTextStream::Ok;
    } else {
        b_status_ = false;
        s_stream_error_ = QString ("Failed to open file; %1")
                .arg (f_out_.errorString ());
    }

    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
ReportBuilder::~ReportBuilder ()
{
    RBLD_TRACE_ENTRY;
    if (out_ != NULL) {
        out_->flush ();
        delete out_;
        out_ = NULL;
    }
    if (f_out_.isOpen ()) {
        f_out_.close ();
    }
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::start()
{
    if (isFlag (F_STARTED)) return;
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;

    flags_ = F_STARTED;
    (*out_) << "<!DOCTYPE html>" << endl
            << "<html lang=\"en-US\">" << endl
            << "<head>" << endl;
    if (!s_title_.isEmpty ()) {
        (*out_) << "<title>" << s_title_ << "</title>" << endl;
    }
    (*out_) << "<style>" << endl
            << " table {border-collapse: collapse;}" << endl
            << " td,th {padding: 0.5em; border: 1px solid #ccc;}" << endl
            << " thead {background-color: #ccc;}" << endl
            << " tfoot {background-color: #ccc;}" << endl
            << " h1 {color: #4F3259; font-size: 1.40em; text-align: center;}" << endl
            << " h2 {color: #3C3454; font-size: 1.35em;}" << endl
            << " h3 {color: #353057; font-size: 1.30em;}" << endl
            << " h4 {color: #2A3257; font-size: 1.25em;}" << endl
            << " h5 {color: #202B59; font-size: 1.20em;}" << endl
            << " h6 {color: #153359; font-size: 1.15em;}" << endl
            << " h7 {color: #052E63; font-size: 1.10em;}" << endl
            << "</style>" << endl
            << "</head>" << endl
            << "<body>" << endl;
    if (!s_title_.isEmpty ()) {
        (*out_) << "<h1>" << s_title_ << "</h1>" << endl;
    }
    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::end()
{
    if (!b_status_) return;
    if (!isFlag (F_STARTED)) return;
    RBLD_TRACE_ENTRY;

    (*out_) << "</body>" << endl
            << "</html>" << endl;
    out_->flush ();

    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::addHLine (int size, quint32 background)
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    start ();
    if (isFlag (F_CODE)) {
        if (size == 1)
            (*out_) << " ---------------------------------------- \n";
        else if (size == 2)
            (*out_) << " ======================================== \n";
        else
            (*out_) << " ######################################## \n";
    } else {
        (*out_) << QString("<hr style=\"height: %1px; border: 0; "
                           "border-top: %2px solid #%3;\"/>\n")
                   .arg (size).arg (size).arg (background, 1, 16);
    }

    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::addEmptyLine ()
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    start ();

    (*out_) << "<br/>" << endl;

    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::addTitle (const QString & s_value, int level)
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    start ();

    if (level < 1) level = 7;
    if (level > 7) level = 7;

    (*out_) << "<h" << level << ">"
            << s_value
            << "</h" << level << ">"
            << endl;

    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::addLine (const QString &s_value)
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    start ();

    if (isFlag (F_CODE)) {
        (*out_) << s_value << endl;
    } else {
        (*out_) << s_value
                << "</br>"
                << endl;
    }
    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::add (const QString &s_value)
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    start ();

    if (isFlag (F_CODE)) {
        (*out_) << s_value << endl;
    } else {
        (*out_) << "<p>"
                << s_value
                << "</p>"
                << endl;
    }

    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::addHtml(const QString &s_value)
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    start ();

    (*out_) << s_value;

    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::beginCode()
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    start ();

    (*out_) << "<pre>" << endl;

    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::endCode()
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    DBG_ASSERT (isFlag (F_STARTED));

    (*out_) << "</pre>" << endl;

    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::beginInsert (
        const QString &s_label, quint32 background,
        quint32 foreground, int border_size)
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    start ();

    QString s_back;
    s_back = QString("background-color: #%1;").arg (background, 1, 16);

    QString s_fore;
    s_fore = QString("color: #%1;").arg (foreground, 1, 16);

    QString s_border;
    if (border_size > 0) {
        s_border = QString("border: %1px solid #%2;")
                .arg (border_size)
                .arg (foreground, 1, 16);
    } else {
        s_border = "";
    }

    (*out_) << "<div style=\""
            << s_back << " "
            << s_fore << " "
            << s_fore << "\">"
            << endl;;

    if (!s_label.isEmpty ()) {
        (*out_) << "<p style=\"font-weight: bold;\">&nbsp;&nbsp;&nbsp;&nbsp;"
                << s_label
                << "</p>" << endl;
    }

    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::endInsert()
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    DBG_ASSERT (isFlag (F_STARTED));

    (*out_) << "</div>" << endl;

    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::beginTable (
        int rows, int columns, bool with_header, bool with_footer)
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    start ();
    table_buffer.clear ();

    if (rows <= 0) {
        DBG_ASSERT(rows > 0);
        rows = 1;
    }

    if (columns <= 0) {
        DBG_ASSERT(columns > 0);
        columns = 1;
    }
    table_cols_ = columns;

    table_buffer.reserve (rows);
    for (int i = 0; i < rows; ++i) {
        QList<QString> l;
        l.reserve (columns);
        table_buffer.append (l);
    }

    setFlag (F_TABLE);
    if (with_header) setFlag (F_TABLE_HEADER);
    if (with_footer) setFlag (F_TABLE_FOOTER);

    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::setTableCell (
        int row, int column, const QString &s_value)
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    DBG_ASSERT (isFlag (F_STARTED));

    // make sure the size is big enough
    if (!isFlag (F_TABLE)) {
        beginTable (row+1, column+1, false);
    }

    QString s;
    while (table_buffer.count () <= row) {
        QList<QString> l; l.append (s);
        table_buffer.append (l);
    }
    QList<QString> & l_row = table_buffer[row];
    while (l_row.count () <= column) {
        l_row.append (s);
    }
    QString & cell = l_row[column];

    cell = s_value;
    if (table_cols_ < column+1) table_cols_ = column+1;

    checkStream ();
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::writeRow (
        const QList<QString> & l_row,
        const QString & s_col_header, const QString & s_col_footer)
{
    for (int c = 0; c < table_cols_; ++c) {
        (*out_) << s_col_header << endl;
        if (c < l_row.count ()) {
            (*out_) << l_row.at (c);
        }
        (*out_) << s_col_footer << endl;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::endTable ()
{
    if (!b_status_) return;
    RBLD_TRACE_ENTRY;
    DBG_ASSERT (isFlag (F_STARTED));
    DBG_ASSERT (isFlag (F_TABLE));

    //(*out_) << "<table width=\"100%\">" << endl;
    (*out_) << "<table border=\"1\" cellpadding=\"0\" cellspacing=\"0\" style=\"margin: 0px auto;\">" << endl;

    for (;;) {

        QString s_row_header;
        QString s_row_footer;
        QString s_col_header;
        QString s_col_footer;

        isFlag (F_TABLE_FOOTER);
        int tot_rows = table_buffer.count ();
        int r = 0;

        if (tot_rows == 0 || table_cols_ == 0) break;

        if (isFlag (F_TABLE_HEADER)) {
            s_row_header = "<thead><tr>";
            s_row_footer = "</tr></thead>";
            s_col_header = "<th>";
            s_col_footer = "</th>";
            (*out_) << s_row_header << endl;
            writeRow (table_buffer[r], s_col_header, s_col_footer);
            (*out_) << s_row_footer << endl;
            ++r;
            if (r >= tot_rows) break;
        }

        s_col_header = "<td>";
        s_col_footer = "</td>";

        if (isFlag (F_TABLE_FOOTER)) {
            s_row_header = "<tfoot><tr>";
            s_row_footer = "</tr></tfoot>";
            (*out_) << s_row_header << endl;
            writeRow (table_buffer[tot_rows-1], s_col_header, s_col_footer);
            (*out_) << s_row_footer << endl;
            --tot_rows;
            if (r >= tot_rows) break;
        }

        (*out_) << "<tbody>" << endl;
        s_row_header = "<tr>";
        s_row_footer = "</tr>";

        for (; r < tot_rows; ++r) {
            (*out_) << s_row_header << endl;
            writeRow (table_buffer[r], s_col_header, s_col_footer);
            (*out_) << s_row_footer << endl;
        }
        (*out_) << "</tbody>" << endl;

        break;
    }
    (*out_) << "</table>" << endl;

    checkStream ();
    resetFlag (F_TABLE_ALL);
    RBLD_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ReportBuilder::checkStream()
{
    if (!b_status_) return;
    b_status_ = out_->status () == QTextStream::Ok;
    if (!b_status_) {
        s_stream_error_ = "Write has failed";
    }
}
/* ========================================================================= */
