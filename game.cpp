#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Game)
{
    ui->setupUi(this);

    grid = new Grid(4, this);
    grid->show();
    grid->repaint();
    KeyboardFilter* filter = new KeyboardFilter(this);

    connect(filter, &KeyboardFilter::pressed_arrow, grid, &Grid::move);
    connect(grid, &Grid::gameOver, this, &Game::gameOver);

    ui->centralwidget->installEventFilter(filter);
    ui->centralwidget->setFocusPolicy(Qt::StrongFocus);  // Allows main window to accept focus for key events
    ui->centralwidget->setFocus();


    /*
    grid->testSet(0,0,8, this);
    grid->testSet(0,1,16, this);
    grid->testSet(0,2,8, this);
    grid->testSet(0,3,16, this);
    grid->testSet(1,0,16, this);
    grid->testSet(1,1,8, this);
    grid->testSet(1,2,16, this);
    grid->testSet(1,3,8, this);
    grid->testSet(2,0,8, this);
    grid->testSet(2,1,16, this);
    grid->testSet(2,2,8, this);
    grid->testSet(2,3,16, this);
    grid->testSet(3,0,32, this);
    grid->testSet(3,1,64, this);
    grid->testSet(3,2,32, this);
    */
/*
    grid->testSet(0,0,1024, this);
    grid->testSet(0,1,1024, this);*/

    grid->testSet(2,1,2, this);

}

void Game::gameOver(bool victory){

    QLabel *label = new QLabel(this);
    label->setGeometry(QRect(QPoint(10, 450), QSize(500, 100)));
    label->setFont(QFont("Times", 30, QFont::Bold));

    if(victory)
        label->setText("You won!");
    else
        label->setText("You lost!");

    label->show();
    label->raise();
    this->update();
}

Game::~Game()
{
    delete ui;
}


bool KeyboardFilter::eventFilter( QObject *dist, QEvent *event )
{
    if( event->type() == QEvent::KeyPress )
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>( event );

        if( QString("1234567890").indexOf( keyEvent->text() ) == -1 ) return true;
        else if ( keyEvent->key() == Qt::Key_Up ) {emit pressed_arrow(EDirection::Up); return true;}
        else if ( keyEvent->key() == Qt::Key_Down ) {emit pressed_arrow(EDirection::Down); return true;}
        else if ( keyEvent->key() == Qt::Key_Left ) {emit pressed_arrow(EDirection::Left); return true;}
        else if ( keyEvent->key() == Qt::Key_Right ) {emit pressed_arrow(EDirection::Right); return true;}
    }

    return false;
}
