#include <sstream>
#include <stdexcept>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "place_result.h"
#include "move.h"
#include "exceptions.h"
#include "human_player.h"
#include "tile_kind.h"
#include "formatting.h"
#include "rang.h"

using namespace std;


// This method is fully implemented.
inline string& to_upper(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

Move HumanPlayer::get_move(const Board& board, const Dictionary& dictionary) const {
	// TODO: begin your implementation here.
    //PLACE - 5 1 POE?MON
    //EXCHANGE aa?
    //PASS 
    //enter a command
    this->print_hand(cout);
    cout << "Your move, " << this->get_name() << ":" << endl;
    string playerInput;
    getline(cin,playerInput);
    //change everything to lowercase
    for (unsigned int i=0; i < playerInput.length(); i++)
    {
        playerInput[i] = tolower(playerInput[i]);
    }
    Move playerMove = parse_move(playerInput);
    if(playerMove.kind == MoveKind::PLACE)
    {
        //see if result is valid on board
        PlaceResult testPlace = board.test_place(playerMove);
        if(testPlace.valid == false)
        {
            throw MoveException("Invalid Move");
        }
        //goes through words in vector<string>
        for(unsigned int i=0; i <testPlace.words.size(); i++)
        {
            if(dictionary.is_word(testPlace.words[i]) == false)
            {
                throw MoveException("Invalid Move");
            }
        }
        return playerMove;
    }
    //exchange handldled in scrabble.cpp
    else if (playerMove.kind == MoveKind::EXCHANGE)
    {
        return playerMove;
    }
    //Pass handled in scrabble.cpp
    else if (playerMove.kind == MoveKind::PASS)
    {
        return playerMove;
    }
    //if none of these move then invalid move
    else
    {
        throw MoveException("Invalid Move");
    }


}

vector<TileKind> HumanPlayer::parse_tiles(string& letters) const{
    // TODO: begin implementation here.
    //reading in a word, parse word
    vector<TileKind> parsed_tiles;
    for(unsigned int i=0; i <letters.size(); i++)
    {
        //throws exception if tile not found
       TileKind current = this->tiles.lookup_tile(letters[i]);
       //question mark tile assignment
       //assigns one AFTER the question mark
       //increment for loop 
       if(current.letter == TileKind::BLANK_LETTER)
       {
            i++;
            // current.letter = letters[i];
            current.assigned = letters[i];
       }
       parsed_tiles.push_back(current);
    }
    //returns a vector of parsed tiles
    return parsed_tiles;
}

Move HumanPlayer::parse_move(string& move_string) const {
	// TODO: begin implementation here.
    //takes in a string and parses it for you
    stringstream ss(move_string);
    string input;
    ss >> input;

    if(input == "exchange")
    {
        //EXCHANGE aa?
        string exchangeTiles;
        ss >> exchangeTiles;
        //get vector of parsed tiles
        vector<TileKind> exchanged_tiles = parse_tiles(exchangeTiles);
        //Move(std::vector<TileKind> tiles) : kind(MoveKind::EXCHANGE), tiles(tiles) {}
        Move playerMove(exchanged_tiles);
        return playerMove;
    }
    else if(input == "place")
    {
        //PLACE - 5 1 POE?MON
        char direction;
        string inputTiles;
        int row; 
        int col; 
        ss >> direction; 
        ss >> row;
        ss >> col;
        ss >> inputTiles;
        if(ss.fail())
        {
            throw CommandException("Invalid Input");
        }
        vector<TileKind> place_tiles = parse_tiles(inputTiles);
        if(direction == '|')
        {
            //convert to 0 indexing 
            Move playerMove(place_tiles, row-1, col-1, Direction::DOWN);
            return playerMove;
        }
        else if(direction == '-')
        {
            //Move(std::vector<TileKind> tiles, size_t row, size_t column, Direction direction)
            Move playerMove(place_tiles, row-1, col-1, Direction::ACROSS);
            return playerMove;
        }
        else
        {
            throw CommandException("Invalid Input");
        }
    }
    else if(input == "pass")
    {
        //Move() : kind(MoveKind::PASS) {}
        Move playerMove;
        return playerMove;
    }
    else
    {
        throw CommandException("Invalid Input");
    }
}


// This function is fully implemented.
void HumanPlayer::print_hand(ostream& out) const {
	const size_t tile_count = tiles.count_tiles();
	const size_t empty_tile_count = this->get_hand_size() - tile_count;
	const size_t empty_tile_width = empty_tile_count * (SQUARE_OUTER_WIDTH - 1);

	for(size_t i = 0; i < HAND_TOP_MARGIN - 2; ++i) {
		out << endl;
	}

	out << repeat(SPACE, HAND_LEFT_MARGIN) << FG_COLOR_HEADING << "Your Hand: " << endl << endl;

    // Draw top line
    out << repeat(SPACE, HAND_LEFT_MARGIN) << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE;
    print_horizontal(tile_count, L_TOP_LEFT, T_DOWN, L_TOP_RIGHT, out);
    out << repeat(SPACE, empty_tile_width) << BG_COLOR_OUTSIDE_BOARD << endl;

    // Draw middle 3 lines
    for (size_t line = 0; line < SQUARE_INNER_HEIGHT; ++line) {
        out << FG_COLOR_LABEL << BG_COLOR_OUTSIDE_BOARD << repeat(SPACE, HAND_LEFT_MARGIN);
        for (auto it = tiles.cbegin(); it != tiles.cend(); ++it) {
            out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL << BG_COLOR_PLAYER_HAND;

            // Print letter
            if (line == 1) {
                out << repeat(SPACE, 2) << FG_COLOR_LETTER << (char)toupper(it->letter) << repeat(SPACE, 2);

            // Print score in bottom right
            } else if (line == SQUARE_INNER_HEIGHT - 1) {
                out << FG_COLOR_SCORE << repeat(SPACE, SQUARE_INNER_WIDTH - 2) << setw(2) << it->points;

            } else {
                out << repeat(SPACE, SQUARE_INNER_WIDTH);
            }
        }
        if (tiles.count_tiles() > 0) {
            out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL;
            out << repeat(SPACE, empty_tile_width) << BG_COLOR_OUTSIDE_BOARD << endl;
        }
    }

    // Draw bottom line
    out << repeat(SPACE, HAND_LEFT_MARGIN) << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE;
    print_horizontal(tile_count, L_BOTTOM_LEFT, T_UP, L_BOTTOM_RIGHT, out);
    out << repeat(SPACE, empty_tile_width) << rang::style::reset << endl;
}
