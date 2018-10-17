Simple Nybble Substitution

This is a simple program that reads in a binary message, divides the bytes into nybbles, substitutes the nybbles according to a user provided key file of 8 bits in length, and then combines the nybbles back into bytes and outputs an encrypted message. Messages can then be decrypted using the program by providing the binary file containing the encrypted message and the same key used to encrypt the message. 

Usage:

The following arguments can be used:
-e for encrypt (default)
-d for decrypt
-b if the keyfile is binary (default)
-x if the keyfile is ASCII-coded hexidecimal
-k <filename> to specify the file containing the key

Command Format:
./nsub -[e/d] -[b/x] -k keyfile < messagefile

Known Bugs: 
Currently there are no known bugs if the program is used correctly. 

Troubleshooting:

If the program has unexpected behavior or crashes, check that the command was formatted correctly such that there are no parameters after "-k" before the keyfile and that there is a space between "-k" and the keyfile. Also confirm that the keyfile and the file containing the message to encrypt are located in the same directory as the program. If decrypting, check that the same key that was used to encrypt is being used to decrypt. Confirm that the proper parameters were specified if using an ASCII-coded hexidecimal key.

If none of the above is effective, please report a bug. 
