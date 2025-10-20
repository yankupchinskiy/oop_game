#pragma once
#include "Cell.h"
#include "Gamefield.h"
#include <SFML/Graphics.hpp>
#include <string>


class Entity {
protected:
    int hp;
    int x_pos;
    int y_pos;
    sf::Sprite sprite;
    sf::Texture texture;

public:

    virtual ~Entity() = default;
    virtual void move(const std::string& direction, Gamefield* gf);
    void get_damaged(int dmg);

    sf::Sprite& getSprite() { return sprite; }                // обычная версия
    const sf::Sprite& getSprite() const { return sprite; }    // константная версия

    int getX() const { return x_pos; }
    int getY() const { return y_pos; }
    int getHP() const { return hp; }

    void syncSprite();


};
