#include "memoryviewer.h"
#include "ui_memoryviewer.h"

MemoryViewer::MemoryViewer(QWidget *parent)
{

}

MemoryViewer::MemoryViewer(Mmu *pmmu, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemoryViewer)
{
    ui->setupUi(this);
    this->setGeometry(5, 280, 600,350);
    divide = 16;
    mmu = pmmu;
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->setColumnCount(divide);
    this->update();
}

MemoryViewer::~MemoryViewer()
{
    delete ui;
}

void MemoryViewer::update()
{
    QStringList Vheaders;
    QString Hhead = "";

    if (ui->comboBox->currentText() == "BOOT")
    {
        ui->tableWidget->setRowCount(BIOS_SIZE / divide);
        Hhead = "BOOT";
        for (int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            Vheaders << Hhead + QString::number(i*divide, 16).toUpper();
            for (int j=0; j<divide; j++)
            {
                uint8_t value = mmu->read_ram(i*divide+j);
                QTableWidgetItem *item = new QTableWidgetItem(QString::number(value, 16).toUpper());
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }
<<<<<<< HEAD
=======

>>>>>>> 9a26bd095dc315697edc3a0680880bcec309c3c4
    else if (ui->comboBox->currentText() == "ROM")
    {
        ui->tableWidget->setRowCount(0x8000 /divide);
        Hhead = "ROM";
        for (int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            Vheaders << Hhead + QString::number(i*divide, 16).toUpper();
            for (int j=0; j<divide; j++)
            {
                uint8_t value = mmu->get_rom()[i*divide+j];
                QTableWidgetItem *item = new QTableWidgetItem(QString::number(value, 16).toUpper());
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }
<<<<<<< HEAD
    else if (ui->comboBox->currentText() == "VRAM")
    {
        ui->tableWidget->setRowCount(0x2000/divide);
        Hhead = "VRAM";
        for (int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            Vheaders << Hhead + QString::number(i*divide, 16).toUpper();
            for (int j=0; j<divide; j++)
            {
                uint8_t value = mmu->get_vram()[i*divide+j];
                QTableWidgetItem *item = new QTableWidgetItem(QString::number(value, 16).toUpper());
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }
    else if (ui->comboBox->currentText() == "EXTRAM")
    {
        ui->tableWidget->setRowCount(0x2000/divide);
        Hhead = "EXTRAM";
        for (int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            Vheaders << Hhead + QString::number(i*divide, 16).toUpper();
            for (int j=0; j<divide; j++)
            {
                uint8_t value = mmu->get_extram()[i*divide+j];
                QTableWidgetItem *item = new QTableWidgetItem(QString::number(value, 16).toUpper());
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }
    else if (ui->comboBox->currentText() == "WRAM")
    {
        ui->tableWidget->setRowCount(0x4000/divide);
        Hhead = "WRAM";
        for (int i=0; i<ui->tableWidget->rowCount(); i++)
        {
            Vheaders << Hhead + QString::number(i*divide, 16).toUpper();
            for (int j=0; j<divide; j++)
            {
                uint8_t value = mmu->get_workingram()[i*divide+j];
                QTableWidgetItem *item = new QTableWidgetItem(QString::number(value, 16).toUpper());
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }
=======
>>>>>>> 9a26bd095dc315697edc3a0680880bcec309c3c4

    ui->tableWidget->setVerticalHeaderLabels(Vheaders);
    for (int i=0; i<ui->tableWidget->columnCount(); i++)
        ui->tableWidget->resizeColumnToContents(i);
}


void MemoryViewer::on_comboBox_currentIndexChanged(const int value)
{
    this->update();
}

