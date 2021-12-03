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
    vramviewer = new VramViewer(mmu, this);

    connect(disassembler, SIGNAL(cpuStepped()), registerviewer, SLOT(onCpuStepped()));
    connect(disassembler, SIGNAL(cpuStepped()), stackviewer, SLOT(onCpuStepped()));
    connect(mmu, SIGNAL(vramWrittenWithWord(uint16_t)), memoryviewer, SLOT(onMmuWritten(uint16_t)));
    connect(mmu, SIGNAL(vramWrittenWithWord(uint16_t)), vramviewer, SLOT(onMmuWritten(uint16_t)));
    connect(cpu, SIGNAL(cpuStepped()), disassembler, SLOT());
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    registerviewer->update();
    stackviewer->update();
    this->show();
}

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
