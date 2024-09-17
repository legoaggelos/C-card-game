#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cassert>
#include <iostream>
#include <vector>
#include <string_view>
#include <bits/stdc++.h>
#include "random.h"
#include <algorithm>
namespace Blackjack
{
    struct Card
    {
        enum Rank
        {
            ace,
            rank_2,
            rank_3,
            rank_4,
            rank_5,
            rank_6,
            rank_7,
            rank_8,
            rank_9,
            rank_10,
            jack,
            queen,
            king,
            max_ranks,
        };
        enum Suit
        {
            club,
            diamond,
            heart,
            spade,
            maxSuits,
        };
        Rank rank{};
        Suit suit{};
        static constexpr std::array allRanks{ace, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, rank_9, rank_10, jack, queen, king};
        static constexpr std::array allSuits{club, diamond, heart, spade};
        friend std::ostream &operator<<(std::ostream &out, const Card &card)
        {
            static constexpr std::array ranks{'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
            static constexpr std::array suits{'C', 'D', 'H', 'S'};
            out << ranks[card.rank] << suits[card.suit];
            return out;
        }
        short value() const
        {
            static constexpr std::array<short,13> rankValues{11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
            return rankValues[rank];
        }
    };
    class Deck
    {
        std::array<Card, 52> deck{};
        short currentCardIndex{0};

    public:
        Deck()
        {
            unsigned int i{0};
            for (auto suit : Card::allSuits)
            {
                for (auto rank : Card::allRanks)
                {
                    deck[i] = Card{rank, suit};
                    i++;
                }
                i++;
            }
        }
        Card dealCard()
        {
            assert(static_cast<unsigned short>(currentCardIndex) < deck.size());

            return deck[static_cast<unsigned short>(currentCardIndex++)];
        }
        void shuffle()
        {
            std::shuffle(deck.begin(), deck.end(), Random::mt);
            currentCardIndex = 0;
        }
    };
    struct Player
    {
        short score{};
        Player(short sc) : score{sc}
        {
        }
    };
    namespace Settings
    {
        // Maximum score before losing.
        constexpr int bust{21};

        // Minium score that the dealer has to have.
        constexpr int dealerStopsAt{17};
    };
    bool dealerTurn(Player &dealer, Deck &deck)
    {
        while (dealer.score < Settings::dealerStopsAt)
        {
            Card card{deck.dealCard()};
            dealer.score += card.value();
            std::cout << "dealer flips a " << card << " and now has score " << dealer.score<<'\n';
        }
        if (dealer.score > Settings::bust)
        {
            std::cout << "The dealer was bust!\n";
            return true;
        }
        return false;
    }
    char getPlay(std::string_view prompt)
    {
        char attempt{};
        bool isFirstAttempt{true};
        do
        {
            if (!isFirstAttempt)
            {
                std::cin.clear();
                std::cout << "That wasn't a valid input.  Try again.\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else
            {
                isFirstAttempt = false;
            }
            std::cout << prompt;
            std::cin >> attempt;

        } while (!std::cin || (attempt != 's' && attempt != 'h'));
        return attempt;
    }
    char playerTurn(Player& player, Deck& deck){
        char playerMove{getPlay("(h) to hit, or (s) to stand: ")};
        assert(playerMove=='s'||(playerMove=='h'&& "getPlay returns wrong value"));
        if(playerMove=='h'){
            Card next{deck.dealCard()};
            player.score+=next.value();
            std::cout<<"you drew a "<<next<<" "<<player.score<<'\n';
        }
         return playerMove=='h' ? 'h':'s';
    }
    bool playBlackjack()
    {
        
        Player dealer{static_cast<short>(Random::get(2, 11))};

        Deck deck{};
        deck.shuffle();
        std::cout << "The dealer has " << dealer.score << '\n';
        Player player{static_cast<short>(Random::get(2, 11) + Random::get(2, 11))};
        std::cout << "You have " << player.score << '\n';
        bool isDealerTurn{false};
        while(true)
        {
            if(!isDealerTurn){
                char playerMove{playerTurn(player,deck)};
                if(playerMove=='s'){
                    isDealerTurn=true;
                } else if(player.score>=21){
                    std::cout<<"You lost!";
                    return false;
                }
            } else{
                bool didDealerLose{dealerTurn(dealer,deck)};
                if(didDealerLose){
                    std::cout<<"You won!";
                    return true;
                }
                bool won {dealer.score < player.score};
                std::cout<<(won?"You won!":"You lost!");
                return won;
            }
        }
    }
};
int main()
{
    Blackjack::playBlackjack();
    return 0;
}