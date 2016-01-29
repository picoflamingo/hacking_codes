DumBotNet. Minimalistic Botnet (master/slave distributed system)
----------------------------------------------------------------
DumBotNet is minimalistic implementation of a BotNet intended for educational purposes.

It can be also used for remote administration of machines, or remote monitoring on networks

Requires NetKitty. Netkitty is used as basic Master server for demonstrations purposes

USAGE
------------------------------
In the master machine launch NetKitty in server mode. Default port is 9999

$ nk -s T,9999

Then on each remote machine launch the bot application, passing as parameter the name you want to give to the slave/bot

Example:

$ ./bot Bot1

Bots are targeted by their respectives names. Alternative all bots can be commanded at once using the identifier ALL.
The command syntax is:

BotName|All:cmd

Examples:

all:uname -a

Bot1:ls /etc

These commands are entered in the master application.


