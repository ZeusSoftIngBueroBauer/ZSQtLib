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

#ifndef ZSDiagram_ProcData_h
#define ZSDiagram_ProcData_h

#include "ZSDiagram/ZSDiagramAux.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Diagram
{
class CDiagObj;
class CDiagScale;
class CDiagTrace;

//******************************************************************************
class ZSDIAGRAMDLL_API CDataDiagram
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Diagram"; }
    static QString ClassName() { return "CDataDiagram"; }
public: // ctors and dtor
    CDataDiagram(
        const QString&     i_strObjName,
        EDiagramUpdateType i_updateType = EDiagramUpdateTypeData );
    virtual ~CDataDiagram();
public: // copy ctor not allowed but diagrams may be cloned
    virtual CDataDiagram* clone( EDiagramUpdateType i_diagramUpdateType ) const;
public: // instance methods
    void setObjName( const QString& i_strObjName );
    QString getObjName() const;
    EDiagramUpdateType getUpdateType() const;
public: // overridables to update the content of the diagram
    virtual void updateDiagram( CDiagObj* i_pDiagObj = nullptr ); // need a method name different from QWidget::update
    virtual void invalidate( CDiagObj* i_pDiagObj, unsigned int i_uUpdateFlags );
    virtual void validate( unsigned int i_uUpdateFlags );
public: // overridables to rescale the diagram or output new X/Y values
    virtual void setSpacing( EScaleDir i_scaleDir, ESpacing i_spacing );
public: // instance methods to add and remove scale objects
    void addDiagScale( CDiagScale* i_pDiagScale );
    void removeDiagScale( CDiagScale* i_pDiagScale );
    CDiagScale* removeDiagScale( const QString& i_strObjName );
    CDiagScale* getDiagScale( const QString& i_strObjName );
public: // instance methods to add and remove trace objects
    void addDiagTrace( CDiagTrace* i_pDiagTrace );
    void removeDiagTrace( CDiagTrace* i_pDiagTrace );
    CDiagTrace* removeDiagTrace( const QString& i_strObjName );
    CDiagTrace* getDiagTrace( const QString& i_strObjName );
public: // instance methods to add, remove, and change diagram objects
    void addDiagObj( CDiagObj* i_pDiagObj );
    void removeDiagObj( CDiagObj* i_pDiagObj );
    CDiagObj* removeDiagObj( const QString& i_strObjName );
    CDiagObj* removeDiagObj( int i_iObjId );
    CDiagObj* getDiagObj( const QString& i_strObjName ) const;
    CDiagObj* getDiagObj( int i_iObjId ) const;
    void showDiagObj( const QString& i_strObjName ) const;
    void showDiagObj( int i_iObjId ) const;
    void hideDiagObj( const QString& i_strObjName ) const;
    void hideDiagObj( int i_iObjId ) const;
    bool isDiagObjVisible( const QString& i_strObjName ) const;
    bool isDiagObjVisible( int i_iObjId ) const;
    void moveDiagObjInPaintList( const QString& i_strObjName, EDiagObjMoveMode i_moveMode, int i_idxCount = 0 );
    void moveDiagObjInPaintList( int i_iObjId, EDiagObjMoveMode i_moveMode, int i_idxCount = 0 );
    void moveDiagObjInPaintList( CDiagObj* i_pDiagObj, EDiagObjMoveMode i_moveMode, int i_idxCount = 0 );
public: // overridables (callbacks from CDiagScale, CDiagTrace and CDiagObj)
    virtual void scaleChanged( ZS::Diagram::CDiagScale* i_pDiagScale );
    virtual void traceChanged( ZS::Diagram::CDiagTrace* i_pDiagTrace );
protected: // overridables to update the content of the diagram
    virtual void updateLayout();
    virtual void updateData();
private: // copy ctor not allowed
    CDataDiagram( const CDataDiagram& );
private: // assignment operator not allowed
    void operator=( const CDataDiagram& );
protected:  // instance members
    QString                   m_strObjName;
    EDiagramUpdateType        m_updateType;
    unsigned int              m_uUpdateFlags;
    EMeasState                m_measState;
    EMeasMode                 m_measMode;
    int                       m_iMeasType;
    ESpacing                  m_arSpacing[EScaleDirCount];
    int                       m_iDiagScaleCount;
    CDiagScale*               m_pDiagScaleFirst;
    CDiagScale*               m_pDiagScaleLast;
    int                       m_iDiagTraceCount;
    CDiagTrace*               m_pDiagTraceFirst;
    CDiagTrace*               m_pDiagTraceLast;
    int                       m_iDiagObjCount;
    CDiagObj*                 m_pDiagObjFirst;
    CDiagObj*                 m_pDiagObjLast;
    CDiagObj*                 m_pDiagObjPaintFirst;
    CDiagObj*                 m_pDiagObjPaintLast;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjUpdate;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjLayout;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjValidate;

}; // class CDataDiagram

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ProcData_h
