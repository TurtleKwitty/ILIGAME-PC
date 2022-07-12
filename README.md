# ILIGAME

A "fantasy console" that exists.

## Goals

The goal of ILIGAME is to be both a fantasy console and a physical small form factor portable game console reminiscent of the game gear et al. To that end the project is seperated in three parts: [the engine][1], [the implementation for the physical console][2], and the [PC implementation (this repo)][3].

The aim of the project is to learn more about the physical side of technology while having fun developing games in the cozy niche of fantasy consoles.

[1]: https://github.com/TurtleKwitty/ILIGAME
[2]: https://github.com/TurtleKwitty/ILIGAME-PHYSICAL
[3]: https://github.com/TurtleKwitty/ILIGAME-PC

## Overview

The ILIGAME will feature a 240x160 screen in a paletted 16 colors where the palette may be changed for each frame draw, a button layout similar to an snes controller along with touchscreen/mouse and keyboard.

An ILIGAME will be compiled to custom bytecode so that it's runtime may be easily ported to the platforms wished to be supported, but the system will support making ILIGAMEs as part of the software on any platform supported.

The ILIGAME VM will have access to spritesheet map and sound data and well as it's own code and memory. The spritesheet map and sound will each possess 8kB within the memory with very flexible custom data access depending on how the game designer wishes to split the memory, more on that in their own sections.

## License

No license is granted at this time, I wish to see how the project develops/if there is interest in the project at all before deciding on licensing the project.
