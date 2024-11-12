#ifndef BLOCK_H
#define BLOCK_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimeLine>

class Block : public QWidget
{
    Q_OBJECT

    const int animLength = 500;
    const int spawnAnimLength = 100;

    int value;
    QPalette back = QPalette(); //background color
    QPalette text = QPalette(); //text color
    QLabel *label;

    QTimeLine *timeline = new QTimeLine(animLength, this);
    QPropertyAnimation *anim = new QPropertyAnimation(this, "pos", this);
    QPropertyAnimation *sizeAnim = new QPropertyAnimation(this, "size", this);
    QPropertyAnimation *labelAnim;

    bool doubled = false;
    int oldX, oldY, newX, newY;
    bool merge, deleteMe; //merge animation, is block marked for deletion

    void refreshColor();
    void animate();
    void spawnAnim();

public slots:
    void onFrameChanged(int frame);

public:
    explicit Block(int x, int y, int value, QWidget *parent = nullptr);

    void updateBlock();
    void doubleValue();
    void setDestination(int x, int y, bool merge = false, bool deleteMe = false); //move animation
    int getValue();

};

#endif // BLOCK_H
