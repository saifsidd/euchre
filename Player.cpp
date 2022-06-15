// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "Player.h"


class Human : public Player {
 private:

  std::string name;
  std::vector<Card> hand;

 public:
  Human(const std::string & name_in)
  : name(name_in) {}

  const std::string & get_name() const override {
    return name;
  }

  void add_card(const Card &c) override {
    hand.push_back(c);
    std::sort(hand.begin(), hand.end());
  }

 
  bool make_trump(const Card &upcard, bool is_dealer,
                          int round, std::string &order_up_suit) const override {
    for (int i = 0; i < static_cast<int>(hand.size()); i++) {
      std::cout << "Human player " << get_name() 
      << "'s hand: [" << i << "] " << hand[i] << "\n";
    }
    std::cout << "Human player " << get_name() << ", please enter a suit, or \"pass\":\n";
    std::string option;
    std::cin >> option;
    if (option == "pass") {
      return false;
    }
    order_up_suit = option;
    return true;
  }

  void add_and_discard(const Card &upcard) override {
      for (int i = 0; i < static_cast<int>(hand.size()); i++) {
        std::cout << "Human player " << get_name() 
        << "'s hand: [" << i << "] " << hand[i] << "\n";
    }
    std::cout << "Discard upcard: [-1]\n";
    std::cout << "Human player " << get_name() << ", please select a card to discard:\n";
    int option;
    std::cin >> option;
    if (option != -1) {
      hand[option] = upcard;
    }
    std::cout << std::endl;
  }

  Card lead_card(const std::string &trump) override {
    for (int i = 0; i < static_cast<int>(hand.size()); i++) {
      std::cout << "Human player " << get_name() 
      << "'s hand: [" << i << "] " << hand[i] << "\n";
    }
    std::cout << "Human player " << get_name() << ", please select a card:\n";
    int option;
    std::cin >> option;
    std::cout << std::endl;
    Card card = hand[option];
    hand.erase(hand.begin() + option);
    return card;
  }

  Card play_card(const Card &led_card, const std::string &trump) override {
    return lead_card(trump);
  }


};

class Simple : public Player {
 private:

  std::string name;
  std::vector<Card> hand;

  bool order_up(const Card &upcard, int round) const {
    int count = 0;
    int criteria = 2;
    std::string suit = upcard.get_suit();
    if (round == 2) {
      suit = Suit_next(upcard.get_suit());
      criteria = 1;
    }
    for (int i = 0; i < static_cast<int>(hand.size()); i++) {
      if (hand[i].get_suit(suit) == suit && hand[i].is_face()) {
        count++;
        if (count == criteria) {
          return true;
        }
      }
    }
    return false;

  }

  bool follows_suit(const Card &led_card, const std::string &trump) {
    for (int i = 0; i < static_cast<int>(hand.size()); i++) {
      if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
        return true;
      }
    }
    return false;
  }

 public:

  Simple(const std::string & name_in)
  : name(name_in) {}

  const std::string & get_name() const override {
    return name;
  }

  //REQUIRES player has less than MAX_HAND_SIZE cards
  //EFFECTS  adds Card c to Player's hand
  void add_card(const Card &c) override {
    hand.push_back(c);
  }

  //REQUIRES round is 1 or 2
  //MODIFIES order_up_suit
  //EFFECTS If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.
  bool make_trump(const Card &upcard, bool is_dealer,
                          int round, std::string &order_up_suit) const override {
    if (round == 1 && order_up(upcard, round)) {
      order_up_suit = upcard.get_suit();
      // if (is_dealer) {
      //   add_and_discard(upcard);
      // }
      return true;
    }
    else if (round == 2 && order_up(upcard, round) && !is_dealer) {
      order_up_suit = Suit_next(upcard.get_suit());
      return true;
    }
    else if (round == 2 && is_dealer) {
      order_up_suit = Suit_next(upcard.get_suit());
      return true;
    }
    return false;
  }

  //REQUIRES Player has at least one card
  //EFFECTS  Player adds one card to hand and removes one card from hand.
  void add_and_discard(const Card &upcard) override {
    int lowestIndex = 0;
    for (int i = 0; i < static_cast<int>(hand.size()); i++) {
      if (Card_less(hand[i], hand[lowestIndex], upcard.get_suit())) {
        lowestIndex = i;
      }
    }
    hand[lowestIndex] = upcard;
  }

  //REQUIRES Player has at least one card, trump is a valid suit
  //EFFECTS  Leads one Card from Player's hand according to their strategy
  //  "Lead" means to play the first Card in a trick.  The card
  //  is removed the player's hand.
  Card lead_card(const std::string &trump) override {
    int highestIndex = 0;
    // create vector of integers that represent non-trump indices
    std::vector<int> nonTrumpIndices;
    for (int i = 0; i < static_cast<int>(hand.size()); i++) {
      if (!hand[i].is_trump(trump)) {
        nonTrumpIndices.push_back(i);
      }
    }
    if (!nonTrumpIndices.empty()) {
      highestIndex = nonTrumpIndices[0];
      for (int i = 0; i < static_cast<int>(nonTrumpIndices.size()); i++) {
        if (Card_less(hand[highestIndex], hand[ nonTrumpIndices[i] ], trump)) {
          highestIndex = nonTrumpIndices[i];
        }
      }
    }
    else {
      for (int i = 0; i < static_cast<int>(hand.size()); i++) {
        if (Card_less(hand[highestIndex], hand[i], trump)) {
          highestIndex = i;
        }
      }
    }
    Card card = hand[highestIndex];
    hand.erase(hand.begin() + highestIndex);
    return card;
  }

  //REQUIRES Player has at least one card, trump is a valid suit
  //EFFECTS  Plays one Card from Player's hand according to their strategy.
  //  The card is removed from the player's hand.
  Card play_card(const Card &led_card, const std::string &trump) override {
    if (follows_suit(led_card, trump)) {
      int highestIndex = 0;
      while (hand[highestIndex].get_suit(trump) != led_card.get_suit(trump)) {
        highestIndex++;
      }
      for (int i = highestIndex; i < static_cast<int>(hand.size()); i++) {
        bool matchesSuit = hand[i].get_suit(trump) == led_card.get_suit(trump);
        if (matchesSuit && Card_less(hand[highestIndex], hand[i], trump)) {
          highestIndex = i;
        }
      }
      Card playCard = hand[highestIndex];
      hand.erase(hand.begin() + highestIndex);
      return playCard;
    }
    else {
      int lowestIndex = 0;
      for (int i = lowestIndex; i < static_cast<int>(hand.size()); i++) {
        if (Card_less(hand[i],hand[lowestIndex], led_card, trump)) {
          lowestIndex = i;
        }
      }
      Card playCard = hand[lowestIndex];
      hand.erase(hand.begin() + lowestIndex);
      return playCard;
    }
  }

};


Player * Player_factory(const std::string &name, const std::string &strategy) {
  if (strategy == "Human") {
    return new Human(name);
  }
  else {
    return new Simple(name);
  }
}
std::ostream & operator<<(std::ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}

