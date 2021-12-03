#include "stackviewer.h"
#include "ui_stackviewer.h"

StackViewer::StackViewer(QWidget *parent)
{

}

StackViewer::StackViewer(Cpu *pcpu, Mmu *pmmu, QWidget *parent):
    QWidget(parent),
    ui(new Ui::StackViewer)
{
    ui->setupUi(this);
    cpu = pcpu;
    mmu =pmmu;
    this->setGeometry(450, 10, 200, 300);
    //this->update();
}

StackViewer::~StackViewer()
{
    delete ui;
}

void StackViewer::update()
{
    ui->listWidget->clear();
    for (uint16_t i = 0xFFFE; i>=cpu->get_curr_SP(); i-=2)
    {
        //TODO: make all number in two digits
        QString tmp = QString::number(i, 16).toUpper() + ": " + QString::number(mmu->read_ram(i), 16).toUpper() + QString::number(mmu->read_ram(i+1), 16).toUpper();
        ui->listWidget->addItem(tmp);
    }
}

void StackViewer::onCpuStepped()
{
    this->update();
}
