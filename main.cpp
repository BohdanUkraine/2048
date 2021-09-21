#include <iostream>
#include <string>
#include <QString>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <unistd.h>

using namespace sf;

class Grid;

bool game_over(Grid grid);

bool game_win(Grid grid);

void draw(RectangleShape bg,
          RectangleShape _void, RectangleShape gridd,
          RectangleShape shape, Text text, Grid grid);

Color getBlockColor(int num){
    if (num == 2)
        return sf::Color(0xeee2d0ff);
    if (num == 4)
        return sf::Color(0xede0c8ff);
    if (num == 8)
        return sf::Color(0xf2b179ff);
    if (num == 16)
        return sf::Color(0xf59563ff);
    if (num == 32)
        return sf::Color(0xf67c5fff);
    if (num == 64)
        return sf::Color(0xf65e3bff);
    if (num == 128)
        return sf::Color(0xedcf72ff);
    if (num == 256)
        return sf::Color(0xedcc61ff);
    if (num == 512)
        return sf::Color(0xedc750ff);
    if (num == 1024)
        return sf::Color(0xedc43fff);
    if (num == 2048)
        return sf::Color(0xedc22eff);
    return Color::Black;
}

Color getColor(int num){
    if(num == 2)
        return Color::Black;
    else if(num == 4)
        return Color::Black;
    else
        return Color::White;

}

int input(Event event){
    if (event.key.code == sf::Keyboard::Up){
        return 1;
    }
    if (event.key.code == sf::Keyboard::Left){
        return 2;
    }
    if (event.key.code == sf::Keyboard::Right){
        return 3;
    }
    if (event.key.code == sf::Keyboard::Down){
        return 4;
    }
    return 0;
}

class Block{
public:
    void set_x(int tempx){
        this->x = tempx;
    }

    void set_y(int tempy){
        this->y = tempy;
    }

    void set_value(int temp_value){
        this->value = temp_value;
    }

    int get_x(){
        return this->x;
    }

    int get_y(){
        return this->y;
    }

    int get_value(){
        return this->value;
    }

    bool get_added(){
        return this->added;
    }

    void set_added(bool temp){
        this->added = temp;
    }

private:
    int x;
    int y;
    int value;
    bool added = false;

};

class Grid{
public:
    Block block[4][4];

