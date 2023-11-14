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
        EColumnSelected       = 0,
        EColumnName           = 1,
        EColumnText           = 2,
        EColumnShow           = 3,
        EColumnAnchor         = 4,
        EColumnShowAnchorLine = 5,
        EColumnError          = 6,
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
    void acceptChanges();
    void rejectChanges();
public: // instance methods
    QString findUniqueLabelName(const QString& i_strPrefix = "") const;
    bool isUniqueLabelName(const QString& i_strName) const;
    int getLabelRowIndex(const QString& i_strName) const;
    QString addLabel();
    void removeLabel(const QString& i_strName);
    QStringList labelNames() const;
    QStringList selectedLabelNames() const;
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
        SLabelSettings();
        SLabelSettings(
            const QString& i_strName, int i_iRowIdx, bool i_bIsPredefinedLabelName,
            const QString& i_strText, const SGraphObjSelectionPoint& i_selPt, bool i_bVisible, bool i_bAnchorLineVisible);
    public: // operators
        bool operator == (const SLabelSettings& i_other) const;
        bool operator != (const SLabelSettings& i_other) const;
    public: // struct members
        /*!< Original name of the label as retrieved from the graphical object.
             Empty if the label is newly added to the model. */
        QString m_strNameOrig;
        /*!< Current name of the label. If the label has been retrieved from
             the graphical object and has not been renamed the current name
             equals to the original name. */
        QString m_strNameCurr;
        /*!< Index in the row of labels. */
        int m_iRowIdx;
        /*!< True if the label belongs to the predefined labels of the graphical object. */
        bool m_bIsPredefinedLabelName;
        /*!< Text to be indicated by the label. */
        QString m_strText;
        /*!< Selection point the label should be anchored to. */
        SGraphObjSelectionPoint m_selPt;
        /*!< True if the label should be visible and added to the graphics scene. */
        bool m_bVisible;
        /*!< True if the anchor line from the label to the selection point sould be drawn. */
        bool m_bAnchorLineVisible;
        /*!< True if the label is selected within the model (e.g. for deletion). */
        bool m_bSelected;
        /*!< Stores error info if the labels are wrongly configured by the user
             (e.g. ambiguous or reserved names are used). */
        ZS::System::SErrResultInfo m_errResultInfo;
    };
protected slots:
    void onGraphObjLabelAdded(CGraphObj* i_pGraphObj, const QString& i_strName);
    void onGraphObjLabelRemoved(CGraphObj* i_pGraphObj, const QString& i_strName);
    void onGraphObjLabelRenamed(CGraphObj* i_pGraphObj, const QString& i_strName, const QString& i_strNameNew);
    void onGraphObjLabelChanged(CGraphObj* i_pGraphObj, const QString& i_strName);
    void onGraphObjAboutToBeDestroyed(CGraphObj* i_pGraphObj);
protected: // instance methods
    void clearModel();
    void fillModel();
protected: // auxiliary instance methods
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
    /*!< Unique key of the graphical object to be edited. */
    QString m_strKeyInTree;
    /*!< If the unique key is set the drawing scene is queried to get the pointer to
         the graphical object which should be edited. */
    CGraphObj* m_pGraphObj;
    /*!< Cached draw settings of the labels of the graphical object.
         The labels are sorted. First the predefined labels are inserted.
         The order is arbitrary. The relation between the row index and the
         label name (which must be unique) is stored in a separate hash. */
    QList<SLabelSettings> m_arLabelSettings;
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
