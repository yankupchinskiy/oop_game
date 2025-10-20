#include "../include/Tower.h"



//constructors and destructors

TeslaTower::TeslaTower(int hp, int dmg, int x, int y, int range){
    this->hp = hp;
    this->setDamage(dmg);
    this->x_pos = x;
    this->y_pos = y;
    this->range = range;

    if (!this->texture.loadFromFile("../sprites/tesla_tower.png")){
        std::cerr<<"file loading error";
    }
    float scaleX = static_cast<float>(CELL_SIZE) / this->texture.getSize().x;
    float scaleY = static_cast<float>(CELL_SIZE) / this->texture.getSize().y;
    
    this->sprite.setTexture(texture);
    std::cout << "scaleX=" << scaleX << " scaleY=" << scaleY << "\n";

    this->sprite.setScale(scaleX,scaleY);
    
    syncSprite();
}


TeslaTower::TeslaTower(const TeslaTower& other)
    : Enemy(other.hp, other.getDamage(), other.x_pos, other.y_pos),
      range(other.range)
{
    if (!this->texture.loadFromFile("../sprites/tesla_tower.png")) {
        std::cerr << "file loading error in copy constructor\n";
    }
    this->sprite.setTexture(this->texture);
    float scaleX = static_cast<float>(CELL_SIZE) / this->texture.getSize().x;
    float scaleY = static_cast<float>(CELL_SIZE) / this->texture.getSize().y;
    this->sprite.setScale(scaleX, scaleY);

    syncSprite();
}


TeslaTower& TeslaTower::operator=(const TeslaTower& other)
{
    if (this == &other)
        return *this;

    this->hp = other.hp;
    this->setDamage(other.getDamage());
    this->x_pos = other.x_pos;
    this->y_pos = other.y_pos;
    this->range = other.range;

    if (!this->texture.loadFromFile("../sprites/tesla_tower.png")) {
        std::cerr << "file loading error in assignment operator\n";
    }

    this->sprite.setTexture(this->texture);

    float scaleX = static_cast<float>(CELL_SIZE) / this->texture.getSize().x;
    float scaleY = static_cast<float>(CELL_SIZE) / this->texture.getSize().y;

    this->sprite.setScale(scaleX, scaleY);

    syncSprite();

    return *this;
}


//functions

void TeslaTower::move(const std::string& direction, Gamefield* gf) {
    std::cout << "TeslaTower at (" << x_pos << ", " << y_pos << ") does not move.\n";
}



void TeslaTower::attack(Gamefield *gf){
    for (int dx = -range; dx <= range; ++dx) {
        for (int dy = -range; dy <= range; ++dy) {
            if (abs(dx) + abs(dy) <= range) {
                Cell* target = gf->getCell(x_pos + dx, y_pos + dy);
                if (target && target->is_occupied()) {
                    Entity* entity = target->is_occupied_by();
                    Player* player = dynamic_cast<Player*>(entity);
                    if (player) {
                        this->cast_spell(gf);
                        return;
                    }
                }
            }
        }
    }
}


void TeslaTower::cast_spell( Gamefield *gf){
    Lightning_bolt spell;
    for (const std::string& direction : {"up", "down", "left", "right"}){
        spell.cast(direction, gf, x_pos, y_pos);
    }
    
}