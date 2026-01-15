#ifndef CURSES_ARGS
#define CURSES_ARGS
// The functions that process CLI arguments. 

// It is intended that the user has the following definition in main() 
// const vector<string> args = {argv + 1, argv + argc}; 


#include"to_curses.hpp" // color_name

#include<vector>
#include<string>
#include<algorithm> // find() 


namespace Board_Game {
    using std::vector;
    using std::string;


// This function can be overloaded for different default_value types. 
int read_arg(const string& arg, const int default_value, const vector<string>& args) {
    
    int result = default_value; 
    
    auto iter = find(args.begin(), args.end(), arg);
    if (iter != (args.end()-1) && iter != args.end())
        try { result = stoi(*(iter+1)); } catch (const std::invalid_argument&) {}
    
    return result; 
} 


vector<int> read_colors(const vector<string>& args) { 
    
    vector<int> result = {};                
                                            // For each argument,
    for (int i = 0; i < args.size(); ++i) { // is it the name of a color? 
        const auto color_iter = find(color_name.begin(), color_name.end(), args[i]); 
        if (color_iter == color_name.end())   { continue; } // It wasn't. 
        if (color_iter == color_name.begin()) { continue; } // COLOR_PAIR(0) 
        result.push_back(color_iter - color_name.begin());  // It was. 
    }
    
    if (result.size() < 2) // Default to Red vs Blue. 
        result = {1,4};    // Luckily, this works for NCurses and PDCurses. 
    
    return result; 
} // This allows for multiple players of the same color. 


} // namespace Board_Game

#endif 
