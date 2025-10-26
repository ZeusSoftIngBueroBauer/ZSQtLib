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

#ifndef ZSDraw_GraphObjPaintOptionsModel_h
#define ZSDraw_GraphObjPaintOptionsModel_h

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
class ZSDRAWDLL_API CModelGraphObjPaintOptions : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CModelGraphObjPaintOptions"; }
public: // type definitions and constants
    enum EColumn {
        EColumnName = 0,
        EColumnIsSet = 1,
        EColumnCount
    };
    static QString column2Str(EColumn i_enumVal);
    enum EPaintOptionName {
        EPaintOptionNameDrawBoundingRect = 0,
        EPaintOptionNameDrawShapePath = 1,
        EPaintOptionNameCount,
        EPaintOptionNameUndefined
    };
    static QString paintOptionName2Str(EPaintOptionName i_enumVal);
public: // ctors and dtor
    CModelGraphObjPaintOptions(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strNameSpace,
        const QString& i_strGraphObjType,
        const QString& i_strObjName,
        QObject* i_pObjParent = nullptr);
    virtual ~CModelGraphObjPaintOptions();
signals:
    /*! This signal is emitted if the indicated content has been changed. */
    void contentChanged();
public: // instance methods
    bool setKeyInTree(const QString& i_strKeyInTree);
public: // instance methods
    bool hasErrors() const;
    bool hasChanges() const;
    void acceptChanges();
    void rejectChanges();
public: // instance methods
    int getOptionRowIndex(EPaintOptionName i_eOptionName) const;
public: // overridables of base class QAbstractItemModel
    int rowCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    int columnCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    QVariant data(const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& i_modelIdx, const QVariant& i_varData, int i_iRole) override;
    QVariant headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& i_modelIdx) const override;
protected: // type definitions and constants
    struct SPaintOption {
    public:
        static SPaintOption fromGraphObj(CGraphObj* i_pGraphObj, EPaintOptionName i_eOptionName, int i_iRowIdx);
    public: // ctors
        SPaintOption() = default;
        SPaintOption(EPaintOptionName i_eOptionName, int i_iRowIdx, bool i_bIsSet);
    public: // operators
        bool operator == (const SPaintOption& i_other) const;
        bool operator != (const SPaintOption& i_other) const;
    public: // struct members
        /*!< Name of the option. */
        EPaintOptionName m_eOptionName = EPaintOptionNameUndefined;
        /*!< Index in the row of labels. */
        int m_iRowIdx = -1;
        /*!< True if the option is set, false otherwise. */
        bool m_bIsSet = false;
    };
protected slots:
    void onGraphObjOptionPaintBoundingRectChanged(CGraphObj* i_pGraphObj, bool i_bOptionSet);
    void onGraphObjOptionPaintShapePathChanged(CGraphObj* i_pGraphObj, bool i_bOptionSet);
    void onGraphObjAboutToBeDestroyed(CGraphObj* i_pGraphObj);
protected: // instance methods
    void clearModel();
    void fillModel();
protected: // auxiliary instance methods
    QList<SPaintOption> getPaintOptions(CGraphObj* i_pGraphObj) const;
protected: // instance methods (tracing emitting signals)
    void emit_contentChanged();
    void _beginInsertRows(const QModelIndex& i_modelIdxParent, int i_iRowFirst, int i_iRowLast);
    void _endInsertRows();
    void _beginRemoveRows(const QModelIndex& i_modelIdxParent, int i_iRowFirst, int i_iRowLast);
    void _endRemoveRows();
    void emit_dataChanged(const QModelIndex& i_modelIdxTL, const QModelIndex& i_modelIdxBR, const QVector<int>& i_ariRoles = QVector<int>());
protected: // instance members
    /*!< Pointer to drawing scene. */
    CDrawingScene* m_pDrawingScene = nullptr;
    /*!< Unique key of the graphical object to be edited. */
    QString m_strKeyInTree;
    /*!< If the unique key is set the drawing scene is queried to get the pointer to
         the graphical object which should be edited. */
    CGraphObj* m_pGraphObj = nullptr;
    /*!< Cached draw settings of the labels of the graphical object.
         The labels are sorted. First the predefined labels are inserted.
         The order is arbitrary. The relation between the row index and the
         label name (which must be unique) is stored in a separate hash. */
    QList<SPaintOption> m_arPaintOptions;
    /*!< Flag to indicate that the content of a data row has been changed while the "contentChanged"
         signal was blocked by the "contentChanged" counter. */
    bool m_bContentChanged = false;
    /*!< Counts how ofter the "contentChanged" signal has been blocked. A value greater than 0
         for the counter means that the signal "contentChanged" should not be emitted. Instead
         the flag m_bContentChanged should be set to true.
         If the counter is decremented and reaches 0 the flag "m_bContentChanged" is checked and
         the signal "contentChanged" is emitted if the flag is set. */
    int m_iContentChangedSignalBlockedCounter = 0;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj = nullptr;
    /*!< Trace admin object to trace noisy methods (often called methods like "data", "columnCount", "RowCount"). */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjNoisyMethods = nullptr;

}; // class CModelGraphObjPaintOptions

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjPaintOptionsModel_h
