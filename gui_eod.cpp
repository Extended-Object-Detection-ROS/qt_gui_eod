#include "gui_eod.h"
#include "./ui_gui_eod.h"
#include <QSyntaxHighlighter>
#include <QFileDialog>
#include <QMessageBox>

gui_eod::gui_eod(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::gui_eod)
{
    ui->setupUi(this);
}

gui_eod::~gui_eod()
{
    delete ui;
}


void gui_eod::on_pb_openImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));
    QPixmap pm(fileName);
    ui->l_image->setPixmap(pm);
    ui->l_image->setScaledContents(true);

}


void gui_eod::on_pb_openBase_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Object Base"), "/home", tr("XML Files (*.xml)"));

    objectBase = new eod::ObjectBase();
    objectBase->loadFromXML(fileName.toStdString());

    QFile base(fileName);
    if(!base.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", base.errorString());
    }
    QTextStream in(&base);
    ui->te_ob_editor->setText(in.readAll());

}

