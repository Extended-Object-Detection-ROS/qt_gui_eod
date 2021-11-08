#include "gui_eod.h"
#include "./ui_gui_eod.h"
#include <QSyntaxHighlighter>
#include <QFileDialog>
#include <QMessageBox>
#include <asmOpenCV.h>

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

void gui_eod::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   display_image(current_display_image);
}

void gui_eod::on_pb_openImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));
    source_image = QPixmap(fileName);
    display_image(source_image);
}

void gui_eod::display_image(QPixmap& im)
{
    current_display_image = im;
    int lh = ui->l_image->size().height();
    int lw = ui->l_image->size().width();

    QPixmap scaled = im.scaled(lw, lh, Qt::KeepAspectRatio);

    ui->l_image->setPixmap(scaled);
    //ui->l_image->setScaledContents(true);
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

void gui_eod::on_pb_detect_clicked()
{
    cv::Mat frame = ASM::QPixmapToCvMat(source_image);
    cv::Mat image2draw = frame.clone();

    for(auto so : objectBase->simple_objects){
        so->Identify(frame, cv::Mat());
        so->draw(image2draw,cv::Scalar(0,255,0));
    }
    QPixmap detected_image = ASM::cvMatToQPixmap(image2draw);
    display_image(detected_image);
}

