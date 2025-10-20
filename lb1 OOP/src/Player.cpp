#include "../include/Player.h"
#include "../include/Hand.h" 
#include "Player.h"

Player::Player(int hp,int damage, int x, int y) {
    this->hp = hp;
    this->damage = damage;
    this->x_pos = x;
    this->y_pos = y;
    this->current_weapon = Player::Weapon(0);
    this->hand = new Hand();

        if (!this->texture.loadFromFile("../sprites/player.png")){
        std::cerr<<"file loading error";
    }

    float scaleX = static_cast<float>(CELL_SIZE) / this->texture.getSize().x;
    float scaleY = static_cast<float>(CELL_SIZE) / this->texture.getSize().y;
    
    this->sprite.setTexture(texture);
    this->sprite.setScale(scaleX,scaleY);
    
    syncSprite();
}


void Player::move(const std::string& direction, Gamefield* gf){
    int new_x = x_pos;
    int new_y = y_pos;

    if (direction == "up") {
        new_y--;
    } else if (direction == "down") {
        new_y++;
    } else if (direction == "left") {
        new_x--;
    } else if (direction == "right") {
        new_x++;
    } else {
        std::cout << "Unknown direction: " << direction << "\n";
        return;
    }

  
    if (new_x < 0 || new_x >= gf->get_leng() || new_y < 0 || new_y >= gf->get_width()) {
        std::cout << "Move out of bounds! (" << new_x << ", " << new_y << ") is outside the gamefield.\n";
        return;
    }

    Cell* target = gf->getCell(new_x, new_y);

  
    if (!target->is_passable()) {
        std::cout << "Cannot move to (" << new_x << ", " << new_y << "): cell is not passable.\n";
        return;
    }

    if (target->is_occupied()) {
        Entity* entity = target->is_occupied_by();
        Enemy* enemy = dynamic_cast<Enemy*>(entity);
        if (enemy) {
            std::cout << "Enemy detected on spell!\n";
            enemy->get_damaged(damage);
        }
        return;
    }

    gf->getCell(x_pos, y_pos)->set_occupied(false);
    x_pos = new_x;
    y_pos = new_y;
    target->set_occupied(true);

    std::cout << "Player moved to (" << x_pos << ", " << y_pos << ").\n";

}


void Player::weapon_change() {
    this->current_weapon = static_cast<Weapon>(static_cast<int>(current_weapon) + 1);
}

void Player::print_weapon() {
    switch (this->current_weapon) {
        case Weapon::melee:
            std::cout << "Current weapon: melee\n";
            break;
        case Weapon::range:
            std::cout << "Current weapon: range\n";
            break;
        case Weapon::hand:
            std::cout << "Current weapon: hand\n";
            break;
    }
}

void Player::cast_spell(std::string direction, Gamefield *gf) {
    std::shared_ptr<Spell> spell = hand->get_spell(); 

    if (spell == nullptr) {
        std::cout << "No spells in hand to cast.\n";
        return;
    }

    spell->cast(direction, gf, this->x_pos, this->y_pos);
}
