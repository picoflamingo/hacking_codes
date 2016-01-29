PoCrypt. Proof Of Concept Crypter
-------------------------------------

This repository contains the code for a simple Proof of Code to demonstrate how
to crypt parts of a binary. The modified binary is extended with a small function 
that decrypts the secured parts of the file at run-time to enable its execution.

Still under investigation, but it looks like this works just like this thanks to
the naive XOR encoding. A more secure cryptographic algorithm won't work... If anybody
tries, please let me know.


HOW TO TRY IT
--------------------
Compile with 'make'

Try to run the target binary. it will crash as it has not been crypted. The application
tried to decrypt a code that has not been encrypted. The application will dump the content
of the memory in the .secure segment

Then encrypt the binary with a command like this

./pocrypter PASSWORD ./target


Now you can run again the target binary and verify that the secure function gets executed

./target PASSWORD

MORE INFORMATION
-------------------------------
A detailed explanation on how the application works can be found here:

http://www.papermint-designs.com/dmo-blog/2016-01-pocrypt-a-proof-of-concept-for-dynamically-decrypt-linux-binaries

TODO
--------------------
Add a flag to know if the binary is encoded or no and avoid the crash on non-encoded binaries

