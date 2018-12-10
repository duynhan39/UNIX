# Rock, Paper, Scissors

* Run the server by `./referee`
* Get the server id specificly for your local server (can be found by `ifconfig`), and the port ID from server's output

## Terminal
* Become a player by `./player [server] [port id]`
* Type `READY` and wait until the server reply `GO` indicating your opponent is also ready
* Pick one of the available choices, terminal will inform you of bad choice
* You will only find out about the score once it's over

## GUI
* Become a player by `./gui-player [server] [port id]`
* Before both player hit `READY` button, all other buttons are unavailable
* Once in the game, `READY` will not be available
* You can pick one of the 3 moves or `QUIT`
* It is strongly recommended **not to close the window before the `READY` stage**
* You will only find out about the score once it's over


## Have fun!
