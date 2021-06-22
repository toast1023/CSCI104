#include "board.h"
#include "board_square.h"
#include "exceptions.h"
#include "formatting.h"
#include <iomanip>
#include <fstream>

using namespace std;


bool Board::Position::operator==(const Board::Position& other) const {
    return this->row == other.row && this->column == other.column;
}

bool Board::Position::operator!=(const Board::Position& other) const {
    return this->row != other.row || this->column != other.column;
}

Board::Position Board::Position::translate(Direction direction) const {
    return this->translate(direction, 1);
}

Board::Position Board::Position::translate(Direction direction, ssize_t distance) const {
    if (direction == Direction::DOWN) {
        return Board::Position(this->row + distance, this->column);
    } else {
        return Board::Position(this->row, this->column + distance);
    }
}

Board Board::read(const string& file_path) {
    ifstream file(file_path);
    if (!file) {
        throw FileException("cannot open board file!");
    }

    size_t rows;
    size_t columns;
    size_t starting_row;
    size_t starting_column;
    file >> rows >> columns >> starting_row >> starting_column;
    Board board(rows, columns, starting_row, starting_column);

    // TODO: complete implementation of reading in board from file here.
    //std::vector<std::vector<BoardSquare>> squares;
    stringstream ss;
    string temp;
    string trash;
    char parseSquare;
    getline(file,trash);

    for(unsigned int i=0; i < (columns); i++)
    {
    	vector<BoardSquare> vRow;
    	getline(file,temp);
    	ss << temp;
    	for (unsigned int j=0; j< (rows); j++)
    	{
    		BoardSquare oneSquare(1,1);
    		ss >> parseSquare; 
    		if (parseSquare == 't')
    		{
    			oneSquare.word_multiplier = 3;
    		}
    		else if (parseSquare == '2')
    		{
    			oneSquare.letter_multiplier = 2;
    		}
    		else if (parseSquare == '3')
    		{
    			oneSquare.letter_multiplier = 3;
    		}
    		else if (parseSquare == 'd')
    		{
    			oneSquare.word_multiplier = 2;
    		}
    		vRow.push_back(oneSquare);
    	}
    	board.squares.push_back(vRow);
    }
    return board;
}

size_t Board::get_move_index() const {
    return this->move_index;
}

