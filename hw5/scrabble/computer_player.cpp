
#include <memory>

#include "computer_player.h"
#include <string>
#include "place_result.h"




void ComputerPlayer::left_part(Board::Position anchor_pos, 
                               std::string partial_word,
                               Move partial_move, 
                               std::shared_ptr<Dictionary::TrieNode> node, 
                               size_t limit,
                               TileCollection& remaining_tiles,
                               std::vector<Move>& legal_moves,
                               const Board& board) const {
    // HW5: IMPLEMENT THIS
  //OUT OF TIME TO IMPLEMENT, ROUGH WEEK SORRY
  // remaining_tiles.remove_tiles()
  //left_part
  //remaining_tiles.add_tiles();
}

void ComputerPlayer::extend_right(Board::Position square,
                                  std::string partial_word,
                                  Move partial_move, 
                                  std::shared_ptr<Dictionary::TrieNode> node,
                                  TileCollection& remaining_tiles,
                                  std::vector<Move>& legal_moves,
                                  const Board& board) const {
    // HW5: IMPLEMENT THIS
}

Move ComputerPlayer::get_move(const Board& board, const Dictionary& dictionary) const {
	std::vector<Move> legal_moves;
    std::vector<Board::Anchor> anchors = board.get_anchors();
    // HW5: IMPLEMENT THIS
    for (unsigned int i=0; i <anchors.size(); i++)
    {
      if (anchors[i].limit > 0)
      {
        // left_part(anchors[i].position, );
      }
      //account for tiles directly to right
      if (anchors[i].limit == 0)
      {
        // extend_right();
      }
    }
	
	
	return get_best_move(legal_moves, board, dictionary);
}

Move ComputerPlayer::get_best_move(std::vector<Move> legal_moves, const Board& board, const Dictionary& dictionary) const {
    Move best_move = Move(); // Pass if no move found	
	// HW5: IMPLEMENT THIS
    //need to account for entire hand somehow
  if(legal_moves.empty())
  {
    return best_move;
  }
  unsigned int best_points = 0;
  for (unsigned int i=0; i <legal_moves.size(); i++)
  {
    PlaceResult testPlace = board.test_place(legal_moves[i]);
    if(testPlace.points > best_points)
    {
      best_move = legal_moves[i];
    }
  }
	
	
	return best_move;	
}
