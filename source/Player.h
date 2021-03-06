//=======================================================================================
//= The player controller, since there is only one player class this will be treated    = 
//= as a kind of container for the player, however if the projects is ever expanded to  =
//= include enemies which it most likely will not, then this wil probably need gutting  =
//=======================================================================================

//The player x y z coordinates don't want to be accessable outside of player.c so they 
//won't be declared in the header, since they don't need to be externals.

//=======================================================================================
//= Function declarations                                                               =
//=======================================================================================

void initPlayer();

void processPlayerInput();


///just some getters
float getPlayerX();
float getPlayerY();
float getPlayerZ();