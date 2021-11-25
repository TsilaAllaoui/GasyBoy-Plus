#ifndef REGISTERVIEWER_H
#define REGISTERVIEWER_H

#include <QWidget>
#include "defs.h"

class Cpu;
class Mmu;

namespace Ui {
class RegisterViewer;
}

class RegisterViewer : public QWidget
{
    Q_OBJECT

public:
    RegisterViewer(QWidget *parent = nullptr);
    RegisterViewer(Cpu *pcpu, Mmu *pmmu, QWidget *parent = nullptr);
    ~RegisterViewer();
    void update();

private:
    Ui::RegisterViewer *ui;
    Cpu *cpu;
    Mmu *mmu;
    bool loop;

public slots:
<<<<<<< HEAD
    void onCpuStepped();
=======
    void updateRegisters();
>>>>>>> 9a26bd095dc315697edc3a0680880bcec309c3c4
};

#endif // REGISTERVIEWER_H
