#include "grid.h"
#include <iostream>
#include <qglobal.h>
#include <QPainter>

Grid::Grid(int gridSize, QWidget *parent)
:QWidget(parent)
{
    size = gridSize;
    kekparent = parent;

    for(int i=0; i<size; i++){
        blocks.push_back(std::vector<Block*>());
        for(int j=0; j<size; j++)
            blocks[i].push_back(nullptr);
    }


    anim->setDuration(500);
    connect(anim, &QPropertyAnimation::finished, this, &Grid::spawnBlock);

}


void Grid::paintEvent(QPaintEvent *event){

    resize(QSize(1000, 1000));
    setAutoFillBackground(true);

    setPalette(QPalette(QPalette::Window, QColor(0x808080)));

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 10, Qt::SolidLine));
    painter.drawLine(0, 0, 0, 550);


    for (int x = 5; x <= 540; x += 110) {
        painter.drawLine(QPoint(x,0),QPoint(x,445));
    }

    for (int y = 5; y <= 540; y += 110) {
        painter.drawLine(QPoint(0,y),QPoint(445,y));
    }


    /*
    QPainter painter(this);
    painter.setPen(QPen(Qt::black));


    painter.drawLine(QPoint(0,0),QPoint(550,550));



    painter.end();*/
}

void Grid::move(EDirection direction){

    if(canMove){

    bool inv = false, anyMoveDone = false;
    int firstStart, firstFinish, iF, secondStart, secondFinish, iS, x, y, row, column;

    std::vector<Block*> del;
    std::vector<QPoint> processing;

    switch (direction){
        case EDirection::Down:
            firstStart = 0;
            firstFinish = size;
            iF = 1;
            secondStart = size-1;
            secondFinish = -1;
            iS = -1;
            inv = false;
            break;
        case EDirection::Up:
            firstStart = 0;
            firstFinish = size;
            iF = 1;
            secondStart = 0;
            secondFinish = size;
            iS = 1;
            inv = false;
            break;
        case EDirection::Left:
            firstStart = 0;
            firstFinish = size;
            iF = 1;
            secondStart = 0;
            secondFinish = size;
            iS = 1;
            inv = true;
            break;
        case EDirection::Right:
            firstStart = 0;
            firstFinish = size;
            iF = 1;
            secondStart = size-1;
            secondFinish = -1;
            iS = -1;
            inv = true;
            break;

    }


    for(int i=firstStart; i!=firstFinish; i+=iF){

        processing.clear();

        for(int j=secondStart; j!=secondFinish; j+=iS){

            if(!inv){
                row = i;
                column = j;
            }else{
                row = j;
                column = i;
            }

            processing.push_back(QPoint(row, column));
            QPoint first = processing.front();

            if(blocks[row][column] != nullptr){
                if(blocks[row][column] == blocks[first.x()][first.y()]){
                    x = first.x()*100 + first.x()*10 + offset.x();
                    y = first.y()*100 + first.y()*10 + offset.y();

                    blocks[row][column]->setDestination(x, y);
                }else
                if(blocks[first.x()][first.y()] == nullptr){
                    //if everything in front of this block is empty - move block to first cell

                    anyMoveDone = true;

                    x = first.x()*100 + first.x()*10 + offset.x();
                    y = first.y()*100 + first.y()*10 + offset.y();

                    blocks[row][column]->setDestination(x, y);

                    std::swap(blocks[first.x()][first.y()], blocks[row][column]);

                }else

                if(blocks[first.x()][first.y()]->getValue() != blocks[row][column]->getValue()){
                    //if blocks have different values then move current to next cell after first

                    processing.erase(processing.begin());

                    first = processing.front();

                    if(first.x() != row || first.y() != column)
                        anyMoveDone = true;

                    x = first.x()*100 + first.x()*10 + offset.x();
                    y = first.y()*100 + first.y()*10 + offset.y();

                    blocks[row][column]->setDestination(x, y);

                    std::swap(blocks[first.x()][first.y()], blocks[row][column]);


                } else

                if(blocks[first.x()][first.y()]->getValue() == blocks[row][column]->getValue()){
                    //move and merge

                    anyMoveDone = true;

                    x = first.x()*100 + first.x()*10 + offset.x();
                    y = first.y()*100 + first.y()*10 + offset.y();

                    blocks[first.x()][first.y()]->setDestination(x, y, false, true);

                    blocks[row][column]->setDestination(x, y, true); //move doubled block to first position
                    blocks[row][column]->raise();

                    del.push_back(blocks[first.x()][first.y()]);

                    blocks[first.x()][first.y()] = blocks[row][column];
                    blocks[row][column] = nullptr;


                    processing.erase(processing.begin());
                }
            }

        }

        processing.clear();

    }


    if(anyMoveDone){
        for(int i=0; i<size; i++)
            for(int j=0; j<size; j++)
                if(blocks[i][j] != nullptr)
                    blocks[i][j]->updateBlock();



        while(!del.empty()){
            del.back()->updateBlock();
            del.pop_back();
        }


        anim->start();
        canMove = false;
    }

    }
}

void Grid::spawnBlock(){

    std::vector<QPoint> freeCells;

    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            if(blocks[i][j] == nullptr)
                freeCells.push_back(QPoint(i,j));


    QPoint spawn;
    if(!freeCells.empty())
        spawn = freeCells[qrand()%freeCells.size()];


    int coord1 = spawn.x()*100 + spawn.x()*10 + offset.x();
    int coord2 = spawn.y()*100 + spawn.y()*10 + offset.y();
    blocks[spawn.x()][spawn.y()] = new Block(coord1, coord2, (qrand() % ((2 + 1) - 1) + 1)*2, kekparent);

    kekparent->update();


    if(freeCells.size() == 1){
        //check for game over (if player has any moves)
        bool flag = false;
        for(int i=1; i<size; i++)
            for(int j=1; j<size; j++) {
                if(flag)
                    break;
                if(blocks[i-1][j] != nullptr && blocks[i][j] != nullptr)
                    if(blocks[i-1][j]->getValue() == blocks[i][j]->getValue())
                        flag = true;

                if(blocks[i][j-1] != nullptr && blocks[i][j] != nullptr)
                    if(blocks[i][j-1]->getValue() == blocks[i][j]->getValue())
                        flag = true;
            }
        if(!flag)
            emit gameOver(false);
    }

    bool win = false;

    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            if(blocks[i][j] != nullptr)
                if(blocks[i][j]->getValue() == 2048)
                    win = true;

    if(win)
        emit gameOver(true);

    canMove = true;
}

void Grid::testSet(int x, int y, int val, QWidget *parent){

    int coord1 = x*100 + x*10 + offset.x();
    int coord2 = y*100 + y*10 + offset.y();

    blocks[x][y] = new Block(coord1, coord2, val, parent);

    //blocks[x][y]->setDestination(coord1+110, coord2, true);
}

void Grid::testPrint(){
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(blocks[j][i] != nullptr){
                std::cout << blocks[j][i]->getValue() << ' ';
            }else{
                std::cout << "0 ";
            }
        }
        std::cout << '\n';
    }
}
