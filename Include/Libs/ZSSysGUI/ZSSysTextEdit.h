/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSSysGUI_TextEdit_h
#define ZSSysGUI_TextEdit_h

#include <QtCore/qstring.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
/*! @brief Reimplements QTextEdit to workaround the problem, that sometimes,
           after resizing the parent, the text edit has a strange size.

    This is accomplished by explicitly setting a preferred width and height
    at the text edit during runtime.
*/
class ZSSYSGUIDLL_API CTextEdit : public QTextEdit
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal::GUI"; }
    static QString ClassName() { return "CTextEdit"; }
public: // ctors and dtor
    CTextEdit( QWidget* i_pWdgtParent = nullptr );
    CTextEdit( const QString& i_strText, QWidget* i_pWdgtParent = nullptr );
    virtual ~CTextEdit();
public: // instance methods
    void setPreferredWidth(int i_cxWidth);
    int preferredWidth() const;
    void setPreferredHeight(int i_cyHeight);
    int preferredHeight() const;
public: // overridables of base class QTextEdit
    virtual QSize sizeHint () const override;
protected: // overridables of base class QTextEdit
    virtual void resizeEvent(QResizeEvent* i_pEv) override;
protected: // instance members
    int m_cxPreferredWidth;
    int m_cyPreferredHeight;

}; // class CTextEdit

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_TextEdit_h
