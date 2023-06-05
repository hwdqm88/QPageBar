#include "widget.h"
#include "qpagebar.h"
#include <QVBoxLayout>
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pageBar->setCount(5, 3);
    connect(ui->pageBar, &QPageBar::pageChanged, this, [&](int idx) {
        ui->stackedWidget->setCurrentIndex(idx - 1);
    });
}

Widget::~Widget()
{
    delete ui;
}

