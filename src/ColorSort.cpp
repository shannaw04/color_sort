/*******************************************************************************
 * Color Sort: ColorSort.cpp
 * Shanna Wallace
 * Updated: 2/10/2024
 * 
 * This file contains implementations for class ColorSort methods:
 *   bool load_level(const std::string &level);   
 *	 void print_bottles();                        
 *   void print_msg_file(const std::string &filename); 
 *   bool make_move(int from, int to);      
 *   bool bottles_valid(int from, int to); 
 *   void transfer_blocks(int from_index, int to_index, int blocks_to_move);          
 *   void reset_level();               
 *	 bool bottle_complete(int bottle); 
 *   bool level_complete();            
 *   int num_bottles_complete();
 *        
 * Descriptions of these methods can be found in the function headers below as 
 * well as the header file ColorSort.hpp
 *******************************************************************************/

#include "../include/ColorSort.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
using namespace std;

/* load_level
 * This function reads a string containing data on the number of colors, 
 * number of bottles, number of blocks per bottle in a Color Sort level, 
 * as well as the initial color value for each block in the bottles for 
 * the level's starting state. It saves these values to the ColorSort 
 * object's data members. 
 * It also saves the level's initial state so the player can reset the
 * level if they wish.
 * 
 * Parameters: const string &level. This string needs to be in the format: 
 *   num_bottles, num_blocks, then the color value for each bottle's 
 *   blocks, starting with the 1st bottle's top block and ending with the 
 *   last bottle's bottom block, each value separated with a space. 
 * Returns: true if the level was loaded sucessfully. 
 *          false if the level could not be loaded */
bool ColorSort::load_level(const string &level){

    int color;
    int i;
    istringstream ss;

    cout << "in load_level" << endl;

    ss.clear();
    ss.str(level);
    ss >> num_bottles >> num_blocks;

    cout << "read num_bottles and num_blocks" << endl;

    if (num_bottles < 5){
        num_colors = num_bottles - 1;
    } else {
        num_colors = num_bottles - 2;
    }

    cout << "num_bottles = " << num_bottles << "   num_blocks = " << num_blocks << "   num_colors = " << num_colors << endl;

    bottles.clear();
    while (ss >> color){
        bottles.push_back(color);
    }
    
    
	// Check if any bottles are already complete
    completed_bottles = 0;
	for (i = 0; i < num_bottles; i++){
		if (bottle_complete(i + 1)){
			completed_bottles++;
		}
	}

    // Save initial state so user can reset level
    completed_init = completed_bottles;
    bottles_init = bottles;

    return true;
}

/* print_bottles
 * This function displays the game state by printing the bottles 
 * to stdout. It prints a number above each bottle starting with 
 * 1, so the player can indicate which bottles they'd like to 
 * move from/to.  */
void ColorSort::print_bottles(){
    
    int i, j;
	int index;

    cout << "in print bottles" << endl;

    if (bottles.size() == 0){
        return;
    }

    // Print number above each bottle so the user can
    // identify bottle numbers to make their moves
    cout << endl;
    for (i = 0; i < num_bottles; i++){
        cout << " (" << i+1 << ")   ";
    }
    cout << endl;

    // Print the top block of each bottle, then the 
    // 2nd block, and so on
	for (i = 0; i < num_blocks; i++){
		for (j = 0; j < num_bottles; j++){		
			index = i + (j*num_blocks);
            //cout << "| " << char(bottles[index] + 'A') << " |  ";
            cout << "| " << bottles[index] << " |  ";
		}
        cout << endl;
	}

    // Bottom of the bottles
	for (i = 0; i < num_bottles; i++){
        cout << "'---'  "; 
    }
    cout << endl;
}


