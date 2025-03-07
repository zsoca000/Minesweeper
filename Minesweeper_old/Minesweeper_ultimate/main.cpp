#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Textbox.h"
#include <string.h>

struct kocka_texturak{
    sf::Texture palya;
    sf::Texture main_simple;
    sf::Texture main_cursor;
    sf::Texture bomb;
    sf::Texture zaszlo;
    sf::Texture empty_0;
    sf::Texture empty_1;
    sf::Texture empty_2;
    sf::Texture empty_3;
    sf::Texture empty_4;
    sf::Texture empty_5;
    sf::Texture empty_6;
    sf::Texture empty_7;
    sf::Texture empty_8;
    sf::Texture scbrd;
    sf::Texture scbrd_cursor;
};


struct nehezsegi_szint{
    sf::Texture levelTexture;
    sf::Texture level_cursorTexture;
    int bombak_szama;
    int felosztas;
};

struct game_status{
    sf::Texture smileTexture;
    sf::Texture smile_cursorTexture;
    int status; //lehetne enum-mal  -1,0,1
};

struct jatekos{
    int score;
    char nev[12];
};


sf::Text szam_to_text(int a, sf::Font font, int x,int y, int meret);
void tabla_creater(int **tabla, int **fedolap, bool **holjartam, nehezsegi_szint level, int &bomba_szamlalo);
void megjelenit(sf::RenderWindow &window, int **tabla, int **fedolap, int bomba_szamlalo,
                nehezsegi_szint level, game_status allas, kocka_texturak txt,sf::Font font, int mp, int p);
void ha_nincs_bomba_korulotte(int x, int y, int **tabla, int **fedolap, bool **holjartam, int felosztas);
bool nyertel(int **tabla, int **fedolap, int felosztas);
bool logicalwindow(sf::RenderWindow &window);
void scoreboard_window(sf::RenderWindow &window, nehezsegi_szint level);
void win_window(nehezsegi_szint level, int time);
void new_score(nehezsegi_szint level, std::string str, int time);


