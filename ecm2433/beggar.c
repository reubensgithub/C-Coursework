#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "riffle.h"

#define DECK_SIZE 52
#define HAND_SIZE 5
#define MAX_ROUNDS 5

/*
int get_card_value(struct Card card);
*/
/*
struct Card {
    int num; // 2-10, Jack=11, Queen=12, King=13, Ace=14
  };
*/


struct Player {
    int player_num;
    int hand_size;
    int index;
    int *hand; // this is the hand they're holding
};

/**
 * Removes and returns the first element of an array of integers.
 *
 * @param hand Pointer to the array of integers representing a hand of cards.
 * @param len Pointer to the integer representing the number of cards in the hand.
 *
 * @return The integer value of the removed card.
 */
int pop(int *hand, int *len) {
      int i;
      int first = hand[0];
      for (i = 0; i < *len-1; i++) {
          hand[i] = hand[i+1];
      }
      (*len)--;
      return first;
  }

/**
 * Plays a turn of the game for a given player, modifying the state of the game.
 *
 * @param player Pointer to the struct representing the player taking the turn.
 * @param pile Pointer to the array of integers representing the game pile.
 *
 * @return A pointer to the array of integers representing the updated game pile.
 */

int* take_turn(struct Player* player, int* pile) {
    int i;
    int top_card = pile[0];
    int* reward = NULL;
    int found_penalty_card = 0;

    // Check if player has a penalty card that matches top card
    for (i = 0; i < player->hand_size; i++) {
        if (player->hand[i] == top_card && top_card > 10) {
            // Remove penalty card from player's cards and add to pile
            pile = (int*) realloc(pile, (pile[0] + 1) * sizeof(int));
            pile[pile[0] + 1] = player->hand[i];
            for (int j = i; j < player->hand_size - 1; j++) {
                player->hand[j] = player->hand[j+1];
            }
            player->hand_size--;
            found_penalty_card = 1;
            break;
        }
    }

    // If player doesn't have a penalty card, give pile to previous player
    if (!found_penalty_card) {
        reward = (int*) malloc(top_card * sizeof(int));
        for (i = 0; i < top_card; i++) {
            reward[i] = pile[i+1];
        }
        player->hand_size += top_card;
        player->hand = (int*) realloc(player->hand, player->hand_size * sizeof(int));
        for (int i = player->hand_size - top_card; i < player->hand_size; i++) {
            player->hand[i] = reward[i - (player->hand_size - top_card)];
        }
        free(pile);
        pile = (int*) malloc(sizeof(int));
        pile[0] = 0;
    } else {
        // If player has a penalty card, add it to the top of the pile
        pile[0]++;
        pile = (int*) realloc(pile, (pile[0] + 1) * sizeof(int));
        pile[pile[0]] = player->hand[i];
        for (int j = i; j < player->hand_size - 1; j++) {
            player->hand[j] = player->hand[j+1];
        }
        player->hand_size--;
    }

    return reward;
}


/**
 * Determines the winner of the game based on the scores of each player.
 *
 * @param players Pointer to the array of structs representing the players in the game.
 * @param Nplayers Integer representing the number of players in the game.
 *
 * @return The index of the winning player in the players array.
 */
int get_winner(struct Player* players, int Nplayers) {
    int i;
    int winner = -1;
    int min_card = 15; // Initialize min_card to a value higher than any possible card
    
    for (i = 0; i < Nplayers; i++) {
        int max_card = -1; // Initialize max_card to a value lower than any possible card
        for (int j = 0; j < players[i].hand_size; j++) {
            int card = players[i].hand[j];
            if (card > max_card && card < min_card) {
                max_card = card;
            }
        }
        if (max_card > -1) {
            min_card = max_card;
            winner = i;
        }
    }
    
    return winner;
}

/**
 * Computes the size of the game pile, which is the sum of the values of all cards in the pile.
 *
 * @param pile Pointer to the array of integers representing the game pile.
 *
 * @return An integer representing the size of the game pile.
 */
int pile_size(int* pile) {
    int size = 0;
    while (pile[size] != 0) {
        size++;
    }
    return size;
}

/**
 * Determines if the game is over based on the scores of each player.
 *
 * @param players Pointer to the array of structs representing the players in the game.
 * @param Nplayers Integer representing the number of players in the game.
 *
 * @return An integer indicating whether the game is over (1) or not (0).
 */
int game_over(struct Player* players, int Nplayers) {
    int i;
    int num_nonempty_hands = 0;
    for (i = 0; i < Nplayers; i++) {
        if (players[i].hand_size > 0) {
            num_nonempty_hands++;
        }
        if (num_nonempty_hands > 1) {
            return 0;
        }
    }
    return 1;
}



/**
 * Simulates a game of Beggar-My-Neighbour with the given number of players and deck of cards.
 *
 * @param Nplayers Integer representing the number of players in the game.
 * @param deck Pointer to the array of integers representing the deck of cards to use in the game.
 * @param talkative Integer indicating whether the game should output verbose information (1) or not (0).
 *
 * @return An integer representing the index of the winning player in the players array.
 */
