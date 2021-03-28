/**
 * Handles all the information about a player, mainly handles the controller input, on boot we get the players initial position 
 * It also keeps all the inventory data and the player unique information scoped to this file
 */ 
#ifndef PLAYER_INCLUDE
#define PLAYER_INCLUDE
/**
 * Initialise the player, set the default position values, and currently init the controllers
 */
void initPlayer();

/**
 * Capture and process the player input through the controller, and pass that onto the player entity
 */
void processPlayerInput();


/**
 * Getters for debug passing to the main file
 */ 
float getPlayerX();
float getPlayerY();
float getPlayerZ();
#endif