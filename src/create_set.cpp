/**************************************************************************************** 
 * Color Sort: create_level_set.cpp
 * Shanna Wallace
 * Updated: 2/10/2024
 * 
 * This program creates a set of multiple levels for Color Sort. It generates the 
 * number and type of levels requested in <input_file>. You can output all of the level 
 * data to one file (one level per line), or create an individual file for each level, 
 * or both.
 * 
 * It takes the following command line arguments:
 * bin/gen_levels <game_name> <output_type> <input_file>
 *  
 * game_name: The name you'd like to give this set of levels.
 * output_type: 
 *   f: Put all level data in one file, one level per line. It will be put in the
 *      games directory with name game_name.txt
 *   d: Create a file for each individual level. It will put the files in the 
 *      level_files directory, in a subdirectory named game_name.
 *      The level files will be named C_L.txt, where C is the number of colors in
 *      the level, and L is the number of the file with that number of colors.
 *   b: Create both one file with all level data, plus individual files for each level.
 * input_file: Name of a file containing the information for how many levels to 
 *   create. This file should contain the number of colors for the levels, followed by
 *   a space, then the number of levels you'd like to create with that number
 *   of colors, followed by a space or newline. 
 *   For example, if you want to create 5 levels with 2 colors and 10 levels with 3 
 *   colors, your input_file would need to be: 
 *    2 5 
 *    3 10 
 *******************************************************************************************/
#include <vector>
#include <iostream>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include "../include/Generator.hpp"
using namespace std;

void print_instructions();

int main(int argc, char *argv[]){

    Generator *gen = new Generator;  // Game generator class
    int num_colors;                  // Number of colors in a level
    int num_levels;                  // Number of levels to make with num_colors
    string game_name;                // Name for this set of levels
    string output_type;              // Whether the user would like all data in 1 file or indiv files
    int i;                           // Loop counter
    DIR *dir;                        // Directory for output
    ifstream input_file;             // Input file containing amount and type of levels to create
    ofstream game_file;              // File for all level data, if writing all levels to 1 file
    ofstream level_file;             // File for individual level
    string level_filename;           // Filename for individual level file
    string level_files_dir;          // Directory to put the individual level files into
    string games_dir;                // Directory to put file that contains all level data


    // Print instructions if given bad command line args
    if (argc != 4){
        cout << "Usage: " << argv[0] << " <game_name> <output_type> <input_file>\n\n";
        print_instructions();
        return -1;
    }

    game_name = argv[1];
    output_type = argv[2];


    // Open directory for output files
    // Make sure level_files directory exists, and create it if necessary
    if (output_type == "d" || output_type == "b"){
        level_files_dir = "level_files";
        dir = opendir(level_files_dir.c_str());

        if (!dir){
            if (mkdir(level_files_dir.c_str(), 0700) == -1){
                cerr << level_files_dir << ": " << strerror(errno) << endl;
                return -1;
            } 
        } 
        closedir(dir);

        // Create directory for levels if it doesn't already exist
        level_files_dir = "level_files/" + game_name;
        dir = opendir(level_files_dir.c_str());
        if (!dir){
            if (mkdir(level_files_dir.c_str(), 0700) == -1){
                cerr << level_files_dir << ": " << strerror(errno) << endl;
                return -1;
            } 
        } 
        closedir(dir);
    } 

    // Open file for level data, if user wants it all in one file. Put this 
    // file in the "games" directory. Create games directory if it doesn't
    // already exist.
    if (output_type == "f" || output_type == "b"){
        // add ".txt" if it isn't there already
        if (game_name.find('.') == string::npos){
            game_name += ".txt";
        }
        
        games_dir = "games";
        dir = opendir(games_dir.c_str());

        if (!dir){
            if (mkdir(games_dir.c_str(), 0700) == -1){
                cerr << games_dir << ": " << strerror(errno) << endl;
                return -1;
            } 
        } 
        closedir(dir);


        game_name = games_dir + "/" + game_name;
        game_file.open(game_name);
        if (game_file.fail()){
            cerr << game_name << ": " << strerror(errno) << endl;
            return -1;
        } 
    
    // Invalid input
    } else {
        print_instructions();
        return -1;
    }

    // Open level file data
    input_file.open(argv[3]);
    if (input_file.fail()){
        cerr << argv[3] << ": " << strerror(errno) << endl;
        return -1;    
    }


    // Read the level data file and create the levels
    while (input_file >> num_colors >> num_levels){

        for (i = 0; i < num_levels; i++){
            if (gen->generate_level(num_colors) == false){
                cerr << "Could not generate level.\n";
            }

            // Write level data to individual file if requested
            if (output_type == "d" || output_type == "b"){            
                level_filename = level_files_dir + "/" + to_string(num_colors) + "_" + to_string(i+1) + ".txt";
                level_file.open(level_filename);
                if (level_file.fail()){
                    cerr << "Could not create level file " << level_filename << endl;
                    cerr << game_name << ": " << strerror(errno) << endl;
                
                } else {
                    gen->print_level_data(level_file);
                    level_file.close();
                }
            } 

            // Add level data to all_levels file if requested
            if (output_type == "f" || output_type == "b"){
                gen->print_level_data(game_file);
            }         
        }
    }

    cout << "Level generation complete.\n";
    if (output_type == "f" || output_type == "b"){
        cout << "Game level data file " << game_name << " created.\n";
        game_file.close();
    }

    if (output_type == "d" || output_type == "b"){
        cout << "Individual game files are located in " << level_files_dir << endl;
    }

    input_file.close();

    return 0;
}

// Instructs on how to use the program.  
void print_instructions(){
    cout << "game_name: the name you'd like to use for this set of game levels.\n";
    cout << "output_type:\n"; 
    cout << " f: to put all level data in one file, one level per line.\n";
    cout << "    This file will be in the games directory named game_name.txt\n";
    cout << " d: to create a file for each level. They will be put in directory \n"; 
    cout << "    level_files/game_name, with the name C_L.txt, where C is the \n";
    cout << "    number of colors and L is the number of the level with that \n";
    cout << "    number of colors.\n";
    cout << " b: if you'd like to create both a file with all level data, plus \n";
    cout << "    individual files for each level.\n";
    cout << "input_file: File containing info of how many levels of each color\n"; 
    cout << "  you'd like to create. Input file should be in the format \n";
    cout << "  \"C L C L ...\" where C is the number of colors in the level and\n"; 
    cout << "  L is the number of levels to create with that number of colors.\n";
}