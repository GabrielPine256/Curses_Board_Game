#ifndef BOARD_GAME_BOARD
#define BOARD_GAME_BOARD


#include"hand.hpp"

#include<vector>
#include<map> 

namespace Board_Game {
    using std::vector;
    using std::map; 
    using std::pair;
    using std::make_pair; 
    using std::sort; 


// Graphical capability for a 2-dimensional set of Pieces. 
class Board {
public: 
    
    // The origin is {1,2} and offset is {2,justify+3} to align with the outline. 
    Board(const vector<vector<Piece>>& vv, const int justify = 1, const Coord& origin = {1,2}, const Coord& offset = {2,4}): vv{vv}, justify{justify}, origin{origin}, offset{{2,justify+3}} {}
    Board(const Coord& dim = {8,8}, const int justify = 1, const Coord& origin = {1,2}, const Coord& offset = {2,4}): vv{vector<vector<Piece>>(dim.y, vector<Piece>(dim.x, Piece{-1,0}))}, justify{justify}, origin{origin}, offset{{2,justify+3}} {}
    
    Coord dim() const { return {int(vv.size()),int(vv[0].size())}; } 
    
    vector<vector<Piece>> vv = {}; 
    const int justify = 1;         // How wide the pieces are 
    
    const Coord origin = {1,2};
    const Coord offset = {2,4}; 
    
}; // class Board


pair<int,int> get_offset(const Board& board, const int y, const int x) { return make_pair(board.origin.y + y*board.offset.y, board.origin.x + x*board.offset.x); } 
pair<int,int> get_offset(const Board& board, const Coord& yx) { return get_offset(board, yx.y, yx.x); }


void display(const Piece& piece, const Board& board, const int y, const int x, const bool highlight = false) { display(piece, get_offset(board, y, x), board.justify, highlight); } // callback to piece.hpp 
void display(const Piece& piece, const Board& board, const Coord& yx,          const bool highlight = false) { display(piece, board, yx.y, yx.x, highlight); } 

void display(const Piece& piece, const Board& board,                           const bool highlight = false) { 
    for (int i = 0; i < board.dim().y; ++i) 
    for (int j = 0; j < board.dim().x; ++j)
        display(piece, board, i, j, highlight); 
} 

void display(                    const Board& board, const int y, const int x, const bool highlight = false) { display(board.vv[y % board.dim().y][x % board.dim().x], board, y, x, highlight); }
void display(                    const Board& board, const Coord& yx,          const bool highlight = false) { display(board, yx.y, yx.x, highlight); }

void display(                    const Board& board,                           const bool highlight = false) { 
    for (int i = 0; i < board.dim().y; ++i) 
    for (int j = 0; j < board.dim().x; ++j)
        display(board, i, j, highlight);  
}


void blank(const Board& board, const int y, const int x) { display({-1, 0}, board, y, x, false); } 
void blank(const Board& board, const Coord& yx)          { blank(board, yx.y, yx.x); }

void blank(const Board& board) { display({-1, 0}, board); }


bool any_nonnegative(const Board& board) { return any_nonnegative(board.vv); } 
bool all_nonnegative(const Board& board) { return all_nonnegative(board.vv); } 


void fill(Board& board, const Piece& piece) { for (auto& vec : board.vv) { fill(vec.begin(), vec.end(), piece); } }
void clear(Board& board) { fill(board, {-1, 0}); }


vector<Piece> count_pieces(const Board& board, const bool count_values = true) {
    map<int,int> m = {}; // Use a map to simplify indexing. 
    
    for (int i = 0; i < board.dim().y; ++i) {
    for (int j = 0; j < board.dim().x; ++j) { 
        m.insert( {get_col(board.vv,i,j), 0} ); // .insert() does nothing if key exists. 
        m[get_col(board.vv,i,j)] += (count_values ? get_val(board.vv,i,j)+1 : 1); // +1 for 0-indexing. 
    }}
    m.erase(0); // Black is used for drawing blank squares. 
    
    vector<Piece> result = {}; 
    for (const auto& count : m) 
        result.push_back(Piece(count.second,count.first)); 
    sort_descend(result); 
    
    return result;
}


// This function takes advantage of base-number dynamics (quotients and remainders) to simplify the wrap-around behavior. 
pair<int,int> grid_move(const Board& board, const Coord& start = {0,-1}, const bool positive = true, const bool horizontal = true) {
    const int max = board.dim().y * board.dim().x; 
    
    int index = (start.y * board.dim().x) + start.x; 
    do {
        if (horizontal) {
            if (positive) { ++index; }
            else          { --index; } 
        } else {
            if (positive) { 
                if (index/board.dim().x == board.dim().y - 1) { ++index; } // Change columns when wrapping around vertically. 
                if (index != max) { index += board.dim().x; } // If the whole board has been crossed, the above was enough. 
            } else { 
                if (index/board.dim().x == 0                ) { --index; } // Change columns when wrapping around vertically. 
                if (index != -1 ) { index -= board.dim().x; } // If the whole board has been crossed, the above was enough. 
            }
        }
        index = (index + max) % max; // Clean index. 
        
    } while (is_nonnegative(get_val(board.vv, index/board.dim().x, index%board.dim().x))); // Continue until valid tile. 
    
    return make_pair(index/board.dim().x, index%board.dim().x); // Return said tile. 
}


pair<int,int> place(Board& board, const Piece& piece) {
    
    if (all_nonnegative(board.vv)) { return make_pair(-1,-1); } 
    
    Coord result = grid_move(board); 
    board.vv[result.y][result.x] = piece;  
    
    display(board, result.y, result.x, true); 
    for (int c = getch(); c != '\n'; c = getch()) {
        board.vv[result.y][result.x] = {-1,0}; 
        blank(board, result.y, result.x); // conceals the display, does not change the value

        if (c == KEY_UP) 
            result = grid_move(board, result, false, false); 
        else if (c == KEY_LEFT) 
            result = grid_move(board, result, false, true); 
        else if (c == KEY_DOWN) 
            result = grid_move(board, result, true, false);
        else if (c == KEY_RIGHT) 
            result = grid_move(board, result, true, true);
        board.vv[result.y][result.x] = piece; 
        
        display(board, result.y, result.x, true); 
        
    } 
    display(board, result.y, result.x, false); 
    return make_pair(result.y,result.x); 
}


void outline_board(const Coord& dim = {8,8}, const Coord& origin = {0,0}, const Coord& offset = {2,4}) {  
    
    string vertical = string(dim.x * offset.x + 1, ' ');
    for (int i = 0; i < vertical.size(); i += offset.x) { vertical[i] = '|'; } 
    
    const string horizontal = string(dim.x * offset.x + 1, '-');
    
    mvaddstr(origin.y, origin.x, horizontal.c_str());
    for (int i = 0; i < dim.y; ++i) { 
        for (int j = 1; j < offset.y; ++j) 
            mvaddstr(origin.y + i*offset.y + j, origin.x, vertical.c_str());
        mvaddstr(origin.y + (i+1)*offset.y, origin.x, horizontal.c_str());
    }   
}
void outline_board(const Board& board) { outline_board(board.dim(), board.origin, board.offset); } 


} // namespace Board_Game

#endif
