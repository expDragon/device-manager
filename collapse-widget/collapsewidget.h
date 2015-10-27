#ifndef COLLAPSEWIDGET_H
#define COLLAPSEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>

enum COLLAPSE_WIDGET_STATE { COLLAPSED, EXPANDED };

class CollapseWidget : public QWidget
{
    Q_OBJECT

private:
    QPushButton *headerButton;
    QListWidget *infoListWidget;

    int listHeight;
    int widgetHeight;
    COLLAPSE_WIDGET_STATE currentState;

    int index;

public:
    CollapseWidget(QWidget *parent = 0);
    CollapseWidget(const QString headerText, const QStringList infoTextList);
    ~CollapseWidget();

    void setHeader(const QString headerText);
    void setList(const QStringList infoTextList);

    void changeState();
    void expand();
    void collapse();

    COLLAPSE_WIDGET_STATE getCurrentState() const;
    int getExactHeight() const;
    void updateExactHeight();

    int getIndex() const;
    void setIndex(int value);

    void sendRowChanged(int row);

    QListWidget *getInfoListWidget() const;
    void setInfoListWidget(QListWidget *value);

signals:
    void currentRowChanged(int index, int row);
    void stateChanged(COLLAPSE_WIDGET_STATE);
};

#endif // COLLAPSEWIDGET_H
