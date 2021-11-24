#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <QWidget>
#include <QMap>
#include "defs.h"
#include <vector>
#include <QTimer>


class Cpu;
class Mmu;


//structure for an opcode
struct opcode
{
    int numberOfBytes;
    QString mnemonic;
};

namespace Ui {
class Disassembler;
}

class Disassembler : public QWidget
{
    Q_OBJECT

public:
    Disassembler(QWidget *parent = nullptr);
    Disassembler(Cpu *pcpu, Mmu *pmmu, QWidget *parent = nullptr);
    ~Disassembler();
    opcode getOpcodeInfos(int index);
    void fillMap();
    void update();

private slots:
    void on_pushButton_clicked();
    //void on_ContinueButton_clicked();
    void on_ContinueButton_clicked();

    void on_pushButton_2_clicked();

    void loop();
signals:
    void cpuStepped();

private:
    Cpu *cpu;
    Mmu *mmu;
    Ui::Disassembler *ui;
    QMap <uint16_t, int> OpMap;
    std::vector<uint8_t> listA;
    std::vector<int> listB;
    bool running;
    int step;
};

#endif // DISASSEMBLER_H
