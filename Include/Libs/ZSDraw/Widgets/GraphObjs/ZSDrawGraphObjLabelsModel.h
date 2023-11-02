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

#ifndef ZSDraw_GraphObjLabelsModel_h
#define ZSDraw_GraphObjLabelsModel_h

#include <QtCore/qabstractitemmodel.h>

#include "ZSDraw/Common/ZSDrawCommon.h"
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
class ZSDRAWDLL_API CModelGraphObjLabels : public QAbstractTableModel
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CModelGraphObjLabels"; }
public: // type definitions and constants
    enum EColumn {
        EColumnSelected          = 0,
        EColumnName              = 1,
        EColumnText              = 2,
        EColumnVisible           = 3,
        EColumnAnchor            = 4,
        EColumnAnchorLineVisible = 5,
        EColumnError             = 6,
        EColumnCount
    };
    static QString column2Str(int i_clm);
public: // ctors and dtor
    CModelGraphObjLabels(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strNameSpace,
        const QString& i_strGraphObjType,
        const QString& i_strObjName,
        QObject* i_pObjParent = nullptr);
    virtual ~CModelGraphObjLabels();
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
    QString findUniqueLabelName(const QString& i_strPrefix = "") const;
    QString addLabel();
    void removeLabel(const QString& i_strName);
    QStringList selectedLabels() const;
    void removeSelectedLabels();
public: // overridables of base class QAbstractItemModel
    int rowCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    int columnCount(const QModelIndex& i_modelIdxParent = QModelIndex()) const override;
    QVariant data(const QModelIndex& i_modelIdx, int i_iRole = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& i_modelIdx, const QVariant& i_varData, int i_iRole) override;
    QVariant headerData(int i_iSection, Qt::Orientation i_orientation, int i_iRole = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& i_modelIdx) const override;
protected: // type definitions and constants
    struct SLabelSettings {
    public:
        static SLabelSettings fromGraphObj(CGraphObj* i_pGraphObj, const QString& i_strLabelName, int i_iRowIdx = -1);
    public: // ctors
        SLabelSettings() :
            m_strName(), m_iRowIdx(-1), m_bIsPredefinedLabelName(false),
            m_strText(), m_selPt(ESelectionPoint::None), m_bVisible(false), m_bAnchorLineVisible(false),
            m_bSelected(false), m_errResultInfo()
        {}
        SLabelSettings(const QString& i_strName, int i_iRowIdx, bool i_bIsPredefinedLabelName) :
            m_strName(i_strName), m_iRowIdx(i_iRowIdx), m_bIsPredefinedLabelName(i_bIsPredefinedLabelName),
            m_strText(), m_selPt(ESelectionPoint::None), m_bVisible(false), m_bAnchorLineVisible(false),
            m_bSelected(false), m_errResultInfo()
        {}
        SLabelSettings(
            const QString& i_strName, int i_iRowIdx, bool i_bIsPredefinedLabelName,
            const QString& i_strText, ESelectionPoint i_selPt, bool i_bVisible, bool i_bAnchorLineVisible) :
            m_strName(i_strName), m_iRowIdx(i_iRowIdx), m_bIsPredefinedLabelName(i_bIsPredefinedLabelName),
            m_strText(i_strText), m_selPt(i_selPt), m_bVisible(i_bVisible), m_bAnchorLineVisible(i_bAnchorLineVisible),
            m_bSelected(false), m_errResultInfo()
        {}
    public: // operators
        bool operator == (const SLabelSettings& i_other) const;
        bool operator != (const SLabelSettings& i_other) const;
    public: // struct members
        QString m_strName;
        int m_iRowIdx;
        bool m_bIsPredefinedLabelName;
        QString m_strText;
        ESelectionPoint m_selPt;
        bool m_bVisible;
        bool m_bAnchorLineVisible;
        bool m_bSelected;
        ZS::System::SErrResultInfo m_errResultInfo;
    };
protected slots:
    void onGraphObjLabelAdded(const QString& i_strName);
    void onGraphObjLabelRemoved(const QString& i_strName);
    void onGraphObjLabelRenamed(const QString& i_strName, const QString& i_strNameNew);
    void onGraphObjLabelChanged(const QString& i_strName);
    void onGraphObjAboutToBeDestroyed(CGraphObj* i_pGraphObj);
protected: // instance methods
    QList<SLabelSettings> getLabelSettings(CGraphObj* i_pGraphObj) const;
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
    /*!< Unique key of the graphical to be edited. Stored and set as key string to also
         support changing geometry of CDrawingView. */
    QString m_strKeyInTree;
    /*!< If the unique key is set the drawing scene is queried to get the pointer to
         graphical object which should be edited. */
    CGraphObj* m_pGraphObj;
    /*!< Cached draw settings of the labels of the graphical object.
         The labels are sorted. First the predefined labels are inserted.
         The order is arbitrary. The relation between the row index and the
         label name (which must be unique) is stored in a separate hash. */
    QList<SLabelSettings> m_arLabelSettings;
    /*!< If a label is changed the corresponding row index can be accessed through this hash. */
    QHash<QString, int> m_hshName2RowIdx;
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

}; // class CModelGraphObjLabels

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLabelsModel_h
