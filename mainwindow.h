#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QTableWidget>
#include <QHeaderView>

#include "collapse-widget/collapselistwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget *headerWidget;

    QStringList headerLabels;
    QListWidget *deviceInfoListWidget;

    CollapseListWidget *deviceGroupsCollapseWidget;
    QTableWidget *deviceInfoTable;

    QLabel *title;
    QPushButton *disableDeviceButton;
    QPushButton *enableDeviceButton;
    QPushButton *closeButton;

    int clickX;
    int clickY;

    void setTableLabels();
    void setupDeviceInfoTable();
    void createHeader();
    void createDeviceGroupsWidget();
    void composeWindow();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setDeviceInfo(const QStringList deviceInfoList);

    void sendDeviceSelectedSignal(int deviceIndex);
    void sendDisableDeviceSignal();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void addDeviceGroup(const QString groupName, const QStringList deviceNames);
signals:
    void deviceGroupSelected(int groupIndex);
    void deviceSelected(int groupIndex, int deviceIndex);
    void disableDeviceButtonPressed();
    void enableDeviceButtonPressed();
};

#endif // MAINWINDOW_H
