# Core Concepts, Terms, and Definitions
## Core Concepts

Build and optimize **systems** to perform some **transformations** through strategic combinations of **components, nodes,** and **modules** via **signals** (called **elements**) over **links** to achieve some level of **capability**.

### Component

A component is the simplest **element** type. A component either produces some signal **output** _or_ accepts some signal **input**. 

##### Meta

Components represent **points** on the **edge** of a system. A component that produces some output is considered an **origin point**. Origin points cannot perform any transformation, but no transformation can occur without at least one origin. A component that accepts some input is considered a **terminal point**. Terminal points perform a single transformation, but cannot do so unless connected (directly or indirectly) to an origin. The collection of all origin points represents the **global origin** of the engine, and the collection of all terminal components represents the **global terminal** of the engine. The complete enumeration of all discrete possible **states** at the global terminal represents the **capability** of the system, or in other words, the range of transformations that can be achieved by the system. The ratio of possible states at the global terminal to the number of points on the global origin represents a system's **efficiency**.

*   A given origin point state
    *   Represents the condition or status of a single origin component at a specific point in the system's operation (e.g., active/inactive, high/low, true/false).
    *   An origin state is a binary value
*   A given global origin state (number of origins)
    *   Defines the simultaneous condition of all origin components within the system at a point in time.
    *   This is a vector of states, where each element corresponds to the state of an individual origin.
    *   The number of possible global origin states = the number of points at the origin
*   All unique global origin states
    *   (2 \* number of  links to the global origin?)
    *   Conditions - all possible global origin states (2 \* number of origins?)
*   a given terminal state (binary)
    *   a given global terminal state (number of terminals)
        *   all unique global terminal states (2 \* number of links to the global termina?)
            *   all possible global terminal states (2 \* number of terminals?)
*   a given origin state and its corresponding terminal state(s)
    *   all unique input → output combinations
        *   all possible unique input → output combinations

### Link

A link is a what allows signals to be sent between different elements.

##### Meta

Links are **polar**; it is not possible to connect an input←→input or output←→output.

### Signal

Signals represent the work being accomplished by the

##### Meta

### Node

A node represents a discrete element with both some input and some output.

##### Meta

### Modules

A module represents any 2 or more linked nodes. 

##### Meta

Conceptually, a module can be considered a node. This means it would be a possible to have a module, composed of modules, which when considered at a macro level is also a node. 

### System

All connected elements

##### Meta

Component

Definition

A component is the simplest element type within a system. A component either produces some signal output or accepts some signal input. By itself, a component can perform no work on its own, and instead must be connected to another element via a link.

Meta

Components represent the functional boundary points of a given system. A component that produces an output is considered an origin point, while a component that accepts an input is considered a terminal point. The collective set of all origin components constitutes the global origin of the system, and the collective set of all terminal components forms the global terminal of the system. The complete enumeration of all discrete possible states at the global terminal defines the total capability or range of transformations achievable by the system.

\*   \*\*A given origin state (binary):\*\* Represents the condition or status of a single origin component at a specific point in the system's operation (e.g., active/inactive, high/low, true/false).  
\*   \*\*A given global origin state (number of origins):\*\* Defines the simultaneous condition of all origin components within the system. This is a vector of states, where each element corresponds to the state of an individual origin.  
\*   \*\*All unique global origin states (2<sup>number of links to the global origin</sup>):\*\* Represents the complete set of all possible initial conditions that can be applied to the system. For a system with \*n\* independent origin points each capable of two states, there are \\(2^n\\) unique global origin states. (Assuming each origin point has a single outgoing link).  
\*   \*\*All possible global origin states (2<sup>number of origins</sup>):\*\* This is the same as "All unique global origin states" if we assume each origin can independently adopt either binary state.

\*   \*\*A given terminal state (binary):\*\* Represents the condition or status of a single terminal component at a specific point in the system's operation, resulting from the propagation of signals through the system.  
\*   \*\*A given global terminal state (number of terminals):\*\* Defines the simultaneous condition of all terminal components within the system. This is a vector of states, representing the overall output of the system for a given global origin state.  
\*   \*\*All unique global terminal states (2<sup>number of links to the global terminal</sup>):\*\* The set of distinct output configurations the system can produce. (Assuming each terminal point has a single incoming link).  
\*   \*\*All possible global terminal states (2<sup>number of terminals</sup>):\*\* This is the same as "All unique global terminal states" if we consider the theoretical maximum number of output configurations for \*m\* terminal points, each with two possible states (\\(2^m\\)). However, the actual achievable global terminal states are limited by the system's internal structure and the global origin states.

\*   \*\*A given origin state and its corresponding terminal state(s):\*\* Describes a specific input-output relationship for a single path or subset of the system.  
\*   \*\*All unique input → output combinations:\*\* The set of all distinct global origin states and the resulting global terminal states the system is capable of producing. This mapping defines the system's functional behavior.  
\*   \*\*All possible unique input → output combinations:\*\* The theoretical maximum number of input-output mappings possible for a system with a given number of origins and terminals (\\(2^{origins} \\times 2^{terminals}\\)), but constrained in reality by the actual system architecture.

Link

Definition

A link is the conductive medium that allows signals to be propagated between different elements within a system.

Meta

Links establish the connectivity and define the flow of signals. They are polar; a link must always connect an element's output to another element's input. It is not permissible to connect an input to an input or an output to an output. Links define the architecture and pathways within a system.

Signal

Definition

Signals are the abstract carrier of information and represent the state or condition being propagated through the system via links. They embody the "work" being accomplished by the system.

Meta

Signals possess a binary nature (e.g., on/off, active/inactive). Their propagation through nodes and modules transforms their state according to the logic defined by those elements. The state of a signal at any point in the system is a direct consequence of the signal's origin state and the transformations it has undergone. Signals are the dynamic aspect of the system, representing the flow of information and the execution of the designed logic.

Node

Definition

A node represents a discrete processing element within a system that accepts one or more signal inputs and produces one or more signal outputs. Nodes perform logical operations or transformations on incoming signals before propagating them.

Meta

Nodes are the functional building blocks of a system, implementing specific logic. They interpret incoming signals and determine the state of outgoing signals based on their defined operation (e.g., AND, OR, NOT, Latching, Momentary). The complexity of a system is derived from the arrangement and types of nodes and how they are interconnected by links.

Modules

Definition

A module represents a collection of two or more linked nodes that together exhibit a specific, potentially more complex, behavior or function than the individual nodes.

Meta

Modules are effectively encapsulated sub-systems. Conceptually, a module can be treated as a single, higher-order node with defined inputs and outputs, regardless of its internal complexity. This allows for hierarchical design and abstraction; a module can be composed of other modules, enabling arbitrarily deep levels of system complexity while maintaining a manageable representation at each level of abstraction. Modules facilitate the creation of reusable patterns and represent accumulated functionality.

System

Definition

A system encompasses all connected elements – components, nodes, and modules – working in concert to perform a specific transformation or achieve a defined outcome based on input signals.

Meta

The system is the complete engineered construct designed by the player. It is the sum of all interconnected elements and their relationships. The performance and capability of the system are determined by the design of its internal architecture, the types of elements used, and the interaction of signals propagating through its links. The ultimate objective is to engineer a system capable of reliably and efficiently producing the required terminal states for a given set of origin states, thereby fulfilling the specified requirement or goal.