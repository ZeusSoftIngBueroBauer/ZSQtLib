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

#ifndef ZSDiagram_ItemsTreeModel_h
#define ZSDiagram_ItemsTreeModel_h


#include "ZSDiagramGUI/ZSDiagramGUIDllMain.h"

#include <QtCore/qabstractitemmodel.h>

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Diagram
{
class CWdgtDiagram;

namespace GUI
{
//******************************************************************************
class ZSDIAGRAMGUIDLL_API CModelDiagramTreeItem
//******************************************************************************
{
public: // ctors and dtor
    CModelDiagramTreeItem(
        const QString& i_strClassName,
        const QString& i_strObjName,
        CModelDiagramTreeItem* i_pParent = nullptr);
    ~CModelDiagramTreeItem();
public: // instance methods
    QString className() const;
    QString objectName() const;
public: // instance methods
    CModelDiagramTreeItem* parent();
    int row() const;
public: // instance methods
    CModelDiagramTreeItem* child(int i_idxRow);
    int childsCount() const;
private: // instance members
    QString m_strClassName;
    QString m_strObjName;
    CModelDiagramTreeItem* m_pParent;
    QVector<CModelDiagramTreeItem*> m_arpChilds;
};

//******************************************************************************
class ZSDIAGRAMGUIDLL_API CModelDiagramItems : public QAbstractItemModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Diagram::GUI"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CModelDiagramItems"; }
public: // type definitions and constants
    enum EColumn {
        EColumnDiagObjClassName = 0,
        EColumnDiagObjName = 1,
        EColumnCount,
        EColumnUndefined
    };
    static QString column2Str(EColumn i_clm);
public: // class methods
    static QString modelIdx2Str(const QModelIndex& i_modelIdx);
public: // ctors and dtor
    CModelDiagramItems( CWdgtDiagram* i_pDiagram, QObject* i_pObjParent = nullptr );
    virtual ~CModelDiagramItems();
public:
    CWdgtDiagram* diagram() { return m_pDiagram; }
public: // must overridables of base class QAbstractItemModel
    virtual int rowCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual int columnCount( const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual QModelIndex index( int i_iRow, int i_iCol, const QModelIndex& i_modelIdxParent = QModelIndex() ) const override;
    virtual QModelIndex parent( const QModelIndex& i_modelIdx ) const override;
    virtual QVariant data( const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole ) const override;
public: // overridables of base class QAbstractItemModel
    virtual Qt::ItemFlags flags(const QModelIndex& i_modelIdx) const;
    virtual QVariant headerData( int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole ) const;
protected slots:
    void onDiagramAboutToBeDestroyed(const QString& i_strObjName);
    void onDiagramScaleAdded(const QString& i_strObjName);
    void onDiagramScaleRemoved(const QString& i_strObjName);
    void onDiagramTraceAdded(const QString& i_strObjName);
    void onDiagramTraceRemoved(const QString& i_strObjName);
    void onDiagramObjAdded(const QString& i_strClassName, const QString& i_strObjName);
    void onDiagramObjRemoved(const QString& i_strClassName, const QString& i_strObjName);
protected: // auxiliary instance methods
    CModelDiagramTreeItem* findDiagScaleItem(const QString& i_strObjName) const;
    CModelDiagramTreeItem* findDiagTraceItem(const QString& i_strObjName) const;
    CModelDiagramTreeItem* findDiagObjItem(const QString& i_strClassName, const QString& i_strObjName) const;
protected: // instance members
    CWdgtDiagram* m_pDiagram;
    CModelDiagramTreeItem* m_pRoot;
    CModelDiagramTreeItem* m_pScales;
    CModelDiagramTreeItem* m_pTraces;
    CModelDiagramTreeItem* m_pDiagObjs;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjNoisyMethods;

}; // class CModelDiagramItems

} // namespace GUI

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagram_ItemsTreeModel_h
