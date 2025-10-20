#include "Enemy.h"
#include "Gamefield.h"
#include "Lightning_bolt.h"

class TeslaTower: public Enemy{
private:
    int range;

public:
    //constructors and destructors
    TeslaTower(int hp, int dmg, int x, int y, int range);
    TeslaTower(const TeslaTower& other);             
    TeslaTower& operator=(const TeslaTower& other);


    //functions
    void move(const std::string& direction, Gamefield* gf) override;
    void attack(Gamefield* gf);
    void cast_spell(Gamefield *gf);



};