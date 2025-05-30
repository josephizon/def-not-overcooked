# Definitely Not Overcooked
A game created with a similar gameplay loop as Overcooked, made in partial fulfillment of the GDEV42 Course. This game was made with C++ and the Raylib Library. 

## How To Run Locally (_playable via [itch.io](https://josephizon.itch.io/notovercooked) too_)

1. Run the wb64devkit and locate the folder containing raylib and other game scripts and assets using:<br>
- `cd ../[folder_name]`

2. Run the following command in the compiler to compile the game:<br>
- `g++ Main.cpp -o out -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm`

3. Run the game via either of the following commands:<br>
- `./out` or `out`


## Controls
**Player1**<br>
| **_KeyPress_** | **_Action_**        |
|:--------------:|:-------------------:|
| WASD           | Movement            |
| E              | Pick Up/Drop Item   |
| F              | Interact/Spawn Item |
| Space Bar      | Dash/Dodge          |

**Player2**<br>
| **_KeyPress_** | **_Action_**        |
|----------------|---------------------|
| UHJK           | Movement            |
| I              | Pick Up/Drop Item   |
| L              | Interact/Spawn Item |
| Dot            | Dash/Dodge          |

## Main Goal
1. Gather components by spawning them from certain chests on the map.
2. Place all three components onto the crafting station and hold the interact button until the new component is produced.
3. Recipes are based on the given orders on the upper left of the screen.
4. Make sure to place all components in the crafting stations in order to build the robot.
5. Deliver the freshly made robot into the delivery station by dropping it there.
6. Rack as many points as you can!

### Improvements from First Game

***StateMachines***<br>
Implemented StateMachines for all Entities: Players, Enemies, Stations, Scoring Systems, and Items. Player Actions now cause the Player 
to swap between different states, making transitions from certain states to another, more defined and clear.

***Tile Sheet***<br>
Map is now created through a tilesheet used to create multiple tilemaps stored into individual text files for ease of access and switching. Old Map was only one full background with manually placed colliders.

### New Game Features:

***Simple Scene Management***<br>
The Game involves a simple enum based scene manager to switch between scenes to be drawn. The different levels are loaded in with one function just replacing the parameters to create the 3 different levels.

***Data Storage***<br>
This is shown in the form of a leaderboard system. At the end of a level the Achieved score will be stored in a leaderboard.txt
if it is higher than one of the top 5 scores for that specific level. These can be viewed anytime from the Level Selector.

***Bezier Curve***<br>
Using Bezier Curve to create a conveyor belt that the players can ride to go across tiles that can't be traversed normally.

***Enemy with Detection***<br>
Added enemies on the final level with detection and aggro that will cause the enemy to keep chasing the player or attempt to hit the player 
which causes them to drop the player's held item.

***Tracking Camera***<br>
A camera that zooms in on one player and tracks them as they move through the stage

***Dash/Dodge Mechanic***<br>
Dashing/Dodging allows for players to avoid enemies as they attempt to hit the player. However, 
there is a 20% chance for the player to drop the held item. 
