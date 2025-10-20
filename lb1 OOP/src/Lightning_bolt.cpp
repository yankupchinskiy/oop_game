#include "../include/Lightning_bolt.h"

//constructor

Lightning_bolt::Lightning_bolt() {
        setName("Lightning Bolt");
        setDamage(10);
        setRange(3);
    }


//functions

void Lightning_bolt::cast(std::string direction, Gamefield* gf,int pl_X, int pl_Y){
    if(direction != "up" && direction != "down" && direction != "left" && direction != "right") {
        std::cout << "Unknown direction: " << direction << "\n";
        return;
    }
    else if (direction == "up"){
        for (int i = 1; i <= getRange(); ++i) {
            Cell* target = gf->getCell(pl_X, pl_Y - i);
            if (target == nullptr) break; 
            if (target->is_occupied()) {
                Entity* entity = target->is_occupied_by();
                
                if (entity) {
                    entity->get_damaged(getDamage());
                    std::cout << "Lightning Bolt hit an somebody at (" << pl_X << ", " << pl_Y - i << ") for " << getDamage() << " damage.\n";
                    break; 
                }
            }
        }
    }
    else if (direction == "down"){
        for (int i = 1; i <= getRange(); ++i) {
            Cell* target = gf->getCell(pl_X, pl_Y + i);
            if (target == nullptr) break; 
            if (target->is_occupied()) {
                Entity* entity = target->is_occupied_by();
                Enemy* enemy = dynamic_cast<Enemy*>(entity);
                if (entity) {
                    entity->get_damaged(getDamage());
                    std::cout << "Lightning Bolt hit an somebody at (" << pl_X << ", " << pl_Y * i << ") for " << getDamage() << " damage.\n";
                    break; 
                }
            }
        }
    }
    else if (direction == "left"){
        for (int i = 1; i <= getRange(); ++i) {
            Cell* target = gf->getCell(pl_X - i, pl_Y);
            if (target == nullptr) break; 
            if (target->is_occupied()) {
                Entity* entity = target->is_occupied_by();
                if (entity) {
                    entity->get_damaged(getDamage());
                    std::cout << "Lightning Bolt hit an somebody at (" << pl_X  - i<< ", " << pl_Y << ") for " << getDamage() << " damage.\n";
                    break; 
                }
            }
        }
    }
    else if (direction == "right"){
        for (int i = 1; i <= getRange(); ++i) {
            Cell* target = gf->getCell(pl_X + i, pl_Y);
            if (target == nullptr) break; 
            if (target->is_occupied()) {
                Entity* entity = target->is_occupied_by();
                if (entity) {
                    entity->get_damaged(getDamage());
                    std::cout << "Lightning Bolt hit an somebody at (" << pl_X + i << ", " << pl_Y << ") for " << getDamage() << " damage.\n";
                    break; 
                }
            }
        }
    }
    
    
}