    void move(int dir, RectangleShape bg,
              RectangleShape _void, RectangleShape gridd,
              RectangleShape shape, Text text, Grid grid){

        int flag = 0; bool worked = false;

        if (dir == 1){ //up can_add
            flag = 0;

            for(int i=0; i<4; i++){

                for(int j=0; j<4; j++){

                    if(flag == 0 && j>=1)
                        j--, flag++;
                    else
                        flag = 0;

                    for(int l=j+1; l<4; l++){

                        if(block[i][l].get_value() > 0
                                && block[i][j].get_value() <= 0){

                            block[i][j].set_value(block[i][l].get_value());
                            block[i][l].set_value(0);

                            block[i][j].set_added(block[i][l].get_added());
                            block[i][l].set_added(false);

                            worked = true;

                            break;
                        }


                        if(block[i][l].get_value() > 0
                                && block[i][j].get_value() == block[i][l].get_value()
                                && block[i][l].get_value() != 0
                                && !block[i][j].get_added()){

                            block[i][j].set_value(block[i][j].get_value() * 2);
                            block[i][l].set_value(0);
                            block[i][j].set_added(true);
                            worked = true;

                            break;
                        }

                        if (block[i][l].get_value() > 0 && block[i][j].get_value() > 0
                                && block[i][l].get_value() != block[i][j].get_value()
                                && abs(l-j) >= 1)
                            break;
                    }
                }
            }
        }
        if (dir == 2){ //left can_add
            flag = 0;


            for(int j=0; j<4; j++){

                for(int i=0; i<4; i++){

                    if(flag == 0 && i>=1)
                        i--, flag++;
                    else
                        flag = 0;

                    for(int l=i+1; l<4; l++){

                        if(block[i][j].get_value() <= 0
                                && block[l][j].get_value() > 0){

                            block[i][j].set_value(block[l][j].get_value());
                            block[l][j].set_value(0);

                            block[i][j].set_added(block[i][l].get_added());
                            block[i][l].set_added(false);

                            worked = true;


                            break;

                        }


                       if(block[i][j].get_value() == block[l][j].get_value()
                                && block[l][j].get_value() > 0
                                && block[l][j].get_value() != 0
                                && !block[i][j].get_added()){

                            block[i][j].set_value(block[i][j].get_value() * 2);
                            block[l][j].set_value(0);
                            block[i][j].set_added(true);
                            worked = true;

                            break;
                        }

                       if (block[l][j].get_value() > 0 && block[i][j].get_value() > 0
                               && block[l][j].get_value() != block[i][j].get_value()
                               && abs(l-i) >= 1)
                           break;
                    }
                }
            }
        }
        if (dir == 3){ //right can_add
            flag = 0;


            for(int j=0; j<4; j++){

                for(int i=3; i>=0; i--){

                    if(flag == 0 && i<=2)
                        i++, flag++;
                    else
                        flag = 0;

                    for(int l=i-1; l>=0; l--){

                        if(block[l][j].get_value() > 0
                                && block[i][j].get_value() <= 0){

                            block[i][j].set_value(block[l][j].get_value());
                            block[l][j].set_value(0);

                            block[i][j].set_added(block[i][l].get_added());
                            block[i][l].set_added(false);
                            worked = true;

                            break;
                        }



                        if(block[l][j].get_value() > 0
                                && block[i][j].get_value() == block[l][j].get_value()
                                && block[l][j].get_value() != 0
                                && !block[i][j].get_added()){

                            block[i][j].set_value(block[i][j].get_value() * 2);
                            block[l][j].set_value(0);
                            block[i][j].set_added(true);
                            worked = true;

                            break;
                        }

                        if (block[l][j].get_value() > 0 && block[i][j].get_value() > 0
                                && block[l][j].get_value() != block[i][j].get_value()
                                && abs(l-i) >= 1)
                            break;
                    }
                }
            }
        }
        if (dir == 4){ //down can_add

            flag = 0;

            for(int i=0; i<4; i++){

                for(int j=3; j>=0; j--){

                    if(flag == 0 && j<=2)
                        j++, flag++;
                    else
                        flag = 0;

                    for(int l=j-1; l>=0; l--){

                        if (block[i][l].get_value() > 0 && block[i][j].get_value() > 0
                                && block[i][l].get_value() != block[i][j].get_value()
                                && abs(l-j) >= 1){
                            break;
                        }

                        if(block[i][l].get_value() > 0
                                && block[i][j].get_value() <= 0){

                            block[i][j].set_value(block[i][l].get_value());
                            block[i][l].set_value(0);

                            block[i][j].set_added(block[i][l].get_added());
                            block[i][l].set_added(false);

                            worked = true;

                            break;
                        }


                        if(block[i][l].get_value() > 0
                                && block[i][j].get_value() == block[i][l].get_value()
                                && block[i][l].get_value() != 0
                                && !block[i][j].get_added()){

                            block[i][j].set_value(block[i][j].get_value() * 2);
                            block[i][l].set_value(0);
                            block[i][j].set_added(true);

                            worked = true;

                            break;
                        }
                    }
                }
            }
        }


        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                block[i][j].set_added(false);

        if(worked)
            this->create_random(grid);

        draw(bg,_void,gridd,shape,text,grid);
    }

    void initialize_grid(Grid grid){

        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                block[i][j].set_value(0);
                block[i][j].set_x(i*110+10);
                block[i][j].set_y(j*110+10);
            }
        }


        //block[0][0].set_value(1024);
        //block[1][0].set_value(1024);

        this->create_random(grid);
        this->create_random(grid);
    }

    void print(){
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                std::cout << "block[" << i <<"][" << j << "]:"
                          << " value=" << block[i][j].get_value() << "\n";

    }

    int get_value(int x, int y){
        return block[x][y].get_value();
    }

    int get_x(int x, int y){
        return block[x][y].get_x();
    }

    int get_y(int x, int y){
        return block[x][y].get_y();
    }

private:

    void create_random(Grid grid){
        int x,y,value,tmp=0;
        int k = 0;

        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                if(grid.get_value(i,j) <= 0)
                    k++;

        if(k != 0){
            if (k > 1){
                x = rand() % 4;
                y = rand() % 4;
                while (block[x][y].get_value() != 0){
                    x = rand() % 4;
                    y = rand() % 4;
                }
            }
            else{
                for(int i=0; i<4; i++)
                    for(int j=0; j<4; j++)
                    if(block[i][j].get_value() <= 0){
                        x = i;
                        y = j;
                        break;
                    }
            }

            while (tmp != 4 && tmp!=2){
                tmp = rand() % 5;
            }

            value = tmp;

            this->block[x][y].set_value(value);
        }
    }
};


