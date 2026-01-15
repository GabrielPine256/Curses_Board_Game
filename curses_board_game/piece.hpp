#ifndef BOARD_GAME_PIECE
#define BOARD_GAME_PIECE


#include"place_among.hpp"
#include"to_curses.hpp" 
#include"coord.hpp"

#include<iostream> // cout
#include<string> 
#include<utility> // pair


namespace Board_Game {
    using std::cout; 
    using std::string;
    using std::to_string;
    using std::pair; 
    

// For a playing card, value can be Rank, color can be Suit. For chess pieces, value would be 9 for a Queen, 5 for a Rook, etc. 
class Piece { 
public: 
    Piece(const int value, const int color): value{value}, color{color} {}
    Piece(const pair<int,int>& p): Piece(p.first, p.second) {}

    int value = 0;
    int color = 0; 
    
    bool operator==(const Piece& p) const { return (value == p.value) && (color == p.color); }
}; // class Piece  


// Better to write this out here, than do all of the math in mvaddstr throughout the program. 
void display(const Piece& piece, const int y, const int x, const int justify, const bool highlight = false) { 
    mvaddstr(y, x, string(justify, ' ').c_str());           // Blank the space.
    if (piece.value < 0) { return; }                        // Use negative values to mean blank. 
    attron(COLOR_PAIR(piece.color + (highlight ? 8 : 0)));  // Set the color according to highlight. 
    const string str = to_string(piece.value + 1);          // Print the 0 indexed value itself. This would be the line to change to represent 5 as 'R' and 9 as 'Q'. 
    mvaddstr(y, x + (justify - str.size()), str.c_str());   // Place the text justified in the space. 
    attroff(COLOR_PAIR(piece.color + (highlight ? 8 : 0))); // Unset the color according to highlight. 
}
void display(const Piece& piece, const Coord& coord, const int justify, const bool highlight = false) { display(piece, coord.y, coord.x, justify, highlight); } 


int get_val(const vector<Piece>& pieces, const int index) { return pieces[index % pieces.size()].value; } 
int get_col(const vector<Piece>& pieces, const int index) { return pieces[index % pieces.size()].color; } 
vector<int> get_val(const vector<Piece>& pieces) { 
    vector<int> result(pieces.size(), 0);
    for (int i = 0; i < pieces.size(); ++i) { result[i] = get_val(pieces, i); } 
    return result; 
}
vector<int> get_col(const vector<Piece>& pieces) { 
    vector<int> result(pieces.size(), 0);
    for (int i = 0; i < pieces.size(); ++i) { result[i] = get_col(pieces, i); } 
    return result; 
}

void set_val(vector<Piece>& pieces, const int index, const int value) { pieces[index % pieces.size()].value = value; } 
void set_col(vector<Piece>& pieces, const int index, const int color) { pieces[index % pieces.size()].color = color; } 
void set_val(vector<Piece>& pieces, const int value)           { for (int i = 0; i < pieces.size(); ++i) { set_val(pieces, i, value); } } 
void set_col(vector<Piece>& pieces, const int color)           { for (int i = 0; i < pieces.size(); ++i) { set_col(pieces, i, color); } } 
void set_val(vector<Piece>& pieces, const vector<int>& values) { for (int i = 0; i < pieces.size() && i < values.size(); ++i) { set_val(pieces, i, values[i]); } } 
void set_col(vector<Piece>& pieces, const vector<int>& colors) { for (int i = 0; i < pieces.size() && i < colors.size(); ++i) { set_col(pieces, i, colors[i]); } }


int get_val(const vector<vector<Piece>>& vv, const int y, const int x) { return get_val(vv[y % vv.size()], x); }
int get_val(const vector<vector<Piece>>& vv, const Coord& yx)          { return get_val(vv, yx.y, yx.x); } 
int get_col(const vector<vector<Piece>>& vv, const int y, const int x) { return get_col(vv[y % vv.size()], x); } 
int get_col(const vector<vector<Piece>>& vv, const Coord& yx)          { return get_col(vv, yx.y, yx.x); } 

vector<int> get_val(const vector<vector<Piece>>& vv, const int y) { return get_val(vv[y % vv.size()]); } 
vector<int> get_col(const vector<vector<Piece>>& vv, const int y) { return get_col(vv[y % vv.size()]); } 

vector<vector<int>> get_val(const vector<vector<Piece>>& vv) { 
    vector<vector<int>> result(vv.size(), vector<int>(vv[0].size(), 0));
    for (int i = 0; i < vv.size(); ++i) { result[i] = get_val(vv, i); } 
    return result;  
} 
vector<vector<int>> get_col(const vector<vector<Piece>>& vv) { 
    vector<vector<int>> result(vv.size(), vector<int>(vv[0].size(), 0));
    for (int i = 0; i < vv.size(); ++i) { result[i] = get_col(vv, i); } 
    return result; 
} 


void set_val(vector<vector<Piece>>& vv, const int y, const int x, const int value) { set_val(vv[y % vv.size()], x, value); } 
void set_val(vector<vector<Piece>>& vv, const Coord& yx, const int value)          { set_val(vv, yx.y, yx.x, value); } 
void set_col(vector<vector<Piece>>& vv, const int y, const int x, const int color) { set_col(vv[y % vv.size()], x, color); } 
void set_col(vector<vector<Piece>>& vv, const Coord& yx, const int color)          { set_col(vv, yx.y, yx.x, color); } 

void set_val(vector<vector<Piece>>& vv, const int y, const vector<int>& values) { set_val(vv[y % vv.size()], values); }
void set_col(vector<vector<Piece>>& vv, const int y, const vector<int>& colors) { set_col(vv[y % vv.size()], colors); } 
void set_val(vector<vector<Piece>>& vv, const int y, const int value) { set_val(vv[y % vv.size()], value); } 
void set_col(vector<vector<Piece>>& vv, const int y, const int color) { set_col(vv[y % vv.size()], color); } 

void set_val(vector<vector<Piece>>& vv, const vector<vector<int>>& values) { for (int i = 0; i < vv.size() && i < values.size(); ++i) { set_val(vv, i, values[i]); } } 
void set_col(vector<vector<Piece>>& vv, const vector<vector<int>>& colors) { for (int i = 0; i < vv.size() && i < colors.size(); ++i) { set_col(vv, i, colors[i]); } } 
void set_val(vector<vector<Piece>>& vv, const int value) { for (int i = 0; i < vv.size(); ++i) { set_val(vv, i, value); } } 
void set_col(vector<vector<Piece>>& vv, const int color) { for (int i = 0; i < vv.size(); ++i) { set_col(vv, i, color); } } 



bool is_nonnegative(const int d) { return !(d < 0); } 
bool any_nonnegative(const vector<int>& vec) {          
    for (const auto d : vec) { if (is_nonnegative(d)) { return true; } } 
    return false; 
} 
bool any_nonnegative(const vector<vector<int>>& vv) {          
    for (const auto vec : vv) { if (any_nonnegative(vec)) { return true; } } 
    return false; 
} 
bool all_nonnegative(const vector<int>& vec) { 
    for (const auto d : vec) { if (!is_nonnegative(d)) { return false; } }
    return true; 
} 
bool all_nonnegative(const vector<vector<int>>& vv) { 
    for (const auto vec : vv) { if (!all_nonnegative(vec)) { return false; } }
    return true; 
} 
bool is_nonnegative(const Piece& piece)               { return  is_nonnegative(piece.value);     } 
bool any_nonnegative(const vector<Piece>& pieces)     { return any_nonnegative(get_val(pieces)); } 
bool any_nonnegative(const vector<vector<Piece>>& vv) { return any_nonnegative(get_val(vv));     } 
bool all_nonnegative(const vector<Piece>& pieces)     { return all_nonnegative(get_val(pieces)); } 
bool all_nonnegative(const vector<vector<Piece>>& vv) { return all_nonnegative(get_val(vv));     } 

// Pass by non-const reference 
void sort_ascend(vector<Piece>& vec)  { sort(vec.begin(), vec.end(), [](const auto& a, const auto& b){return a.value < b.value;}); } 
void sort_descend(vector<Piece>& vec) { sort(vec.begin(), vec.end(), [](const auto& a, const auto& b){return a.value > b.value;}); }


void change_value(Piece& piece, const int y, const int x, const int justify, const int max, const vector<int>& exist = {}) { 
    if (max <= 0) { return; } 
    
    vector<int> existing = clean_vector(exist, max); 
    existing.erase(find(existing.begin(), existing.end(), piece.value)); // Don't count the piece's own value as an obstacle. 
    
    const int range = max - existing.size(); 
    if (range <= 0) { return; }
    
    if (!existing.empty()) // If there are obstacles, begin the value at the smallest available. 
        for (piece.value = 0; count(existing.begin(), existing.end(), piece.value); ++piece.value);  
    int d = piece.value;
    
    display(piece, y, x, justify, true);
    for (int c = getch(); c != '\n'; c = getch()) {
        if      (c == KEY_UP)   { d = (d + 1)         % range; } 
        else if (c == KEY_DOWN) { d = (d - 1 + range) % range; } 
        piece.value = (existing.empty() ? d : place_among(d, existing, max)); 
        display(piece, y, x, justify, true);
    }
    display(piece, y, x, justify, false);
}
void change_value(Piece& piece, const Coord& coord, const int justify, const int max, const vector<int>& exist = {}) { change_value(piece, coord.y, coord.x, justify, max, exist); } 


template <typename T>
T extract(vector<T>& vec, int index = 0) { 
    if (!( index < vec.size() )) { index = vec.size() - 1; } // range-checking 
    const auto result = vec[index]; 
    vec.erase(index + vec.begin()); 
    return result; 
}


template <typename T> // T must support the == operator. 
vector<bool> find_dup(const vector<T>& vec) { // depends on a sorted input vector 
    vector<bool> result(vec.size(), false); 
    for (int i = 0; i < vec.size() - 1; ++i) {
    
        if (!(vec[i] == vec[i+1])) { continue; } // If this matches the next, activate the corresponding bool. 
        result[i] = true;  
        
        int j = i+1; // Activate all following that match. 
        for (int j = i+1; j < vec.size(); ++j) {
            if (!(vec[i] == vec[j])) { break; } 
            result[j] = true; 
        }
        i = j; // Pick up from where the loop left off. 
    }
    return result; 
}


// To stdout, not curses. 
void print_pieces(const vector<Piece>& vec) { 
    for (const auto& piece : vec) 
        cout << color_name[piece.color] << "\t" << piece.value << "\n";
}


} // namespace Board_Game

#endif