int main()
{



    sf::RenderWindow window(sf::VideoMode(500, 600), "Minesweeper");

    kocka_texturak txt;
    txt.palya.loadFromFile("textures/palya.jpg");
    txt.main_simple.loadFromFile("textures/main_simple.jpg");
    txt.main_cursor.loadFromFile("textures/main_cursor.jpg");
    txt.bomb.loadFromFile("textures/bomb.jpg");
    txt.zaszlo.loadFromFile("textures/zaszlo.jpg");
    txt.empty_0.loadFromFile("textures/empty_0.jpg");
    txt.empty_1.loadFromFile("textures/empty_1.jpg");
    txt.empty_2.loadFromFile("textures/empty_2.jpg");
    txt.empty_3.loadFromFile("textures/empty_3.jpg");
    txt.empty_4.loadFromFile("textures/empty_4.jpg");
    txt.empty_5.loadFromFile("textures/empty_5.jpg");
    txt.empty_6.loadFromFile("textures/empty_6.jpg");
    txt.empty_7.loadFromFile("textures/empty_7.jpg");
    txt.empty_8.loadFromFile("textures/empty_8.jpg");
    txt.scbrd.loadFromFile("textures/scbrd.jpg");
    txt.scbrd_cursor.loadFromFile("textures/scbrd_cursor.jpg");

    nehezsegi_szint easy;
    easy.felosztas = 10;
    easy.bombak_szama = 15;
    easy.levelTexture.loadFromFile("textures/easy.jpg");
    easy.level_cursorTexture.loadFromFile("textures/easy_cursor.jpg");
    nehezsegi_szint medium;
    medium.felosztas = 20;
    medium.bombak_szama = 51;
    medium.levelTexture.loadFromFile("textures/medium.jpg");
    medium.level_cursorTexture.loadFromFile("textures/medium_cursor.jpg");
    nehezsegi_szint hard;
    hard.felosztas = 25;
    hard.bombak_szama = 81;
    hard.levelTexture.loadFromFile("textures/hard.jpg");
    hard.level_cursorTexture.loadFromFile("textures/hard_cursor.jpg");
    nehezsegi_szint level; //valozo
    level=medium;

    game_status lose;
    lose.status=-1;
    lose.smileTexture.loadFromFile("textures/lose.jpg");
    lose.smile_cursorTexture.loadFromFile("textures/lose_cursor.jpg");
    game_status win;
    win.status=1;
    win.smileTexture.loadFromFile("textures/win.jpg");
    win.smile_cursorTexture.loadFromFile("textures/win_cursor.jpg");
    game_status game;
    game.status=0;
    game.smileTexture.loadFromFile("textures/game.jpg");
    game.smile_cursorTexture.loadFromFile("textures/game_cursor.jpg");
    game_status allas; //valtozo
    allas=game;


    sf::Font font;
    if(!font.loadFromFile("font/basic.ttf")){
        throw("COULD NOT LOAD FONT!");
    }

    int** tabla = new int *[25]; for(int i=0; i<25; i++){tabla[i] = new int [25];}
    int** fedolap = new int *[25]; for(int i=0; i<25; i++){fedolap[i] = new int [25];}
    bool** holjartam = new bool *[25]; for(int i=0; i<25; i++){holjartam[i] = new bool [25];}

    int bomba_szamlalo;

    tabla_creater(tabla,fedolap,holjartam, level,bomba_szamlalo);

    bool aktiv_ora=0;
    sf::Clock clock;
    int ketyeges=0;
    int mp=0, p=0;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                if(logicalwindow(window)){
                    window.close();
                }
            }
            if(event.type == sf::Event::MouseButtonPressed){
                sf::Vector2i mousePos=sf::Mouse::getPosition(window);;
                if(mousePos.y<600 && mousePos.y>100){
                    int oszlop_koo,sor_koo;
                    oszlop_koo=mousePos.x*level.felosztas/500;
                    sor_koo=(mousePos.y-100)*level.felosztas/500;
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                        if(fedolap[sor_koo][oszlop_koo]==0){

                            if(aktiv_ora==0){
                                aktiv_ora=1;
                            }

                            if(tabla[sor_koo][oszlop_koo]!=9){
                                fedolap[sor_koo][oszlop_koo]=1;
                                if(tabla[sor_koo][oszlop_koo]==0){
                                    ha_nincs_bomba_korulotte(sor_koo,oszlop_koo,tabla, fedolap, holjartam, level.felosztas);
                                }
                            }else{
                                //vesztettel
                                aktiv_ora=0;
                                allas=lose;
                                for(int i=0; i<level.felosztas; i++){
                                    for(int j=0; j<level.felosztas; j++){
                                        fedolap[i][j]=1;
                                    }
                                }
                            }
                        }
                    }else{
                        if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                            if(fedolap[sor_koo][oszlop_koo]==0){
                                if(bomba_szamlalo>0){
                                    fedolap[sor_koo][oszlop_koo]=2;
                                    bomba_szamlalo--;
                                }
                            }else{
                                if(fedolap[sor_koo][oszlop_koo]==2){
                                    fedolap[sor_koo][oszlop_koo]=0;
                                    bomba_szamlalo++;
                                }
                            }
                        }
                    }
                    if(nyertel(tabla,fedolap,level.felosztas)){
                        //nyertel
                        aktiv_ora=0;
                        bomba_szamlalo=0;
                        allas=win;
                        for(int i=0; i<level.felosztas; i++){
                            for(int j=0; j<level.felosztas; j++){
                                if(tabla[i][j]==9){
                                    fedolap[i][j]=2;
                                }
                            }
                        }
                    }
                }else{
                    if(mousePos.y>14 && mousePos.y<86 && mousePos.x>214 && mousePos.x<286){
                        if(allas.status == game.status){
                            if(logicalwindow(window)){
                                mp=0; p=0; ketyeges=0; clock.restart();
                                tabla_creater(tabla,fedolap,holjartam, level,bomba_szamlalo);
                                aktiv_ora=0;
                            }
                        }else{
                            if(allas.status == lose.status){
                                allas=game;
                                aktiv_ora=0;
                                mp=0; p=0; ketyeges=0; clock.restart();
                                tabla_creater(tabla,fedolap,holjartam, level,bomba_szamlalo);
                            }else{
                                if(allas.status == win.status){
                                    //SCORE SAVEING
                                    win_window(level,60*p+mp);
                                    allas=game;
                                    aktiv_ora=0;
                                    mp=0; p=0; ketyeges=0; clock.restart();
                                    tabla_creater(tabla,fedolap,holjartam, level,bomba_szamlalo);
                                }
                            }
                        }
                    }else{
                        if(mousePos.y>25 && mousePos.y<75 && mousePos.x>107 && mousePos.x<207){
                            if(level.bombak_szama == easy.bombak_szama && level.felosztas == easy.felosztas){ //akar logicalwindow()
                                level=medium;
                                allas=game;
                                aktiv_ora=0;
                                mp=0; p=0; ketyeges=0; clock.restart();
                                tabla_creater(tabla,fedolap,holjartam, level,bomba_szamlalo);
                            }else{
                                if(level.bombak_szama == medium.bombak_szama && level.felosztas == medium.felosztas){ //akar logicalwindow()
                                    level=hard;
                                    allas=game;
                                    aktiv_ora=0;
                                    mp=0; p=0; ketyeges=0; clock.restart();
                                    tabla_creater(tabla,fedolap,holjartam, level,bomba_szamlalo);
                                }else{
                                    if(level.bombak_szama == hard.bombak_szama && level.felosztas == hard.felosztas){ //akar logicalwindow()
                                        level=easy;
                                        allas=game;
                                        aktiv_ora=0;
                                        mp=0; p=0; ketyeges=0; clock.restart();
                                        tabla_creater(tabla,fedolap,holjartam, level,bomba_szamlalo);
                                    }
                                }
                            }
                        }else{
                            if(mousePos.x>296 && mousePos.x<355 && mousePos.y>14 && mousePos.y<86){
                                scoreboard_window(window,level);
                            }
                        }
                    }
                }

            }
        }
        sf::Time time = clock.getElapsedTime();
        if(ketyeges!=(int)time.asSeconds()){
            ketyeges=(int)time.asSeconds();
            if(aktiv_ora){
                mp++;
                if(mp>60){
                    mp=mp-60;
                    p++;
                }
            }
        }
        //std::cout<<mp<<std::endl;



        megjelenit(window, tabla, fedolap, bomba_szamlalo, level, allas, txt, font, mp, p);


    }

    return 0;
}