/* make_move
 * This function determines the number of blocks of color to be 
 * moved from the "from" bottle to the "to" bottle. 
 * 
 * It calls bottles_valid to make sure bottle numbers are valid.
 * Then it finds the top color in the "from" bottle and counts the 
 * number of consecutive blocks of that color. 
 * Then it finds the top color in the "to" bottle and counts the 
 * number of empty blocks available. 
 * It then calls transfer_blocks to move the blocks from "from" 
 * to "to", with blocks_to_move being either the number of 
 * consecutive blocks in "from" or number of empty blocks in "to", 
 * whichever is smaller. 
 * Checks if "to" bottle is complete after move, and if so, updates
 * count of completed bottles.
 * 
 * Parameters:
 *   int from: Number of bottle moving from. 
 *   int to: Number of bottle moving to. 
 *   These are the numbers printed above the bottles in the game
 * Returns: 
 *   true if the move can be made
 *   false if the move can't be made:
 *     If the "from" bottle is empty
 *     If the "to" bottle is full
 *     If the top colors in the bottles don't match
 *     If either bottle has already been sorted */
bool ColorSort::make_move(int from, int to){

	int i;             // Loop counter
	int color;        // The color being moved (top color in "from")
	int from_index;    // Index of the "from" bottle in the bottles vector
    int to_index;      // Index of the "to" bottle in the bottles vector
    int from_start;    // The index of the top block of color in "from", relative
                       // to the bottle's top index
	int consec_blocks; // Number of consecutive blocks of a color in "from" bottle
	int empty_blocks;  // Number of consecutive empty blocks in "to" bottle

    // This checks that bottle numbers are in range, "from" bottle isn't empty, 
    // "to" bottle isn't full, and neither bottle is already sorted.
    if (bottles_valid(from, to) == false){
        return false;
    }

    // Get index of top block in each bottle. Since the game numbers the bottles 
    // starting with 1, but the vector indexes starting with 0, need to -1
	from_index = (from - 1) * num_blocks;
	to_index = (to - 1) * num_blocks; 


	// Find the top color in the "from" bottle and number of 
    // consecutive blocks of that color
	color = 0;
    consec_blocks = 1; 
	for (i = 0; i < num_blocks; i++){
        if (bottles[from_index + i] != 0){
            // If color was 0, then this is the top color
            // Set color = this color. Save the index the color
            // starts at with from_start
            if (color == 0){
                color = bottles[from_index + i];
                from_start = i;
            
            // Keep count of consecutive blocks of top color
            } else if (bottles[from_index + i] == color){
                consec_blocks++;
            
            // Break the loop if the block is a different color
            // than the top color 
            } else {
                break;
            }
        }
    }

    // Increament from_index to the 1st block of color within the bottle
    from_index += from_start;


    // Iterate through the "to" bottle
	for (i = 0; i < num_blocks; i++){
        // If the bottle isn't empty, check that its top color matches
        // the color being moved from "from"
        if (bottles[to_index + i] != 0){
            if (bottles[to_index + i] != color){
                return false;
            }
            break;
        }
    }

    // If i > num_blocks, that means we iterated through the bottle
    // without finding any colors, so the bottle must be empty. 
    // Otherwise, i will equal the number of empty blocks found before 
    // finding a color and breaking out of the loop
    if (i > num_blocks){
        empty_blocks = num_blocks;
    } else {
         empty_blocks = i;
    }

    // Increment to_index to the bottom-most empty block in the bottle
    to_index += (empty_blocks - 1);

	if (transfer_blocks(from_index, to_index, min(empty_blocks, consec_blocks))){
        // Check if "to" is complete after transfer
        if (bottle_complete(to)){
            completed_bottles++;
        }
        return true;

    } else {
        return false;
    }


}


/* transfer_blocks 
 * This function starts at the top-most block of color in "from" and swaps it 
 * with the bottom-most empty block in "to". If there are more blocks to be 
 * transfered, it continues iterating down the "from" bottle and up the "to" 
 * bottle, until all blocks_to_move have been transfered. 
 * 
 * Parameters:
 *   int from_index: The index in the bottles vector of the top-most block of 
 *     color in the "from" bottle
 *   int to_index: The index in the bottles vector of the bottom-most empty 
 *     block in the "to" bottle 
 *   int blocks_to_move: The number of blocks of color being moved from "from"
 *     bottle to "to" bottle. 
 * Returns: 
 *   true if blocks transfered sucessfully
 *   false if any parameters are out of range */
