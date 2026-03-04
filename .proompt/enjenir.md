# enjenir
“Under the hood” the core _gameplay_ concept centers around boolean logic. The goal is to enable nearly infinite complexity from a few simple base primitives. The core elements of the game should be simple enough to be explainable within 5 minutes, simple enough to be played (and enjoyed) by a 5 year old, but with enough optional complexity to be enjoyed by a PhD, and infinitely re-playable.

## Game Architecture Decisions

#### State Advancement

- [x] Turn Based

#### Opponents

- [x] PVP
- [ ] PVE

#### Player Format

- [ ] Co-Op
- [x] Free-for-All

#### Win Condition

- [ ] Infinite
- [ ] Timeout/Turn Limit (depends on state advancement)
- [x] Event
    - [ ] Score
    - [x] Goal
        - [ ] Individual
        - [x] Global
    - [ ] Resource depletion

At this time, I am _planning_ on making the game… turn based, PvP, free-for-all, first to global goal 

* * *

## Gameplay

The following descriptions about game design are technical and specific, but by the time it gets to the player (and what the UI communicates to the player), the game should convey itself in abstracted, generalized and simplified language.

### Turing Complete (core gameplay; medium priority)

From a game _design_ perspective, the core concept is making a game around the smallest set of primitives that can be used to create a turing machine, and therefore infinite complexity. In order to accomplish this, there needs to be gameplay mechanics for…

*   **Memory**
*   **Logic**
*   **Control**

As the game progresses, players will build their own turing ‘engines’ to accomplish the goal or goals necessary to win. Players will build their engines by playing cards to create boolean logic ‘circuits’ and then feed inputs to that engine via cards (potentially and/or some other action economy)

### Boolean logic (core gameplay; high priority)

The game language and terminology will refer to 

*   **Binary switch**
    *   Latching
    *   Momentary
*   **AND**
*   **OR**
*   **NOT**
*   **Clock**
    *   game clock, based on game ‘ticks’
    *   global clock, literal NTP

### Tech Tree (core gampeplay; medium priority)

The simple boolean primitives can be chained or possibly upgraded to create more complex ‘machines’. E.G. an \[n\]\* position switch, NAND, \[n\]AND gate, NOR, \[n\]OR, flip-flop, \[n\] bit adder, etc. The game doesn't have to explicitly hardcode or even enumerate the tech tree unless its needs for gameplay functionality, the beauty is that a theoretical tech tree will simply exist by nature of the primitives. There should be an in game cheat sheet for the tech tree that describes behavior

*   Engine - the partial or complete system that the player is engineering in order to win (synonyms: system. model, graph, diagram, engine, map, machine)
    *   Module - a collection of components and/or nodes that exhibits (synonyms: class, collection, assembly, pattern)
        *   Node - has at least one input and one output (synonyms: valve, function, junction, union, circuit)
            *   Component - a single input or single output object; the most basic and discreet object type

\*(\[n\] = some integer value).

### Deck Builder (core gameplay; medium priority)

Players are dealt a randomized ‘hand’ from a deck, and their hand determines what they are able to ‘play’ each turn. 

- [ ] Balatro style ‘store’ where players can buy from a randomized set of cards that resets each turn. Potentially have a dominion style ‘supply’ that is the same for all players. Potentially have a wingspan style ‘store’ with a randomized set of 5 cards available to all players. Potentially all of the above.

*   **Object Cards** physically placed to a fixed position on the play canvas (note, fixed doesn't mean that the ‘object’ can't move, just that the user explicitly plays it to a fixed location)
*   **Effect Cards**
    *   Upgrades
    *   Buff
    *   Debuff

### Base Builder (potential gameplay; low priority)

Each player could have a dedicated play area where ‘object’ cards could be played and physically ‘placed’ into that play area. There could be gameplay mechanics involving proximity, orientation, arrangment, etc. Could introduce gameplay 

### Resource Management (potential gameplay; low priority)

- [ ] Players could be competing for global resources.
- [ ] Some Resources could be finite.
- [ ] Finite could apply to either or both location and quantity.
- [ ] Certain resources could be required to… 
    - [ ] perform certain actions (play a card, upgrade an object, ‘level up’, etc)
    - [ ] accomplish certain goals (win if resource total quantity ≥ \[value\], win if resource/turn harvest rate ≥ \[value\], game ends if  \[resource\] depleted, etc)
- [ ] Player classes could have modifiers specific to different resource types. 
    - [ ] Player classes may be exclusively allowed or disallowed from collecting or using certain resources.

### Tower Defense (potential gameplay; low priority)