sf::Text szam_to_text(int a, sf::Font font, int x, int y , int meret){
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(meret);
    text.setColor(sf::Color::Red);
    text.setPosition(x,y);
    std::stringstream ss;
    std::string str;
    if(a<10){
        ss<<0;
    }
    ss << a;

    str=ss.str();
    text.setString(str);
    return text;
}

void tabla_creater(int **tabla, int **fedolap, bool **holjartam, nehezsegi_szint level, int &bomba_szamlalo){
    bomba_szamlalo=level.bombak_szama;
    for(int i=0; i<level.felosztas; i++){
        for(int j=0; j<level.felosztas; j++){
            tabla[i][j]=0;
            fedolap[i][j]=0;
            holjartam[i][j]=false;
        }
    }
    srand(time(0));
    int a,x=rand()%47+24;
    for(int i=0; i<level.bombak_szama; i++){
        a=(i*i*i+(i+1)*x-23*i)%(level.felosztas*level.felosztas);
        while(tabla[(int)(a/level.felosztas)][a%level.felosztas]==9){
            a=(a+1)%(level.felosztas*level.felosztas);
        }
        tabla[(int)(a/level.felosztas)][a%level.felosztas]=9;
    }


    for(int i=0; i<level.felosztas; i++){
        for(int j=0; j<level.felosztas; j++){
            if(tabla[i][j]==9){
                if(i!=0 && tabla[i-1][j]!=9){
                    tabla[i-1][j]++;
                }
                if(i!=level.felosztas-1 && tabla[i+1][j]!=9){
                    tabla[i+1][j]++;
                }
                if(j!=0 && tabla[i][j-1]!=9){
                    tabla[i][j-1]++;
                }
                if(j!=level.felosztas-1 && tabla[i][j+1]!=9){
                    tabla[i][j+1]++;
                }
                if(i!=0 && j!=0 && tabla[i-1][j-1]!=9){
                    tabla[i-1][j-1]++;
                }
                if(i!=0 && j!=level.felosztas-1 && tabla[i-1][j+1]!=9){
                    tabla[i-1][j+1]++;
                }
                if( i!=level.felosztas-1 && j!=0 && tabla[i+1][j-1]!=9){
                    tabla[i+1][j-1]++;
                }
                if(i!=level.felosztas-1 && j!=level.felosztas-1 && tabla[i+1][j+1]!=9){
                    tabla[i+1][j+1]++;
                }
            }
        }
    }
}

