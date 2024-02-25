/***************************************************************************
 * Color Sort: Generator.cpp
 * Shanna Wallace
 * Updated: 2/10/2024
 * 
 * This file contains the implementations for class Generator methods:
 *   Generator();                                  
 *   Generator(const int &blocks, const int &max); 
 *   bool generate_level(const int &number_colors);           
 *   void print_level_display(std::ostream& stream);        
 *   void print_level_data(std::ostream& stream);                
 *   int get_num_bottles(); // Returns number of bottles in the level
 *   int get_max_colors();   
 * 
 * Descriptions of these methods can be found in the function headers below
 * as well as the header file Generator.hpp
 ***************************************************************************/

#include "../include/Generator.hpp"
#include <iostream>
#include <vector>

// headers needed to shuffle the colors
#include <algorithm>
#include <random>
#include <time.h>
#include <ctime>
#include <chrono>

using namespace std;


/* Default Constructor
 * Sets max_colors and num_blocks to default values:
 *    12 maximum colors
 *    4 blocks of color per bottle
 * Creates a bank of max_colors colors to use. 
 * The "colors" are letters, starting with 'A' */
Generator::Generator(){
    
    int i;
    
    max_colors = 12;
    num_blocks = 4;
    num_bottles = 0;
    
    for (i = 1; i < max_colors; i++){
        colors.push_back(i);
    }
}


/* Generator(const int &blocks, const int &max)
 * Allow user to set non-default values for num_blocks and max_colors.
 * Parameters: 
 *   blocks: requested value for num_blocks
 *   max: requested value for max_colors, between 2 and 12.
 * If max is out of range, it will just set max_colors to the 
 * default value of 12. */  
Generator::Generator(const int &blocks, const int &max){
        
    int i;
    
    if (max < 2 || max > 26){
        max_colors = 12;
    }

    max_colors = max;
    num_blocks = blocks;
    num_bottles = 0;
    
    for (i = 0; i < max_colors; i++){
        colors.push_back(int(i) + 'A');
    }

}


/* generate_level
 * Creates num_colors bottles, with each bottle containing num_blocks 
 * blocks of colors randomly shuffled up. There will be num_blocks 
 * blocks of each color in the level. 
 * It then adds 1 empty bottle if the level has 2 or 3 colors, or 2 
 * empty bottles if the level has 4+ colors.
 * 
 * Parameters: 
 *   num_colors: Number of colors for the level to contain (between 
 *     2 and max_colors)
 * Returns: 
 *   true if level creation successful.
 *   false if num_colors is out of range. */
bool Generator::generate_level(const int &number_colors){

    int i, j;
    int empty_blocks;

    // num_colors must be >= 2 and <= max_colors
    if (number_colors > max_colors || number_colors < 2){
        return false;
    }

    num_colors = number_colors;

    // If there are 2 or 3 colors, user will get 1 empty bottle
    // If there are 4+ colors, user will get 2 empty bottles
    if (num_colors <= 3){
        num_bottles = num_colors + 1;
        empty_blocks = 1 * num_blocks;
    } else {
        num_bottles = num_colors + 2;
        empty_blocks = 2 * num_blocks;
    }

    // Put colors in bottles, shuffle the colors, then add empty bottles
    bottles.clear();
    for (i = 0; i < num_colors; i++){
        for (j = 0; j < num_blocks; j++){
            bottles.push_back(colors[i]);
        }
    }    

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(bottles.begin(), bottles.end(), default_random_engine(seed));

    for (i = 0; i < empty_blocks; i++){
        // - for an empty block
        bottles.push_back(0);
    }

    return true;
}

/* print_level
 * Prints the level to stream with ASCII art in the format it will
 * appear when loaded in bin/color_sort and bin/play1 
 * 
 * Parameters: 
 *   ostream &stream: stream you'd like to print the display to. cout 
 *     for stdout or a filestream for a file. */
void Generator::print_level_display(ostream &stream){

    int i, j;
	int index;

    if (bottles.size() == 0){
        return;
    }

    // Bottle numbers
    stream << endl;
    for (i = 0; i < num_bottles; i++){
        stream << " (" << i+1 << ")   ";
    }
    stream << endl;

    // Print the top block of each bottle, then the 2nd block, and so on
	for (i = 0; i < num_blocks; i++){
		for (j = 0; j < num_bottles; j++){		
			index = i + (j*num_blocks);
            stream << "| " << bottles[index] << " |  ";
		}
        stream << endl;
	}

    // Bottom of the bottles
	for (i = 0; i < num_bottles; i++){
        stream << "'---'  "; 
    }
    stream << endl;

}

/* print_level_data()
 * prints the level's data to stdout. 
 * It prints num_colors, followed by a space, num_blocks followed by a space, 
 * num_bottles followed by a space, then the color value for each bottle's 
 * blocks, starting with the 1st bottle's top block and ending with the last 
 * bottle's bottom block, each separated with a space. 
 * 
 * Parameters: 
 *   ostream &stream: stream you'd like to print the display to. cout 
 *     for stdout or a filestream for a file.   */
void Generator::print_level_data(ostream& stream){

    size_t i;

    stream << num_bottles << endl << num_blocks << endl;
    for (i = 0; i < bottles.size(); i++){
        stream << bottles[i] << endl;
    }
}


/* Returns the number of bottles in the level */
int Generator::get_num_bottles(){
    return num_bottles;
}


/* Returns maximum number colors allowed to be used */
int Generator::get_max_colors(){
    return max_colors;
}


/* Returns number of blocks per bottle */
int Generator::get_num_blocks(){
    return num_blocks;
}