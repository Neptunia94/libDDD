#include "previous.hpp"

#include <iostream>
#include <boost/functional/hash.hpp>


/**
 * 
 * This Class is used for modelise the reverse operation of one hit on the game.
 * From one possible state of game status, we must undo the current configuration by remove one hit
 * ALGO :
 *  1) First we check the game correct game status ([PA Win], [PA to play], [PB Win], [PB to play])
 *  2) Second we change the new game status after removing one hit, must be [PA to play] or [PB to play]
 *  3) Third we remove the hit by an EMPTY Cell 
 * 
 */
class _previous :public StrongHom
{
private:
    /**
     * Liste of member variables = homomorphism parameters
     */
    int cell;     			   // The cell to play : 0 <= cell < 9
    game_status_type status;   // The status game (Winner 1, Winner 0 and No winner)
    
  public:

    /**
   * The constructor binds the homomorphism parameters cell and player
     */
	  _previous (int c,game_status_type s) : cell(c), status(s)
    {
    }

    /**
     * Define the target variable to skip in the current route in apply
     */
    bool
        skip_variable ( int vr ) const
    {
      return vr != cell && vr != static_cast<int>(STATE_SYSTEM_CELL);
    }

    /**
     * PHI [1] : called if the terminal 1 is encountered, returns a constant DDD
     */
    GDDD
        phiOne() const
    {
      return DDD::top;
    }

    /**
     * PHI [vr,vl] : called on each arc labeled with "vl" when the homomorphism is applied to a node
     * of variable "vr" such that vr is NOT skipped.
     * When the hom is applied to a node this function is called for each arc, and the result
     * (a homomorphism) is applied to the successor node pointed by the arc.
     *  vr ==> Variable of current node
     *  vl ==> arc label (an integer) of current arc
     */
    GHom
        phi ( int vr, int vl ) const
    {
      if ( vr == static_cast<int>(STATE_SYSTEM_CELL) )
      {
    	  return GHom ( vr, vl, GHom(this) )   ;
      }
      else
      {
		  /* We have found one case to remove */
		  return GHom ( vr, EMPTY, GHom::id ); // e-(joueur)-> ID
      }
    }

    /**
     * Hash function used for unique table storage.
     */
    size_t
        hash() const
    {
      // hash function should exhibit reasonable spread and involve as many parameters as possible.
      std::size_t seed = 3863;
      boost::hash_combine ( seed, cell );
      boost::hash_combine ( seed, status );
      return seed ;
    }

    /**
     * Overloading StrongHom default print with a customized pretty-print
     */
    void
        print ( std::ostream & os ) const
    {
      os << "Previous( cell:" << cell << ", player:" << status << " )";
    }

    /**
     * Overload of operator== necessary for unique table storage
     * argument is typed StrongHom as == is part of StrongHom contract, simlarly to java's bool equals(Object)
     */
    bool
        operator== ( const StrongHom &s ) const
    {
      // direct "hard" cast to own type is ok, type checks already made in library
      const _previous& ps = dynamic_cast<const _previous&> ( s );
      // basic comparator behavior, just make sure you put all attributes there.
      return cell == ps.cell && status == ps.status ;
    }

    /**
     * Clone current homomorphism, used for unique storage.
     */
    _GHom *
        clone () const
    {
      return new _previous ( *this );
    }
};


/**
 * Factory of _previous Instance
 */
GHom
	previous(int cell,game_status_type status){
	return _previous(cell,status);
}


DDD
	previous(DDD& succ, Hom pred)
{
	succ = succ * pred(succ);
  	return succ;
}