void megjelenit(sf::RenderWindow &window, int **tabla, int **fedolap, int bomba_szamlalo,
                nehezsegi_szint level, game_status allas, kocka_texturak txt,sf::Font font , int mp, int p)
{
    window.clear();
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    sf::RectangleShape kocka(sf::Vector2f(500/(float)level.felosztas,500/(float)level.felosztas));
    sf::RectangleShape palya(sf::Vector2f(500.0f,600.0f)); palya.setTexture(&txt.palya);


    sf::RectangleShape smile(sf::Vector2f(72.0f,72.0f));
    if(mousePos.y>14 && mousePos.y<86 && mousePos.x>214 && mousePos.x<286){
        smile.setTexture(&allas.smile_cursorTexture);
    }else{
        smile.setTexture(&allas.smileTexture);
    }

    sf::RectangleShape szint(sf::Vector2f(100.0f,50.0f));
    if(mousePos.y>25 && mousePos.y<75 && mousePos.x>107 && mousePos.x<207){
        szint.setTexture(&level.level_cursorTexture);
    }else{
        szint.setTexture(&level.levelTexture);
    }

    sf::RectangleShape scbrd(sf::Vector2f(61.0f,72.0f));
    if(mousePos.x>296 && mousePos.x<355 && mousePos.y>14 && mousePos.y<86){
        scbrd.setTexture(&txt.scbrd_cursor);
    }else{
        scbrd.setTexture(&txt.scbrd);
    }


    window.draw(palya);
    smile.setPosition(214.0f,14.0f);
    window.draw(smile);
    szint.setPosition(107.0f,25.0f);
    window.draw(szint);
    window.draw(szam_to_text(bomba_szamlalo,font,39,19,50));
    scbrd.setPosition(296.0f,14.0f);
    window.draw(scbrd);

    window.draw(szam_to_text(mp,font,431,19,50));
    window.draw(szam_to_text(p,font,381,19,50));

    for(int i=0; i<level.felosztas; i++){
        for(int j=0; j<level.felosztas; j++){
            kocka.setPosition(j*500/level.felosztas, i*500/level.felosztas+100);
            if(fedolap[i][j]==0){
                if(j == (int)((mousePos.x)*level.felosztas/500) && i == (int)((mousePos.y-100)*level.felosztas/500) && mousePos.y>100 && mousePos.y<600 && mousePos.x>0 && mousePos.x<500){
                    kocka.setTexture(&txt.main_cursor);
                }else{
                    kocka.setTexture(&txt.main_simple);
                }
                window.draw(kocka);
            }else{
                if(fedolap[i][j]==1){
                    switch(tabla[i][j])
                    {
                        case 0: kocka.setTexture(&txt.empty_0); break;
                        case 1: kocka.setTexture(&txt.empty_1); break;
                        case 2: kocka.setTexture(&txt.empty_2); break;
                        case 3: kocka.setTexture(&txt.empty_3); break;
                        case 4: kocka.setTexture(&txt.empty_4); break;
                        case 5: kocka.setTexture(&txt.empty_5); break;
                        case 6: kocka.setTexture(&txt.empty_6); break;
                        case 7: kocka.setTexture(&txt.empty_7); break;
                        case 8: kocka.setTexture(&txt.empty_8); break;
                        case 9: kocka.setTexture(&txt.bomb); break;
                    }
                    window.draw(kocka);
                }else{
                    kocka.setTexture(&txt.zaszlo);
                    window.draw(kocka);
                }
            }




        }
    }


    window.display();
}

