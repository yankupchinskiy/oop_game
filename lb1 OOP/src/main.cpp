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

    Gamefield gf(10, 10);
    Player player(100, 20, 0, 0);
    Enemy enemy(50, 10, 3, 0);
    TeslaTower tower(60, 15, 2, 2, 3);

    gf.getCell(0, 0)->set_occupied_by(&player);
    gf.getCell(3, 0)->set_occupied_by(&enemy);
    gf.getCell(0, 0)->set_occupied(true);
    gf.getCell(3, 0)->set_occupied(true);
    
    player.print_weapon();
    player.weapon_change();
    player.print_weapon();
    player.weapon_change();
    player.print_weapon();
    
    player.getHand()->add_random_spell();
    player.getHand()->add_random_spell();
    player.getHand()->add_random_spell();




    std::cout << enemy.getHP() << "\n";
    player.cast_spell("right", &gf);
    std::cout << enemy.getHP() << "\n";
    player.cast_spell("right", &gf);
    std::cout << enemy.getHP() << "\n";
    player.cast_spell("right", &gf);
    std::cout << enemy.getHP() << "\n";
    enemy.move("left", &gf);
    std::cout << enemy.getHP() << "\n";
    enemy.move("left", &gf);
    std::cout << enemy.getHP() << "\n";
    


    std::cout << "Player hp before:\n" << player.getHP() << "\n";
    tower.attack(&gf);
    std::cout << "Player hp after:\n" << player.getHP() << "\n";
    
/*
    Game game(10,10);

    while (game.running()) {

        

        // Обновление состояния игры
        game.update();
        

        // Отрисовка текущего состояния игры
        game.render();
    }
*/



    return 0;
    
}
