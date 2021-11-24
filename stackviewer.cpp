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
    for (uint16_t i = 0xFFFE; i>=cpu->get_curr_SP(); i-=2)
    {
        QString tmp = QString::number(i, 16) + ": " + QString::number(mmu->read_ram(i), 16) + QString::number(mmu->read_ram(i+1), 16);
        ui->listWidget->addItem(tmp);
    }
}
