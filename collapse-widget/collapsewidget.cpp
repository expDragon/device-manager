#include "collapsewidget.h"
#include <QDebug>

QListWidget *CollapseWidget::getInfoListWidget() const
{
    return infoListWidget;
}

void CollapseWidget::setInfoListWidget(QListWidget *value)
{
    infoListWidget = value;
}

CollapseWidget::CollapseWidget(QWidget *parent)
    : QWidget(parent)
{
    headerButton = new QPushButton;
    headerButton->setObjectName("collapse-widget-header");
    connect(headerButton, &QPushButton::clicked, this, &CollapseWidget::changeState);

    currentState = EXPANDED;
    index = -1;

    infoListWidget = new QListWidget;
    connect(infoListWidget, &QListWidget::currentRowChanged, this, &CollapseWidget::sendRowChanged);
    infoListWidget->setFrameStyle(QFrame::NoFrame);

    QVBoxLayout *rootLayout = new QVBoxLayout;
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    rootLayout->addWidget(headerButton);
    rootLayout->addWidget(infoListWidget);
    setLayout(rootLayout);
}

CollapseWidget::CollapseWidget(const QString headerText, const QStringList infoTextList) : CollapseWidget()
{
    setHeader(headerText);
    setList(infoTextList);
    changeState();
}

CollapseWidget::~CollapseWidget()
{

}

int CollapseWidget::getExactHeight() const
{
    return widgetHeight;
}

void CollapseWidget::setHeader(const QString headerText)
{
    headerButton->setText(headerText);
}

void CollapseWidget::setList(const QStringList infoTextList)
{
    for(QString listItem : infoTextList)
    {
        infoListWidget->addItem(listItem);
    }
}

void CollapseWidget::changeState()
{
    currentState == COLLAPSED ? expand() : collapse();
    updateExactHeight();
    headerButton->style()->polish(headerButton);
    this->style()->polish(this);
    infoListWidget->style()->polish(infoListWidget);
    emit stateChanged(currentState);
}

void CollapseWidget::expand()
{
    infoListWidget->setMaximumHeight(listHeight);
    currentState = EXPANDED;
    setObjectName("collapse-widget-expanded");
}

void CollapseWidget::collapse()
{
    listHeight = infoListWidget->maximumHeight();
    infoListWidget->setFixedHeight(0);
    currentState = COLLAPSED;
    setObjectName("collapse-widget-collapsed");
}

void CollapseWidget::updateExactHeight()
{
    widgetHeight = headerButton->sizeHint().height();

    if (currentState == EXPANDED)
    {
        for(int i = 0; i < infoListWidget->count(); i++)
        {
            widgetHeight += infoListWidget->sizeHintForRow(i);
        }
    }
}

COLLAPSE_WIDGET_STATE CollapseWidget::getCurrentState() const
{
    return currentState;
}

int CollapseWidget::getIndex() const
{
    return index;
}

void CollapseWidget::setIndex(int value)
{
    index = value;
}

void CollapseWidget::sendRowChanged(int row)
{
    emit currentRowChanged(index, row);
}
