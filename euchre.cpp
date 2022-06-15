// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <iostream>
#include <fstream>
#include <cstring>
#include "Player.h"
#include "Pack.h"

using namespace std;

void init_players(char* const argv[], Player * players[]) {
    int count = 0;
    for (int i = 4; i < 12; i+=2) {
        players[count] = Player_factory(argv[ i ], argv[ i + 1 ]);
        count++;
    }

}

Card deal_pack(char* const argv[], Player * players[], int const &round, Pack &p) {
    if (strcmp(argv[2], "shuffle") == 0) {
        p.shuffle();
    }
    // Deals cards
    for (int i = 0; i < 8; i++) {
        if ((i < 4 && i % 2 == 0) || (i >= 4 && i % 2 == 1)) {
            for (int j = 0; j < 3; j++) {
                players[ (i + round + 1) % 4 ]->add_card(p.deal_one());
            }
        }
        else {
            for (int j = 0; j < 2; j++) {
                players[ (i + round + 1) % 4 ]->add_card(p.deal_one());
            }
        }
    }
    cout << "\nHand " << round << "\n";
    cout << *players[ round % 4 ] << " deals" << "\n";
    return p.deal_one();
}

int making_trump(const Card &upcard, string &order_up_suit, int const &round, 
                                                                Player * players[]) {

    int i = 0;
    bool trumpMade = false;
    // Round one
    while (!trumpMade && i < 4) {
        if (i == 3) {
            trumpMade = players[ (i + round + 1) % 4]->make_trump(upcard, true, 1, 
            order_up_suit);
        }
        else {
            trumpMade = players[ (i + round + 1) % 4]->make_trump(upcard, false, 1, 
            order_up_suit);
        }
        if (trumpMade) {
            cout << *players[ (i + round + 1) % 4 ] << " orders up " << order_up_suit 
            << "\n\n";
            players[ round % 4 ]->add_and_discard(upcard);
            order_up_suit = upcard.get_suit();

            return (i + round + 1) % 4;
        }
        else {
            cout << *players[ (i + round + 1) % 4 ] << " passes\n";
        }
        i++;
    }
    // Round 2
    int j = 0;
    while (!trumpMade && j < 4) {
        if (j == 3) {
            // Screw the Dealer
            order_up_suit = Suit_next(upcard.get_suit());
            cout << *players[ (j + round + 1) % 4 ] << " orders up " << order_up_suit 
            << "\n\n";
            trumpMade = true;
        }
        else {
            trumpMade = players[ (j + round + 1) % 4]->make_trump(upcard, false, 2, 
            order_up_suit);
        }
        if (trumpMade) {
            order_up_suit = Suit_next(upcard.get_suit());
            cout << *players[ (j + round + 1) % 4 ] << " orders up " << order_up_suit 
            << "\n\n";
            return (j + round + 1) % 4;
        }
        else {
            cout << *players[ (j + round + 1) % 4 ] << " passes\n";
        }
        j++;
    }
    return -1;
}

int play_tricks(const int leader, const string trump, Player * players[], 
int trickCount[]) {
    // Leading
    Card played[4];
    played[leader] = players[leader]->lead_card(trump);
    cout << played[leader] << " led by " << *players[leader] << "\n";
    // Playing
    for (int    i = 0; i < 3; i++) {
        played[(i + leader + 1) % 4] = 
            players[ (i + leader + 1) % 4]->play_card(played[leader], trump);
        cout << played[(i + leader + 1) % 4] << " played by " << 
            *players[ (i + leader + 1) % 4] << "\n";
    }
    // Determining highest card]
    int highestIndex = 0;
    for (int i = 0; i < 4; i++) {
        if (Card_less(played[highestIndex], played[i], played[leader], trump)) {
            highestIndex = i;
        }
    }
    cout << *players[highestIndex] << " takes the trick\n\n";
    trickCount[ highestIndex % 2]++; 
    return highestIndex;
}

void update_score(int trickCount[], int scoreCount[], const int trumpMaker, 
Player * players[]) {
    if (trickCount[0] > trickCount[1]) {
        if (trumpMaker % 2 == 0 && trickCount[0] != 5) {
            scoreCount[0] += 1;
        }
        else {
            scoreCount[0] += 2;
        }
    }
    else {
        if (trumpMaker % 2 == 1 && trickCount[1] != 5) {
            scoreCount[1] += 1;
        }
        else {
            scoreCount[1] += 2;
        }
    }
    if (trickCount[0] > trickCount[1]) {
        cout << *players[0] << " and " << *players[2] << " win the hand\n";
        if (trumpMaker % 2 == 1 ) {
            cout << "euchred!\n";
        }
        else if (trumpMaker % 2 == 0 && trickCount[0] == 5) {
            cout << "march!\n";
        }
    }
    else {
        cout << *players[1] << " and " << *players[3] << " win the hand\n";
        if (trumpMaker % 2 == 0 ) {
            cout << "euchred!\n";
        }
        else if (trumpMaker % 2 == 1 && trickCount[1] == 5) {
            cout << "march!\n";
        }
    }
    cout << *players[0] << " and " << *players[2] << " have " << scoreCount[0] 
        << " points\n";
    cout << *players[1] << " and " << *players[3] << " have " << scoreCount[1] 
        << " points\n";
}

void end_game(int scoreCount[], Player * players[]) {
    if (scoreCount[0] > scoreCount[1]) {
        cout << "\n" << *players[0] << " and " << *players[2] << " win!\n";
    }
    else {
        cout << "\n" << *players[1] << " and " << *players[3] << " win!\n";
    }
}

void delete_players(Player * players[]) {
    for (int i = 0; i < 4; i++) {
        delete players[i];
    }
}


int main(int argc, char* argv[]) {


    bool conditions = (
        (argc == 12) &&
        (atoi(argv[3]) >= 1 && atoi(argv[3]) <= 100) &&
        (strcmp(argv[2], "shuffle") == 0 || strcmp(argv[2], "noshuffle") == 0) && 
        (strcmp(argv[5], "Simple") == 0 || strcmp(argv[5], "Human") == 0) &&
        (strcmp(argv[7], "Simple") == 0 || strcmp(argv[7], "Human") == 0) &&
        (strcmp(argv[9], "Simple") == 0 || strcmp(argv[9], "Human") == 0) &&
        (strcmp(argv[11], "Simple") == 0 || strcmp(argv[11], "Human") == 0)
    );

    if (!conditions) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
    }

    for (int i = 0; i < 12; i++) {
        cout << argv[i] << " ";
    }

    ifstream inputPack(argv[1]);

    if (!inputPack.is_open()) {
        cout << "Error opening " << argv[1] << endl;
        return 1;
    }
    int scoreCount[2] = {0,0};
    int roundNum = 0;
    Pack pack(inputPack);
    Player * players[4];
    init_players(argv, players);
    
    while (scoreCount[0] < atoi(argv[3]) && scoreCount[1] < atoi(argv[3])) {
        int trickCount[2] = {0,0};
        Card upcard = deal_pack(argv, players, roundNum, pack);
        cout << upcard << " turned up\n";
        string trumpSuit;
        const int trumpMaker = making_trump(upcard, trumpSuit, roundNum, players);
        int leader = play_tricks((roundNum + 1) % 4, trumpSuit, players, trickCount);
        for (int i = 0; i < 4; i++) {
            leader = play_tricks(leader, trumpSuit, players, trickCount);
        }
        update_score(trickCount, scoreCount, trumpMaker, players);
        pack.reset();
        roundNum++;
    }

    end_game(scoreCount, players);
    delete_players(players);

    return 0;
}