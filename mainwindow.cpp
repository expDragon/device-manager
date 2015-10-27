#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setTableLabels();
    setupDeviceInfoTable();
    createHeader();
    createDeviceGroupsWidget();
    composeWindow();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setTableLabels()
{
    headerLabels << "GUID";
    headerLabels << "Install ID";
    headerLabels << "Path";
    headerLabels << "Driver";
    headerLabels << "Enumerator";
    headerLabels << "Hardware ID";
    headerLabels << "Local information";
    headerLabels << "Physical Device";
    headerLabels << "Capabilities";
}

void MainWindow::setupDeviceInfoTable()
{
    deviceInfoTable = new QTableWidget;
    deviceInfoTable->setRowCount(headerLabels.size());
    deviceInfoTable->setColumnCount(1);
    deviceInfoTable->horizontalHeader()->hide();
    deviceInfoTable->horizontalHeader()->setStretchLastSection(true);
    deviceInfoTable->setVerticalHeaderLabels(headerLabels);
}

void MainWindow::createHeader()
{
    title = new QLabel("Device Manager");

    closeButton = new QPushButton;
    closeButton->setObjectName("close-button");
    connect(closeButton, &QPushButton::clicked, this, &MainWindow::close);

    disableDeviceButton = new QPushButton;
    disableDeviceButton->setObjectName("disable-device-button");
    connect(disableDeviceButton, &QPushButton::clicked, this, &MainWindow::disableDeviceButtonPressed);

    enableDeviceButton = new QPushButton;
    enableDeviceButton->setObjectName("enable-device-button");
    connect(enableDeviceButton, &QPushButton::clicked, this, &MainWindow::enableDeviceButtonPressed);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(title);
    headerLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
    headerLayout->addWidget(enableDeviceButton);
    headerLayout->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
    headerLayout->addWidget(disableDeviceButton);
    headerLayout->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Fixed));
    headerLayout->addWidget(closeButton);

    headerWidget = new QWidget;
    headerWidget->setLayout(headerLayout);
    headerWidget->setObjectName("header");
}

void MainWindow::createDeviceGroupsWidget()
{
    deviceGroupsCollapseWidget = new CollapseListWidget;
    connect(deviceGroupsCollapseWidget, &CollapseListWidget::itemSelected, this, &MainWindow::deviceSelected);
}

void MainWindow::composeWindow()
{
    QHBoxLayout *listWidgetsLayout = new QHBoxLayout;
    listWidgetsLayout->setContentsMargins(1, 0, 1, 1);
    listWidgetsLayout->addWidget(deviceGroupsCollapseWidget);
    listWidgetsLayout->addWidget(deviceInfoTable);

    QVBoxLayout *rootLayout = new QVBoxLayout;
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    rootLayout->addWidget(headerWidget);
    rootLayout->addLayout(listWidgetsLayout);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(rootLayout);
    setCentralWidget(centralWidget);

    setWindowFlags(Qt::FramelessWindowHint);
}

void MainWindow::addDeviceGroup(const QString groupName, const QStringList deviceNames)
{
    deviceGroupsCollapseWidget->addGroup(groupName, deviceNames);
    deviceGroupsCollapseWidget->resetSize();
}

void MainWindow::setDeviceInfo(const QStringList deviceInfoList)
{
    deviceInfoTable->clearContents();

    for(int i = 0; i < deviceInfoList.size(); i++)
    {
        deviceInfoTable->setItem(i, 0, new QTableWidgetItem(deviceInfoList.at(i)));
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    clickX = event->x();
    clickY = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - clickX, event->globalY() - clickY);
}
