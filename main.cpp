#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Window.hpp>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//set movement enums
enum movementEnum{up,down,left,right};

//set key enum for key checking
enum keyPressedEnum{upKey,downKey,leftKey,rightKey};


int main()
{
    //window size
    float windowX = 200.f;
    float windowY = 200.f;

    //Create Window
    //Style does not allow resizing
    sf::RenderWindow window(sf::VideoMode(windowX,windowY), "Snake",sf::Style::Close);

    //Create snake head
    sf::RectangleShape head(sf::Vector2f(18.f,18.f));
    head.setFillColor(sf::Color(150,50,250));

    //Create apple shape
    sf::CircleShape apple(10.f);
    apple.setFillColor(sf::Color(250,0,0));

    //Random int x and y to use for apple positions
    //Initialize random seed
    std::srand (time(NULL));

    //Initialize variables that will hold the apples x and y positions
    int appleX = windowX/2.f;
    int appleY = windowY/2.f;
    //Set apples initial position
    apple.setPosition(appleX,appleY);

    //snake starting movement is right
    movementEnum direction = right;

    //set default key pressed as right
    keyPressedEnum keyPressed = rightKey;

    //create font
    sf::Font endFont;
    endFont.loadFromFile("ARCADECLASSIC.TTF");
    //Set end game text
    sf::Text endText;
    endText.setFont(endFont);
    endText.setString("GAME OVER");
    endText.setCharacterSize(windowX/8);
    endText.setFillColor(sf::Color::Green);
    endText.setPosition(windowX/2-windowX/4,windowY/2);

    //QUEUE FOR HOLDING SNAKE CIRCLES POSITIONS
    //Create queue for storing of positions of snake body
    std::deque<sf::Vector2f> positions;
    //Create vector2f for head position
    sf::Vector2f headPosition(0.f,0.f);

    //Vector2f to hold new body position if new circle gets added
    sf::Vector2f newBodyPosition;
    //Create starting snake with length of 3 pushing head position first
    newBodyPosition = headPosition;
    for(unsigned int i=0 ; i<3 ; i++)
    {
        //push front first to add head position first before adding 20.f to x every loop
        positions.push_front(newBodyPosition);
        newBodyPosition.x += 20.f;
    }

    //set boolean for if an apple collision has occured
    bool appleCollision = false;
    //set boolean for if an snake collision has occured
    bool snakeCollision = false;
    //bool used to check if apple generated in snake body
    bool validApplePosition = true;

    //Add time for delay after every window is rendered
    sf::Time delay = sf::seconds(0.1f);

    while(window.isOpen())
    {
        //std::cout << head.getPosition().x;

        //Poll for events
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            //Check for snake movement input and set boolean that will be check before updating snake movement
            //Check that snake can not double back on itself
            if(event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::Up:
                        keyPressed = upKey;
                        break;
                    case sf::Keyboard::Down:
                        keyPressed = downKey;
                        break;
                    case sf::Keyboard::Left:
                        keyPressed = leftKey;
                        break;
                     case sf::Keyboard::Right:
                        keyPressed = rightKey;
                        break;
                     default:
                        keyPressed = rightKey;
               }
            }
        }

        //Check that snake cannot double back on itself
        if(keyPressed == upKey && !(direction == down))
                direction = up;
        if(keyPressed == downKey && !(direction == up))
                direction = down;
        if(keyPressed == leftKey && !(direction == right))
                direction = left;
        if(keyPressed == rightKey && !(direction == left))
                direction = right;

        //check if a appleCollision with an apple has occured
        //truncate vector2f.x(float) and cast to int for == comparison to be accurate with appleX(int)
        if((appleX == static_cast<int>(std::trunc(positions[0].x))) && (appleY == static_cast<int>(std::trunc((positions[0].y)))))
                appleCollision = true;

        if(appleCollision)
        {
            //std::cout << "COLLISION\n";
            //Add body circle
            newBodyPosition.x = positions[positions.size()-1].x - 20.f;
            newBodyPosition.y = positions[positions.size()-1].y;
            positions.push_back(newBodyPosition);

            //move apple to new position
            //get random numbers between 1 and window size/20(snake body size) - 1 and multiply by 20(snake body size)
            //min positions 20(snake body size), max position window size - snake body size
            do
            {
                appleX = (std::rand() % (static_cast<int>(windowX)/20-1) + 1)*20;
                appleY = (std::rand() % (static_cast<int>(windowY)/20-1) + 1)*20;
                apple.setPosition(appleX,appleY);
                validApplePosition = true;
                //make sure apple is not generated in snake body
                for(unsigned int i=0; i<positions.size(); i++)
                {
                    if((appleX == positions[i].x) && (appleY == positions[i].y))
                        validApplePosition = false;
                }
            }while(!validApplePosition);


            appleCollision=false;
        }

        //check if snake collides with itself
        for(unsigned int i=1; i<positions.size(); i++)
        {
            if(positions[0] == positions[i])
                snakeCollision = true;
        }

        //Update object movement
        switch(direction)
        {
            case right:
                head.setPosition(positions.front());
                headPosition = positions.front();
                headPosition.x += 20.f;
                positions.pop_back();
                positions.push_front(headPosition);
                break;
            case left:
                head.setPosition(positions.front());
                headPosition = positions.front();
                headPosition.x -= 20.f;
                positions.pop_back();
                positions.push_front(headPosition);
                break;
            case up:
                head.setPosition(positions.front());
                headPosition = positions.front();
                headPosition.y -= 20.f;
                positions.pop_back();
                positions.push_front(headPosition);
                break;
            case down:
                head.setPosition(positions.front());
                headPosition = positions.front();
                headPosition.y += 20.f;
                positions.pop_back();
                positions.push_front(headPosition);
                break;
            default:
                head.setPosition(positions.front());
                headPosition = positions.front();
                headPosition.x += 20.f;
                positions.pop_back();
                positions.push_front(headPosition);

        }

        //Wrapping object
        //Wrap in x axis
        if(head.getPosition().x > (window.getSize().x - 20.f))
        {
            headPosition.x = 0.f;
            positions.pop_back();
            positions.push_front(headPosition);
        }
        if(head.getPosition().x < 0.f)
        {
            headPosition.x = window.getSize().x - 20.f;
            positions.pop_back();
            positions.push_front(headPosition);
        }
        //Wrap in y axis
        if(head.getPosition().y > (window.getSize().y - 20.f))
        {
            headPosition.y = 0.f;
            positions.pop_back();
            positions.push_front(headPosition);

        }
        if(head.getPosition().y < 0.f)
        {
            headPosition.y = window.getSize().y - 20.f;
            positions.pop_back();
            positions.push_front(headPosition);
        }


        //Clear Screen
        window.clear(sf::Color::Black);
        //Draw object
        for(unsigned int i=0; i<positions.size(); i++)
        {
            head.setPosition(positions[i]);
            window.draw(head);
            //std::cout << "draw: " << i << std::endl;
        }
        window.draw(apple);

        //Sleep in order for event loop to only update every 0.1 seconds
        sf::sleep(delay);

        //End game screen if collide with snake body
        if(snakeCollision)
        {
           window.clear(sf::Color::Black);
           window.draw(endText);
        }

        //Update the window
        window.display();

    }
    return 0;
}


