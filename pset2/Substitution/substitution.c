#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    SUCCESS,
    BAD_ARG,
} return_code;

return_code check_key_length(int argc, string argv[]);
return_code check_letter(int argc, string argv[]);
return_code encrypt_text(int argc, string argv[], string text);

int main(int argc, string argv[])
{
    if (check_key_length(argc, argv) == BAD_ARG)
    {
        return BAD_ARG;
    }
    if (check_letter(argc, argv) == BAD_ARG)
    {
        return BAD_ARG;
    }
    // run this following code if valid command-line arguments inputted:
    // prompt user for plain text
    string text = get_string("plaintext: ");

    // the following function prints the encrypted text:
    encrypt_text(argc, argv, text);

    // script ran successfully, so return SUCCESS
    return SUCCESS;
}

// check if there is a single string for key, if not return BAD_ARG and end program
return_code check_key_length(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s key\nwhere there should only be a single key.\n", argv[0]);
        return BAD_ARG;
    }
    else
    {
        return SUCCESS;
    }
}

// check for each char of the key if it is a letter, and if length of key is 26,
// and check for duplicate characters.
// If invalid key then return BAD_ARG and end program.
return_code check_letter(int argc, string argv[])
{
    // variable to check at the end of main for loop if there are duplicate characters
    int counterCheck = 0;

    int n = strlen(argv[1]);
    // return error and end program if key length is not 26
    if (n != 26)
    {
        printf("Usage: %s key\nwhere the number of characters in key should be 26.\n", argv[0]);
        return BAD_ARG;
    }

    for (int i = 0; i < n; i++)
    {
        // return error and end program if non-digit char is detected in key
        if (!isalpha(argv[1][i]) && argv[1][i] != ' ')
        {
            printf("Usage: %s key\nwhere each character in key is a letter.\n", argv[0]);
            return BAD_ARG;
        }

        // check for duplicate characters with inner for loop, regardless of letter case
        int counter = 1;
        for (int j = i + 1; j < n; j++)
        {
            if (tolower(argv[1][i]) == tolower(argv[1][j]))
            {
                counter++;
                // set argv[1][j] = ' ' to avoid printing duplicate character many times
                argv[1][j] = ' ';
            }
        }
        // for i_th character, if counter > 1 then print duplicate character and its counter
        if (counter > 1 && argv[1][i] != ' ')
        {
            printf("Error - duplicate character \"%c\" detected \"%i\" times.\n", argv[1][i], counter);
            counterCheck = counter;
        }
    }
    // if there are duplicate variables then return BAD_ARG and end program.
    if (counterCheck > 1)
    {
        return BAD_ARG;
    }
    else
    {
        return SUCCESS;
    }
}

// function to encrypt text
return_code encrypt_text(int argc, string argv[], string text)
{
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
            // encryption for upper case characters
            // 'A' has ascii code of 65
            if (isupper(text[i]))
            {
                // shift text character by 65 so alphabet is between 0-25
                // then B would be 1, then look up element in index 1 in key
                // use toupper() to maintain original case regardless of key
                char upperChar = toupper(argv[1][(text[i] - 65)]);
                printf("%c", upperChar);
            }

            // encryption for lower case characters
            // 'a' has ascii code of 97
            if (islower(text[i]))
            {
                // shift text character by 97 so alphabet is between 0-25
                // then b would be 1, then look up element in index 1 in key
                // use tolower() to maintain original case regardless of key
                char lowerChar = tolower(argv[1][(text[i] - 97)]);
                printf("%c", lowerChar);
            }
        }
    }
    // print new line after printing all of the ciphertext
    printf("\n");
    return SUCCESS;
}