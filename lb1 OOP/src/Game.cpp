#include "../include/Game.h"
#include  <iostream>
#include <memory>


//Private functions
void Game::initVariables(){
    this->window = nullptr;

}



void Game::initWindow() {
    this->videoMode.width = gamefield.get_leng() * cellSize;
    this->videoMode.height = gamefield.get_width() * cellSize;

    this->window = new sf::RenderWindow(
        this->videoMode,
        "Custom Gamefield",
        sf::Style::Titlebar | sf::Style::Close
    );

    this->window->setFramerateLimit(60);
}



//Constructors and Destructors
Game::Game()
    : gamefield(10, 10), player(100, 20, 0, 0)   
{
    this->initVariables();
    this->initWindow();
    this->initEntities();

}

Game::Game(int width, int length)
    : gamefield(width, length),player(100, 20, 2, 0)  
{
    this->initVariables();
    this->initWindow();
    this->initEntities();


}

Game::~Game(){
    delete this->window;
}
//accessors

const bool Game::running() const
{
    return this->window->isOpen();
}

//Functions
void Game::initEntities() {
    std::unique_ptr<Player> player;
    player = std::make_unique<Player>(100, 20, 2, 0);

    this->enemies.clear();

    this->enemies.push_back(std::make_unique<Enemy>(50, 10, 3, 0));
    this->enemies.push_back(std::make_unique<TeslaTower>(60, 15, 2, 2, 3));
}




void Game::update(){
    this->PollEvents();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        this->player.move("up", &this->gamefield);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        this->player.move("down", &this->gamefield);        
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        this->player.move("left", &this->gamefield);        
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        this->player.move("right", &this->gamefield);
    }
}

void Game::render() {
    window->clear(sf::Color::Black);


    for (int y = 0; y < gamefield.get_width(); ++y) {
        for (int x = 0; x < gamefield.get_leng(); ++x) {
            Cell* cell = gamefield.getCell(x, y);
            if (cell) {
                sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                rect.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                rect.setFillColor(cell->is_passable() ? sf::Color(150, 200, 150) : sf::Color(150, 50, 50));
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(1.f);
                window->draw(rect);
            }
        }
    }

    window->draw(this->player.getSprite());

    for (const auto& enemy : enemies)
        window->draw(enemy->getSprite());



    window->display();
}




void Game::PollEvents(){
    while (this->window->pollEvent(this->ev)){
        switch(this->ev.type){

            case sf::Event::Closed:
                this->window->close();
                break;
            

            case sf::Event::KeyPressed:
                if(this->ev.key.code == sf::Keyboard::Escape)
                    this->window->close();
                break;

        }
    }  
}