void ha_nincs_bomba_korulotte(int x, int y, int **tabla, int **fedolap, bool **holjartam, int felosztas){
    holjartam[x][y]=true;
    if(x!=0 && y!=0){
        fedolap[x-1][y-1]=1;
        if((tabla[x-1][y-1]==0)&&(holjartam[x-1][y-1]==false)){
            ha_nincs_bomba_korulotte(x-1, y-1, tabla, fedolap, holjartam, felosztas);
        }
    }
    if(x!=0){
        fedolap[x-1][y]=1;
        if((tabla[x-1][y]==0)&&(holjartam[x-1][y]==false)){
            ha_nincs_bomba_korulotte(x-1, y, tabla, fedolap, holjartam, felosztas);
        }
    }
    if(x!=0 && y!=felosztas-1){
        fedolap[x-1][y+1]=1;
        if((tabla[x-1][y+1]==0)&&(holjartam[x-1][y+1]==false)){
            ha_nincs_bomba_korulotte(x-1, y+1, tabla, fedolap, holjartam, felosztas);
        }
    }
    if(y!=felosztas-1){
        fedolap[x][y+1]=1;
        if((tabla[x][y+1]==0)&&(holjartam[x][y+1]==false)){
            ha_nincs_bomba_korulotte(x, y+1, tabla, fedolap, holjartam, felosztas);
        }
    }
    if(x!=felosztas-1 && y!=felosztas-1){
        fedolap[x+1][y+1]=1;
        if((tabla[x+1][y+1]==0)&&(holjartam[x+1][y+1]==false)){
            ha_nincs_bomba_korulotte(x+1, y+1, tabla, fedolap, holjartam, felosztas);
        }
    }
    if(x!=felosztas-1){
        fedolap[x+1][y]=1;
        if((tabla[x+1][y]==0)&&(holjartam[x+1][y]==false)){
            ha_nincs_bomba_korulotte(x+1, y, tabla, fedolap, holjartam, felosztas);
        }
    }
    if(x!=felosztas-1 && y!=0){
        fedolap[x+1][y-1]=1;
        if((tabla[x+1][y-1]==0)&&(holjartam[x+1][y-1]==false)){
            ha_nincs_bomba_korulotte(x+1, y-1, tabla, fedolap, holjartam, felosztas);
        }
    }
    if(y!=0){
        fedolap[x][y-1]=1;
        if((tabla[x][y-1]==0)&&(holjartam[x][y-1]==false)){
            ha_nincs_bomba_korulotte(x, y-1, tabla, fedolap, holjartam, felosztas);
        }
    }

}

bool nyertel(int **tabla, int **fedolap, int felosztas){
    for(int i=0; i<felosztas; i++){
        for(int j=0; j<felosztas; j++){
            if((fedolap[i][j]==0 || fedolap[i][j]==2) && tabla[i][j]!=9){
                return false;
            }
            if(fedolap[i][j]==1 && tabla[i][j]==9){
                return false;
            }
        }
    }
    return true;
}

