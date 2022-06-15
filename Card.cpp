// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include "Card.h"
// add any necessary #include or using directives here

// rank and suit names -- do not remove these
constexpr const char* const Card::RANK_TWO;
constexpr const char* const Card::RANK_THREE;
constexpr const char* const Card::RANK_FOUR;
constexpr const char* const Card::RANK_FIVE;
constexpr const char* const Card::RANK_SIX;
constexpr const char* const Card::RANK_SEVEN;
constexpr const char* const Card::RANK_EIGHT;
constexpr const char* const Card::RANK_NINE;
constexpr const char* const Card::RANK_TEN;
constexpr const char* const Card::RANK_JACK;
constexpr const char* const Card::RANK_QUEEN;
constexpr const char* const Card::RANK_KING;
constexpr const char* const Card::RANK_ACE;

constexpr const char* const Card::SUIT_SPADES;
constexpr const char* const Card::SUIT_HEARTS;
constexpr const char* const Card::SUIT_CLUBS;
constexpr const char* const Card::SUIT_DIAMONDS;

// add your code below

 //EFFECTS Initializes Card to the Two of Spades
  Card::Card() 
  : rank(RANK_TWO), suit(SUIT_SPADES)
  {}

  //REQUIRES rank is one of "Two", "Three", "Four", "Five", "Six", "Seven",
  //  "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"
  //  suit is one of "Spades", "Hearts", "Clubs", "Diamonds"
  //EFFECTS Initializes Card to specified rank and suit
  Card::Card(const std::string &rank_in, const std::string &suit_in) 
  : rank(rank_in), suit(suit_in)
  {}

  //EFFECTS Returns the rank
  std::string Card::get_rank() const {
      return rank;
  }

  //EFFECTS Returns the suit.  Does not consider trump.
  std::string Card::get_suit() const {
      return suit;
  }

  //REQUIRES trump is a valid suit
  //EFFECTS Returns the suit
  //HINT: the left bower is the trump suit!
  std::string Card::get_suit(const std::string &trump) const {
      if (is_left_bower(trump)) {
          return trump;
      }
      return suit;
  }

  //EFFECTS Returns true if card is a face card (Jack, Queen, King or Ace)
  bool Card::is_face() const {
      return (rank == RANK_ACE || rank == RANK_KING 
      || rank == RANK_QUEEN || rank == RANK_JACK);
  }

  //REQUIRES trump is a valid suit
  //EFFECTS Returns true if card is the Jack of the trump suit
  bool Card::is_right_bower(const std::string &trump) const {
      return (rank == RANK_JACK && suit == trump);
  }

  //REQUIRES trump is a valid suit
  //EFFECTS Returns true if card is the Jack of the next suit
  bool Card::is_left_bower(const std::string &trump) const {
      return (Suit_next(suit) == trump && rank == RANK_JACK);
  }

  //REQUIRES trump is a valid suit
  //EFFECTS Returns true if the card is a trump card.  All cards of the trump
  // suit are trump cards.  The left bower is also a trump card.
  bool Card::is_trump(const std::string &trump) const {
      return (is_left_bower(trump) || suit == trump);
  }


// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec (see the appendices) before implementing
// the following operator overload functions:
//   operator<<
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=

bool operator<(const Card &lhs, const Card &rhs) {
    // Compare ranks!!!
    int lRank, rRank;
    lRank = rRank = -1;
    for (int i = 0; i < NUM_RANKS; i++) {
        if (lhs.get_rank() == RANK_NAMES_BY_WEIGHT[i]) {
            lRank = i;
        }
        if (rhs.get_rank() == RANK_NAMES_BY_WEIGHT[i]) {
            rRank = i;
        }
    }
    // Return true if lRank < rRank
    // False if lRank > rRank
    if (lRank < rRank) {
        return true;
    }
    else if (lRank > rRank) {
        return false;
    }
    // Compares suits
    int lSuit, rSuit;
    lSuit = rSuit = 0;
    for (int i = 0; i < NUM_SUITS; i++) {
        if (lhs.get_suit() == SUIT_NAMES_BY_WEIGHT[i]) {
            lSuit = i;
        }
        if (rhs.get_suit() == SUIT_NAMES_BY_WEIGHT[i]) {
            rSuit = i;
        }
    }
    if (lSuit < rSuit) {
        return true;
    }

    return false;
}

