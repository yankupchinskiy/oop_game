#include "../include/Game.h"
#include  <iostream>
#include <memory>
#include <cmath>
#include <algorithm>
#include <climits>
#include <fstream>

// Private functions

void Game::initVariables(){
    this->window = nullptr;
    this->state = GameState::IN_GAME;
    this->turn = TurnState::PLAYER;
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


// Constructors and Destructors

Game::Game()
    : gamefield(10, 10)
{
    this->initVariables();
    this->initWindow();
    this->initEntities(currentLevel);
}


Game::Game(int width, int length)
    : gamefield(width, length)
{
    this->initVariables();
    this->initWindow();
    this->initEntities(currentLevel);
}

Game::~Game(){
    delete this->window;
}

// accessors

const bool Game::running() const
{
    return this->window && this->window->isOpen();
}


// Functions

void Game::initEntities(int level) {

    for (int y = 0; y < gamefield.get_width(); ++y) {
        for (int x = 0; x < gamefield.get_leng(); ++x) {
            Cell* c = gamefield.getCell(x, y);
            if (c) {
                c->set_occupied(false);
                c->set_occupied_by(nullptr);
            }
        }
    }

    // Игрок
    this->player = Player(100 + level * 10, 20 + level * 2, 0, 0);

    // Враги и башни
    enemies.clear();
    towers.clear();

    int enemyCount = 1 + level;
    int enemyHP = 50 + level * 10;
    int enemyDamage = 10 + level * 2;

    for (int i = 0; i < enemyCount; i++) {
        int ex = rand()%10 + i;
        int ey = rand()%10;

        auto e = std::make_unique<Enemy>(enemyHP, enemyDamage, ex, ey);

        enemies.push_back(std::move(e));

        Cell* c = gamefield.getCell(ex, ey);
        c->set_occupied(true);
        c->set_occupied_by(enemies.back().get());
    }

        int tx = rand()%10;
        int ty = rand()%10;

    towers.push_back(
        std::make_unique<TeslaTower>(60 + 10 * level, 15 + 5 * level, tx, ty, 3 + level)
    );

    Cell* tc = gamefield.getCell(tx, ty);
    tc->set_occupied(true);
    tc->set_occupied_by(towers[0].get());
}


// Helper

bool Game::isAdjacent(int ax, int ay, int bx, int by) {
    int dx = std::abs(ax - bx);
    int dy = std::abs(ay - by);
    return (dx + dy) == 1; 
}


void Game::update(){
    this->PollEvents();

bool madeAction = false;

    if (player.moveClock.getElapsedTime().asSeconds() > player.moveDelay) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            this->player.move("up", &this->gamefield);
            madeAction = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            this->player.move("down", &this->gamefield);
            madeAction = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            this->player.move("left", &this->gamefield);
            madeAction = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            this->player.move("right", &this->gamefield);
            madeAction = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
            this->player.weapon_change();
            madeAction = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            this->player.cast_spell("right", &this->gamefield);
            madeAction = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            this->player.cast_spell("left", &this->gamefield);
            madeAction = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            this->player.cast_spell("up", &this->gamefield);
            madeAction = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            this->player.cast_spell("down", &this->gamefield);
            madeAction = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5)) {
            SaveGame("savegame.txt");
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F6)) {
            LoadGame("savegame.txt");
        }

        if (madeAction) {
            player.moveClock.restart();
            if (this->turn == TurnState::PLAYER)
                this->turn = TurnState::ENEMY; 
        }
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (this->turn == TurnState::PLAYER) {
            this->turn = TurnState::ENEMY;
        }
    }

    this->updateTurn();

    if (this->checkVictory()) {
        currentLevel++;
        std::cout << "=== LEVEL COMPLETE  ===" << std::endl;
        std::cout << "=== CONGARTULATIONS  ===" << std::endl;
        std::cout << "=== YO`RE NOW AT LEVEL" << currentLevel << "  ===" << std::endl;
        initEntities(currentLevel);
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

    for (const auto& tw : towers)
        window->draw(tw->getSprite());

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
            default:
                break;
        }
    }  
}


void Game::updateTurn() {
    switch (this->turn) {
        case TurnState::PLAYER:
            break;

        case TurnState::ENEMY:
            this->handleEnemies();
            this->turn = TurnState::EN_TOWER;

            enemies.erase(
                std::remove_if(enemies.begin(), enemies.end(),
                    [](const std::unique_ptr<Enemy>& e){
                        return !e->isAlive();
                    }
                ),
                enemies.end()
            );
            this->handlePlayerDeath();
            if (state == GameState::GAME_OVER) return;
            break;


        case TurnState::EN_TOWER:
            this->handleTowers();
            this->turn = TurnState::PLAYER;
            this->handlePlayerDeath();
            if (state == GameState::GAME_OVER) return;
            break;

        default:
            break;
    }
}


