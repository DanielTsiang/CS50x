#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // check is key has valid length of 1, if not return 1 and end program
    if (argc != 2)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }

    // check for each char of the key if it is a digit
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            // return error and end program if non-digit char is detected in key
            printf("Usage: %s key\n", argv[0]);
            return 1;
        }
    }

    // run this following code if all char in key are digits:
    // prompt user for plain text
    string text = get_string("plaintext: ");

    // convert command-line argument key from string to integer
    int key = atoi(argv[1]);

    // the following code prints the encrypted text:
    printf("ciphertext: ");
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // check if the char is alphabetical char
        // not an alphabetical char so no encryption performed on char
        if (!isalpha(text[i]))
        {
            printf("%c", text[i]);
        }
        else
        {
            // caesar's algorithm for rotating each character by k positions:
            // c_i = (p_i + k) % 26 but p_i has to be between 0-25

            // encryption for upper case characters
            if (isupper(text[i]))
            {
                char upperChar = (((text[i] - 65) + key) % 26) + 65;
                printf("%c", upperChar);
            }

            // encryption for lower case characters
            if (islower(text[i]))
            {
                char lowerChar = (((text[i] - 97) + key) % 26) + 97;
                printf("%c", lowerChar);
            }
        }
    }

    // print new line after printing all of the ciphertext
    printf("\n");

    // script ran successfully, so return 0
    return 0;
}