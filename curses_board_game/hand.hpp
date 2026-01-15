#ifndef BOARD_GAME_HAND
#define BOARD_GAME_HAND


#include"piece.hpp"
#include"coord.hpp"
#include"place_among.hpp"


#include<vector>
#include<string> 
#include<algorithm> // sort() 


namespace Board_Game {
    using std::vector;
    using std::string;
    using std::pair;
    using std::make_pair; 
    using std::sort; 


// A graphical vector<Piece> 
class Hand {
public:

    Hand(const vector<Piece>& hand = {}, const int justify = 1, const pair<int,int>& origin = make_pair(0,0), const pair<int,int>& offset = make_pair(0,2)): vec{hand}, justify{justify}, origin{origin}, offset{{0,justify+1}} {}
    
    vector<Piece> vec = {}; 
    const int justify = 1; 
    
    const Coord origin = {0,0}; 
    const Coord offset = {0,2}; 
    
}; // class Hand 


pair<int,int> get_offset(const Hand& hand, const int index) { return make_pair(hand.origin.y + index*hand.offset.y, hand.origin.x + index*hand.offset.x); }

void display(const Piece& piece, const Hand& hand, const int index, const bool highlight = false) { display(piece, get_offset(hand, index), hand.justify, highlight); } // callback to piece.hpp 
void display(const Piece& piece, const Hand& hand,                  const bool highlight = false) { for (int i = 0; i < hand.vec.size(); ++i) { display(piece, hand, i,     highlight); } }
void display(                    const Hand& hand, const int index, const bool highlight = false) {                 display(hand.vec[index % hand.vec.size()], hand, index, highlight); }
void display(                    const Hand& hand,                  const bool highlight = false) { for (int i = 0; i < hand.vec.size(); ++i) { display(       hand, i,     highlight); } }

//mvaddch(origin.y, origin.x, '\n'); 
void blank(const Hand& hand, const int index) { display({-1, 0}, hand, index); } 
void blank(const Hand& hand)                  { display({-1, 0}, hand);        } 


bool any_nonnegative(const Hand& hand) { return any_nonnegative(hand.vec); } 
bool any_nonnegative(const vector<Hand>& vec) {          
    for (const auto h : vec) { if (any_nonnegative(h)) { return true; } } 
    return false; 
} 
bool all_nonnegative(const Hand& hand) { return all_nonnegative(hand.vec); } 
bool all_nonnegative(const vector<Hand>& vec) { 
    for (const auto h : vec) { if (!all_nonnegative(h)) { return false; } }
    return true; 
} 
bool any_all_nonnegative(const vector<Hand>& vec) {         
    for (const auto h : vec) { if (all_nonnegative(h)) { return true; } } 
    return false; 
} 
bool all_any_nonnegative(const vector<Hand>& vec) {          
    for (const auto h : vec) { if (!any_nonnegative(h)) { return false; } }
    return true; 
}


void change_value(Hand& hand, const int index, const int max, const bool no_collisions = false) { change_value(hand.vec[index], get_offset(hand,index), hand.justify, max, (no_collisions ? get_val(hand.vec) : vector<int>{})); }


int select(const Hand& hand) { 
    const int size = hand.vec.size(); 
    
    int sel = 0;  
    
    display(hand, sel, true); 
    for (int c = getch(); c != '\n'; c = getch())
        if (c == KEY_RIGHT) {
            display(hand, sel, false);  
            sel = (sel + 1) % size;
            display(hand, sel, true); 
        } else if (c == KEY_LEFT) {
            display(hand, sel, false); 
            sel = (sel - 1 + size) % size; 
            display(hand, sel, true); 
        }
    display(hand, sel, false); 
    
    return sel; 
}

Piece take(Hand& hand, const int selection) { return extract(hand.vec, selection); }
Piece take(Hand& hand) { 
    const int index = select(hand); 
    blank(hand); 
    const auto result = take(hand, index);
    display(hand); 
    return result; 
} 


} // namespace Board_Game

#endif