void Game::handleEnemies() {
    if(playerDeathHandled)
        return;
    for (auto& eptr : enemies) {
        Enemy* e = eptr.get();
        if (!e) continue;

        if (!e->isAlive()) {
            if (Cell* c = gamefield.getCell(e->getX(), e->getY())) {
                c->set_occupied(false);
                c->set_occupied_by(nullptr);
            }
            continue;
        }

        int ex = e->getX();
        int ey = e->getY();
        int px = player.getX();
        int py = player.getY();

        if (isAdjacent(ex, ey, px, py)) {
            int dmg = e->getDamage();
            player.get_damaged(dmg);
            continue;
        }
        


        std::string dir;

        int dx = px - ex;
        int dy = py - ey;

        if (std::abs(dx) > std::abs(dy)) {
            dir = (dx > 0) ? "right" : "left";
        } else {
            dir = (dy > 0) ? "down" : "up";
        }

        e->move(dir, &gamefield);
    }
}


void Game::handleTowers() {
    if(playerDeathHandled)
    return;
    for (auto& tptr : towers) {
        TeslaTower* t = tptr.get();
        if (!t) continue;

        t->attack(&gamefield);
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& e){
                return !e->isAlive();
            }),
        enemies.end()
        
    );

    towers.erase(
        std::remove_if(towers.begin(), towers.end(),
            [&](const std::unique_ptr<TeslaTower>& t) {
                if (!t->isAlive()) {
                    if (Cell* c = gamefield.getCell(t->getX(), t->getY())) {
                        c->set_occupied(false);
                        c->set_occupied_by(nullptr);
                    }
                    return true; 
                }
                return false;
            }),
        towers.end()
    );
}


bool Game::checkVictory() {
    if (playerDeathHandled || state == GameState::GAME_OVER)
    return false;

    if (player.getHP() <= 0) return true;

    bool anyAlive = false;
    for (auto& e : enemies)
        if (e->isAlive()) anyAlive = true;

    return !anyAlive;
}


void Game::handlePlayerDeath() {

    if (player.getHP() > 0) return;            
    if (playerDeathHandled) return;            

    playerDeathHandled = true;
    
    if (player.getHP() <= 0) {
        std::cout << "=== GAME OVER ===\n";
        std::cout << "=== MAYBE GET GOOD? ===\n";

        this->state = GameState::GAME_OVER;

        if (Cell* c = gamefield.getCell(player.getX(), player.getY())) {
            c->set_occupied(false);
            c->set_occupied_by(nullptr);
        }

        if (this->window) {
            this->window->close();
        }
    }
}


void Game::SaveGame(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file for saving: " << filename << "\n";
        return;
    }

    // Сохраняем размеры поля
    file << gamefield.get_leng() << " " << gamefield.get_width() << "\n";

    // Сохраняем клетки
    for (int y = 0; y < gamefield.get_width(); ++y) {
        for (int x = 0; x < gamefield.get_leng(); ++x) {
            Cell* c = gamefield.getCell(x, y);
            file << x << " " << y << " "
                 << c->is_passable() << " "
                 << c->is_occupied() << "\n";
        }
    }

    // Игрок
    file << player.getX() << " " << player.getY() << " " << player.getHP() << "\n";

    // Враги
    file << enemies.size() << "\n";
    for (auto& e : enemies) {
        file << e->getX() << " " << e->getY() << " " << e->getHP() << " " << e->getDamage() << "\n";
    }

    // Башни
    file << towers.size() << "\n";
    for (auto& t : towers) {
        file << t->getX() << " " << t->getY() << " "
             << t->getHP() << " " << t->getDamage() << " " << t->getRange() << "\n";
    }

    file.close();
    std::cout << "Game saved to " << filename << "\n";
}


void Game::LoadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file for loading: " << filename << "\n";
        return;
    }

    int width, height;
    file >> width >> height;

    gamefield = Gamefield(width, height);

    // Загружаем клетки
    for (int i = 0; i < width * height; ++i) {
        int x, y, passable, occupied;
        file >> x >> y >> passable >> occupied;
        Cell* c = gamefield.getCell(x, y);
        c->set_passable(passable != 0);
        c->set_occupied(occupied != 0);
        c->set_occupied_by(nullptr); 
    }

    // Игрок
    int px, py, php;
    file >> px >> py >> php;
    player.setPosition(px, py);
    player.setHP(php);
    gamefield.getCell(px, py)->set_occupied(true);
    gamefield.getCell(px, py)->set_occupied_by(&player);

    // Враги
    size_t numEnemies;
    file >> numEnemies;
    enemies.clear();
    for (size_t i = 0; i < numEnemies; ++i) {
        int ex, ey, ehp, edmg;
        file >> ex >> ey >> ehp >> edmg;
        auto e = std::make_unique<Enemy>(ehp, edmg, ex, ey);
        gamefield.getCell(ex, ey)->set_occupied(true);
        gamefield.getCell(ex, ey)->set_occupied_by(e.get());
        enemies.push_back(std::move(e));
    }

    // Башни
    size_t numTowers;
    file >> numTowers;
    towers.clear();
    for (size_t i = 0; i < numTowers; ++i) {
        int tx, ty, thp, tdmg, trange;
        file >> tx >> ty >> thp >> tdmg >> trange;
        auto t = std::make_unique<TeslaTower>(thp, tdmg, tx, ty, trange);
        gamefield.getCell(tx, ty)->set_occupied(true);
        gamefield.getCell(tx, ty)->set_occupied_by(t.get());
        towers.push_back(std::move(t));
    }

    file.close();
    std::cout << "Game loaded from " << filename << "\n";
}



/// Для 4ой лабы
void Game::showMainMenu(){}
void Game::handleGameOver(){}
void Game::startNewGame(){}

