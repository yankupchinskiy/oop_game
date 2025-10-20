#pragma once
#include "Entity.h"
#include "Cell.h"
#include "Enemy.h"
#include "Spell.h"
#include <memory>

class Hand;

class Player : public Entity {
private:
    enum class Weapon{melee, range, hand};
    Weapon current_weapon;
    int damage;
    Hand* hand;
    int moves;



public:
    int score;

    Player(int hp, int damage, int x, int y);

    void move(const std::string& direction, Gamefield* gf) override;

    Hand* getHand() const { return hand; }
    void weapon_change();
    void print_weapon();
    void cast_spell(std::string direction, Gamefield* gf);
    
};
