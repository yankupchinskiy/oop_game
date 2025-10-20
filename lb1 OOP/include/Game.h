#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Gamefield.h"
#include "Enemy.h"
#include "Player.h"
#include "Cell.h"
#include "Tower.h"


class Game{
    private:
    //variables

    //window
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;

    //game objects
    sf::Sprite sprite;
    sf::Texture texture;
    Gamefield gamefield;
    const int cellSize = CELL_SIZE;

    Player player;                  
    std::vector<std::unique_ptr<Enemy>> enemies;  
      
    //Privare functions
    void initVariables();
    void initWindow();
    void initEntities();


    public:
    // Constructors and Destructors
    
    Game(int width, int length);
    Game();
    virtual ~Game();

    //accessors
    const bool running() const;

    //functions
    void update();
    void render();
    void PollEvents();

};