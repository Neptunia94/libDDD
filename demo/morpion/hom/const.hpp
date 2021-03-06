// author : S. Hong,Y.Thierry-Mieg
// date : Jan 2009
#pragma once

#include <boost/multi_array.hpp>
#include <cmath>

extern size_t NBCELL;
extern size_t STATE_SYSTEM_CELL;
extern size_t LINE;
extern size_t COLUMN;

// players
/**
 *
 */
enum game_status_type
{ 
  TO_PA = -1 /// Player A to play
, TO_PB = -2 /// Player B to play
, EMPTY = -1 /// Ligne Vide
, PA    =  0 /// Joueur A
, PB    =  1 /// Joueur B
};

// Type definition
typedef boost::multi_array<int, 2> array_type;
