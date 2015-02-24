/**
 * @file reportbuilder.h
 * @brief Declarations for ReportBuilder class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_REPORTBUILDER_H_INCLUDE
#define GUARD_REPORTBUILDER_H_INCLUDE

#include <reportbuilder/reportbuilder-config.h>
#include <QFile>
#include <QTextStream>
#include <QList>

//! A report builder
class REPORTBUILDER_EXPORT ReportBuilder {
    //
    //
    //
    //
    /*  DEFINITIONS    ----------------------------------------------------- */

public:

    enum Flags {
        F_NONE          = 0x00000000,
        F_STARTED       = 0x00000001,

        F_INSERT        = 0x01000000,
        F_CODE          = 0x02000000,

        F_TABLE         = 0x00010000,
        F_TABLE_HEADER  = 0x00020000,
        F_TABLE_FOOTER  = 0x00040000,
        F_TABLE_ALL = F_TABLE | F_TABLE_HEADER | F_TABLE_FOOTER

    };


    /*  DEFINITIONS    ===================================================== */
    //
    //
    //
    //
    /*  DATA    ------------------------------------------------------------ */

private:

    QString s_title_;
    QFile f_out_;
    QTextStream * out_;

    QList<quint32> foreground_;
    QList<quint32> background_;
    QList<QString> font_name_;
    QList<int> font_size_;

    QList< QList<QString> > table_buffer;
    int table_cols_;

    int flags_;

    bool b_status_;
    QString s_stream_error_;

    /*  DATA    ============================================================ */
    //
    //
    //
    //
    /*  FUNCTIONS    ------------------------------------------------------- */


public:

    //! constructor
    explicit ReportBuilder (
            const QString & s_file);

    //! destructor
    virtual
    ~ReportBuilder ();

    //! write the header if not already there
    void
    setDocumentTitle (
            const QString & s_title) {
        s_title_ = s_title;
    }

    //! The last error.
    const QString &
    errorString () {
        return s_stream_error_;
    }

    //! Everything all right up until this point?
    bool
    isOK () {
        return b_status_;
    }

    //! write the header if not already there
    void
    start ();

    //! write the footer if not already there
    void
    end ();


    void
    addHLine (
            int size,
            quint32 background = 0);

    void
    addEmptyLine ();

    void
    addTitle (
            const QString & s_value,
            int level);

    void
    addLine (
            const QString & s_value);

    void
    add (
            const QString & s_value);

    void
    addHtml (
            const QString & s_value);

    void
    beginCode ();

    void
    endCode ();

    void
    beginInsert (
            const QString & s_label = QString(),
            quint32 background = 0,
            quint32 foreground = 0,
            int border_size = 1);

    void
    endInsert ();

    void
    beginTable (
            int rows,
            int columns,
            bool with_header = false,
            bool with_footer = false);

    void
    setTableCell (
            int rows,
            int columns,
            const QString & s_value);

    void
    endTable ();

private:

    void
    setFlag (
            int flg) {
        flags_ = flags_ | flg;
    }

    void
    resetFlag (
            int flg) {
        flags_ = flags_ & (~flg);
    }

    bool
    isFlag (
            int flg) {
        return (flags_ & flg) == flg;
    }

    void
    checkStream ();


    void
    writeRow (
            const QList<QString> & l_row,
            const QString & s_col_header,
            const QString & s_col_footer);

}; // class ReportBuilder





#endif // GUARD_REPORTBUILDER_H_INCLUDE