PlaceResult Board::test_place(const Move& move) const {
    // TODO: complete implementation here

    //Compute the scores of the main word and all crosswise words created and add them up
	//Check that the word is adjacent to at least one other tile on the board

    //get position of first move position where placing tiles
   	Position current(move.row, move.column);
   	string newWord;
   	Direction cDirection = move.direction;
   	bool adjacentWords = false;

   	if(at(current).has_tile())
   	{
   		return PlaceResult("First tile already occupied");
   	}
   	//translate/move backwards to check for exisiting words
   	current = current.translate(cDirection, -1);
   	while(this->in_bounds_and_has_tile(current))
   	{	
   		current = current.translate(cDirection,-1);
   		adjacentWords = true;
   	}
   	//the one tile case
   	//have to check in BOTH directions
   	if((this->in_bounds_and_has_tile(current) == false) && (move.tiles.size() == 1) 
   		&& adjacentWords ==false)
   	{
   		//reset and start again
   		cDirection = move.direction;
   		if (move.direction == Direction::DOWN)
	   	{
	   		cDirection = Direction::ACROSS;
	   	}
	   	else
	   	{
	   		cDirection = Direction::DOWN;
	   	}
	   	current.row = move.row;
	   	current.column = move.column;
	   	current = current.translate(cDirection, -1);
	   	while(this->in_bounds_and_has_tile(current))
	   	{	
	   		current = current.translate(cDirection,-1);
	   		adjacentWords = true;
	   	}
   	}
   	//when loop breaks have to increment back by 1
   	//gives position of FIRST letter
   	current = current.translate(cDirection, 1);

   	//tracks position
   	vector<Board::Position> posTracker;
   	//builds new word as you translate through 
   	while(this->in_bounds_and_has_tile(current))
   	{
   		posTracker.push_back(current);
   		if(at(current).get_tile_kind().letter == '?')
   		{
   			newWord = newWord + at(current).get_tile_kind().assigned;
   		}
   		else
   		{
   			newWord = newWord + at(current).get_tile_kind().letter;
   		}
   		current = current.translate(cDirection,1);
   		adjacentWords=true;
   	}

   	//PLACE NEW TILES
   	bool hasStart = false;
   	PlaceResult badResult("Invalid Move");
   	//goes through new tiles, tile by tile
   	for (unsigned int i =0; i <(move.tiles.size()); i++)
   	{
   		//check if start
   		//Check that, if this is the first word placed, it crosses the start square
   		if(current == start)
   		{
   			if(this->get_move_index() == 0)
   			{
   				hasStart = true;
   			}
   			else
   			{
   				return badResult;
   			}
   		}
   		//Either down or across
   		//iterate through each tile and check if in bounds
   		if(cDirection== Direction::DOWN)
   		{
   			if(this->is_in_bounds(current) == false)
   			{
   				return badResult;
   			}
   		}
   		else if (cDirection == Direction::ACROSS)
   		{
   			if(this->is_in_bounds(current) == false)
   			{
   				return badResult;
   			}
   		}
   		//if no tile proceed as normal
   		//else want to skip tile if there is already tile there
   		//IS PART OF FINAL SCORE
   		if(this->at(current).has_tile() == false)
   		{
   			if(move.tiles[i].letter == '?')
	   		{
	   			newWord = newWord+ move.tiles[i].assigned;
	   		}
	   		else
	   		{
	   			newWord = newWord+ move.tiles[i].letter;
	   		}
   			// cerr << newWord << endl;
   		}
   		else
   		{
   			newWord = newWord + this->at(current).get_tile_kind().letter;
   			i--;
   			adjacentWords = true;
   		}
   		posTracker.push_back(current);
   		current = current.translate(cDirection, 1);
   	}
   	if((squares[start.row][start.column].has_tile() == false) && (hasStart == false))
   	{
   		return badResult;
   	}
   	//check for additional tiles after word
   	while(in_bounds_and_has_tile(current))
   	{
   		newWord = newWord + at(current).get_tile_kind().letter;
   		posTracker.push_back(current);
   		current = current.translate(cDirection,1);
   		adjacentWords=true;
   	}

   	//calculate total points of newWord
   	int total_word_points = 0;
   	int total_word_multiplier = 1;
   	int newTileTracker = 0;
   	for (unsigned int i =0; i < posTracker.size(); i++)
   	{
   		//NEW TILE
   		if(at(posTracker[i]).has_tile() == false)
   		{
   			//bonus applies 
   			//(tile points)*(letter multiplier )
   			total_word_points = total_word_points
   			+ (move.tiles[newTileTracker].points * at(posTracker[i]).letter_multiplier);
   			total_word_multiplier = total_word_multiplier * at(posTracker[i]).word_multiplier;
   			newTileTracker++;
   		}
   		//tile already exists on board, add regular points
   		//bonuses do not apply
   		else
   		{
   			total_word_points = total_word_points + at(posTracker[i]).get_points();
   		}
   	}

   	//REMEMBER TO DELETE
   	// cerr << "current word: " << newWord << endl;
   	// cerr << "total points: " << total_word_points << endl;
   	// cerr << "multiplier: " << total_word_multiplier << endl;
   	total_word_points = total_word_points * total_word_multiplier;


   	//OTHER WORDS
   	//check adjacent words
   	//iterate through move.tiles and look in the OPPOSITE direction for occupied tiles
   	//only need to check in opposite direction
   	int other_points_temp =0;
   	int other_word_multiplier = 1;
   	std::vector<std::string> finals;

   	Direction other_direction;
   	if (cDirection == Direction::DOWN)
   	{
   		other_direction = Direction::ACROSS;
   	}
   	else
   	{
   		other_direction = Direction::DOWN;
   	}
   	//temp goes through new tiles one by one
	Position temp(move.row,move.column);
	Position original(move.row,move.column);
	string other_word;
	bool found = false;
	bool found2=false; 
   	//only want to iterate through newly placed tiles
   	for (unsigned int i=0; i<move.tiles.size(); i++)
   	{
   		other_word = "";
   		temp = original;
   		other_points_temp = 0;
   		other_word_multiplier = 1;
   		if(this->at(original).has_tile() == true)
   		{
   			original= original.translate(cDirection, 1);
   			temp = original;
   			i--;
   			continue;
   		}
   		else
   		{
   			temp = temp.translate(other_direction, -1);
	   		//checks in opposite direction of move for tiles
	   		//goes to very front of the other word
	   		while(this->in_bounds_and_has_tile(temp))
	   		{
	   			temp= temp.translate(other_direction,-1);
	   		}
	   		temp = temp.translate(other_direction,1);
	   		// cerr << "temp row: " << temp.row << " temp col: "<<  temp.column  <<endl;
	   		// cerr  << "origial row: " << original.row << " original col: " << original.column <<  endl;
	   		if (temp != original)
	   		{	
	   			while(this->in_bounds_and_has_tile(temp))
	   			{
	   				other_word = other_word + this->at(temp).get_tile_kind().letter;
	   				other_points_temp = other_points_temp + move.tiles[i].points * at(temp).letter_multiplier;
	   				other_word_multiplier = other_word_multiplier * at(temp).word_multiplier;
	   				temp = temp.translate(other_direction,1);
	   				found = true; 
	   			}
	   		}
	   		if (found == true)
	   		{
	   			other_word = other_word + move.tiles[i].letter;
	   			other_points_temp = other_points_temp + move.tiles[i].points * at(temp).letter_multiplier;
	   			other_word_multiplier = other_word_multiplier * at(temp).word_multiplier;
	   			adjacentWords = true;
	   		}
	   		//need to check further for tiles
			temp = temp.translate(other_direction,1);
		   	while(this->in_bounds_and_has_tile(temp))
		   	{
		   		other_word = other_word + this->at(temp).get_tile_kind().letter;
		   		other_points_temp = other_points_temp + move.tiles[i].points * at(temp).letter_multiplier;
	   			other_word_multiplier = other_word_multiplier * at(temp).word_multiplier;
		   		temp = temp.translate(move.direction,1);
		   	}
		   	if (found2==true)
		   	{
		   		other_word = move.tiles[i].letter + other_word;
		   		other_points_temp = other_points_temp + move.tiles[i].points * at(temp).letter_multiplier;
	   			other_word_multiplier = other_word_multiplier * at(temp).word_multiplier;
	   			adjacentWords = true;
		   	}
   		}
   		if(other_word.size() > 1)
   		{
   			finals.push_back(other_word);
   			total_word_points = total_word_points + (other_points_temp * other_word_multiplier);
   			// cerr << "Other word: " << other_word << endl;
   		}
   		original= original.translate(move.direction, 1);
   	}

   	if ((adjacentWords == false) && (hasStart==false))
   	{
   		return badResult;
   	}
   	finals.push_back(newWord);
   	PlaceResult final(finals, total_word_points);
   	return final;


   	// std::vector<std::string> finals;
   	// finals.push_back(newWord);

   	// PlaceResult final(finals, total_word_points);
   	// return final;


}

