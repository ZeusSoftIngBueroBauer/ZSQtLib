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

#ifndef ZSDraw_GraphObjGeometryModel_h
#define ZSDraw_GraphObjGeometryModel_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSSys/ZSSysErrResult.h"

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingScene;
class CGraphObj;

//******************************************************************************
class ZSDRAWDLL_API CModelGraphObjGeometry : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CModelGraphObjGeometry"; }
public: // type definitions and constants
    enum EColumn {
        EColumnName     = 0,
        EColumnXVal     = 1,
        EColumnYVal     = 2,
        EColumnShowVals = 3,
        EColumnShowLine = 4,
        EColumnCount
    };
    static QString column2Str(int i_clm);
public: // ctors and dtor
    CModelGraphObjGeometry(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strNameSpace,
        const QString& i_strGraphObjType,
        const QString& i_strObjName,
        const ZS::System::CEnumScaleDimensionUnit& i_eDimensionUnit,
        QObject* i_pObjParent = nullptr);
    virtual ~CModelGraphObjGeometry();
signals:
    /*! This signal is emitted if the indicated content has been changed. */
    void contentChanged();
public: // instance methods
    bool setKeyInTree(const QString& i_strKeyInTree);
public: // instance methods
    bool hasErrors() const;
    bool hasChanges() const;
    void applySettings();
public: // instance methods
    void clearModel();
    void fillModel();
public: // instance methods
    int getValueRowIndex(const QString& i_strValueName) const;
    QStringList valueNames() const;
public: // overridables of base class QAbstractItemModel
    int rowCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    int columnCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    QVariant data(const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& i_modelIdx, const QVariant& i_varData, int i_iRole) override;
    QVariant headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& i_modelIdx) const override;
protected: // type definitions and constants
    struct SDataRow {
    public:
        static SDataRow fromGraphObj(
            CGraphObj* i_pGraphObj, const QString& i_strValueName,
            const ZS::PhysVal::CUnit& i_unit, int i_iRowIdx = -1);
    public: // ctors
        SDataRow();
        SDataRow(
            const QString& i_strValueName, int i_iRowIdx,
            const ZS::PhysVal::CPhysVal& i_physValX, const ZS::PhysVal::CPhysVal& i_physValY,
            bool i_bVisible, bool i_bLineVisible);
    public: // operators
        bool operator == (const SDataRow& i_other) const;
        bool operator != (const SDataRow& i_other) const;
    public: // struct members
        /*!< Name of the value as retrieved from the graphical object.
             Possible names could be "P1", "Center", "Size", "Length", "Angle".
             "Length" and "Angle" are treated special as they are single values
             and not X/Y value pairs. */
        QString m_strValueName;
        /*!< Index in the row of labels. */
        int m_iRowIdx;
        /*!< X Value. For "Size" this is the width of the object's bounding rectangle.
             Also used for "Length" and "Angle". */
        ZS::PhysVal::CPhysVal m_physValX;
        /*!< Y Value. For "Size" this is the height of the object's bounding rectangle.
             Not used (set invalid) for "Length" and "Angle". */
        ZS::PhysVal::CPhysVal m_physValY;
        /*!< True if the value should be shown. */
        bool m_bVisible;
        /*!< True if a line should be drawn from the label showing the value and the
             corresponding shape point of the object.
             For "Line" this means that a dimension line is drawn.
             For "Angle" this means that a circle segment is drawn. */
        bool m_bLineVisible;
    };
protected slots:
    //void onGraphObjShapePointAdded(CGraphObj* i_pGraphObj, const QString& i_strName);
    //void onGraphObjShapePointRemoved(CGraphObj* i_pGraphObj, const QString& i_strName);
    void onGraphObjGeometryChanged(CGraphObj* i_pGraphObj);
    void onGraphObjAboutToBeDestroyed(CGraphObj* i_pGraphObj);
protected: // instance methods
    QList<SDataRow> getDataRows(CGraphObj* i_pGraphObj) const;
protected: // instance methods (tracing emitting signals)
    void emit_contentChanged();
    void _beginInsertRows(const QModelIndex& i_modelIdxParent, int i_iRowFirst, int i_iRowLast);
    void _endInsertRows();
    void _beginRemoveRows(const QModelIndex& i_modelIdxParent, int i_iRowFirst, int i_iRowLast);
    void _endRemoveRows();
    void emit_dataChanged(const QModelIndex& i_modelIdxTL, const QModelIndex& i_modelIdxBR, const QVector<int>& i_ariRoles = QVector<int>());
protected: // instance members
    /*!< Pointer to drawing scene. */
    CDrawingScene* m_pDrawingScene;
    /*!< Values may be indicated either in metrics of pixels unit. */
    ZS::System::CEnumScaleDimensionUnit m_eDimensionUnit;
    /*!< Unique key of the graphical object to be edited. */
    QString m_strKeyInTree;
    /*!< If the unique key is set the drawing scene is queried to get the pointer to
         the graphical object which should be edited. */
    CGraphObj* m_pGraphObj;
    /*!< Cached geometry settings of the graphical object.
         The order is defined by the graphical object returning the list of values.
         The relation between the row index and the value names (which must be unique)
         is stored in a separate hash. */
    QList<SDataRow> m_arDataRows;
    /*!< Flag to indicate that the content of a data row has been changed while the "contentChanged"
         signal was blocked by the "contentChanged" counter. */
    bool m_bContentChanged;
    /*!< Counts how ofter the "contentChanged" signal has been blocked. A value greater than 0
         for the counter means that the signal "contentChanged" should not be emitted. Instead
         the flag m_bContentChanged should be set to true.
         If the counter is decremented and reaches 0 the flag "m_bContentChanged" is checked and
         the signal "contentChanged" is emitted if the flag is set. */
    int m_iContentChangedSignalBlockedCounter;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    /*!< Trace admin object to trace noisy methods (often called methods like "data", "columnCount", "RowCount"). */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjNoisyMethods;

}; // class CModelGraphObjGeometry

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjGeometryModel_h
