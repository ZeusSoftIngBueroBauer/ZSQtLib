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
class CDataDiagram;
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
/*! @brief Base class for all diagram objects.
*/
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
        const QString& i_strClassName,
        const QString& i_strObjName,
        CDiagTrace* i_pDiagTrace,
        ZS::System::ELayoutPos i_layoutPos );
    CDiagObj(
        const QString& i_strClassName,
        const QString& i_strObjName,
        CDiagScale* i_pDiagScaleX,
        CDiagScale* i_pDiagScaleY,
        ZS::System::ELayoutPos i_layoutPos );
public: // dtor
    virtual ~CDiagObj();
signals:
    // Please note that the following signals may only be thrown by the
    // derived classed after updating its internal data.
    void valueChanged( ZS::Diagram::CDiagObj* );
    void valueChanged( ZS::System::EScaleAxis, ZS::Diagram::CDiagObj* );
    void valueXChanged( ZS::Diagram::CDiagObj* );
    void valueYChanged( ZS::Diagram::CDiagObj* );
    void visibilityChanged( ZS::Diagram::CDiagObj* );
public: // instance methods
    QString className() const { return m_strClassName; }
public: // instance methods
    void setObjName( const QString& i_strObjName );
    QString getObjName() const;
    CDataDiagram* getDiagram();
    CDiagScale* getDiagScale( const ZS::System::CEnumScaleAxis& i_scaleAxis );
    virtual void setDiagTrace( CDiagTrace* i_pDiagTrace );
    CDiagTrace* getDiagTrace();
    ZS::System::ELayoutPos getLayoutPos() const;
public: // overridables
    virtual PhysVal::CPhysValRes getValRes( const ZS::System::CEnumScaleAxis& i_scaleAxis ) const;
    virtual PhysVal::CPhysVal getVal( const ZS::System::CEnumScaleAxis& i_scaleAxis ) const;
public: // overridables
    virtual void setAdjustContentRect2DiagPartCenter( bool i_bAdjust );
    virtual bool getAdjustContentRect2DiagPartCenter() const;
    virtual void show( bool i_bInformDiagram = true );
    virtual void hide( bool i_bInformDiagram = true );
    virtual bool isVisible() const;
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
    /*!< Class name which will be set by the constructor.
         When creating the trace admin object this class name is used. */
    QString m_strClassName;
    /*!< Name of the diagram object (same as QObject::objectName).
         The name will also be saved here for easier visualizing the name in debug sessions. */
    QString m_strObjName;
    /*!< Reference to diagram the object belongs to. */
    CDataDiagram* m_pDiagram;
    /*!< Reference to diagram trace instance if the object needs to access the values
         for calculating the internal data. nullptr if object don't need to access
         the values. */
    CDiagTrace* m_pDiagTrace;
    /*!< Reference to diagram X and Y scale instance if the object needs to access the scales
         for calculating the internal data. Either the X or Y scale elements (or both) may
         be nullptr if object don't need to access a scales. */
    QVector<CDiagScale*> m_arpDiagScale;
    /*!< Bitmap with flags from Diagram::EUpdate to indicate which process depth need to be recalculated. */
    unsigned int m_uUpdateFlags;
    /*!< Layout position of the object within the diagram. */
    ZS::System::ELayoutPos m_layoutPos;
    /*!< Rectangle used by the object to indicate the object within the diagram. */
    QRect m_rectContent;
    /*!< To decide whether the object needs repainting the previous rectangle are is stored here. */
    QRect m_rectContentPrev;
    /*!< Flag to indicate that the content rectangle should be adjusted to the center
         of the diagrams output area. */
    bool m_bAdjustContentRect2DiagPartCenter;
    /*!< True if the diagram object is visible, false otherwise. */
    bool m_bVisible;
    /*!< Current state of the object (objects may be focused (highlighted) or their values
         might be edited (e.g. when moving markers). */
    EDiagObjState m_state;
    /*!< Flag to indicate whether the object is focusable (markers usually are). */
    bool m_bIsFocusable;
    /*!< When selecting an object (putting the focus on it) the point the object was touched
         is stored in "m_ptFocused". The Valid flag indicates whether the stored point is valid. */
    bool m_bPtFocusedValid;
    /*!< When selecting an object (putting the focus on it) the point the object was touched
         is stored in "m_ptFocused".  */
    QPoint m_ptFocused;
    /*!< Flag to indicate whether the object is editable (markers usually are). */
    bool m_bIsEditable;
    /*!< When selecting an object and starting an edit session (e.g. moving a marker)
         the point the object was moved to is stored in "m_ptEditSession".
         The Valid flag indicates whether the stored point is valid. */
    bool m_bPtEditSessionValid;
    /*!< When starting the edit session (e.g. starting to move a marker) the point the
         object was moved to is stored in "m_ptEditSession".  */
    QPoint m_ptEditSession;
protected:  // instance members
    /*!< Trace admin object for the most common (but not too often) called methods. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    /*!< Trace admin object to control trace output of the update method. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjUpdate;
    /*!< Trace admin object to control trace output for event methods. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjEvents;
    /*!< Trace admin object to control trace output of layout processing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjLayout;
    /*!< Trace admin object to control trace output of the validate and invalidate methods. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjValidate;

}; // class CDiagObj

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObj_h
