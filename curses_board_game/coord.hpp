#ifndef BOARD_GAME_COORD
#define BOARD_GAME_COORD


#include<utility> // pair


namespace Board_Game {
    using std::pair;


// Makes the rest of the program clearer than .first and .second .
class Coord { 
public:
    Coord(const int y, const int x): y{y}, x{x} {}
    Coord(const pair<int,int>& p): Coord(p.first, p.second) {} 

    int y = 0;
    int x = 0;
    
    bool operator==(const Coord& c) const { return (y == c.y) && (x == c.x); }
    bool operator==(const pair<int,int>& p) const { return (y == p.first) && (x == p.second); }
}; // class Coord 


} // namespace Board_Game

#endif
