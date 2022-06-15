// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include "Pack.h"


  // EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on.
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
  Pack::Pack() {
      int count = 0;
      for (int i = 0; i < NUM_SUITS; i++) {
          for (int j = 7; j < NUM_RANKS; j++) {
              cards[count] = Card(RANK_NAMES_BY_WEIGHT[j], SUIT_NAMES_BY_WEIGHT[i]);
              count++;
          }
      }
      next = 0;
  }

  // REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
  Pack::Pack(std::istream& pack_input) {
      std::string rank, junk, suit;
      int count = 0;
      while (pack_input.good() && count < PACK_SIZE) {
          pack_input >> rank >> junk >> suit;
          cards[count] = Card(rank, suit);
          count++;
      }
      next = 0;
  }

  // REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
  Card Pack::deal_one() {
      ++next;
      return cards[next - 1];
  }

  // EFFECTS: Resets next index to first card in the Pack
  void Pack::reset() {
      next = 0;
  }

  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
  void Pack::shuffle() {
      next = 0;
      for (int j = 0; j < 7; j++) {
        int count = 0;
        std::array<Card, PACK_SIZE> shuffled;
        for (int i = 0; i < PACK_SIZE / 2; i++) {
            Card first = cards[i];
            Card second = cards[(PACK_SIZE /2) + i];
            
            shuffled[count] = second;
            shuffled[count + 1] = first;
            count += 2;
        }
        for (int i = 0; i < PACK_SIZE; i++) {
            cards[i] = shuffled[i];
        }
      }
  }

  // EFFECTS: returns true if there are no more cards left in the pack
  bool Pack::empty() const {
      return next == 24;
  }