/*********************************************************************
 * Color Sort: ColorSort.hpp
 * Shanna Wallace
 * Updated: 2/10/2024
 * 
 * This file contains the declaration for class ColorSort, used by 
 * the Color Sort game. 
 * 
 * Use load_level(string level) to load the level's data. It needs to
 * be in the format: 
 *   num_colors, num_blocks, num_bottles each separated by a 
 *   space, then the color value (char) for each bottle's blocks, 
 *   starting with the 1st bottle's top block and ending with the last 
 *   bottle's bottom block, each separated with a space.
 *   Indicate empty blocks with the '-' character.
 * 
 * print_bottles() will print the game state to stdout with ASCII art.
 *  
 * You can use print_msg_file(string filename) to load and display
 * messages from a file in the game, such as a welcome message or 
 * gameplay instructions.
 * 
 * reset_level() will reset the level to its initial state. 
 * 
 * make_move(int from, int to) will move blocks of color from 
 * the "from" bottle to the "to" bottle. Use the number printed
 * above the bottle. make_move will call bottles_valid to error
 * check the from and to values.
 * 
 * The game keeps track of the number of bottles that have been 
 * sorted, and when this number of completed_bottles equals
 * the number of colors in the level, level_complete() will return 
 * true.
 ********************************************************************/
#include <vector>
#include <string>

class ColorSort{
public:
    // Read a string containing level's data and store the values
    bool load_level(const std::string &level);   

    // Print the bottles to stdout with ASCII art
	void print_bottles();                        

    // Use to print messages from a file in the game, such as 
    // a welcome message or gameplay instructions
    void print_msg_file(const std::string &filename); 

    // Figures out if colors can be moved from "from" to "to" 
    // and how many blocks of colors with be moved
    bool make_move(int from, int to);      
                                           
    // Checks if bottle numbers for a move are in range, "from" isn't
    // empty, "to" isn't full, and neither bottle is already sorted 
    bool bottles_valid(int from, int to);  
   
    // Moves the blocks from "from" to "to"
	bool transfer_blocks(int from_index, int to_index, int blocks_to_move); 

    // Resets level to initial state                
    void reset_level();               

    // Checks whether the bottle is full and only contains 1 color
	bool bottle_complete(int bottle); 

    // Checks if the number of complete bottles == the number of colors
    // in the level, which means the colors are sorted and the level is
    // complete
    bool level_complete();            

    // Return the number of bottles that have been completed so far
    int num_bottles_complete();       

protected:
	std::vector <char> bottles;      // Current state of bottles
    std::vector <char> bottles_init; // Initial state of bottles
    int num_colors;                  // Number of colors in the level
    int num_blocks;                  // Number of blocks per bottle
    int num_bottles;                 // Number of bottles in the level
    int completed_bottles;           // Number of completed bottles so far
    int completed_init;              // Initial number of completed bottles
};