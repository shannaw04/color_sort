/***************************************************************************
 * Color Sort: play_1.cpp
 * Shanna Wallace
 * Updated: 2/10/2024
 * 
 * This program allows you to play one level of Color Sort. 
 * 
 * It takes the following command line arguments:
 * bin/play_1.cpp <input_file>
 * input_file: the name of a file containing data for the levels.
 * This file should have one line in the format:
 *   num_colors, num_blocks, and num_bottles, each separated by a space, 
 *   then the values for each block in the bottles, starting with the 
 *   top block in the 1st bottle and ending with the bottom block in 
 *   the last bottle, each separated by a space. 
 * 
 * To play the game, the user may enter:
 *   H to see instructions on how to play Color Sort
 *   R to reset the level
 *   Q to quit.
 *   B1 B2 to make a move, where B1 is the number of the source bottle and 
 *       B2 is the number of the destination bottle, separated by a space.
****************************************************************************/
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstring>
#include <fstream>
#include "../include/ColorSort.hpp"
using namespace std;


string WELCOME_MSG = "game_msgs/welcome.txt";
string INSTR = "game_msgs/how_to.txt";

int main(int argc, char *argv[]) {

    string input;        // User's input
    int from, to;        // Number of bottles moving color from and to
    string level;        // String containing level data
    ifstream level_file; // File to read input from
    ColorSort *game = new ColorSort;  // Game

    // Open file containing level data
    if (argc != 2){
        cerr << "Usage: " << argv[0] << " <level_file>\n";
        return -1;
    }

    level_file.open(argv[1]);
    if (level_file.fail()){
        cerr << argv[1] << ": " << strerror(errno) << endl; 
        cerr << "Could not load game data.\n";
        return -1;
    }

    // Load level
    if (!game->load_level(level)){
        cerr << "Could not load level.\n";
        return -1;
    }
    level_file.close();

    cout << "loaded the level." << endl;
    // Play level
    game->print_msg_file(WELCOME_MSG);
    while (game->level_complete() == false){
        cout << "in the while loop" << endl;
        game->print_bottles();
        
        cin.clear();
        cout << "ENTER MOVE: ";
        getline(cin, input);

        // Reset level
        if (input == "R" || input == "r"){
            game->reset_level();
        
        // Quit game
        } else if (input == "Q" || input == "q"){
            cout << "\nThanks for playing Color Sort!\nGoodbye\n";
            return 0;
        
        // Print instructions
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

    game->print_bottles();
    cout << "\n**********  LEVEL COMPLETE!  **********\n\n";
    return 0;
}