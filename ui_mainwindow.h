/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionY_zoom;
    QAction *actionX_zoom;
    QAction *actionPan;
    QAction *actionMeasure;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *xAxisLabel;
    QLabel *yAxisLabel;
    QWidget *placeHolder;
    QLabel *measures;
    QListView *selectedPoints;
    QComboBox *seriesCombo;
    QPushButton *configureButton;
    QPushButton *deleteButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(388, 409);
        actionY_zoom = new QAction(MainWindow);
        actionY_zoom->setObjectName(QStringLiteral("actionY_zoom"));
        actionY_zoom->setCheckable(true);
        actionX_zoom = new QAction(MainWindow);
        actionX_zoom->setObjectName(QStringLiteral("actionX_zoom"));
        actionX_zoom->setCheckable(true);
        actionPan = new QAction(MainWindow);
        actionPan->setObjectName(QStringLiteral("actionPan"));
        actionPan->setCheckable(true);
        actionMeasure = new QAction(MainWindow);
        actionMeasure->setObjectName(QStringLiteral("actionMeasure"));
        actionMeasure->setCheckable(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        centralwidget->setMinimumSize(QSize(0, 0));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        xAxisLabel = new QLabel(centralwidget);
        xAxisLabel->setObjectName(QStringLiteral("xAxisLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(xAxisLabel->sizePolicy().hasHeightForWidth());
        xAxisLabel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(xAxisLabel, 7, 1, 1, 1, Qt::AlignHCenter);

        yAxisLabel = new QLabel(centralwidget);
        yAxisLabel->setObjectName(QStringLiteral("yAxisLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(yAxisLabel->sizePolicy().hasHeightForWidth());
        yAxisLabel->setSizePolicy(sizePolicy1);
        yAxisLabel->setTextFormat(Qt::RichText);

        gridLayout->addWidget(yAxisLabel, 0, 0, 7, 1);

        placeHolder = new QWidget(centralwidget);
        placeHolder->setObjectName(QStringLiteral("placeHolder"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(placeHolder->sizePolicy().hasHeightForWidth());
        placeHolder->setSizePolicy(sizePolicy2);
        placeHolder->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(placeHolder, 0, 1, 7, 1);

        measures = new QLabel(centralwidget);
        measures->setObjectName(QStringLiteral("measures"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(measures->sizePolicy().hasHeightForWidth());
        measures->setSizePolicy(sizePolicy3);

        gridLayout->addWidget(measures, 5, 2, 3, 3, Qt::AlignTop);

        selectedPoints = new QListView(centralwidget);
        selectedPoints->setObjectName(QStringLiteral("selectedPoints"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(selectedPoints->sizePolicy().hasHeightForWidth());
        selectedPoints->setSizePolicy(sizePolicy4);
        selectedPoints->setSelectionMode(QAbstractItemView::SingleSelection);

        gridLayout->addWidget(selectedPoints, 3, 2, 2, 3);

        seriesCombo = new QComboBox(centralwidget);
        seriesCombo->setObjectName(QStringLiteral("seriesCombo"));
        sizePolicy.setHeightForWidth(seriesCombo->sizePolicy().hasHeightForWidth());
        seriesCombo->setSizePolicy(sizePolicy);
        seriesCombo->setMinimumSize(QSize(0, 35));

        gridLayout->addWidget(seriesCombo, 0, 2, 2, 2, Qt::AlignTop);

        configureButton = new QPushButton(centralwidget);
        configureButton->setObjectName(QStringLiteral("configureButton"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(configureButton->sizePolicy().hasHeightForWidth());
        configureButton->setSizePolicy(sizePolicy5);
        configureButton->setMinimumSize(QSize(0, 35));
        QIcon icon;
        QString iconThemeName = QStringLiteral("configure");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QStringLiteral(""), QSize(), QIcon::Normal, QIcon::Off);
        }
        configureButton->setIcon(icon);

        gridLayout->addWidget(configureButton, 0, 4, 2, 1, Qt::AlignTop);

        deleteButton = new QPushButton(centralwidget);
        deleteButton->setObjectName(QStringLiteral("deleteButton"));
        deleteButton->setMinimumSize(QSize(0, 35));
        QIcon icon1;
        iconThemeName = QStringLiteral("edit-delete");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QStringLiteral(""), QSize(), QIcon::Normal, QIcon::Off);
        }
        deleteButton->setIcon(icon1);

        gridLayout->addWidget(deleteButton, 2, 4, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 388, 31));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actionY_zoom);
        toolBar->addAction(actionX_zoom);
        toolBar->addAction(actionPan);
        toolBar->addAction(actionMeasure);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionY_zoom->setText(QApplication::translate("MainWindow", "Y zoom", 0));
        actionX_zoom->setText(QApplication::translate("MainWindow", "X zoom", 0));
        actionPan->setText(QApplication::translate("MainWindow", "Pan", 0));
        actionMeasure->setText(QApplication::translate("MainWindow", "Measure", 0));
        xAxisLabel->setText(QApplication::translate("MainWindow", "X axis", 0));
        yAxisLabel->setText(QApplication::translate("MainWindow", "Y axis", 0));
        measures->setText(QString());
        configureButton->setText(QApplication::translate("MainWindow", "Configure", 0));
        deleteButton->setText(QApplication::translate("MainWindow", "Delete", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
