#ifndef STACKVIEWER_H
#define STACKVIEWER_H

#include <QWidget>
#include "defs.h"

class Mmu;
class Cpu;

namespace Ui {
class StackViewer;
}

class StackViewer : public QWidget
{
    Q_OBJECT

public:
    StackViewer(QWidget *parent = nullptr);
    StackViewer(Cpu *pcpu, Mmu *pmmu, QWidget *parent = nullptr);
    ~StackViewer();
    void update();

<<<<<<< HEAD
public slots:
    void onCpuStepped();

=======
>>>>>>> 9a26bd095dc315697edc3a0680880bcec309c3c4
private:
    Ui::StackViewer *ui;
    Mmu *mmu;
    Cpu *cpu;
};

#endif // STACKVIEWER_H
