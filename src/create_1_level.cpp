/************************************************************************
 * Color Sort: create_1_level.cpp
 * Shanna Wallace
 * Updated: 2/10/2024
 * 
 * This program generates one Color Sort level. 
 * 
 * It reads the number of colors the level will contain from stdin
 * and prints the generated level's data in 1 line to stdout, in the 
 * format the Color Sort game uses to read level data: 
 *   num_colors, num_blocks, and num_bottles, each separated by a space, 
 *   then the values for each block in the bottles, starting with the 
 *   top block in the 1st bottle and ending with the bottom block in 
 *   the last bottle, each separated by a space.
 ***********************************************************************/

#include <vector>
#include <iostream>
#include "../include/Generator.hpp"
using namespace std;

int main(){

    Generator *gen = new Generator;  // Level generator
    int num_colors;                  // Number of colors in the level

    cin >> num_colors;

    if (gen->generate_level(num_colors) == false){
        cerr << "Could not generate level.\n";
        return -1;
    }

    gen->print_level_data(cout);

    return 0;
}