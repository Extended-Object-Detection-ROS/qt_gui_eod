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
        tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    source_image = QPixmap(fileName);

    if( source_image.isNull() )
        return;

    display_image(source_image);
    check_ready();
}

void gui_eod::display_image(QPixmap& im)
{
    current_display_image = im;
    int lh = ui->l_image->size().height();
    int lw = ui->l_image->size().width();

    QPixmap scaled = im.scaled(lw, lh, Qt::KeepAspectRatio);

    ui->l_image->setPixmap(scaled);
}

void gui_eod::check_ready(){
    if(objectBase != NULL && !source_image.isNull()  )
        ui->pb_detect->setEnabled(true);
}

void gui_eod::on_pb_openBase_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Object Base"), "/home", tr("XML Files (*.xml)"));

    objectBase = new eod::ObjectBase();
    if( !objectBase->loadFromXML(fileName.toStdString()) ){
        objectBase = NULL;
        return;
    }

    from_base_to_list_view();

    // to editor
    QFile base(fileName);
    if(!base.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", base.errorString());
    }
    QTextStream in(&base);
    ui->te_ob_editor->setText(in.readAll());

    check_ready();
}

void gui_eod::on_pb_detect_clicked()
{
    cv::Mat frame = ASM::QPixmapToCvMat(source_image);
    cv::Mat image2draw = frame.clone();

    // TODO: from editor to object base

    //for(auto so : objectBase->simple_objects){
    for( int i = 0 ; i < objectBase->simple_objects.size(); i++){
        if(ui->lw_objects->item(i)->checkState() == Qt::Checked){
            objectBase->simple_objects[i]->Identify(frame, cv::Mat());
            objectBase->simple_objects[i]->draw(image2draw,cv::Scalar(0,255,0));
        }
    }
    QPixmap detected_image = ASM::cvMatToQPixmap(image2draw);
    display_image(detected_image);
}

void gui_eod::from_base_to_list_view(){
    if( !objectBase )
        return;
    for(auto so : objectBase->simple_objects){
        //ui->lv_objects->insertAction()
        ui->lw_objects->addItem(QString::fromStdString(so->name));
    }
    QListWidgetItem* item = 0;
    for(int i = 0; i < ui->lw_objects->count(); ++i){
        item = ui->lw_objects->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }

}