‘object’ cards could be ‘towers’ that are played inside of the players ‘base’, and ‘towers’ could ‘harvest resources’ that are traveling along a path; resources that make it to end without being harvest are lost. This introduces a strategy element of making the most efficient use of ‘towers’ and ‘resources’. In the context of an engineering game, this would be like towers are workers or machines that placed along an assembly line.

The more traditional option would be to have ‘non-reward’ entities travel along a path, and the ‘towers’ the user places remove the entities because if they make it to the end the player loses money, score, etc. In the context of an engineering game this might be like engineering 

### Automation (potential gameplay; low priority)

- [ ] Players might first need to **build** the path that resources or outputs travel along
- [ ] The player explicitly has to manage both ‘signal’ (low voltage, pneumatic/hydraulic pilot, ‘sensors’, etc) and ‘power’ (high voltage, psi, lb/ft, etc) (_optional_)
    *   This might make sense in the context of classes
- [ ] There is no meaningful distinction between ‘signal’ and ‘power’ (_optional_)

### Economy (core gameplay; high priority)

#### Action Economy

Players have a finite amount of ‘energy’ or ‘actions’ that can be performed in a given term. There should be gameplay mechanics that increase/decrease energy.

#### Resource Economy

See “Resource Management”

#### Currency

All players have a base salary, and there will be gameplay mechanics that increase/decrease Salary and reward/remove currency. When classes are introduced, salary is affected by class and will have different base/growth stats

### Free Market (potential gameplay; **high priority**)

This feature will not really comply with the original ‘gameplay simplicity’ directive. However, it _does_ adhere to the principle of getting the most potential gameplay possible out of a small set of primitives.

I want the literal actual chaos of a free market. _**Any**_ resource, object, and asset available to the player should be tradable. Players can offer up for trade their turns, actions, cards, money, etc. These trades could be offered via direct contracts or by creating open contracts on an exchange. While the primary gameplay loop would be turn based, the market aspect would be real time and players would be able to engage in creating/signing “infinite” contracts at any point in the game state. Contracts are the basic market mechanism that power all of the market functionality.

#### Contracts

Contracts are conditionally executed transactions. Anything can be offered for ‘sale’, anything can be offered for ‘payment’.

##### Enforceable Contracts

Players can draft a contract and take advantage of explicit hooks or events emitted by the game as conditions for forced execution of the contract. Examples of an Enforceable Contract could be selling a card for currency, trading a turn for an action, borrowing currency from another player with a repayment plan/period, offering to attack another player on the next turn, etc.

###### Promises

Note that the ‘attack another player’ example is a promise to fulfill some obligation at some point in the future. Another major example of a enforceable promise contract would giving out/receiving a loan, which also introduces the potential for debt. Anytime a contract involves a promise, there is the potential for a party to fail to uphold the terms of the contract. Failure to uphold the terms of a contract will result in enforced penalties.

##### Penalties

Some potential penalties would be ‘bankruptcy’, ‘wage garnishing’, ‘forced auction of assets’, etc. Potentially could result in automatic loss or disqualification from the round/match.

##### Unenforced Contracts

Players can draft contracts with ambiguous and arbitrary terms that the contract parties agree to uphold via “social contract”. Some examples of ‘unenforced contracts’ are offering to buy lunch for a coworker if he gives a card, offering up an in game turn in exchange for $5, etc

##### Externally Enforced Contract

In order to greater legitimize Unenforced Contracts, there could be optional conditions on the creation/signing. such as naming a judge who will determine whether or not the conditions of the contract were met. The parties would both have to agree on a judge in order for the contract to be ratified. The judge could be another player or an entirely outside 3rd party. Once a judge is nominated to officiate, a unique token is generated, and that token must be provided by the judge while viewing the contract in the game UI. In order for the judge to receive said token, they would either need to be present as a player or spectator in the game session, or the parties provide the judges contact information. If the judge is connected to the game session, they are served a dialogue that prompts them to save/open unique token where they can rule whether or not the contract was upheld. If the judge is _not_ connected to the session, they will texted/emailed the token along with a uniquely generated url that should let them view the contract ruling dialogue.

Note that if a 3rd party judge is nominated but does not rule, there will be no penalty enforcement.

##### Derivatives

Contracts can be speculative in their stipulation, including speculation on the outcome of other contracts, effectively enabling the existence of derivatives. For example, player A could offer a contract with some payout that they expect player B to go into debt. Player C disagrees, so player C agrees to the open contract. In the event the player B goes into debt, player C would be forced to deliver on their terms of the contract.

 Another example would be player A creates a contract to attack another player. Player B is confident this will not happen, so he opens a new contract speculating that player A will fail to deliver. 

