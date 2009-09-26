/********************************************************************************
** Form generated from reading ui file 'startupdlg.ui'
**
** Created: Sat Sep 26 15:22:46 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STARTUPDLG_H
#define UI_STARTUPDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_Startupdlg
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QComboBox *comboBox;
    QCheckBox *checkBox;
    QLabel *label;
    QGroupBox *groupBox_2;
    QComboBox *comboBox_2;

    void setupUi(QDialog *Startupdlg)
    {
        if (Startupdlg->objectName().isEmpty())
            Startupdlg->setObjectName(QString::fromUtf8("Startupdlg"));
        Startupdlg->setWindowModality(Qt::WindowModal);
        Startupdlg->resize(400, 191);
        Startupdlg->setModal(true);
        buttonBox = new QDialogButtonBox(Startupdlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(290, 20, 81, 241));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBox = new QGroupBox(Startupdlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 261, 81));
        groupBox->setAutoFillBackground(false);
        groupBox->setFlat(false);
        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 30, 131, 27));
        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(150, 30, 93, 23));
        label = new QLabel(Startupdlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(230, 140, 161, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setFrameShape(QFrame::StyledPanel);
        label->setFrameShadow(QFrame::Sunken);
        label->setLineWidth(2);
        label->setOpenExternalLinks(true);
        groupBox_2 = new QGroupBox(Startupdlg);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 80, 241, 51));
        comboBox_2 = new QComboBox(groupBox_2);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(10, 20, 231, 29));

        retranslateUi(Startupdlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), Startupdlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Startupdlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(Startupdlg);
    } // setupUi

    void retranslateUi(QDialog *Startupdlg)
    {
        Startupdlg->setWindowTitle(QApplication::translate("Startupdlg", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Startupdlg", "Screen Resolution", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("Startupdlg", "800x600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Startupdlg", "1024x768", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Startupdlg", "1280x800", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Startupdlg", "1440x900", 0, QApplication::UnicodeUTF8)
        );
        checkBox->setText(QApplication::translate("Startupdlg", "Fullscreen", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Startupdlg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Times New Roman'; font-size:18pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://www.szabster.net/tag/prototype2d\" style=\"text-decoration:none;\"><span style=\" color:#e30000;\">\320\237\321\200\320\276\321\202\320\276\321\202\321\213\320\277\320\2652D</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Startupdlg", "Games", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Startupdlg);
    } // retranslateUi

};

namespace Ui {
    class Startupdlg: public Ui_Startupdlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTUPDLG_H
