#include "player.h"
#include "iostream"

using namespace std;


// TODO: implement member functions

// Adds points to player's score
void Player::add_points(size_t points)
{
	this->points = this->points+points;
}

// Subtracts points from player's score
void Player::subtract_points(size_t points)
{
	if(points > this->points)
	{
		this->points = 0;
		return;
	}
	this->points = this->points-points;
}

size_t Player::get_points() const
{
	return (this->points);
}

const std::string& Player::get_name() const
{
	return (this->name);
}

// Returns a VALID move that can be executed (can be placed, exchanged, or passed).
// Move Player::get_move(const Board& board, const Dictionary& d) const = 0
// {

// }

// Returns the number of tiles in a player's hand.
size_t Player::count_tiles() const
{
	return (this->tiles.count_tiles());
}

// Removes tiles from player's hand.
void Player::remove_tiles(const std::vector<TileKind>& tiles)
{
	// try
	// {
		for(auto it = tiles.begin(); it != tiles.end(); ++it)
		{
			this->tiles.remove_tile(*it);
		}
	// }
	// catch(out_of_range& oor)
	// {
	// 	cerr << "Error" << endl;
	// 	cout << "Error in move: not enough tiles!" << endl;
	// 	cout << "Error in move: unknown command!" << endl;
	// }
}

// Adds tiles to player's hand.
void Player::add_tiles(const std::vector<TileKind>& tiles)
{
	for (auto it=tiles.begin(); it != tiles.end(); ++it)
	{
		this->tiles.add_tile(*it);
	}
}

// Checks if player has a matching tile.
bool Player::has_tile(TileKind tile)
{
	try
	{
		this->tiles.lookup_tile(tile.letter);
	}
	catch(out_of_range& oor)
	{
		return false; 
	}
	return true;
}

// Returns the total points of all tiles in the players hand.
unsigned int Player::get_hand_value() const
{
	return (this->tiles.total_points());
}

size_t Player::get_hand_size() const
{
	return (this->hand_size);
}