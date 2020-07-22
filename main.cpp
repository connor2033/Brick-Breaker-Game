#include <SFML/Graphics.hpp>
#include <iostream>
#include <Box2D/Box2D.h>
#include "physics.h"
#include <vector>
#include "resources.h"
#include <string>
#include <SFML/Audio.hpp>
using namespace sf;
using namespace std;




int main()
{
    RenderWindow window(VideoMode(1200, 900), "Brick Breaker");
    window.setFramerateLimit(60);
    srand (time(NULL));

    b2World world(b2Vec2(0.0,0.0));

    RectangleShape box1(Vector2f(172,110));
    box1.setPosition(10, 530);
    box1.setFillColor(Color::Transparent);
    box1.setOutlineColor(Color::White);
    box1.setOutlineThickness(-3);

    RectangleShape box2(Vector2f(172,165));
    box2.setPosition(10, 475);
    box2.setFillColor(Color::Transparent);
    box2.setOutlineColor(Color::White);
    box2.setOutlineThickness(-3);

    Font font1;
    font1.loadFromFile("strenuous bl.ttf");

    Text Text1;
    Text1.setFont(font1);
    Text1.setCharacterSize(100);
    Text1.setPosition(600, 600);
    Text1.setString("0");

    Text freightText;
    freightText.setFont(font1);
    freightText.setCharacterSize(20);
    freightText.setPosition(20, 540);
    freightText.setString("FREIGHT TRAIN");

    Text psizeText;
    psizeText.setFont(font1);
    psizeText.setCharacterSize(20);
    psizeText.setPosition(20, 570);
    psizeText.setString("MEGA PADDLE");

    Text ssText;
    ssText.setFont(font1);
    ssText.setCharacterSize(20);
    ssText.setPosition(20, 600);
    ssText.setString("SUPER-SPEED");

    /////dead screen
    Text dead;
    dead.setFont(font1);
    dead.setCharacterSize(80);
    dead.setOrigin(dead.getGlobalBounds().width/2 , dead.getGlobalBounds().height/2);
    dead.setPosition(600, 450);
    dead.setString("oof u dead bruh -_-");

    Text retry;
    retry.setFont(font1);
    retry.setCharacterSize(60);
    retry.setOrigin(retry.getGlobalBounds().width/2 , retry.getGlobalBounds().height/2);
    retry.setPosition(600, 600);
    retry.setColor(Color(43, 239, 89));
    retry.setString("Restart");

    ////////////////////////////////////////////////////////sounds

    //hit
    SoundBuffer bhit;
    bhit.loadFromFile("hit.wav");

    Sound hit;
    hit.setBuffer(bhit);


    //creating static blocks (ex. 'ground' etc.)
    Block wallleft = physics::createBox(world, -100, -1000, 100, 3900, b2_staticBody);
    Block wallright = physics::createBox(world, 1200, -1000, 100, 3900, b2_staticBody);

    Block wallbottom = physics::createBox(world, -1000, 1200, 3200, 100, b2_staticBody);

    Block walltop = physics::createBox(world, 0, -1000, 1200, 1000, b2_staticBody);

    //ball
    Texture bawl;
    bawl.loadFromFile("ballthing.jpg");

    Ball b1(world, 600, 500, 17);
    //b1.setColor(Color::White);



///////////////////////////////////////////////lives

    int lives = 4;
    bool die = false;


    ////////////////lives display
    CircleShape life1(17);
    life1.setPosition(25, 488);
    life1.setFillColor(Color::Transparent);
    life1.setOutlineColor(Color::White);
    life1.setOutlineThickness(-3);

    CircleShape life2(17);
    life2.setPosition(80, 488);
    life2.setFillColor(Color::Transparent);
    life2.setOutlineColor(Color::White);
    life2.setOutlineThickness(-3);

    CircleShape life3(17);
    life3.setPosition(135, 488);
    life3.setFillColor(Color::Transparent);
    life3.setOutlineColor(Color::White);
    life3.setOutlineThickness(-3);



    //paddle
    Texture padtex;
    padtex.loadFromFile("paddlething.jpg");

    Paddle paddle1 (world, 525, 800, 150, 20);

    //setPaddleTexture(paddle1, &padtex);

    //Bricks
    vector <Brick> Bricks;

    int brickrows = 8;
    int brickcolumns = 17;
    cout<<"There are "<<brickrows*brickcolumns<<" bricks."<<endl;



    // # of rows (e)
    for (int e = 0; e < brickrows; e++)
    {
        // # of columns (i)
        for (int i = 0; i < brickcolumns; i++)
        {
            Bricks.push_back(Brick(world, 15 + i*70, 50 + e*50, 50, 30, b2_staticBody));
            //physics::setBlockColor(Bricks.back(),Color(rand()% 256 + 1, rand()% 256 + 1, rand()% 256 + 1));
        }
    }



//    vector<Vector2f> bg;
//    bg.push_back(Vector2f(0, 0));
//    bg.push_back(Vector2f(1200, 0));
//    bg.push_back(Vector2f(1200, 900));
//    bg.push_back(Vector2f(0,900));

    Texture bkg;
    bkg.loadFromFile("background.jpg");

    Sprite background;
    background.setTexture(bkg);
    background.setPosition(0,0);

///////////////////////////////power up stuff/////////////////
    int chance;
    bool padsize_pu = false;
    bool freighttrain_pu = false;
    bool superspeed_pu = false;


    /////////////////////score
    int score;


/////////////////////////////////////////////LOOP///////////////////////////////////////////////////
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }


    if (die == false)
        {
        paddle1.updatePosition();
        b1.updatePosition();
        if (b1.isColliding(paddle1.body)) b1.resetAngle(paddle1.getPosition());


        /////////////////////////////////////////////////ball colliding with bricks
        for (int i = 0; i < Bricks.size(); i++)
        {
            if (Bricks[i].isColliding(b1))
            {
                Bricks[i].deleteBody(world);
                Bricks.erase(Bricks.begin()+i);
                hit.play();


                ////////////////////////////////power up stuff///////////
                chance = rand() % 100;
                //cout<<chance<<endl;

                //paddle size
                if (chance > 90 && padsize_pu == false)
                {
                    padsize_pu = true;
                    cout<<"Big long boi"<<endl;
                }
                else if (chance > 90 && padsize_pu == true) padsize_pu = false;

                //freight train
                if (chance > 70 && chance < 90 && freighttrain_pu == false)
                {
                    freighttrain_pu = true;
                    cout<<"FREIGHTTTTTT TRAIIIIIIIIIIIIIIIINNNNNNNNN"<<endl;
                }
                else if (chance > 40 && chance < 90 && freighttrain_pu == true) freighttrain_pu = false;

                //Super Speed
                if (chance > 60 && chance < 70 && freighttrain_pu == false)
                {
                    superspeed_pu = true;
                    cout<<"SUPERSPEED!"<<endl;
                }
                else if (chance > 20 && chance < 60 && freighttrain_pu == true) superspeed_pu = false;


                /////////////////////////////////////

                //paddle size
                if (padsize_pu == true)
                    {
                        paddle1.resetSize(world, 2.0);
                        psizeText.setFillColor(Color::Cyan);
                    }
                else
                    {
                     paddle1.resetSize(world, 1.0);
                     psizeText.setFillColor(Color::White);
                    }

                //freight train
                if (freighttrain_pu == true)
                    {
                        b1.setFreightTrain(true);
                        freightText.setFillColor(Color::Cyan);
                        b1.setColor(Color::White);
                    }

                else
                    {
                        b1.setFreightTrain(false);
                        freightText.setFillColor(Color::White);
                        b1.setColor(Color::Transparent);
                    }

                //super speed
                if (superspeed_pu == true)
                    {
                        paddle1.setSuperSpeed(true);
                        ssText.setFillColor(Color::Cyan);
                    }

                else
                    {
                        paddle1.setSuperSpeed(false);
                        ssText.setFillColor(Color::White);
                    }
            }
        }


//////////keeping ball in game with freight train power up
        if (physics::checkCollision(wallleft) == true)
        {
            freighttrain_pu = false;
            b1.setFreightTrain(false);
            freightText.setFillColor(Color::White);
            b1.setColor(Color::Transparent);
            hit.play();
        }
        if (physics::checkCollision(wallright) == true)
        {
            freighttrain_pu = false;
            b1.setFreightTrain(false);
            freightText.setFillColor(Color::White);
            b1.setColor(Color::Transparent);
            hit.play();
        }
        if (physics::checkCollision(walltop) == true)
        {
            freighttrain_pu = false;
            b1.setFreightTrain(false);
            freightText.setFillColor(Color::White);
            b1.setColor(Color::Transparent);
            hit.play();
        }
        if (physics::checkCollision(wallbottom) == true)
        {
            freighttrain_pu = false;
            b1.setFreightTrain(false);
            freightText.setFillColor(Color::White);
            b1.deleteBall(world);
            lives = lives - 1;
            Ball b1(world, 600, 500, 17);
        }



        if (lives < 1)
        {
            b1.deleteBall(world);
            die = true;
        }
        cout<<"Lives: "<<lives<<endl;



        //////////////////////////////////////////
        if (Keyboard::isKeyPressed(Keyboard::Space))
            paddle1.resetSize(world, 2.0);

        if (Keyboard::isKeyPressed(Keyboard::M))
            paddle1.resetSize(world, 1);

        int score = 136 - Bricks.size();
        Text1.setString(to_string(score));
        Text1.setCharacterSize(30 + score);
        Text1.setOrigin(Text1.getGlobalBounds().width/2, Text1.getGlobalBounds().height/2);
        Text1.setPosition(600, 600);
        }




        window.clear();
        window.draw(background);

    if (die == false)
    {
        window.draw(Text1);
        window.draw(freightText);
        window.draw(psizeText);
        window.draw(ssText);

        if (lives > 3)
        {
            window.draw(life1);
            window.draw(life2);
            window.draw(life3);
        }
        else if (lives > 2)
        {
            window.draw(life1);
            window.draw(life2);
        }
        else if (lives > 1)
        {
            window.draw(life1);
        }


        window.draw(box1);
        window.draw(box2);
    }
    else if (die = true)
    {
        dead.setOrigin(dead.getGlobalBounds().width/2 , dead.getGlobalBounds().height/2);
        dead.setPosition(600, 500);
        window.draw(dead);

        cout<<"("<<Mouse::getPosition(window).x<<", "<<Mouse::getPosition(window).y<<")"<<endl;
        if (Mouse::getPosition(window).x > 470 && Mouse::getPosition(window).x < 735 && Mouse::getPosition(window).y > 645 && Mouse::getPosition(window).y < 690)
        {
            retry.setCharacterSize(70);
        }
        else retry.setCharacterSize(60);

        retry.setOrigin(retry.getGlobalBounds().width/2 , retry.getGlobalBounds().height/2);
        retry.setPosition(600, 650);
        window.draw(retry);

    }

        physics::displayWorld(world, window);
        window.display();
    }

    return 0;
}
