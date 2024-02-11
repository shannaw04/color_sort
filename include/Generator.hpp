/************************************************************************
 * Color Sort: Generator.hpp
 * Shanna Wallace
 * Updated: 2/10/2024
 * 
 * This file contains the declaration for class Generator, used to 
 * generate levels for Color Sort.
 * 
 * Each Color Sort level contains num_bottles number of bottles, with 
 * each bottle containing num_blocks blocks of colors. 
 * You can generate levels with a minimum of 2 colors and maximum 
 * of max_colors (12 by default) with 
 * generate_level(const int &num_colors).
 * 
 * The generator will shuffle up the colors and add 1 empty bottle for 
 * levels containing 2 or 3 colors and 2 empty bottles for levels
 * containing 4+ colors.
 * 
 * The default constructor Generator() sets max_colors = 12 and 
 * num_blocks = 4. You can create a Generator with non-default values 
 * using Generator(const int &blocks, const int &max), where max is a 
 * value between 2 and 12. 
 * Both constructors create a bank of max_colors colors to be used, 
 * where the "colors" are chars starting with 'A'. 
 * 
 * You can use print_level_display(ostream &stream) to print the level 
 * with ASCII art, the way it will be displayed in the Color Sort game.
 * 
 * print_level_data(ostream &stream) prints the level's data in the
 * format that bin/color_sort will use to load the level data: 
 *   num_colors, num_blocks, then num_bottles each separated by a 
 *   space, then the color value for each bottle's blocks, starting 
 *   with the 1st bottle's top block and ending with the last bottle's 
 *   bottom block, each separated with a space. 
**********************************************************************/
#include <vector>
#include <string>
#include <fstream>

class Generator {
public:
    // Sets default vals: max_colors = 12 and num_blocks = 4
    Generator();                                  
    // Use to specify non-default values for max_colors and num_blocks
    Generator(const int &blocks, const int &max);  

    // Create level with num_colors colors
    bool generate_level(const int &number_colors);  

    // Print level with ASCII art to stdout        
    void print_level_display(std::ostream& stream);             

    // Print level's data to stdout
    void print_level_data(std::ostream& stream);                     

    // Returns number of bottles in the level
    int get_num_bottles(); 

    // Returns maximum number colors allowed to be used
    int get_max_colors();   

    // Returns the number of blocks per bottle
    int get_num_blocks();

protected:
    std::vector <char> colors;   // Bank of colors available to use 
                                 // (chars starting with 'A')
    std::vector <char> bottles;  // Contents of the bottles
    int num_colors;              // Number of colors in the level
    int max_colors;              // Max number of colors level can have
    int num_blocks;              // Number of blocks of color per bottle
    int num_bottles;             // Number of bottles in the level
};