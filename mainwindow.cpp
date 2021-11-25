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

<<<<<<< HEAD
    connect(disassembler, SIGNAL(cpuStepped()), registerviewer, SLOT(onCpuStepped()));
    connect(disassembler, SIGNAL(cpuStepped()), stackviewer, SLOT(onCpuStepped()));
=======
    connect(disassembler, SIGNAL(cpuStepped()), registerviewer, SLOT(updateRegisters()));
>>>>>>> 9a26bd095dc315697edc3a0680880bcec309c3c4
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
<<<<<<< HEAD

void MainWindow::reset()
{
    delete mmu;
    delete cpu;
    /*delete disassembler;
    delete registerviewer;
    delete memoryviewer;
    delete stackviewer;*/
    mmu = new Mmu();
    cpu = new Cpu(mmu);
    disassembler = new Disassembler(cpu, mmu, this);
    registerviewer = new RegisterViewer(cpu, mmu, this);
    memoryviewer = new MemoryViewer(mmu, this);
    stackviewer = new StackViewer(cpu, mmu, this);
    this->update();
    this->show();
}
=======
>>>>>>> 9a26bd095dc315697edc3a0680880bcec309c3c4
