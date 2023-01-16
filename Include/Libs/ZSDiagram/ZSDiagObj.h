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

#ifndef ZSDiagObj_h
#define ZSDiagObj_h

#include "ZSDiagram/ZSDiagramAux.h"

#include <QtCore/qrect.h>

class QPaintDevice;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}
namespace PhysVal
{
class CUnit;
}
namespace Diagram
{
class CDiagScale;
class CDiagTrace;

//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjMoveEvent
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Diagram"; }
    static QString ClassName() { return "CDiagObjMoveEvent"; }
public: // type definitions and constants
    typedef enum {
        ETypeMin = 0,
        ETypePointAbs = 0, // absolute to given X/Y coordinates
        ETypePointRel = 1, // relative with given X/Y coordinates
        ETypeResRel   = 2, // relative with whole number multiple of scale resolution
        ETypeValAbs   = 3, // absolute to given value
        ETypeValRel   = 4, // relative with given value
        ETypeMax      = 4,
        ETypeCount,
        ETypeUndefined
    }   EType;
public: // ctors and dtor
    CDiagObjMoveEvent();
    CDiagObjMoveEvent( EType i_type, const QPoint& i_pt ); // for ETypePointAbs or ETypePointRel
    CDiagObjMoveEvent( EType i_type, double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ); // for ETypeResRel or ETypeValAbs or ETypeValRel
    ~CDiagObjMoveEvent();
public: // instance methods
    void setType( EType i_type );
    EType type() const;
    void setPoint( const QPoint& i_pt );
    QPoint point() const;
    void setVal( double i_fVal, PhysVal::CUnit* i_pUnit = nullptr );
    double val( PhysVal::CUnit* i_pUnit = nullptr ) const;
protected: // instance members
    EType           m_type;
    QPoint          m_pt;
    double          m_fVal;
    PhysVal::CUnit* m_pUnit;

}; // class CDiagObjMoveEvent


//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObj : public QObject
//******************************************************************************
{
friend class CDataDiagram;
friend class CPixmapDiagram;
friend class CWdgtDiagram;
public: // class methods
    static QString NameSpace() { return "ZS::Diagram"; }
    static QString ClassName() { return "CDiagObj"; }
    Q_OBJECT
protected: // ctors
    CDiagObj(
        const QString& i_strObjName,
        CDiagTrace*    i_pDiagTrace,
        ELayoutPos     i_layoutPos );
    CDiagObj(
        const QString& i_strObjName,
        CDiagScale*    i_pDiagScaleX,
        CDiagScale*    i_pDiagScaleY,
        ELayoutPos     i_layoutPos );
public: // dtor
    virtual ~CDiagObj();
signals:
    // Please note that the following signals may only be thrown by the
    // derived classed after updating its internal data.
    void valueChanged( ZS::Diagram::CDiagObj* );
    void valueChanged( EScaleDir, ZS::Diagram::CDiagObj* );
    void valueXChanged( ZS::Diagram::CDiagObj* );
    void valueYChanged( ZS::Diagram::CDiagObj* );
    void visibilityChanged( ZS::Diagram::CDiagObj* );
public: // instance methods
    void setObjName( const QString& i_strObjName );
    QString getObjName() const;
    int getObjId() const;
    CDataDiagram* getDiagram();
    CDiagScale* getDiagScale( EScaleDir i_scaleDir );
    CDiagTrace* getDiagTrace();
    ELayoutPos getLayoutPos() const;
public: // overridables
    virtual void setDiagTrace( CDiagTrace* i_pDiagTrace );
    virtual void setAdjustContentRect2DiagPartCenter( bool i_bAdjust );
    virtual bool getAdjustContentRect2DiagPartCenter() const;
    virtual void show( bool i_bInformDiagram = true );
    virtual void hide( bool i_bInformDiagram = true );
    virtual bool isVisible() const;
    virtual double getValRes( EScaleDir i_scaleDir, PhysVal::CUnit* i_pUnit = nullptr ) const;
    virtual double getValRes( EScaleDir i_scaleDir, double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ) const;
    virtual PhysVal::CPhysVal getVal( EScaleDir i_scaleDir ) const;
    virtual void setIsFocusable( bool i_bFocusable );
    virtual bool isFocusable() const;
    virtual bool isFocused() const;
    virtual void setFocus( bool i_bInformDiagram = true );
    virtual void setFocus( const QPoint& i_pt, bool i_bInformDiagram = true );
    virtual void unsetFocus( bool i_bInformDiagram = true );
    virtual void setIsEditable( bool i_bEditable );
    virtual bool isEditable() const;
    virtual bool isEditing() const;
    virtual void startEditSession( bool i_bInformDiagram = true );
    virtual void startEditSession( const QPoint& i_pt, bool i_bInformDiagram = true );
    virtual void stopEditSession( bool i_bInformDiagram = true );
    virtual bool isHit( const QPoint& i_pt ) const;
    virtual void moveEvent( CDiagObjMoveEvent* i_pEv, bool i_bInformDiagram = true );
    virtual QRect geometry() const;
    virtual void setGeometry( const QRect& i_rect, bool i_bInformDiagram = true );
    virtual QSize sizeHint();
    virtual void invalidate( unsigned int i_uUpdateFlags, bool i_bInformDiagram = true );
    virtual void validate( unsigned int i_uUpdateFlags );
public: // must overridables
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const = 0;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr ) = 0;
//public: // overridables of base class QObject
//    virtual bool event( QEvent* i_pEv );
//protected: // overridable event methods
//    virtual void onReceivedError( ZS::System::CMsgErr* i_pMsgErr );
//    virtual ZS::System::SErrResultInfo onReceivedReqShow( CMsgReqShow* i_pMsgReq );
//    virtual ZS::System::SErrResultInfo onReceivedReqHide( CMsgReqHide* i_pMsgReq );
//    virtual ZS::System::SErrResultInfo onReceivedReqSetValue( CMsgReqSetValue* i_pMsgReq );
private: // copy ctor not allowed
    CDiagObj( const CDiagObj& );
private: // assignment operator not allowed
    void operator=( const CDiagObj& );
protected:  // instance members
    QString                   m_strObjName;
    int                       m_iObjId; // on adding the object to the diagram the diagram will assign the id to the object
    CDataDiagram*             m_pDiagram;
    CDiagTrace*               m_pDiagTrace;
    CDiagScale*               m_arpDiagScale[EScaleDirCount];
    unsigned int              m_uUpdateFlags;
    ELayoutPos                m_layoutPos;
    QRect                     m_rectContent;
    QRect                     m_rectContentPrev;
    bool                      m_bAdjustContentRect2DiagPartCenter;
    bool                      m_bVisible;
    EDiagObjState             m_state;
    bool                      m_bIsFocusable;
    bool                      m_bPtFocusedValid;
    QPoint                    m_ptFocused;
    bool                      m_bIsEditable;
    bool                      m_bPtEditSessionValid;
    QPoint                    m_ptEditSession;
    CDiagObj*                 m_pDiagObjNext;
    CDiagObj*                 m_pDiagObjPrev;
    CDiagObj*                 m_pDiagObjPaintNext;
    CDiagObj*                 m_pDiagObjPaintPrev;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjUpdate;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjLayout;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjValidate;

}; // class CDiagObj

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObj_h
