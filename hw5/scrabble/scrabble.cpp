#include "scrabble.h"
#include "formatting.h"
#include <iostream>
#include <iomanip>
#include <map>

using namespace std;


// Given to you. this does not need to be changed
Scrabble::Scrabble(const ScrabbleConfig& config)
    : hand_size(config.hand_size)
    , minimum_word_length(config.minimum_word_length)
    , tile_bag(TileBag::read(config.tile_bag_file_path, config.seed))
    , board(Board::read(config.board_file_path))
    , dictionary(Dictionary::read(config.dictionary_file_path)) {}

void Scrabble::add_players()
{
    string playerName; 
    bool askName = true;
    unsigned int numPlayers = 0;
    while(askName)
    {
        cout << "Please enter number of players: ";
        cin >> numPlayers;
        if (numPlayers < 1 || numPlayers > 8)
        {
            cin.clear();
            cin.ignore();
            cout << "Error: Please enter number of players (1-8): " << endl;
        }
        else 
        {
            break;
        }
    }
    cout << numPlayers << " players confirmed." << endl;


    for (unsigned int i=0; i < numPlayers; i++)
    {
        cout << "Please enter name for player " << i+1 << endl; 
        cin >> playerName;
        cout << "Player " << i+1 << ", named \"" << playerName << "\", has been added." << endl;
        HumanPlayer newPlayer(playerName, hand_size);
        std::shared_ptr<HumanPlayer> person = std::make_shared<HumanPlayer> (newPlayer);
        players.push_back(person);
        //add random tiles to player hand, removing form tilebag
        vector<TileKind> playerTiles = tile_bag.remove_random_tiles(hand_size);
        players[i]->add_tiles(playerTiles);
    }

}
// Game Loop should cycle through players and get and execute that players move
// until the game is over.
void Scrabble::game_loop() {
    // TODO: implement this.

    // Useful cout expressions with fancy colors. Expressions in curly braces, indicate values you supply.
    // cout << "You gained " << SCORE_COLOR << {points} << rang::style::reset << " points!" << endl;
    // cout << "Your current score: " << SCORE_COLOR << {points} << rang::style::reset << endl;
    // cout << endl << "Press [enter] to continue.";
    //tracks number of consecutive passes
    unsigned int consPass = 0;
    //REMEMBER TO ADD: game also ends when tile bag is empty 
    //game keeps going as long as players have not passed
    while (consPass != players.size())
    {
        for (unsigned int i=0; i <players.size(); i++)
        {
            Move playerMove;
            unsigned int totalPoints = 0;
            unsigned int playerPoints = players[i]->get_points();
            while (true) 
            {
                try 
                {
                    board.print(cout);
                    playerMove = players[i]->get_move(board,dictionary);
                    break;
                } 
                catch (const std::out_of_range& oor) 
                {
                    // process any error
                    cerr << oor.what() << endl;
                }
                catch (CommandException& ce)
                {
                    cerr << ce.what() << endl;
                }
                catch (MoveException& me)
                {
                    cerr << me.what() << endl;
                }
            }
            if (playerMove.kind == MoveKind::PLACE)
            {
                //place tile and remove tiles from hand
                PlaceResult goodPlace = board.place(playerMove);
                players[i]->remove_tiles(playerMove.tiles);
                vector<TileKind> addTiles = tile_bag.remove_random_tiles(playerMove.tiles.size());
                players[i]->add_tiles(addTiles);
                consPass = 0;

                //calculate score
                if (players[i]->get_hand_size() == 0)
                {
                    players[i]->add_points(EMPTY_HAND_BONUS);
                }
                players[i]->add_points(goodPlace.points);
                //cout gained points and total points
                totalPoints = players[i]->get_points();
                unsigned int gainedPoints = totalPoints - playerPoints;
                cout << "You gained " << gainedPoints << " points!" << endl;
            }
            else if(playerMove.kind==MoveKind::EXCHANGE)
            {
                //removes tiles from hand and add them back to bag
                players[i]->remove_tiles(playerMove.tiles);
                for(unsigned int j=0; j<playerMove.tiles.size();j++)
                {
                    tile_bag.add_tile(playerMove.tiles[j]);
                }

                vector<TileKind> addTiles = tile_bag.remove_random_tiles(playerMove.tiles.size());
                players[i]->add_tiles(addTiles);
                consPass = 0;
            }
            else if (playerMove.kind == MoveKind::PASS)
            {
                consPass++;
            }
            cout << "Your current score: " << players[i]->get_points() << endl;
            cout << endl;
            cout << "Press [enter] to continue.";
            cin.ignore();
            if(consPass == players.size())
            {
                break;
            }
        }
    }

}

// Performs final score subtraction. Players lose points for each tile in their
// hand. The player who cleared their hand receives all the points lost by the
// other players.
void Scrabble::final_subtraction(vector<shared_ptr<Player>> & plrs) {
    // TODO: implement this method.
    // Do not change the method signature.
    for(unsigned int i=0; i <plrs.size(); i++)
    {
        if(plrs[i]->count_tiles() == 0)
        {
            for(unsigned int j=0; j<plrs.size(); j++)
            {
                plrs[i]->add_points(plrs[j]->get_hand_value());
            }
        }
        else 
        {
            plrs[i]->subtract_points(plrs[i]->get_hand_value());
        }
    }
}

// You should not need to change this function.
void Scrabble::print_result() {
	// Determine highest score
    size_t max_points = 0;
	for (auto player : this->players) {
		if (player->get_points() > max_points) {
			max_points = player->get_points();
        }
	}

	// Determine the winner(s) indexes
	vector<shared_ptr<Player>> winners;
	for (auto player : this->players) {
		if (player->get_points() >= max_points) {
			winners.push_back(player);
        }
	}

    cout << (winners.size() == 1 ? "Winner:" : "Winners: ");
	for (auto player : winners) {
		cout << SPACE << PLAYER_NAME_COLOR << player->get_name();
	}
	cout << rang::style::reset << endl;

	// now print score table
    cout << "Scores: " << endl;
    cout << "---------------------------------" << endl;

	// Justify all integers printed to have the same amount of character as the high score, left-padding with spaces
    cout << setw(static_cast<uint32_t>(floor(log10(max_points) + 1)));

	for (auto player : this->players) {
		cout << SCORE_COLOR << player->get_points() << rang::style::reset << " | " << PLAYER_NAME_COLOR << player->get_name() << rang::style::reset << endl;
	}
}

// You should not need to change this.
void Scrabble::main() {
    add_players();
    game_loop();
    final_subtraction(this->players);
    print_result();
}
