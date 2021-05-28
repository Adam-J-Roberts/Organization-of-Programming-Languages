    /************************************************************
     * Name:  Adam Roberts                                      *
     * Project:  Project 1 / GO STOP C++                        *
     * Class:  CMPS 366 01                                      *
     * Date:  February 18, 2020                                 *
     ************************************************************/

#ifndef Deck_H
#define Deck_H
#include <vector>
#include <string>

using namespace std;

class Deck
{
    private:
        static vector<string> playDeck;

    public:
        Deck(int count);

        static void addCard(string card);
        static int deckSize();
        static string getDeckCard(int loc);
        static void printDeck();
        static void removeTopCard();
        static void shuffle();

};
#endif