#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mmu = new Mmu();
    cpu = new Cpu(mmu);
    disassembler = new Disassembler(cpu, mmu, this);
    registerviewer = new RegisterViewer(cpu, mmu, this);
    memoryviewer = new MemoryViewer(mmu, this);
    stackviewer = new StackViewer(cpu, mmu, this);

    connect(disassembler, SIGNAL(cpuStepped()), registerviewer, SLOT(updateRegisters()));
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    registerviewer->update();
    memoryviewer->update();
    stackviewer->update();
    this->show();
}