bool logicalwindow(sf::RenderWindow &window){
    sf::RectangleShape hide(sf::Vector2f(500.0f,500.0f));
    sf::Texture hideTexture; hideTexture.loadFromFile("textures/hide.jpg");
    hide.setTexture(&hideTexture);
    hide.setPosition(0.0f,100.0f);
    window.draw(hide);
    window.display();
    sf::RenderWindow biztos(sf::VideoMode(400,200),"ARE YOU SURE?", sf::Style::Titlebar);

    sf::RectangleShape hatter(sf::Vector2f(400.0f,200.0f));
    sf::Texture hatterTexture; hatterTexture.loadFromFile("textures/hatter.jpg");
    hatter.setTexture(&hatterTexture);

    sf::RectangleShape yes(sf::Vector2f(120.0f,60.0f));
    sf::Texture yesTexture; yesTexture.loadFromFile("textures/yes.jpg");
    sf::Texture yes_cursorTexture; yes_cursorTexture.loadFromFile("textures/yes_cursor.jpg");
    yes.setPosition(40.0f,120.0f);

    sf::RectangleShape no(sf::Vector2f(120.0f,60.0f));
    sf::Texture noTexture; noTexture.loadFromFile("textures/no.jpg");
    sf::Texture no_cursorTexture; no_cursorTexture.loadFromFile("textures/no_cursor.jpg");
    no.setPosition(240.0f,120.0f);


    while (biztos.isOpen()){
        sf::Vector2i mousePos = sf::Mouse::getPosition(biztos);
        sf::Event katt;
        while (biztos.pollEvent(katt)){
            if(katt.type == sf::Event::MouseButtonPressed){
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    if(mousePos.x>40 && mousePos.x<160 && mousePos.y>120 && mousePos.y<180){
                        return true;
                    }else{
                        if(mousePos.x>240 && mousePos.x<360 && mousePos.y>120 && mousePos.y<180){
                            return false;
                        }
                    }
                }
            }

        }
        biztos.clear();
        if(mousePos.x>40 && mousePos.x<160 && mousePos.y>120 && mousePos.y<180){
            yes.setTexture(&yes_cursorTexture);
        }else{
            yes.setTexture(&yesTexture);
        }
        if(mousePos.x>240 && mousePos.x<360 && mousePos.y>120 && mousePos.y<180){
            no.setTexture(&no_cursorTexture);
        }else{
            no.setTexture(&noTexture);
        }
        biztos.draw(hatter);
        biztos.draw(yes);
        biztos.draw(no);
        biztos.display();

    }
}

void scoreboard_window(sf::RenderWindow &window, nehezsegi_szint level){
    sf::RectangleShape hide(sf::Vector2f(500.0f,500.0f));
    sf::Texture hideTexture; hideTexture.loadFromFile("textures/hide.jpg");
    hide.setTexture(&hideTexture);
    hide.setPosition(0.0f,100.0f);
    window.draw(hide);
    window.display();


    sf::RenderWindow scoreboard_wdw(sf::VideoMode(500, 600), "Scoreboard");
    sf::RectangleShape scoreboard_shape(sf::Vector2f(500.0f,600.0f));
    sf::Texture scoreboard_texture; scoreboard_texture.loadFromFile("textures/scoreboard.jpg");
    scoreboard_shape.setTexture(&scoreboard_texture);

    sf::Font font;
    if(!font.loadFromFile("font/basic.ttf")){
        throw("COULD NOT LOAD FONT!");
    }
    sf::Text text;
    text.setCharacterSize(40);
    text.setColor(sf::Color::Red);
    text.setFont(font);


    sf::RectangleShape szint(sf::Vector2f(100.0f,50.0f));
    szint.setTexture(&level.levelTexture);
    scoreboard_wdw.draw(scoreboard_shape);

    std::ifstream f;

    if(level.felosztas==25){
        f.open("scoreboards/scoreboard_hard.txt");
    }else{
        if(level.felosztas==20){
            f.open("scoreboards/scoreboard_medium.txt");
        }else{
            if(level.felosztas==10){
                f.open("scoreboards/scoreboard_easy.txt");
            }
        }
    }

    jatekos scoreboard[5];
    for(int i=0; i<5;i++){
        f>>scoreboard[i].score;
        f>>scoreboard[i].nev;
        text.setPosition(209.0f,(float)(26+i*111));
        text.setString(scoreboard[i].nev);
        scoreboard_wdw.draw(text);
        scoreboard_wdw.draw(szam_to_text(scoreboard[i].score,font,270,78+i*111,40));
        szint.setPosition(140.0f,(float)(78+i*111));
        scoreboard_wdw.draw(szint);
    }
    f.close();


    scoreboard_wdw.display();

    while(scoreboard_wdw.isOpen()){
        sf::Event event;
        while(scoreboard_wdw.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                scoreboard_wdw.close();
            }
        }
    }

}

