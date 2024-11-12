#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "block.h"
#include "grid.h"
#include <QKeyEvent>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui {
class Game;
}
QT_END_NAMESPACE

class Game : public QMainWindow
{
    Q_OBJECT

    Grid* grid;

public:
    Game(QWidget *parent = nullptr);
    ~Game();

public slots:
    void gameOver(bool victory);

private:
    Ui::Game *ui;
};


class KeyboardFilter : public QObject
{
    Q_OBJECT

public:
    KeyboardFilter( QObject *parent = nullptr ) : QObject( parent ) {}

signals:
    void pressed_arrow(EDirection direction);

protected:
    bool eventFilter( QObject *dist, QEvent *event );

};


#endif // GAME_H
