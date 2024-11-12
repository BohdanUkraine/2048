#ifndef GRID_H
#define GRID_H

#include <vector>
#include "block.h"
#include <QWidget>

enum EDirection{
    Up,
    Down,
    Left,
    Right
};

class Grid : public QWidget {

    Q_OBJECT

    const QPoint offset = QPoint(10,10);

    int size;
    std::vector<std::vector<Block*>> blocks;
    QWidget *kekparent;
    QPropertyAnimation *anim = new QPropertyAnimation(this, "pos", this);
    bool canMove = true;

public:

    Grid(int gridSize, QWidget *parent = nullptr);

    void testPrint();
    void testSet(int x, int y, int val, QWidget *parent = nullptr);

public slots:
    void spawnBlock();
    void move(EDirection direction);

signals:
    void gameOver(bool victory);

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // GRID_H
