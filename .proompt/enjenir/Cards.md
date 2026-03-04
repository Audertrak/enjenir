# Cards
*   No input : Single Output
    *   Source
    *   Sink/Ground
    *   Timer
*   Single Input : Single Output
    *   Momentary Switch
    *   Latching
*   Multiple Input : Single Output
    *   AND
    *   OR
*   Single Input : Multiple Output
    *   Bus

## Cards

*   Default hand size: 5 cards
*   Max hand size: 10 cards (at one time)
*   Base deck depends on scenario

### Objects

#### Inputs

*   Always On (no input, single output, always on)
*   Always Off (no input, single output, always off)
*   Timer (outputs ‘TRUE’ once per tick by default; each timer can have 8 outputs; each output is ‘programmable’, e.g. number of ticks between pulses,)
*   Operator ('tape'; is fed ‘instructions’ which it then executes in a loop)

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
*   Work Instruction (select on object; on this turn, you can specify any possible output for that object)
*   Standard Operating Procedure (reassign the ‘default’ condition of a circuit or set timer increments)
*   Kaizen (May change the default condition of all programmable circuits in play but circuits are disabled until the start of the players next turn)
*   5S (May change the arrangement of all circuits in play; circuits are disabled until the next turn)
*   Blueprint (When played, the user selects 2 adjacent objects currently in play, and the player can play a copy; blueprint resets and is discarded)
*   Schematic (Similar to blueprint. The card starts ‘empty’ until first played; user selects any 1 or 2 adjacent components, and the schematic copies the configuration, after which the schematic is discarded. When next drawn, the user can either choose to play the current schematic, or choose two new objects to copy)
*   Corrective Maintenance (Fix a circuit that has been attacked; circuit will be in play at turn end)
*   Preventative Maintenance (Prevent attack on all circuits until your next hand; does not apply on already damaged circuit)
*   Rev-Up (Add an input or output to the selected circuit)

#### Attacks

*   Corporate Espionage (View another player's circuit and select an object; on that players next turn, the target object is deactivated)
*   Stuxnet (View another player's circuit and select an object; the target object is discarded)
*   Poach Talent (View another players hand; if a card is selected, card is permanently stolen from target player, placed into discard and poach talent is permanently removed from play)
*   Attrition (Select a player; 2 cards are randomly discarded from their hand)
*   Black Monday (All players, including the current player, must discard down to 3 cards; current players circuit deactivated until next turn)
*   Daylight Savings (until the players next turn, all other players clock signals are ‘off by 1’)
*   Market Research (view another players circuit; optionally copy a circuit in play)

#### Deck Building/Card Draw

*   Re-Org (discard hand; draw to full hand)
*   Requisition (draw cards)
*   Reverse Engineering (select an opponent and view a card from their hand; Reverse Engineering is permanently removed from play and copy of selected card is added to discard)
*   Job Fair ('card pack'; optionally pick one of 3 randomized cards to be permanently added to deck, Job Fair is permanently removed from the deck)