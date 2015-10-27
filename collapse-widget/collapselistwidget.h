#ifndef COLLAPSELISTWIDGET_H
#define COLLAPSELISTWIDGET_H

#include <QListWidget>
#include <QList>
#include <QVBoxLayout>
#include <QSignalMapper>
#include "collapsewidget.h"
#include "enum-devices/device.h"

class CollapseListWidget : public QListWidget
{
    Q_OBJECT

private:
    QList<CollapseWidget*> *groupWidgetsList;
    QListWidget *groupsListBox;

public:
    CollapseListWidget();
    CollapseListWidget(const QStringList headersList, const QList<QStringList> textList);
    void setGroups(const QStringList headersList, const QList<QStringList> textList);
    void addGroup(const QString header, QStringList list);
    void resetSize();
    void reactToStateChange(COLLAPSE_WIDGET_STATE groupState);
    QListWidget *getGroupsListBox() const;
    void setGroupsListBox(QListWidget *value);
    void sendSelectionChangedSignal(int groupIndex, int selectedItemIndex);

    QList<CollapseWidget *> *getGroupWidgetsList() const;
    void setGroupWidgetsList(QList<CollapseWidget *> *value);

signals:
    void itemSelected(int groupIndex, int itemIndex);
};

#endif // COLLAPSELISTWIDGET_H
