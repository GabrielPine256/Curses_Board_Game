#ifndef BOARD_GAME_DICE
#define BOARD_GAME_DICE


#include"place_among.hpp"

#include<vector>


namespace Board_Game {
    using std::vector;
    using std::pair;


// Simulate a dice roll. "srand(time(0));" must be ran before this function. 
int roll_die(const int sides = 6) { return rand() % sides; }

// Roll a die to produce a unique value. 
int roll_die(const vector<int>& existing, const int sides = 6) { return place_among(rand() % (sides - clean_vector(existing, sides).size()), existing, sides); }

// Roll multiple dice. 
vector<int> roll_dice(const int amount, const int sides = 6) {
    vector<int> result(amount, 0);
    for (auto& d : result) { d = roll_die(sides); } // non-const ref
    return result; 
}
vector<int> roll_dice(const pair<int,int>& amount_sides) { return roll_dice(amount_sides.first, amount_sides.second); } 


} // namespace Board_Game

#endif