void win_window(nehezsegi_szint level, int time){
    sf::RenderWindow window(sf::VideoMode(500, 249), "",sf::Style::Titlebar);
    sf::RectangleShape win_window(sf::Vector2f(500,249));
    sf::Texture win_windowTexture; win_windowTexture.loadFromFile("textures/win_window.jpg");
    win_window.setTexture(&win_windowTexture);

    sf::RectangleShape ok(sf::Vector2f(100,50));
    ok.setPosition(41,165);
    sf::Texture okTexture; okTexture.loadFromFile("textures/ok.jpg");
    sf::Texture ok_cursorTexture; ok_cursorTexture.loadFromFile("textures/ok_cursor.jpg");

    sf::Font font;
    font.loadFromFile("font/basic.ttf");

    Textbox textbox1(40,sf::Color::Red,false);
    textbox1.setFont(font);
    textbox1.setPosition(sf::Vector2f(76.0f,104.0f));
    textbox1.setLimit(true,12);

    while (window.isOpen())
    {
        sf::Vector2i mousePos= sf::Mouse::getPosition(window);
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::TextEntered:
                textbox1.typedOn(event);
                break;
            case sf::Event::MouseButtonPressed:
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    if(mousePos.y<215 && mousePos.y>165 && mousePos.x > 41 && mousePos.x < 141){
                        window.close();
                        new_score(level,textbox1.getText(),time);
                    }else{
                        if(mousePos.x<444 && mousePos.x>59 && mousePos.y >102 && mousePos.y<151){
                            textbox1.setSelected(true);
                        }else{
                            textbox1.setSelected(false);
                        }
                    }
                }
            }
        }
        window.clear();
        window.draw(win_window);
        if(mousePos.y<215 && mousePos.y>165 && mousePos.x > 41 && mousePos.x < 141){
            ok.setTexture(&ok_cursorTexture);
        }else{
            ok.setTexture(&okTexture);
        }
        window.draw(szam_to_text(time,font,300,165,40));
        window.draw(ok);
        textbox1.drawTo(window);
        window.display();
    }

}

void new_score(nehezsegi_szint level, std::string str,int time){

    std::ifstream f;
    if(level.felosztas==10){
        f.open("scoreboards/scoreboard_easy.txt");
    }else{
        if(level.felosztas==20){
            f.open("scoreboards/scoreboard_medium.txt");
        }else{
            if(level.felosztas==25){
                f.open("scoreboards/scoreboard_hard.txt");
            }
        }
    }
    jatekos scoreboard[5],uj;
    for(int i=0; i<str.length(); i++){
        uj.nev[i]=str[i]; // valamiert tesz karaktereket +ba amikor atmasolom ide
    }
    uj.nev[str.length()]='\0';

    uj.score=time;

    for(int i=0; i<5; i++){
        f>>scoreboard[i].score;
        f>>scoreboard[i].nev;
    }
    f.close();

    int uj_helyezes=4;
    if(uj.score<scoreboard[4].score){
        while(uj.score<scoreboard[uj_helyezes].score && uj_helyezes>=0){
            uj_helyezes--;
        }
        uj_helyezes++;
    }else{
        uj_helyezes=5;
    }

    if(uj_helyezes!=5){
        for(int i=4; i>uj_helyezes; i--){
           scoreboard[i]=scoreboard[i-1];
        }
        scoreboard[uj_helyezes]=uj;
    }

    std::ofstream g;
    if(level.felosztas==10){
        g.open("scoreboards/scoreboard_easy.txt");
    }else{
        if(level.felosztas==20){
            g.open("scoreboards/scoreboard_medium.txt");
        }else{
            if(level.felosztas==25){
                g.open("scoreboards/scoreboard_hard.txt");
            }
        }
    }
    for(int i=0; i<5; i++){
        g<<scoreboard[i].score<<std::endl;
        g<<scoreboard[i].nev<<std::endl;      // valamiert tesz karaktereket amikor atmasolom ide
    }

    g.close();
}
