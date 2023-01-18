/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_GraphObjModelEntry_h
#define ZSDraw_GraphObjModelEntry_h

#if 0

#include <QtCore/qabstractitemmodel.h>

#include "ZSDraw/Common/ZSDrawCommon.h"

class QGraphicsItems;

namespace ZS
{
namespace Draw
{
class CDrawingScene;
class CGraphicsItemsModel;
class CGraphObj;

//******************************************************************************
class ZSDRAWDLL_API CGraphObjModelEntry
//******************************************************************************
{
public: // ctors and dtor
    CGraphObjModelEntry(
        CGraphicsItemsModel* i_pModel,
        CDrawingScene*       i_pDrawingScene,
        EGraphObjType        i_graphObjType,
        const QString&       i_strGraphObjName,
        const QString&       i_strGraphObjId );
    ~CGraphObjModelEntry();
public: // instance methods
    CDrawingScene* getDrawingScene() { return m_pDrawingScene; }
public: // instance methods
    void setParentEntry( CGraphObjModelEntry* i_pEntry );
    CGraphObjModelEntry* getParentEntry() { return m_pParent; }
    QModelIndex getParentModelIdx() const;
public: // instance methods
    void setRowId( int i_iRow );
    int getRowId() const { return m_modelIdx.row(); }
    //void setModelIdx( const QModelIndex& i_modelIdx );
    QModelIndex getModelIdx() const { return m_modelIdx; }
public: // instance methods
    int getChildCount() const { return m_arpChilds.size(); }
    int appendChildEntry( CGraphObjModelEntry* i_pEntry ); // appends the entry at the end
    int insertChildEntry( CGraphObjModelEntry* i_pEntry ); // inserts the entry sorted by name
    void removeChildEntry( int i_iRow );
    void setChildEntry( int i_iRow, CGraphObjModelEntry* i_pEntry );
    CGraphObjModelEntry* getChildEntry( int i_iRow );
    CGraphObjModelEntry* findChildEntry( const QString& i_strNodeName );
public: // instance methods
    EGraphObjType getGraphObjType() const { return m_graphObjType; }
    void setGraphObjName( const QString& i_strObjName );
    QString getGraphObjName() const { return m_strGraphObjName; }
public: // instance methods
    void setGraphObjId( const QString& i_strObjId );
    QString getGraphObjId() const { return m_strGraphObjId; }
    QGraphicsItem* getGraphicsItem();
    CGraphObj* getGraphObj();
private: // copy operator not allowed
    CGraphObjModelEntry( const CGraphObjModelEntry& );
private: // assignment operator not allowed
    CGraphObjModelEntry& operator = ( const CGraphObjModelEntry& );
protected: // struct members
    CGraphicsItemsModel*        m_pModel;
    CDrawingScene*              m_pDrawingScene;
    CGraphObjModelEntry*        m_pParent;
    QModelIndex                 m_modelIdx;
    QList<CGraphObjModelEntry*> m_arpChilds;
    EGraphObjType               m_graphObjType;
    QString                     m_strGraphObjName;
    QString                     m_strGraphObjId;

}; // struct CGraphObjModelEntry

} // namespace Draw

} // namespace ZS

#endif // #if 0

#endif // #ifndef ZSDraw_GraphObjModelEntry_h
