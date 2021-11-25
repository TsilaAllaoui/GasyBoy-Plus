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

<<<<<<< HEAD
using namespace std;

=======
>>>>>>> 9a26bd095dc315697edc3a0680880bcec309c3c4
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
<<<<<<< HEAD
    void on_ContinueButton_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_2_clicked();

signals:
    void cpuStepped();
    void emuReset();
=======
    //void on_ContinueButton_clicked();
    void on_ContinueButton_clicked();

    void on_pushButton_2_clicked();

    void loop();
signals:
    void cpuStepped();
>>>>>>> 9a26bd095dc315697edc3a0680880bcec309c3c4

private:
    Cpu *cpu;
    Mmu *mmu;
    Ui::Disassembler *ui;
    QMap <uint16_t, int> OpMap;
    std::vector<uint8_t> listA;
    std::vector<int> listB;
    bool running;
<<<<<<< HEAD
    vector<int> breakPoints;
=======
    int step;
>>>>>>> 9a26bd095dc315697edc3a0680880bcec309c3c4
};

#endif // DISASSEMBLER_H