Derivatives are not enumerated, and contracts are not meaningfully limited in their potential scope, allowing the creation of contract representing indexes, options, futures, etc. This could result in scenarios with arbitrarily deep and complex unwinding of contracts.

#### Exchanges

In contrast to a contract directly made between a player and one or many other players, there will be the option to make a contract ‘open’; meaning it will be listed on a ‘global’ exchange (exists always, available to all players). Through a contract, players can also create ‘private’ exchanges (conditionally allow player access), which can also be a target for ‘open’ contracts, though this would be a lower priority feature.

### Classes (potential gameplay; high priority)

Expansion, season pass, stretch goal candidate. Classes are comprised of different Engineering Disciplines. Each discipline has a quirk and/or specialty. Generally some form of passive buff, class specific cards, and perhaps class specific behavior on base cards.

#### Civil Engineer

Probably highly specialized, Not sure what the playstyle would be. Highest defense, base building and/or resource management bonuses and synergy, highest scaling

#### Mechanical Engineer

Slightly specialized. Themed around pneumatics, hydraulics, levers/pulleys, other mechanical systems. Less options for handling signaling, state management, and complex logic, but unrivaled in ‘raw power’

#### Electrical Engineer

Slightly specialized. Better at signals, state, and logic than the ME, worse than the SE. Less ‘powerful’ than the ME but number 2 and still with plenty compared to other disciplines.

#### Process Engineer

Generalist. Borrows something from all other disciplines (except the armchair engineer) but with lower base ‘stats’. Gets bonuses to efficiency/throughput, weak in PvP ("not a real engineer")

#### Software Engineer

Highly specialized. 

- [ ] Highest salary.
- [ ] Best class for handling signals (explicitly allow conditionals and/or typing for signals?) and best at complex logic. Sucks at ‘power’, maybe even has to ‘buy’ anything that actually accomplishes ‘work’.
- [ ] **No** ‘extra’ behavior on any base cards; the player has to ‘program’ them to be more meaningful/useful
    - [ ] Can write API calls to the source code
- [ ] Write literal code as condtionals for enforceable contracts
- [ ] Specifically weak to ‘AI’ opponents.

#### Chemical Engineer

I think I want this to be highly specialized, but not sure what the playstyle would be. Synthesis, catalysis… Has unique abilities oriented around combining cards in deck into new cards, breaking down complex/higher level cards into simpler base components, and use of consumable cards to trigger or empower.

#### Armchair Engineer

Generalist, excepts gets an even more significant downgrade across the board than the Process Engineer. Major bonus to enhanced PvP. Maybe has a ‘ditto’, ‘kirby’, ‘mirror’, ‘mimic’ style mechanic where the armchair engineer imitates an actual engineer, but can only do so for one discipline at a time and has to observe the behavior before mimicking/countering.

#### Train/Locomotive Engineer

Choo choo motherfucker. Gets bonuses centered around chaining inline/distance

Complete joke class, but should be playable.

### Subclasses (potential gameplay; low priority)

Expansion, season pass, stretch goal candidate. Could be a way to implement a ‘skill tree’, players could ‘level up’ their class and specialize and get even stronger bonuses, potentially with even more specific bonuses. e.g. Civil → Structural, Mechanical → Aerospace, Chemical → Petroleum, etc.

#### Sound Engineer

Expansion, season pass, stretch goal candidate. Not sure what the playstyle would be.

#### Corrugated Engineer

The ‘character’ would be named Jerry Buckley (R.I.P.). Expansion, season pass, stretch goal candidate. Not sure what the playstyle would be.

#### Mathematician 

Expansion, season pass, stretch goal candidate. Subclass available to all classes. Represents the career path of an Engineer who got burnt out. Not sure what the playstyle would be.

* * *

## Aesthetic/UI

The game should be graphically simple, which will follow the emphasis on architectural and conceptual simplicity, making programming simpler. The UI should borrow from iconic engineering software such as CAD software, as well as enterprise software and forms/documentation. Rather than rely on heavy/busy/cluttered design, the the aesthetic and UI should primarily rely on typography, pleasant ratios, and symmetry. 

#### Whiteboard

The game basically takes place on a large “infinite" canvas, sort of like a whiteboard. 

#### Drawings/Blueprints/Schematics

The ‘rendering’, ‘drawing’, and ‘design language’ should resemble clean, neat, and simple engineering documents such as engineering blueprints, process diagrams, schematics, etc. 

Could offer different ‘themes’ based around these diagram types and disciplines (light mode = white background, clean and straight black ‘inked’ lines; dark mode = blue background, silver ‘inked’ lines; muted or subtle = cream/beige background with graphite lines… etc)