PlaceResult Board::place(const Move& move) {
    // TODO: Complete implementation here
    PlaceResult result = test_place(move);
    if(!result.valid)
    {
    	//Tests fail when throwing out error here for some reason?
    	// cerr << "ERROR" << endl;
    	return result;
    }
    Position current(move.row, move.column);
    for (unsigned int i=0; i<move.tiles.size(); i++)
    {
    	//no tile at current locaiton
    	if(at(current).has_tile())
    	{
    		i--;
    	}
    	//if tile at current location, run for loop again (same parameters)
    	//but with current location moved; 
    	else
    	{
    		squares[current.row][current.column].set_tile_kind(move.tiles[i]);
			squares[current.row][current.column].letter_multiplier = 1;
			squares[current.row][current.column].word_multiplier = 1;
    	}
    	current = current.translate(move.direction,1);
    }
    return result; 
}

// The rest of this file is provided for you. No need to make changes.

BoardSquare& Board::at(const Board::Position& position) {
    return this->squares.at(position.row).at(position.column);
}

const BoardSquare& Board::at(const Board::Position& position) const {
    return this->squares.at(position.row).at(position.column);
}

bool Board::is_in_bounds(const Board::Position& position) const {
    return position.row < this->rows && position.column < this->columns;
}

bool Board::in_bounds_and_has_tile(const Position& position) const{
    return is_in_bounds(position) && at(position).has_tile();
}