bool ColorSort::transfer_blocks(int from_index, int to_index, int blocks_to_move){

	int i;
    int color;

    if (blocks_to_move > num_blocks || blocks_to_move < 0){
        return false;
    }

    if (from_index < 0 || from_index >= int(bottles.size())){
        return false;
    }

    if (to_index < 0 || to_index >= int(bottles.size())){
        return false;
    }

	for (i = 0; i < blocks_to_move; i++){
        color = bottles[from_index +  i];
		bottles[from_index +  i] = 0;
		bottles[to_index - i] = color;
	}

    return true;
}


/* bottle_complete
 * This function checks if a bottle is complete. A bottle is complete if it
 * is full and all blocks are the same color. 
 * 
 * Parameters:
 *   int bottle: Number of bottle being checked. This is the number printed 
 *     above the bottle in the game. 
 * Returns: 
 *   true if the bottle is complete
 *   false if the bottle is not full or color blocks do not match */
bool ColorSort::bottle_complete(int bottle){

	int color; // Color of the top block in the bottle
	int index;  // Index of the bottle's top block in the bottles vector
	int i;      // Used to iterate through bottle
	
    // Subtract 1 since game numbers bottles starting at 1 and bottles
    // vector indexes starting at 0
	index = (bottle - 1) * num_blocks;
	
    // If the top color is 0, then the bottle is not full
	color = bottles[index];
	if (color == 0){
		return false;
	}
	
	// iterate through the rest of the bottle and return false if find 
    // a block that doesn't match top color
	for (i = 1; i < num_blocks; i++){
		if (bottles[index + i] != color){
			return false;
		}
	}

	return true;
}


/* level_complete
 * This function checks if the level is complete. 
 * Once the number of completed bottles equals the number of colors
 * in the level, then sorting is done and the level has been completed. 
 * 
 * Returns: 
 *   true if completed_bottles == num_colors
 *   false if completed_botles != num_colors */
bool ColorSort::level_complete(){

    if (completed_bottles == num_colors){
        return true;
    } else {
        return false;
    }

}


/* reset_level
 * This function resets the level back to its initial state */
void ColorSort::reset_level(){
    bottles = bottles_init;
    completed_bottles = completed_init;
}


/* print_msg_file
 * This function will open a file and display its contents in the 
 * Color Sort game. This can be used for things such as welcome
 * messages and gameplay instructions. 
 * 
 * Parameters:
 *   const string &filename: the name of the file to open. */
void ColorSort::print_msg_file(const string &filename){

    ifstream infile;
    string line;

    infile.open(filename);
    if (infile.fail()){
        cerr << "Failed to load game message file " << filename << "\n";
   
    } else {     
        cout << endl;
        while(getline(infile, line)){
            cout << line << endl;
        }
        cout << endl;
        infile.close();
    }
}


/* num_bottles_complete
 * This function returns the number of bottles that have been completed 
 * so far. 
 * 
 * Returns: int completed_bottles */
int ColorSort::num_bottles_complete(){
    return completed_bottles;
}


/* bottles_valid
 * This function verifies whether the from and to values are valid by
 * checking that: 
 *   from and to are within range 1 to num_bottles
 *   from and to are not the same
 *   from isn't empty and to isn't full
 *   from or to have not already been completed
 * 
 * Parameters: 
 *   int from: Number of "from" bottle 
 *   int to: Number of "to" bottle
 * Returns: 
 *   true if from and to are valid
 *   false if they are not valid */
bool ColorSort::bottles_valid(int from, int to){
    
	// Make sure bottle numbers are valid
	if (from == to){
		return false;
	}
    if (from < 1 || from > num_bottles 
       || to < 1 || to > num_bottles){
		return false;
	}

    // Don't move to/from if one of the bottles is already sorted.
    if (bottle_complete(from) || bottle_complete(to)){
        return false;
    }

    // Get index of top block in each bottle in the bottles vector. 
    // Subtract 1 since the game numbers the bottles starting at 1 
	from = (from - 1) * num_blocks;
	to = (to - 1) * num_blocks; 

	// Check that the bottle moving to isn't already full or that 
    // bottle moving from isn't empty
	if (bottles[to] != 0){
		return false;
	}
	if (bottles[from + (num_blocks - 1)] == 0){
		return false;
	}

    return true;
}