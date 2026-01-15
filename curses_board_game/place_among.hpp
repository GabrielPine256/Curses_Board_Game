#ifndef PLACE_AMONG
#define PLACE_AMONG


#include<vector> 
#include<algorithm> // sort() 


namespace Board_Game {
    using std::vector; 
    using std::sort;  


// A function to run vector<int>s through to make sure the next function behaves sensibly. 
vector<int> clean_vector(vector<int> vec, const int max = 0) { // local copy of vec 
    sort(      vec.begin(),                              vec.end() );                              // Sort
    vec.erase( unique(vec.begin(), vec.end()),           vec.end() );                              // Remove duplicates
    vec.erase( vec.begin(),                              lower_bound(vec.begin(), vec.end(), 0) ); // Remove negatives 
    if (max) { vec.erase( lower_bound(vec.begin(), vec.end(), max), vec.end() ); }                 // Remove over max if max is not default. 
    return vec; 
}


// A function to return the val-th number in the range 0-max that doesn't collide with existing. 
// A number N might have 3 numbers smaller than it mentioned in existing. 
// upper_bound will return an index of 3. 
// Then the result should be N+3. 
// A number N+3 might have another number smaller than it mentioned in existing. 
// Once we've increased enough not to run into more collisions, 
// it means we've accommodated all relevant collisions. 
int place_among(const int val, const vector<int>& existing, const int max) { 
    const vector<int> exist = clean_vector(existing, max); 
    for (int coll = 0, prev = 0; val + coll < max; ) {
        prev = coll; 
        coll = upper_bound(exist.begin(), exist.end(), val + coll) - exist.begin(); 
        if (coll == prev) { return val + coll; } 
    }
    return -1; 
}


} // namespace Board_Game


/*
    sort(vec.begin(), vec.end()); 

    // Remove duplicates. 
    for (int i = 0; i < vec.size() - 1; ) 
        if (vec[i] == vec[i+1]) { vec.erase(vec.begin() + i+1); }
        else                    { ++i; }
    
    // Remove negatives. 
    for (int i = 0; i < vec.size(); ) 
        if (vec[i] < 0) { vec.erase(vec.begin() + i); }
        else            { ++i; } 
    
    // Remove over max.
    if (max) // != 0
        for (int i = 0; i < vec.size(); ) 
            if (vec[i] >= max) { vec.erase(vec.begin() + i); }
            else               { ++i; } 
*/


#endif