RenderWindow window(VideoMode(700, 700), "2048", Style::Default);

void draw(RectangleShape bg,
          RectangleShape _void, RectangleShape gridd,
          RectangleShape shape, Text text, Grid grid){

        window.clear();
        window.draw(bg);
        int xpos=0, ypos=0;

        //draw background

        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++){
                _void.setPosition(i*110 + 10, j*110 + 10);
                window.draw(_void);
            }

        for(int l=0; l<5; l++){
            gridd.setSize(Vector2f(450,10));

            gridd.setPosition(0,l*110);
            window.draw(gridd);

            gridd.setSize(Vector2f(10,450));

            gridd.setPosition(l*110,0);
            window.draw(gridd);

        }
        //end drawing background


        //draw non-void blocks

        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){

                if (grid.get_value(i,j) > 0){

                    shape.setPosition(grid.get_x(i,j), grid.get_y(i,j));
                    shape.setFillColor(getBlockColor(grid.get_value(i,j)));
                    window.draw(shape);

                    text.setString(std::to_string(grid.get_value(i,j)));


                    //std::cout << std::to_string(grid.get_value(i,j));


                    if(text.getString().getSize() == 1){
                        xpos = grid.get_x(i,j) + 43;
                        ypos = grid.get_y(i,j) + 35;
                    } else if(text.getString().getSize() == 2){
                        xpos = grid.get_x(i,j) + 32;
                        ypos = grid.get_y(i,j) + 33;
                    } else if(text.getString().getSize() == 3){
                        xpos = grid.get_x(i,j) + 28;
                        ypos = grid.get_y(i,j) + 38;
                    } else if(text.getString().getSize() == 4){
                        xpos = grid.get_x(i,j) + 24;
                        ypos = grid.get_y(i,j) + 34;
                    }

                    text.setFillColor(getColor(grid.get_value(i,j)));
                    text.setPosition(xpos,ypos);

                    window.draw(text);
                }
            }
        }

        //end drawing non-void blocks

        if(game_win(grid)){
            text.setString("You won!");
            text.setFillColor(Color::Black);
            text.setPosition(450,450);
            window.draw(text);
        }

        else if(game_over(grid)){
            text.setString("You lose!");
            text.setFillColor(Color::Black);
            text.setPosition(450,450);
            window.draw(text);
        }
    window.display();
}


bool game_win(Grid grid){
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++){
            if(grid.get_value(i,j) == 2048)
                return true;
        }
    return false;
}

bool game_over(Grid grid){

    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++){
        if(grid.get_value(i,j) <= 0)
            return false;

        if(i+1 < 4)
            if(grid.get_value(i+1,j) == grid.get_value(i,j))
                return false;

        if(j+1 < 4){
            if(grid.get_value(i,j+1) == grid.get_value(i,j))
                return false;
        }
    }
    return true;
}

int main()
{
        RectangleShape shape, _void, bg, gridd;
        Text text;
        Font font;

        if (!font.loadFromFile("../2048/Arial.ttf"))
        {
            // error...
            std::cout << "failed to load the font\n";
        }
        text.setCharacterSize(27);
        text.setFont(font);

        bg.setSize(Vector2f(700,700));
        bg.setPosition(0, 0);
        bg.setFillColor(Color::White);
        Color color;
        color.r = 205;
        color.g = 193;
        color.b = 180;
        Color grid_color;
        grid_color.r = 187;
        grid_color.g = 173;
        grid_color.b = 160;
        gridd.setFillColor(grid_color);
        shape.setSize(Vector2f(100, 100));
        shape.setFillColor(Color::Red);
        _void.setSize(Vector2f(100,100));
        _void.setFillColor(color);
        Grid grid;
        bool worked = false;
        grid.initialize_grid(grid);

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed){

                    window.close();
                    return 0;
                }
                if (event.type == sf::Event::KeyPressed){

                    if (!worked && !game_over(grid) && !game_win(grid)){

                        grid.move(input(event), bg,_void, gridd, shape, text, grid);
                        worked = true;
                    }
                }
                if (event.type == sf::Event::KeyReleased){

                    if (worked)
                        worked = false;
                }
            }
                draw(bg,_void,gridd,shape,text,grid);
        }

        return 0;
}
