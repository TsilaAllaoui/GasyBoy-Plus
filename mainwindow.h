#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "defs.h"


class Mmu;
class Cpu;
class Disassembler;
class RegisterViewer;
class MemoryViewer;
class StackViewer;
class VramViewer;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void update();

public slots:
    void reset();

private:
    Ui::MainWindow *ui;

private:
    QStringListModel *opcodes;
    Cpu *cpu;
    Mmu *mmu;
    Disassembler *disassembler;
    RegisterViewer *registerviewer;
    MemoryViewer *memoryviewer;
    StackViewer *stackviewer;
    VramViewer *vramviewer;

};
#endif // MAINWINDOW_H
