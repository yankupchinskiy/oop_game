#include <iostream>
#include "../include/Player.h"
#include "../include/Enemy.h"
#include "../include/Gamefield.h"
#include "../include/Game.h"
#include "../include/Hand.h" 
#include <SFML/Graphics.hpp> 
#include <optional>
#include <cstdlib>
#include <ctime>


int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    Game game(10,10);

    while (game.running()) {

        

        // Обновление состояния игры
        game.update();
        

        // Отрисовка текущего состояния игры
        game.render();
    }




    return 0;
    
}
