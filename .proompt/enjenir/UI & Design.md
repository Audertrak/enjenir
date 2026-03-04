# UI & Design
The core visual identity and aesthetic principles for the _\[Game Name Placeholder\]_ deckbuilding card game. The goal is to create a distinct, memorable, and thematic look and feel deeply rooted in engineering concepts, drawing inspiration from iconic engineering software interfaces, technical documentation styles (blueprints, schematics, diagrams), Bauhaus, and retro-futuristic mid-century modern aesthetics.  
typography, and aesthetic ratios/proportions.

This guide serves as a reference for all visual asset creation and UI/UX design to ensure consistency and reinforce the game's unique theme.

* * *

**2\. Core Visual Concept: Whiteboard**

The primary visual metaphor for the main gameplay UI is a “virtual whiteboard”, which in implementation is a pseudo “infinite canvas”. The imagery is meant to evoke a corporate office whiteboard, drafting table, or digital design environment upon which the game elements (cards, UI panels, indicators) are placed, arranged, and interacted with.

The key characteristics of this concept are:

*   **Spaciousness:** Elements feel like they are laid out on a large, open surface.
*   **Modularity:** UI elements (like hand, deck, discard piles, resource indicators) appear as distinct panels or areas anchored to the workbench, akin to tool palettes or docked windows in engineering software.
*   **Interaction as Annotation/Manipulation:** Playing cards, triggering effects, and managing resources should visually feel like drawing, annotating, connecting, placing, or manipulating components on this technical surface.

* * *

**3\. Visual Language & Imagery: Technical Diagrams**

All game visual assets, including card "illustrations," UI icons, and visual effects, should adhere to a technical, diagrammatic style.

*   **Style:**
    *   Clear, simple line art is paramount. Avoid heavy rendering, gradients, or textures unless specifically used to mimic paper/board texture in the background.
    *   Imagery should resemble simplified engineering drawings:
        *   Orthographic or Isometric projections of components.
        *   Circuit schematic symbols.
        *   Process flow diagram shapes and connectors.
        *   Technical graphs and charts.
        *   Mechanical assembly diagrams.
    *   Focus on conveying information and function through clear lines and shapes rather than photorealistic depiction.
    *   Use standard technical drawing conventions where appropriate (e.g., dashed lines for hidden components, specific arrow types for flow/direction).
*   **Content:**
    *   Represent game concepts (components, actions, upgrades, resources) as abstract or simplified technical representations.
    *   A "Card Draw" effect might be represented by an arrow moving from a deck symbol to a hand symbol.
    *   A "Resource Gain" might be shown as a simple counter incrementing next to a stylized resource icon (like a gear for 'material' or a lightning bolt for 'energy').
    *   An "Attack" action might be depicted as a stylized force vector or a simplified impact diagram.

* * *

**4\. Color Palette: Engineering Themes**

The UI should be designed with a minimal palette to offer a clean and simple light mode default, but be implemented in such a way to enable customizable “Themes”. Each theme defines the primary background and line colors, with accent colors for interactive elements, highlights, and important information. Some potential themes might be as follows:

*   **Theme 1: Draft (Light Mode)**
    *   **Background:** Clean White (`#FFFFFF`) - Mimics white paper or a bright whiteboard.
    *   **Primary Lines/Ink:** Stark Black (`#000000`) - Crisp technical drawing ink.
    *   **Secondary Lines/Annotations:** Dark Grey (`#333333`) or a muted Blue (`#6699CC`) - For grid lines, subtle details, or annotations.
    *   **Accent/Highlight:** A vibrant but not overly bright color (e.g., Safety Orange `#FF6600` or Cyan `#00CCFF`) - Used sparingly for selected items, interactive elements, or important notifications.
*   **Theme 2: Blueprint (Dark Mode)**
    *   **Background:** Deep Blueprint Blue (`#0A3366` or similar) - Classic blueprint paper.
    *   **Primary Lines/Ink:** Bright Silver/White (`#E0E0E0` or `#FFFFFF`) - The iconic blueprint lines.
    *   **Secondary Lines/Annotations:** A slightly darker Blue (`#052244`) or light Grey (`#A0A0A0`) - For grid lines, subtle details.
    *   **Accent/Highlight:** A contrasting, still technical color (e.g., Electric Yellow `#FFFF00` or Bright Cyan `#00FFFF`) - For interactive elements, highlights, ensuring visibility against the dark blue.
*   **Theme 3: Sketch (Subtle Mode)**
    *   **Background:** Soft Cream or Beige (`#F5F5DC` or `#F0E6CC`) - Mimics aged paper or a sketchpad.
    *   **Primary Lines/Ink:** Dark Graphite Grey (`#444444` or `#333333`) - Pencil or charcoal lines.
    *   **Secondary Lines/Annotations:** Lighter Grey (`#888888`) or a muted Brown (`#A09070`) - For grid lines, sketch details.
    *   **Accent/Highlight:** A color that pops gently against the muted background (e.g., Forest Green `#228B22` or Rust Red `#B22222`) - For interactive elements and highlights.