bool operator<=(const Card &lhs, const Card &rhs) {
    return (lhs == rhs || lhs < rhs);
}

//EFFECTS Returns true if lhs is higher value than rhs.
//  Does not consider trump.
bool operator>(const Card &lhs, const Card &rhs) {
    return !(lhs <= rhs);
}

//EFFECTS Returns true if lhs is higher value than rhs or the same card as rhs.
//  Does not consider trump.
bool operator>=(const Card &lhs, const Card &rhs) {
    return !(lhs < rhs);
}

//EFFECTS Returns true if lhs is same card as rhs.
//  Does not consider trump.
bool operator==(const Card &lhs, const Card &rhs) {
    return (lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit());
}

//EFFECTS Returns true if lhs is not the same card as rhs.
//  Does not consider trump.
bool operator!=(const Card &lhs, const Card &rhs) {
    return !(lhs == rhs);
}

//REQUIRES suit is a valid suit
//EFFECTS returns the next suit, which is the suit of the same color
std::string Suit_next(const std::string &suit) {
      int i = 0;
      while (SUIT_NAMES_BY_WEIGHT[i] != suit) {
          i++;
      }
      if (i == 0) {
          return "Clubs";
      }
      if (i == 1) {
          return "Diamonds";
      }
      if (i == 2) {
          return "Spades";
      }
      if (i == 3) {
          return "Hearts";
      }
      return "";
}

//EFFECTS Prints Card to stream, for example "Two of Spades"
std::ostream & operator<<(std::ostream &os, const Card &card) {
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if b is greater than a.  Uses trump to determine
// order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const std::string &trump) {
    bool at = a.is_trump(trump);
    bool bt = b.is_trump(trump);
    if (a == b) {
        return false;
    }
    if ((at || bt) && !(at && bt)) {
        return !at;
    }
    if (!at && !bt) {
        return a < b;
    }
    if (at && bt) {
        if (b.is_right_bower(trump) || (b.is_left_bower(trump) 
        && !a.is_right_bower(trump))) {
            return true;
        }
        if (a.is_right_bower(trump) || (a.is_left_bower(trump) 
        && !b.is_right_bower(trump))) {
            return false;
        }
    }
    return a < b;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if a is lower value than b.  Uses both the trump suit
//  and the suit led to determine order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const Card &led_card,
               const std::string &trump) {
    if (led_card.is_trump(trump)) {
        return Card_less(a, b, trump);
    }

    if (a == b) {
        return false;
    }
    if (a.is_trump(trump) || b.is_trump(trump)) {
        if (a.is_trump(trump) && !b.is_trump(trump)) {
            return false;
        }
        if (!a.is_trump(trump) && b.is_trump(trump)) {
            return true;
        }
        if (a.is_right_bower(trump)) {
            return false;
        }
        if (b.is_right_bower(trump)) {
            return true;
        }
        if (a.is_left_bower(trump)) {
            return false;
        }
        if (b.is_left_bower(trump)) {
            return true;
        }
        return b > a;
    }

    if (a.get_suit() == led_card.get_suit() || b.get_suit() == led_card.get_suit()) {
        if (a.get_suit() == led_card.get_suit() && b.get_suit() != led_card.get_suit()) {
            return false;
        }
        if (a.get_suit() != led_card.get_suit() && b.get_suit() == led_card.get_suit()) {
            return true;
        }
        return b > a;
    }
    return b > a;
}
