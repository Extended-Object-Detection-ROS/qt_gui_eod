#ifndef GUI_EOD_H
#define GUI_EOD_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "ObjectBase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class gui_eod; }
QT_END_NAMESPACE

class gui_eod : public QMainWindow
{
    Q_OBJECT

public:
    gui_eod(QWidget *parent = nullptr);
    ~gui_eod();

private slots:
    void on_pb_openImage_clicked();

    void on_pb_openBase_clicked();

private:
    Ui::gui_eod *ui;

    eod::ObjectBase * objectBase;
};
#endif // GUI_EOD_H