*   **General Color Principles:**
    *   Color use should be functional and intentional, not merely decorative.
    *   Maintain high contrast between lines/text and backgrounds within each theme for readability.
    *   Accent colors should stand out for interactive elements but not be overwhelming.
    *   Consistent use of color within a theme is crucial.

* * *

**5\. Typography: The Monospaced Standard**

To reinforce the technical, typewriter-like feel, the primary typeface will be monospaced.

*   **Primary Font:** Courier New or a similar, highly readable monospaced font (e.g., Liberation Mono, Roboto Mono, Jetbrains Mono - chosen based on final testing for readability across sizes).
*   **Usage:** This font will be used for the vast majority of in-game text, including:
    *   Card Titles (may use bold or slightly larger size)
    *   Card Effect Text
    *   UI Labels (buttons, meters, descriptions)
    *   Log/Status Messages
*   **Hierarchy:** Use font size, weight (bold), and potentially the accent color to create visual hierarchy, as variations within the Courier family or a selected alternative allow. Avoid using drastically different font _styles_.
*   **Readability:** Testing is essential to ensure the chosen monospaced font is easily readable at various sizes, especially for card effect text. Adjust line spacing or letter spacing if necessary.
*   **Alternative (Conditional):** A secondary simple, technical-looking sans-serif font _could_ be considered _only_ if absolutely necessary for specific large headings or complex UI elements where a monospaced font proves challenging for layout or legibility. However, the strong preference is to achieve hierarchy using variations of the monospaced font.

* * *

**6\. UI / Layout Principles**

The UI should reflect the "Whiteboard" concept and the functional aesthetic of engineering software.

*   **Background:** The large, flat color of the chosen theme, possibly with a toggle for a subtle grid overlay.
*   **Element Placement:** UI panels (hand, deck, discard, resources, turn indicator) should appear as distinct, clean-edged "sheets" or "tool windows" placed upon the background. They should have clear borders.
*   **Information Density:** Like engineering interfaces, the UI needs to convey a good amount of information efficiently. Use icons (designed in the technical diagram style) to supplement text.
*   **Interaction:** Dragging cards should feel like moving physical items or digital objects on the surface. Hover states and selections should use the theme's accent color for clear feedback.
*   **Minimalism:** Avoid excessive ornamentation, skeuomorphism (beyond the paper/board metaphor), or unnecessary animations. Interactions and visuals should feel precise and functional.
*   **Scalability:** The design should ideally scale well across different resolutions, maintaining clarity due to the focus on clean lines and simple shapes.

* * *

**7\. Specific Element Guidelines**

*   **Cards:**
    *   Rectangular shape in the proportion of standard copy paper (8.5 x 11) with clean, defined borders.
    *   Layout within the card should be clear: Title area (monospaced font, potentially bold), "Illustration" area (technical diagram/symbol), Effect Text area (monospaced font), Cost/Resource icons (simple, geometric shapes or technical symbols).
    *   Card borders or backgrounds can subtly change based on card type (e.g., component, action, upgrade) but should remain within the overall technical aesthetic of the chosen theme.
    *   Card Back: A simple, recognizable pattern using theme colors – perhaps a grid, a stylized gear/atom/circuit symbol, or a miniature blueprint icon.
*   **Buttons & Interactive Elements:**
    *   Clean, geometric shapes (rectangles, circles).
    *   Labels use the monospaced font.
    *   Interactive states (hover, pressed, disabled) clearly indicated using theme's accent color or changes in line weight/fill opacity.
*   **Icons:**
    *   Designed strictly in the technical diagram line-art style.
    *   Should be instantly recognizable and functionally clear (e.g., a small gear for 'material', a simplified battery for 'power', arrows for 'move' or 'draw').

* * *

**8\. Sound Design**

*   Use public domain/open source midi files or scores/annotations of medieval, renaissance, and baroque period compositions
    *   Feature pieces/work that feel highly structured (mathematical): fugues, canons, etc.
*   Use software instruments
    *   Should be easily identifiable to the source material, but distinct; perhaps imitating Wendy Carlos
*   (Future state) Algorithmically generated dynamic music generation.
*   TODO: think about sound effects

* * *

**9\. Tone and Feel**

The overall tone and feel of the game's visual design should be:

*   **Precise and Functional:** Every visual element has a purpose.
*   **Clean and Organized:** Reflecting good engineering practices.
*   **Intelligent and Strategic:** Evoking problem-solving and system building.
*   **Distinctive and Thematic:** Clearly communicating the engineering theme without being generic.
*   **Calm and Focused:** The whiteboard/blueprint background should feel like a workspace conducive to planning and execution, avoiding visual clutter.