#ifndef COMBODELEGATE_H
#define COMBODELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QComboBox>

class ComboDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // COMBODELEGATE_H
