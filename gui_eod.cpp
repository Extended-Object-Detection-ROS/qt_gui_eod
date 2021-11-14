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
    ui->l_image->setContextMenuPolicy(Qt::CustomContextMenu);
    seq = 0;

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
    last_image_path = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    source_image = QPixmap(last_image_path);

    if( source_image.isNull() ){
        last_image_path = "";
        return;
    }

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
    if(objectBase != NULL && !source_image.isNull()  ){
        ui->pb_detect->setEnabled(true);
    }
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
    ui->cb_check_all->setEnabled(true);
#ifdef USE_IGRAPH
    ui->cb_check_all_complex->setEnabled(true);
#endif
    // to editor
    QFile base(fileName);
    if(!base.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", base.errorString());
    }
    QTextStream in(&base);
    ui->te_ob_editor->setText(in.readAll());
    ui->pb_refresh->setEnabled(false);

    check_ready();
}

void gui_eod::on_pb_detect_clicked()
{
    cv::Mat frame = ASM::QPixmapToCvMat(source_image);
    cv::Mat image2draw = frame.clone();

    seq ++;

    // TODO: from editor to object base

    for( int i = 0 ; i < objectBase->simple_objects.size(); i++){
        if(ui->lw_objects->item(i)->checkState() == Qt::Checked){
            objectBase->simple_objects[i]->Identify(frame, cv::Mat(), seq);
            objectBase->simple_objects[i]->draw(image2draw,cv::Scalar(0,255,0));
        }
    }
#ifdef USE_IGRAPH
    for( int i = 0 ; i < objectBase->complex_objects_graph.size(); i++){
        if(ui->lw_objects->item(i+objectBase->simple_objects.size())->checkState() == Qt::Checked){
            objectBase->complex_objects_graph[i]->Identify(frame, cv::Mat(), seq);
            objectBase->complex_objects_graph[i]->drawAll(image2draw, cv::Scalar(255,255,0), 2);
        }
    }
#endif

    QPixmap detected_image = ASM::cvMatToQPixmap(image2draw);
    display_image(detected_image);
}

void gui_eod::from_base_to_list_view(){
    if( !objectBase )
        return;
    ui->lw_objects->clear();
    for(auto so : objectBase->simple_objects){
        ui->lw_objects->addItem(QString::fromStdString(so->name));
    }
#ifdef USE_IGRAPH
    for(auto co : objectBase->complex_objects_graph){
        ui->lw_objects->addItem(QString::fromStdString(co->name));
    }
#endif
    QListWidgetItem* item = 0;
    //for(int i = 0; i < ui->lw_objects->count(); ++i){
    for(int i = 0; i < objectBase->simple_objects.size(); i++){
        item = ui->lw_objects->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        ui->cb_check_all->isChecked() ? item->setCheckState(Qt::Checked) : item->setCheckState(Qt::Unchecked);
    }
    for(int i = 0; i < objectBase->complex_objects_graph.size(); i++){
        item = ui->lw_objects->item(i + objectBase->simple_objects.size());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        ui->cb_check_all->isChecked() ? item->setCheckState(Qt::Checked) : item->setCheckState(Qt::Unchecked);
    }

}

void gui_eod::on_cb_check_all_stateChanged(int arg1)
{
    Qt::CheckState state = arg1 ? Qt::Checked : Qt::Unchecked;    
    for(int i = 0; i < objectBase->simple_objects.size(); i++){
        ui->lw_objects->item(i)->setCheckState(state);
    }

}

void gui_eod::on_pb_refresh_clicked()
{
    ui->pb_refresh->setEnabled(false);
    objectBase->clear();
    objectBase->loadFromTextData(ui->te_ob_editor->toPlainText().toStdString(),  objectBase->getPath());
    from_base_to_list_view();
}


void gui_eod::on_l_image_customContextMenuRequested(const QPoint &pos)
{
    if( current_display_image.isNull())
        return;
    if( last_image_path == "") // should not be is current_display_image is not null, but for extra safety
        return;

    QMenu contextMenu(tr("Context menu"), ui->l_image);
    QAction action1("Save image as...", ui->l_image);

    connect(&action1, SIGNAL(triggered()), this, SLOT(save_image()));
    contextMenu.addAction(&action1);

    //contextMenu.exec(mapToGlobal(pos));// wrong pose
    contextMenu.exec(mapToGlobal(pos) + QPoint(ui->gb_control->size().width(), 0));// kostylish but best
    //contextMenu.exec(mapFromParent(pos));// too wrong pose
    //contextMenu.exec(pos);//wrong pose
    //contextMenu.exec(mapTo(ui->l_image,pos));//crashes
    //contextMenu.exec(ui->l_image->viewport()->mapToGlobal(pos));
}

void gui_eod::save_image(){

    std::string stdstr = last_image_path.toStdString();
    std::size_t lastIndex = stdstr.find_last_of(".");
    stdstr.insert(lastIndex, "_detected");

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    QString::fromStdString(stdstr),
                                                    tr("Images (*.png *.bmp *.jpg *.jpeg)"));
    current_display_image.save(fileName);
}


void gui_eod::on_cb_check_all_complex_clicked()
{
    Qt::CheckState state = ui->cb_check_all_complex->isChecked() ? Qt::Checked : Qt::Unchecked;
    for(int i = 0; i < objectBase->complex_objects_graph.size(); i++){
        ui->lw_objects->item(i + objectBase->simple_objects.size())->setCheckState(state);
    }
}


void gui_eod::on_te_ob_editor_textChanged()
{
    ui->pb_refresh->setEnabled(true);
}

