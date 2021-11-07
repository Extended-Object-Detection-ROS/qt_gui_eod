#include "gui_eod.h"
#include "./ui_gui_eod.h"
#include <QSyntaxHighlighter>

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

