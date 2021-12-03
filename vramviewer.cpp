#include "vramviewer.h"
#include "ui_vramviewer.h"
#include <QGraphicsRectItem>

VramViewer::VramViewer(QWidget *parent)
{

}

VramViewer::VramViewer(Mmu *pmmu, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VramViewer)
{
    ui->setupUi(this);
    mmu = pmmu;
    painter = new QPainter(this);
    this->setGeometry(600, 4, 300, 440);
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene, ui->frame);
    view->setScene(scene);
    pixel = new QGraphicsRectItem(0,0,10,10);
    q = new QGraphicsRectItem**[0xC0];
    for (int i=0; i<0xC0; i++)
        q[i] = new QGraphicsRectItem*[0x80];
    for (int i=0; i<0xC0; i++)
    {
        for (int j=0; j<0x80; j++)
        {
            q[i][j] = new QGraphicsRectItem(j*2,i*2,1*2,1*2);
            q[i][j]->setBrush(Qt::white);
            scene->addItem(q[i][j]);
        }
    }
     drawer = new drawerThread(mmu,q);
    this->update();
}

VramViewer::~VramViewer()
{
    delete ui;
}

void VramViewer::onMmuWritten(uint16_t addr)
{
    drawer->setAddr(addr);
    drawer->start();
}

void VramViewer::update()
{
    view->show();
}

void drawerThread::run()
{
    uint8_t first_half = 0;
    uint8_t second_half = 0;
    int i = 0;
    if (addr % 2 == 0)
    {
         first_half = mmu->read_ram(addr);
         second_half = mmu->read_ram(addr+1);
    }
    else
    {
        first_half = mmu->read_ram(addr-1);
        second_half = mmu->read_ram(addr);
    }
    switch (addr & 0xF)
    {
        case 0:
        case 1:
           i = 0; break;
        case 2:
        case 3:
           i = 1; break;
        case 4:
        case 5:
           i = 2; break;
        case 6:
        case 7:
           i = 3; break;
        case 8:
        case 9:
           i = 4; break;
        case 0xA:
        case 0xB:
           i = 5; break;
        case 0xC:
        case 0xD:
           i = 6; break;
        case 0xE:
        case 0xF:
           i = 7; break;

    }
    int rowJump = ((addr & 0xF00) >> 8)*8;
    int colJump = ((addr & 0xF0) >> 4)*8;
    for (int j=0; j<8; j++)
    {
        uint8_t tmp = 1 << (7-j);
        int unary = (first_half & tmp) >> (7-j);
        int binary = (second_half & tmp) >> (7-j);
        if (unary == 0)
        {
            if (binary == 0)
                q[i+rowJump][j+colJump]->setBrush(Qt::white);
            else if (binary == 1)
                q[i+rowJump][j+colJump]->setBrush(QColor(196,196,196));
        }
        else
        {
            if (binary == 1)
              q[i+rowJump][j+colJump]->setBrush(Qt::black);
            else if (binary == 0)
                q[i+rowJump][j+colJump]->setBrush(QColor(54,54,54));
        }
    }
}

drawerThread::drawerThread(Mmu *pmmu, QGraphicsRectItem ***p)
{
    mmu = pmmu;
    q = new QGraphicsRectItem**[0xC0];
    for (int i=0; i<0xC0; i++)
        q[i] = new QGraphicsRectItem*[0x80];
    q = p;
    if (q == p)
        exit(98);
    else exit(25);
}

void drawerThread::setAddr(uint16_t value)
{
    addr = value;
}
