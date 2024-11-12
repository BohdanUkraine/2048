#include "block.h"
#include <QColor>
#include <iostream>
#include <QWidget>
#include <QLayout>


Block::Block(int x, int y, int value, QWidget *parent)
    : QWidget{parent}
{
    oldX = x;
    oldY = y;
    this->value = value;

    //label pre-sets
    label = new QLabel(this);
    label->resize(100, 100);
    label->setFont(QFont("Times", 30, QFont::Bold));
    label->setAlignment(Qt::AlignCenter);
    label->setText(QString::number(value));

    //animation pre-sets

    timeline->setFrameRange(0, 100);
    timeline->setUpdateInterval(5);
    connect(timeline, &QTimeLine::frameChanged, this, &Block::onFrameChanged);

    anim->setEasingCurve(QEasingCurve::OutQuad);
    anim->setDuration(animLength);

    sizeAnim->setEasingCurve(QEasingCurve::OutQuad);
    sizeAnim->setDuration(animLength);

    labelAnim = new QPropertyAnimation(label, "pos");
    labelAnim->setEasingCurve(QEasingCurve::OutQuad);
    labelAnim->setDuration(animLength);

    setAutoFillBackground(true);
    refreshColor();
    this->setGeometry(QRect(QPoint(x, y), QSize(100, 100)));

    this->show();
    spawnAnim();

}

void Block::onFrameChanged(int frame){

    if (frame >= 85 && !doubled && !deleteMe){
        doubleValue();
        label->setText(QString::number(value));
        refreshColor();
        doubled = true;
    }
}

int Block::getValue(){
    return this->value;
}

void Block::spawnAnim(){
    anim->setDuration(spawnAnimLength);
    sizeAnim->setDuration(spawnAnimLength);
    labelAnim->setDuration(spawnAnimLength);

    anim->setKeyValueAt(0, QPoint(oldX, oldY));
    anim->setKeyValueAt(0.3, QPoint(oldX, oldY));
    anim->setKeyValueAt(0.6, QPoint(oldX-5, oldY-5));
    anim->setKeyValueAt(0.1, QPoint(oldX, oldY));

    sizeAnim->setKeyValueAt(0.3, QSize(100, 100));
    sizeAnim->setKeyValueAt(0.6, QSize(110, 110));
    sizeAnim->setKeyValueAt(0.1, QSize(100, 100));

    labelAnim->setKeyValueAt(0.3, QPoint(0, 0));
    labelAnim->setKeyValueAt(0.6, QPoint(5, 5));
    labelAnim->setKeyValueAt(0.1, QPoint(0, 0));


    anim->start();
    sizeAnim->start();
    labelAnim->start();
}

void Block::refreshColor(){

    //SET BACKGROUND COLOR

    switch (this->value){

    case 2:
        back.setColor(QPalette::Window, QColor(0xeee2d0));
        break;
    case 4:
        back.setColor(QPalette::Window, QColor(0xede0c8));
        break;
    case 8:
        back.setColor(QPalette::Window, QColor(0xf2b179));
        break;
    case 16:
        back.setColor(QPalette::Window, QColor(0xf59563));
        break;
    case 32:
        back.setColor(QPalette::Window, QColor(0xf67c5f));
        break;
    case 64:
        back.setColor(QPalette::Window, QColor(0xf65e3b));
        break;
    case 128:
        back.setColor(QPalette::Window, QColor(0xedcf72));
        break;
    case 256:
        back.setColor(QPalette::Window, QColor(0xedcc61));
        break;
    case 512:
        back.setColor(QPalette::Window, QColor(0xedc750));
        break;
    case 1024:
        back.setColor(QPalette::Window, QColor(0xedc43f));
        break;
    case 2048:
        back.setColor(QPalette::Window, QColor(0xedc22e));
        break;
    default:
        back.setColor(QPalette::Window, Qt::black);
        break;
    }

    this->setPalette(back);

    //SET TEXT COLOR

    if(this->value == 2 || this->value == 4)
        text.setColor(QPalette::Window, Qt::black);
    else
        text.setColor(QPalette::Window, Qt::white);

}

void Block::updateBlock(){

    doubled = false;
    animate();

    anim->start();
    if(merge){
        timeline->start();
        sizeAnim->start();
        labelAnim->start();
    }

    if(deleteMe)
        connect(anim, &QPropertyAnimation::finished, this, &QWidget::deleteLater);

    merge = false;
    oldX = newX;
    oldY = newY;
}

void Block::doubleValue(){
    value *= 2;
}

void Block::animate(){

    anim->setKeyValues({});
    sizeAnim->setKeyValues({});
    labelAnim->setKeyValues({});

    anim->setDuration(animLength);
    sizeAnim->setDuration(animLength);
    labelAnim->setDuration(animLength);

    if(deleteMe || !merge){
        anim->setKeyValueAt(0, QPoint(oldX, oldY));
        anim->setKeyValueAt(0.7, QPoint(newX, newY));
        anim->setKeyValueAt(1, QPoint(newX, newY));
    }else
        if(merge){
            anim->setKeyValueAt(0, QPoint(oldX, oldY));
            anim->setKeyValueAt(0.7, QPoint(newX, newY));
            anim->setKeyValueAt(0.85, QPoint(newX-5, newY-5));
            anim->setKeyValueAt(1, QPoint(newX, newY));

            sizeAnim->setKeyValueAt(0.7, QSize(100, 100));
            sizeAnim->setKeyValueAt(0.85, QSize(110, 110));
            sizeAnim->setKeyValueAt(1, QSize(100, 100));

            //TO KEEP LABEL CENTERED DURING MERGE
            labelAnim->setKeyValueAt(0.7, QPoint(0, 0));
            labelAnim->setKeyValueAt(0.85, QPoint(5, 5));
            labelAnim->setKeyValueAt(1, QPoint(0, 0));
        }
}

void Block::setDestination(int x, int y, bool merge, bool deleteMe){
    newX = x;
    newY = y;
    this->merge = merge;
    this->deleteMe = deleteMe;
}
