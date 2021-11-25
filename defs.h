#pragma once

#include <QStringList>
#include <QStringListModel>
#include <QListWidget>
#include <QAbstractItemView>
#include <QDebug>
#include <QMessageBox>
#include <vector>

#include "cpu.h"
#include "register.h"
#include "mmu.h"
#include "disassembler.h"
#include "registerviewer.h"
#include "memoryviewer.h"
#include "stackviewer.h"

#define MAXCYCLE 70224
#define WIDTH 160
#define HEIGHT 144
#define VRAM_WIDTH 128
#define VRAM_HEIGHT 192
#define BG_MAP_WIDTH 256
#define BG_MAP_HEIGTH 256
#define PX 2
#define SPEED 3.6
#define BIOS_SIZE 0xFF
#define VARM_SIZE 0x2000
#define ERAM_SIZE 0x2000
#define WRAM_SIZE 0x4000

/*****TEMPORARY DIMENSION*****/
/* TODO: CHANGE TO REAL GAMMEBOY RESOLUTION */
#define HEIGHT_TEMP 768
#define WIDTH_TEMP 1024


/*#ifndef REGISTERVIEWER_H
#define REGISTERVIEWER_H

//#include <QMainWindow>
#include "defs.h"
#include <QHBoxLayout>

class Cpu;
class Mmu;

//namespace Ui {
//class RegisterViewer;
//}

class RegisterViewer : public QWidget
{
    Q_OBJECT

public:
    RegisterViewer(Cpu *cpu, Mmu *mmu, QMainWindow *MainWindow);
    ~RegisterViewer();

private:
    //Ui::RegisterViewer *ui;
    Mmu *mmu;
    Cpu *cpu;
    QHBoxLayout *layout;
};

#endif // REGISTERVIEWER_H
*/




/*#include "registerviewer.h"
#include "ui_registerviewer.h"
#include <QPushButton>

RegisterViewer::RegisterViewer(Cpu *cpu, Mmu *mmu, QMainWindow *MainWindow) //:
    //ui(new Ui::RegisterViewer)
{
    //ui->setupUi(MainWindow);
    layout = new QHBoxLayout(MainWindow);
    QPushButton *btn = new QPushButton("Push");
    layout->addWidget(btn);
}

RegisterViewer::~RegisterViewer()
{
    //delete ui;
}
*/


