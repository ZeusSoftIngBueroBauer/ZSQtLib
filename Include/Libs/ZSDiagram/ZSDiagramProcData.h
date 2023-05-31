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
    //virtual void updateDiagram( CDiagObj* i_pDiagObj = nullptr ); // need a method name different from QWidget::update
    virtual void update( CDiagObj* i_pDiagObj, const QRect& i_rect );
    virtual void invalidate( CDiagObj* i_pDiagObj, unsigned int i_uUpdateFlags );
    virtual void validate( unsigned int i_uUpdateFlags );
public: // overridables to rescale the diagram or output new X/Y values
    virtual void setSpacing( EScaleDir i_scaleDir, ESpacing i_spacing );
public: // instance methods to add and remove scale objects
    void addDiagScale( CDiagScale* i_pDiagScale );
    void removeDiagScale( CDiagScale* i_pDiagScale );
    CDiagScale* removeDiagScale( const QString& i_strObjName );
    CDiagScale* findDiagScale( const QString& i_strObjName ) const;
    int getDiagScalesCount() const;
    CDiagScale* getDiagScale( int i_idx ) const;
public: // instance methods to add and remove trace objects
    void addDiagTrace( CDiagTrace* i_pDiagTrace );
    void removeDiagTrace( CDiagTrace* i_pDiagTrace );
    CDiagTrace* removeDiagTrace( const QString& i_strObjName );
    CDiagTrace* findDiagTrace( const QString& i_strObjName ) const;
    int getDiagTracesCount() const;
    CDiagTrace* getDiagTrace( int i_idx ) const;
public: // instance methods to add and remove diagram objects
    void addDiagObj( CDiagObj* i_pDiagObj );
    void removeDiagObj( CDiagObj* i_pDiagObj );
    CDiagObj* removeDiagObj( const QString& i_strObjName );
    CDiagObj* findDiagObj( const QString& i_strObjName ) const;
    int getDiagObjsCount() const;
    CDiagObj* getDiagObj( int i_idx ) const;
public: // instance methods to change diagram objects
    void showDiagObj( const QString& i_strObjName ) const;
    void showDiagObj( int i_idx ) const;
    void hideDiagObj( const QString& i_strObjName ) const;
    void hideDiagObj( int i_idx ) const;
    bool isDiagObjVisible( const QString& i_strObjName ) const;
    bool isDiagObjVisible( int i_idx ) const;
    void moveDiagObjInPaintList( const QString& i_strObjName, EDiagObjMoveMode i_moveMode, int i_idxCount = 0 );
    void moveDiagObjInPaintList( int i_idx, EDiagObjMoveMode i_moveMode, int i_idxCount = 0 );
    void moveDiagObjInPaintList( CDiagObj* i_pDiagObj, EDiagObjMoveMode i_moveMode, int i_idxCount = 0 );
public: // overridables (callbacks from CDiagScale, CDiagTrace and CDiagObj)
    virtual void scaleChanged( ZS::Diagram::CDiagScale* i_pDiagScale );
    virtual void traceChanged( ZS::Diagram::CDiagTrace* i_pDiagTrace );
protected: // overridables (auxiliary instance methods)
    virtual void updateLayout();
    virtual void updateData();
protected: // CWdgtDiagram needs to overwrite the method for emitting the signals
    virtual void emit_diagScaleAdded(const QString& i_strObjName) {}
    virtual void emit_diagScaleRemoved(const QString& i_strObjName) {}
    virtual void emit_diagTraceAdded(const QString& i_strObjName) {}
    virtual void emit_diagTraceRemoved(const QString& i_strObjName) {}
    virtual void emit_diagObjAdded(const QString& i_strObjName) {}
    virtual void emit_diagObjRemoved(const QString& i_strObjName) {}
private: // copy ctor not allowed
    CDataDiagram( const CDataDiagram& );
private: // assignment operator not allowed
    void operator=( const CDataDiagram& );
protected:  // instance members
    QString m_strObjName;
    EDiagramUpdateType m_updateType;
    unsigned int m_uUpdateFlags;
    EMeasState m_measState;
    EMeasMode m_measMode;
    int m_iMeasType;
    ESpacing m_arSpacing[EScaleDirCount];
    /*!< List with scale objects. */
    QVector<CDiagScale*> m_arpDiagScales;
    /*!< List with traces. */
    QVector<CDiagTrace*> m_arpDiagTraces;
    /*!< List with diagram objects. The diagram object at the lowest index will be painted first. */
    QVector<CDiagObj*> m_arpDiagObjs;
    /*!< Hash with diagram objects. The key is the name of the object. */
    QHash<QString, CDiagObj*> m_hshpDiagObjs;
private:  // instance members
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjUpdate;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjEvents;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjLayout;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjValidate;

}; // class CDataDiagram

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ProcData_h
