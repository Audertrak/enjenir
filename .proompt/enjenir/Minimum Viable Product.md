# Minimum Viable Product
## Overview

A text heavy, minimal UI, turn based ‘card’ game where the player takes turns playing hands form a randomized deck. 

*   Gameplay UI
    *   Header: displays current scenario/game state
    *   Play Area: grid to which object cards are played. Displays the current state of a players ‘circuit’
    *   Deck: displays current hand, remaining cards in deck, and number of cards in discard pile
*   There is a scenario posed to the players that they must solve by using cards from their deck.
*   The default deck consists of a mix of ‘object', ‘action’, and ‘effect’ cards.
    *   Object cards are persistent once played.
    *   Action and effect cards are exhausted once played and move to the discard pile (unless otherwise specified)
    *   If players exhaust the full deck, their discarded deck is shuffled and becomes their new deck, Dominion style.
*   Circuits are theoretical, they do not require ‘power’
*   The player is provided a ‘global’ and ‘infinite’ bus for ‘source’ (always true) and ‘sink’ (always false)
*   When a circuit is placed, default behavior is to connect all inputs to global sink (off, or false)
*   Objects placed ‘adjacent’ to one another may be connected without the need for additional ‘wiring’
*   Wiring to adjacent objects and global source/sink may be changed freely at any time
*   The first player to successfully play the required ‘solution’ wins.

## Scenario

#### Minimum Viable Product

A set of ‘state’ conditions is defined (effectively a finite state machine) and players must construct a circuit capable of producing the target states and manipulate the objects into said states

##### Test FSM

*   1 momentary switch
*   1 latching switch
*   1 AND
*   1 OR

## Cards

*   Default hand size: 5 cards
*   Max hand size: 10 cards (at one time)
*   Deck size ~52 cards

### Objects

#### Circuits

*   Momentary Switch (when played, user selects either ‘ON’ or ‘OFF’ default condition; outputs default condition unless triggered, returns to default condition after trigger)
*   Latching Switch (when played, user selects either ‘ON’ or ‘OFF’ default condition; outputs default condition unless triggered by timer or action; once triggered, output is ‘toggled’ to new state)
*   AND (when played, user selects either ‘ON’ or ‘OFF’ default condition)
*   OR (when played, user selects either ‘ON’ or ‘OFF’ default condition)
*   Wire/trace/path (connect an available input/output of a circuit to an available input/output; ‘infinite’ length)
*   Bus (takes a single input and repeats it across 8 outputs)

### Actions

*   Deviation (select on object; on this turn, you can specify any possible output state for that object)
*   Change Request (select on object; specify an alternate circuit; until the next turn, circuit is changed to selected object)
*   Change Order (select an object; specify an alternate circuit; object is converted permanently to new circuit type and Change Order is removed from play)
*   Work Instruction (select an object; object is removed from play and may be replayed somewhere else on the board)
*   Standard Operating Procedure (reassign the ‘default’ condition of a circuit or set timer increments)
*   Kaizen (May change the default condition of all programmable circuits in play but circuits are disabled until the start of the players next turn)
*   5S (May change the arrangement of all circuits in play; circuits are disabled until the next turn)
*   Blueprint: When played, the user selects 2 adjacent objects currently in play, and the player can play a copy; blueprint resets and is discarded
*   Schematic (Similar to blueprint. The card starts ‘empty’ until first played; user selects any 1 or 2 adjacent components, and the schematic copies the configuration, after which the schematic is discarded. When next drawn, the user can either choose to play the current schematic, or choose two new objects to copy)
*   Parts Bin (Select an object currently in play; user may play a copy of selected object and Parts bin is discarded)
*   Rev-Up (Add an input or output to the selected circuit)

#### Deck Building/Card Draw