#include "registerviewer.h"
#include "ui_registerviewer.h"

RegisterViewer::RegisterViewer(QWidget *parent)
{

}

RegisterViewer::RegisterViewer(Cpu *pcpu, Mmu *pmmu, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterViewer)
{
    ui->setupUi(this);
    cpu = pcpu;
    mmu = pmmu;
    loop = false;
    this->setGeometry(270, 10, 200, 300);
    this->update();
}

RegisterViewer::~RegisterViewer()
{
    delete ui;
}

void RegisterViewer::update()
{
    ui->ALabel->setText(QString::number(cpu->get_register('A'), 16).toUpper());
    ui->ALabel->setStyleSheet("background-color: white; qproperty-alignment: AlignCenter;");
    ui->BLabel->setText(QString::number(cpu->get_register('B'), 16).toUpper());
    ui->CLabel->setText(QString::number(cpu->get_register('C'), 16).toUpper());
    ui->DLabel->setText(QString::number(cpu->get_register('D'), 16).toUpper());
    ui->ELabel->setText(QString::number(cpu->get_register('E'), 16).toUpper());
    ui->HLabel->setText(QString::number(cpu->get_register('H'), 16).toUpper());
    ui->LLabel->setText(QString::number(cpu->get_register('L'), 16).toUpper());
    ui->SPLabel->setText(QString::number(cpu->get_curr_SP(), 16).toUpper());
    ui->PCLabel->setText(QString::number(cpu->get_PC(), 16).toUpper());
    ui->ZcheckBox->setCheckState(cpu->get_specialRegister().get_zflag() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    ui->NcheckBox->setCheckState(cpu->get_specialRegister().get_subflag() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    ui->HcheckBox->setCheckState(cpu->get_specialRegister().get_halfcarryflag() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    ui->CcheckBox->setCheckState(cpu->get_specialRegister().get_carryflag() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
}

<<<<<<< HEAD
void RegisterViewer::onCpuStepped()
=======
void RegisterViewer::updateRegisters()
>>>>>>> 9a26bd095dc315697edc3a0680880bcec309c3c4
{
    this->update();
}
