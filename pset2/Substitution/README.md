# Specification

Design and implement a program, substitution, that encrypts messages using a substitution cipher.

* Implement your program in a file called substitution.c in a directory called substitution.
* Your program must accept a single command-line argument, the key to use for the substitution. The key itself should be case-insensitive, so whether any character in the key is uppercase or lowercase should not affect the behavior of your program.
* If your program is executed without any command-line arguments or with more than one command-line argument, your program should print an error message of your choice (with printf) and return from main a value of 1 (which tends to signify an error) immediately.
* If the key is invalid (as by not containing 26 characters, containing any character that is not an alphabetic character, or not containing each letter exactly once), your program should print an error message of your choice (with printf) and return from main a value of 1 immediately.
* Your program must output ```plaintext:``` (without a newline) and then prompt the user for a string of plaintext (using get_string).
* Your program must output ```ciphertext:``` (without a newline) followed by the plaintext’s corresponding ciphertext, with each alphabetical character in the plaintext substituted for the corresponding character in the ciphertext; non-alphabetical characters should be outputted unchanged.
* Your program must preserve case: capitalized letters must remain capitalized letters; lowercase letters must remain lowercase letters.
* After outputting ciphertext, you should print a newline. Your program should then exit by returning 0 from main.