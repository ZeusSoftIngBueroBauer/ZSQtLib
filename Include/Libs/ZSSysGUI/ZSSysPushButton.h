/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSSysGUI_PushButton_h
#define ZSSysGUI_PushButton_h

#include <QtCore/qstring.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"

class QLabel;

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CPushButton : public QPushButton
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CPushButton( QWidget* i_pWdgtParent = nullptr );
    CPushButton( const QString& i_strText, QWidget* i_pWdgtParent = nullptr );
    virtual ~CPushButton();
public: // instance methods
    void setText( const QString& i_strText );
    QString text() const;
    void setPixmap( const QPixmap& i_pxm );
    QPixmap pixmap() const;
    void setScaledContents ( bool i_bScaled );
    bool hasScaledContents () const;
public: // overridables of base class QWidget
    virtual QSize sizeHint() const;
protected: // instance members
    QLabel* m_pLabel;

}; // class CPushButton

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_PushButton_h
