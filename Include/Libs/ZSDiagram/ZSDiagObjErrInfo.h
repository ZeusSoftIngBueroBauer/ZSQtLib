/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDiagObjErrInfo_h
#define ZSDiagObjErrInfo_h

#include "ZSDiagram/ZSDiagramDllMain.h"
#include "ZSDiagram/ZSDiagObj.h"

class QTimer;

namespace ZS
{
namespace Diagram
{
//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjErrInfo : public CDiagObj
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CDiagObjErrInfo(
        const QString& i_strObjName,
        ELayoutPos     i_layoutPos );
    virtual ~CDiagObjErrInfo();
public: // instance methods
    void setErrResultInfo( const ZS::System::SErrResultInfo& i_errResultInfo );
    ZS::System::SErrResultInfo getErrResultInfo() const;
    void setTimeOut( ZS::System::EResultSeverity i_severity, int i_iTimeout_ms );
    int getTimeOut( ZS::System::EResultSeverity i_severity ) const;
    void setColText( ZS::System::EResultSeverity i_severity, const QColor& i_col );
    QColor getColText(ZS::System::EResultSeverity i_severity ) const;
    void setFont( ZS::System::EResultSeverity i_severity, const QFont& i_fnt );
    QFont getFont( ZS::System::EResultSeverity i_severity ) const;
    void setMarginTop( int i_iMargin );
    int getMarginTop() const;
    void setMarginBottom( int i_iMargin );
    int getMarginBottom() const;
    void setMarginLeft( int i_iMargin );
    int getMarginLeft() const;
    void setMarginRight( int i_iMargin );
    int getMarginRight() const;
    void setTextDirection( ETextDirection i_textDirection );
    ETextDirection getTextDirection() const;
    QString getText() const;
public: // overridables of base class CDiagObj
    virtual QSize sizeHint();
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr );
protected: // overridables of base class CDiagObj
    virtual void onReceivedError( ZS::System::CMsgErr* i_pMsgErr );
protected slots:
    virtual void timeout();
private: // copy ctor not allowed
    CDiagObjErrInfo( const CDiagObjErrInfo& );
private: // assignment operator not allowed
    void operator=( const CDiagObjErrInfo& );
protected:  // instance members
    ZS::System::SErrResultInfo m_errResultInfoCurr;
    ZS::System::SErrResultInfo m_errResultInfoPrev;
    QTimer*            m_pTimer;
    int                m_ariTimeout_ms[ZS::System::EResultSeverityCount];
    QColor             m_arcolText[ZS::System::EResultSeverityCount];
    QFont              m_arfntText[ZS::System::EResultSeverityCount];
    int                m_iMarginTop;
    int                m_iMarginBottom;
    int                m_iMarginLeft;
    int                m_iMarginRight;
    ETextDirection     m_textDirection;
    QString            m_strText;
    QRect              m_rectTextCurr;
    QRect              m_rectTextPrev;

}; // class CDiagObjErrInfo

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjErrInfo_h