void Board::print(ostream& out) const {
	// Draw horizontal number labels
	for (size_t i = 0; i < BOARD_TOP_MARGIN - 2; ++i) {
		out << std::endl;
	}
	out << FG_COLOR_LABEL << repeat(SPACE, BOARD_LEFT_MARGIN);
	const size_t right_number_space = (SQUARE_OUTER_WIDTH - 3) / 2;
	const size_t left_number_space = (SQUARE_OUTER_WIDTH - 3) - right_number_space;
	for (size_t column = 0; column < this->columns; ++column) {
		out << repeat(SPACE, left_number_space) << std::setw(2) << column + 1 << repeat(SPACE, right_number_space);
	}
	out << std::endl;

	// Draw top line
	out << repeat(SPACE, BOARD_LEFT_MARGIN);
    print_horizontal(this->columns, L_TOP_LEFT, T_DOWN, L_TOP_RIGHT, out);
    out << endl;

	// Draw inner board
	for (size_t row = 0; row < this->rows; ++row) {
        if (row > 0) {
            out << repeat(SPACE, BOARD_LEFT_MARGIN);
			print_horizontal(this->columns, T_RIGHT, PLUS, T_LEFT, out);
			out << endl;
		}

		// Draw insides of squares
		for (size_t line = 0; line < SQUARE_INNER_HEIGHT; ++line) {
			out << FG_COLOR_LABEL << BG_COLOR_OUTSIDE_BOARD;

			// Output column number of left padding
			if (line == 1) {
				out << repeat(SPACE, BOARD_LEFT_MARGIN - 3);
				out << std::setw(2) << row + 1;
				out << SPACE;
			} else {
				out << repeat(SPACE, BOARD_LEFT_MARGIN);
			}

            // Iterate columns
			for (size_t column = 0; column < this->columns; ++column) {
				out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL;
				const BoardSquare& square = this->squares.at(row).at(column);
				bool is_start = this->start.row == row && this->start.column == column;

				// Figure out background color
				if (square.word_multiplier == 2) {
					out << BG_COLOR_WORD_MULTIPLIER_2X;
				} else if (square.word_multiplier == 3) {
					out << BG_COLOR_WORD_MULTIPLIER_3X;
				} else if (square.letter_multiplier == 2) {
					out << BG_COLOR_LETTER_MULTIPLIER_2X;
				} else if (square.letter_multiplier == 3) {
					out << BG_COLOR_LETTER_MULTIPLIER_3X;
				} else if (is_start) {
					out << BG_COLOR_START_SQUARE;
                }

				// Text
                if (line == 0 && is_start) {
                    out << "  \u2605  ";
                } else if (line == 0 && square.word_multiplier > 1) {
                    out << FG_COLOR_MULTIPLIER << repeat(SPACE, SQUARE_INNER_WIDTH - 2) << 'W' << std::setw(1) << square.word_multiplier;
                } else if (line == 0 && square.letter_multiplier > 1) {
                    out << FG_COLOR_MULTIPLIER << repeat(SPACE, SQUARE_INNER_WIDTH - 2) << 'L' << std::setw(1) << square.letter_multiplier;
				} else if (line == 1 && square.has_tile()) {
                    char l = square.get_tile_kind().letter == TileKind::BLANK_LETTER ? square.get_tile_kind().assigned : ' ';
                    out << repeat(SPACE, 2) << FG_COLOR_LETTER << square.get_tile_kind().letter << l << repeat(SPACE, 1);
				} else if (line == SQUARE_INNER_HEIGHT - 1 && square.has_tile()) {
					out << repeat(SPACE, SQUARE_INNER_WIDTH - 1) << FG_COLOR_SCORE << square.get_points();
				} else {
					out << repeat(SPACE, SQUARE_INNER_WIDTH);
				}
			}

			// Add vertical line
			out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL << BG_COLOR_OUTSIDE_BOARD << std::endl;
		}
	}

	// Draw bottom line
	out << repeat(SPACE, BOARD_LEFT_MARGIN);
	print_horizontal(this->columns, L_BOTTOM_LEFT, T_UP, L_BOTTOM_RIGHT, out);
	out << endl << rang::style::reset << std::endl;
}
