/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license restrictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

#ifndef ZSRemCmd_Parser_h
#define ZSRemCmd_Parser_h

#include <QtCore/qobject.h>

#include "ZSRemCmd/ZSRemCmdDllMain.h"
#include "ZSSys/ZSSysErrResult.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace RemCmd
{
//******************************************************************************
class ZSREMCMDDLL_API CRemCmdParser : public QObject
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::RemCmd"; }
    static QString ClassName() { return "CRemCmdParser"; }
public: // class methods
    static ZS::System::SErrResultInfo Parse(
        const QByteArray& i_byteArrData,
        QString*          o_pstrCmd,
        bool*             o_pbIsQuery,
        QByteArray*       o_pByteArrArgs,
        char              i_chCmdSeparator,
        char              i_chArgsSeparator );
    static QByteArray RemoveLeadingAndTrailingWhitespaces( const QByteArray& i_byteArrArg );
    static QList<QByteArray> RemoveLeadingAndTrailingWhitespaces( const QList<QByteArray>& i_arByteArrArgs );
    static QByteArray NormalizeStringArg( const QByteArray& i_byteArrArg );
    static QList<QByteArray> NormalizeStringArg( const QList<QByteArray>& i_arByteArrArgs );
public: // ctors and dtor
    CRemCmdParser(
        const QString& i_strObjName,
        char           i_chCmdSeparator = ' ',
        char           i_chArgsSeparator = ',' );
    virtual ~CRemCmdParser();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    void setCmdSeparator( char i_chCmdSeparator );
    char getCmdSeparator() const { return m_chCmdSeparator; }
    void setArgsSeparator( char i_chArgsSeparator );
    char getArgsSeparator() const { return m_chArgsSeparator; }
public: // instance methods
    ZS::System::SErrResultInfo parse( const QByteArray& i_byteArrData, QString* o_pstrCmd, bool* o_pbIsQuery, QByteArray* o_pByteArrArgs ) const;
public: // instance methods
    QList<QByteArray> splitArgs(
        const QByteArray& i_byteArrArgs,
        bool              i_bRemoveLeadingAndTrailingWhiteSpaces = true,
        bool              i_bNormalizeStringArgs = true ) const;    // Removes quotation marks
protected: // instance members
    char                 m_chCmdSeparator;
    char                 m_chArgsSeparator;
    Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // class CRemCmdParser

} // namespace RemCmd

} // namespace ZS

#endif // #ifndef ZSRemCmd_Parser_h
