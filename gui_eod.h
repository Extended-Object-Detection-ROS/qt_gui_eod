#ifndef GUI_EOD_H
#define GUI_EOD_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "ObjectBase.h"
#include "XML_highlighter.h"
#include <QFile>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class gui_eod; }
QT_END_NAMESPACE

enum LOG_TYPES {LOG_INFO, LOG_WARN, LOG_ERROR};

class gui_eod : public QMainWindow
{
    Q_OBJECT

public:
    gui_eod(QWidget *parent = nullptr);
    ~gui_eod();

private slots:
    void on_pb_openImage_clicked();

    void on_pb_openBase_clicked();

    void on_pb_detect_clicked();

    void on_cb_check_all_stateChanged(int arg1);

    void on_pb_refresh_clicked();

    void on_l_image_customContextMenuRequested(const QPoint &pos);

    void save_image();

    void on_cb_check_all_complex_clicked();

    void on_te_ob_editor_textChanged();

    void on_pb_save_base_clicked();

private:
    Ui::gui_eod *ui;

    eod::ObjectBase * objectBase = NULL;

    QPixmap source_image;
    QPixmap current_display_image;

    void display_image(QPixmap& im);
    void resizeEvent(QResizeEvent*);

    void check_ready();

    void from_base_to_list_view();

    void display_log(QString log, LOG_TYPES type = LOG_INFO);

    QString last_image_path;

    size_t seq;

    XML_Highlighter *highlighter;
    QElapsedTimer timer;

    QFile* log_file;
    QTextStream* log_stream;
};
#endif // GUI_EOD_H
