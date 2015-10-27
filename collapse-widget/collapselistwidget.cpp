#include "collapselistwidget.h"
#include <QScrollBar>

QListWidget *CollapseListWidget::getGroupsListBox() const
{
    return groupsListBox;
}

void CollapseListWidget::setGroupsListBox(QListWidget *value)
{
    groupsListBox = value;
}

QList<CollapseWidget *> *CollapseListWidget::getGroupWidgetsList() const
{
    return groupWidgetsList;
}

void CollapseListWidget::setGroupWidgetsList(QList<CollapseWidget *> *value)
{
    groupWidgetsList = value;
}

CollapseListWidget::CollapseListWidget()
{
    groupWidgetsList = new QList<CollapseWidget*>;
    groupsListBox = new QListWidget;
    groupsListBox->setSelectionMode(QAbstractItemView::NoSelection);
    groupsListBox->setFrameStyle(QFrame::NoFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout;
    rootLayout->addWidget(groupsListBox);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    groupsListBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setLayout(rootLayout);
}

CollapseListWidget::CollapseListWidget(const QStringList headersList, const QList<QStringList> textList) :CollapseListWidget()
{
    setGroups(headersList, textList);
}

void CollapseListWidget::setGroups(const QStringList headersList, const QList<QStringList> textList)
{
    groupsListBox->clear();
    groupWidgetsList->clear();

    for(int i = 0; i < headersList.size(); i++)
    {
        addGroup(headersList.at(i), textList.at(i));
    }
}

void CollapseListWidget::addGroup(const QString header, QStringList list)
{
    CollapseWidget *groupWidget = new CollapseWidget(header, list);
    groupWidget->setIndex(groupsListBox->count());
    groupWidgetsList->append(groupWidget);
    connect(groupWidget, &CollapseWidget::stateChanged, this, &CollapseListWidget::reactToStateChange);
    connect(groupWidget, &CollapseWidget::currentRowChanged, this, &CollapseListWidget::sendSelectionChangedSignal);

    groupsListBox->addItem(new QListWidgetItem);
    groupsListBox->setItemWidget(groupsListBox->item(groupsListBox->count() - 1), groupWidgetsList->at(groupsListBox->count() - 1));
}

void CollapseListWidget::resetSize()
{
    int count = groupsListBox->count();
    for(int i = 0; i < count; i++)
    {
      QListWidgetItem *item = groupsListBox->item(i);
      item->setSizeHint(QSize(item->sizeHint().width(), groupWidgetsList->at(i)->getExactHeight()));
    }
}

void CollapseListWidget::reactToStateChange(COLLAPSE_WIDGET_STATE groupState)
{
    resetSize();
}

void CollapseListWidget::sendSelectionChangedSignal(int groupIndex, int selectedItemIndex)
{
    int count = groupsListBox->count();
    for(int i = 0; i < count; i++)
    {
        groupWidgetsList->at(i)->getInfoListWidget()->clearSelection();
    }

    emit itemSelected(groupIndex, selectedItemIndex);
}
