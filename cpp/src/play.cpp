/*************************************************************************
 * Color Sort: play.cpp
 * Shanna Wallace
 * Updated: 2/10/2024
 * 
 * This program allows you to play multiple levels of Color Sort. 
 * 
 * It takes the following command line arguments:
 * bin/play <input_file>
 * 
 * input_file: the name of a file containing data for the levels.
 * This file should have data for one level per line in the format:
 *   num_colors, num_blocks, and num_bottles, each separated by a space, 
 *   then the values for each block in the bottles, starting with the 
 *   top block in the 1st bottle and ending with the bottom block in 
 *   the last bottle, each separated by a space. 
 * 
 * The program will load the level on the 1st line of the input_file 
 * first, then when that level has been completed, will load the next level
 * in the input_file. This will continue until the user quits the game
 * or completes all of the levels in the input_file. 
 * 
 * To play the game, the user may enter:
 *   H to see instructions on how to play Color Sort
 *   R to reset the level
 *   Q to quit.
 *   B1 B2 to make a move, where B1 is the number of the source bottle and 
 *       B2 is the number of the destination bottle, separated by a space.
 *************************************************************************/ 
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include "../include/ColorSort.hpp"
using namespace std;

// Files containing messages to be displayed in the game
string WELCOME_MSG = "game_msgs/welcome.txt";
string INSTR = "game_msgs/how_to.txt";


int main(int argc, char *argv[]) {

    string input;        // User's input for game action
    int from, to;        // Number of the bottles color is being moved from and to    
    int level_num;       // Number of the level user is on
    ifstream input_file; // File containing game data
    string level;        // Data for individual level
    ColorSort *game = new ColorSort; // Game level 

    if (argc != 2){
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return -1;
    }


    // Open file with game data
    input_file.open(argv[1]);
    if (input_file.fail()){
        cerr << argv[1] << ": " << strerror(errno) << endl; 
        cerr << "Could not load game data.\n";
        return -1;
    }


    // Print welcome message
    game->print_msg_file(WELCOME_MSG);

    // Load each level
    level_num = 1;
    while (getline(input_file, level)){
       
        // Play level
        if (game->load_level(level)){     
            cout << "LEVEL " << level_num << ":\n";

            while (game->level_complete() == false){

                game->print_bottles();

                cout << "ENTER MOVE: ";
                getline(cin, input);

                // Reset level
                if (input == "R" || input == "r"){
                    game->reset_level();
                
                // Quit game
                } else if (input == "Q" || input == "q"){
                    cout << "\nThanks for playing Color Sort!\nGoodbye\n";
                    input_file.close();
                    return 0;
                
                // Print gameplay instructions
                } else if (input == "H" || input == "h"){
                    game->print_msg_file(INSTR);

                // Make move
                } else if (sscanf(input.c_str(), "%d %d", &from, &to) == 2){
                    if (game->make_move(from, to) == false){ 
                        cerr << "Cannot move from " << from << " to " << to << ".\n";
                    }
                } else {
                    cerr << "Invalid input.\n";
                }
            }

            // If they get out of this while loop, they've completed the level
            game->print_bottles();
            cout << "\n\n**********  LEVEL COMPLETE!  **********\n\n";
        
        } else {
            cerr << "Could not load level " << level_num << endl;
        }
        level_num++;
    }


    cout << "\n\n**********  CONGRATS! YOU HAVE COMPLETED ALL LEVELS  **********\n";
    input_file.close();
    return 0;
}