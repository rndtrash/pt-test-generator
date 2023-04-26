#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>

#include <libdocx/libdocx.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    try {
        libdocx::DocX docx("test.docx");
    } catch(std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