int beggar(int Nplayers, int *deck, int talkative) {
  int cards_per_player = DECK_SIZE / Nplayers;
  
  struct Player players[Nplayers];
  
  int i;
  int j;
  for (i = 0; i < Nplayers; i++) {
      players[i].player_num = i+1;
      players[i].hand = malloc(cards_per_player * sizeof(int));
      players[i].hand_size = cards_per_player;
  }
  // Deal cards
    int card_index = 0;
    for (i = 0; i < Nplayers; i++) {
        for (j = 0; j < cards_per_player; j++) {
            players[i].hand[j] = deck[card_index];
            card_index++;
        }
    }
  
    
    // Set first player to left of dealer
    int current_player = (Nplayers - 1) % Nplayers;
  
  // Initialize pile and rounds
    int *pile = calloc(52, sizeof(int));
    int rounds = 0;
    
    // Main game loop
    while (1) {
        // Print pile and player's hand
        if (talkative) {
            printf("Pile: ");
            for (i = 0; i < pile_size(pile); i++) {
                printf("%d ", pile[i]);
            }
            printf("\n");
            for (i = 0; i < Nplayers; i++) {
                printf("Player %d's hand: \n", i);
                for (j = 0; j < players[i].hand_size; j++) {
                    printf("%d ", players[i].hand[j]);
                }
                printf("\n");
            }
            /*
            printf("Player %d's hand: ", current_player);
            for (i = 0; i < players[current_player].hand_size; i++) {
                printf("%d ", players[current_player].hand[i]);
            }
            */
            printf("\n");
        }
        
        // Take turn and get reward
        int* reward = take_turn(&players[current_player], pile);
        
        if (reward != NULL) {
            // Add reward to player's hand
            for (i = 0; i < pile_size(pile); i++) {
                players[current_player].hand[players[current_player].hand_size] = pile[i];
                players[current_player].hand_size++;
            }
            players[current_player].hand[players[current_player].hand_size] = reward[0];
            players[current_player].hand_size++;
            free(reward);
        } else {
            // Give pile to previous player
            int prev_player = (current_player - 1 + Nplayers) % Nplayers;
            for (i = 0; i < pile_size(pile); i++) {
                players[prev_player].hand[players[prev_player].hand_size] = pile[i];
                players[prev_player].hand_size++;
            }
            pile[0] = 0;
            current_player = prev_player;
        }

        rounds++;
        printf("ROUND %d \n", rounds);

        // Check if game is over
        if (game_over(players, Nplayers)) {
            if (talkative) {
                printf("Game over after %d rounds.\n", rounds);
            }
            return get_winner(players, Nplayers);
        } else if (rounds >= MAX_ROUNDS) {
            if (talkative) {
                printf("Max rounds reached. Game over.\n");
            }
            return -1;
        }
        
        // Set next player
        current_player = (current_player + 1) % Nplayers;
    }

}
  /*
    while (is_finished == false) {
        for (i = 0; i < Nplayers; i++) {
            int popped = pop(players[i].hand, &players[i].hand_size);
            int laid;
            if (popped == 11) { // if they lay a Jack down
                for (laid = 0; laid < 1; laid++) {
                    printf("REACH\n");
                    int penalty = pop(players[(i + 1) % Nplayers].hand, &players[(i + 1) % Nplayers].hand_size);
                    printf("PENALTY IS: %d\n", penalty);
                    if (penalty == 11 || penalty == 12 || penalty == 13 || penalty == 14) {
                        printf("SWITCH PENALTY\n");
                        continue;
                    }
                    pile[pile_size] = penalty;
                    pile_size++;
                }
                int s;
                int t;
                pile_size = sizeof(pile) / sizeof(pile[0]);
                if (pile_size >= initial_size) {
                    int *new_pile = realloc(pile, 2 * initial_size * sizeof(int));
                    if (new_pile == NULL) {
                        printf("REALLOCATION FAILED!");
                    } else {
                        pile = new_pile;
                        initial_size *= 2;
                    }
                }
                for (s = players[i].hand_size, j = 0; j < pile_size; s++, j++) {
                    if (pile[j] == 0) {
                        continue;
                    }
                    players[i].hand[s].num = pile[j];
                    printf("ADDING PEN CARD %d TO PLAYER %d HAND\n", pile[j], i);
                }
                // clear the pile and set its size to 0
                memset(pile, 0, sizeof(pile));
                pile_size = 0;
            }
            if (players[i].hand_size == 0) {
                printf("GAME OVER! PLAYER %d WON!\n", i+1);
                is_finished = true;
                break;
            } else {
                pile_size = 0;
                while (pile[pile_size] != 0) {
                    pile_size++;
                }
                for (j = 1; j < pile_size; j++) {
                    pile[j-1] = pile[j];
                }

                pile[pile_size] = popped;
                pile_size++;
                printf("PILE SIZE IS %d\n", pile_size);
                for (j = 0; j < pile_size; j++) {
                    printf("%d ", pile[j]);
                }
                printf("ELEMENT ADDED: %d\n", pile[pile_size - 1]);
                if (pile_size >= initial_size) {
                    printf("REALLOCATING\n");
                    int *new_pile = realloc(pile, 2 * initial_size * sizeof(int));
                    if (new_pile == NULL) {
                        printf("REALLOCATION FAILED!\n");
                    } else {
                        pile = new_pile;
                        initial_size *= 2;
                    }
                }
            }
        }
    }
*/
  
  // Display actions
  /*
  if (talkative != 0) {
      for (i = 0; i < Nplayers; i++) {
          printf("Player %d's hand:\n", players[i].player_num);
          for (j = 0; j < players[i].hand_size; j++) {
              printf("%d ", players[i].hand[j].num);
          }
          printf("\n");
      }
  }
  
  // Free memory allocated for players' hands
  for (i = 0; i < Nplayers; i++) {
      if (players[i].hand_size == 0) {
          continue;
      } else {
      free(players[i].hand);
      }
  }
  
  return 0;
  */

int main() {
    int deck[52] = {2,3,4,5,6,7,8,9,10,11,12,13,14,2,3,4,5,6,7,8,9,10,11,12,13,14,2,3,4,5,6,7,8,9,10,11,12,13,14,2,3,4,5,6,7,8,9,10,11,12,13,14,};
    riffle(deck, 52, sizeof(int), 5);
    beggar(4, deck, 1);
    return 0;
}
  