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
return_code decrypt_text(int argc, string argv[], string text);
char get_upperChar(int argc, string argv[], string text, int i);
char get_lowerChar(int argc, string argv[], string text, int i);

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
    string text = get_string("ciphertext: ");

    // the following function prints the decrypted text:
    decrypt_text(argc, argv, text);

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

// function to decrypt text
return_code decrypt_text(int argc, string argv[], string text)
{
    printf("plaintext: ");
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // check if the char is alphabetical char
        // not an alphabetical char so no decryption performed on char
        if (!isalpha(text[i]))
        {
            printf("%c", text[i]);
        }
        else
        {
            // decryption for upper case characters
            // 'A' has ascii code of 65
            if (isupper(text[i]))
            {
                // function to obtain the upper case character
                char upperChar = get_upperChar(argc, argv, text, i);
                printf("%c", upperChar);
            }

            // decryption for lower case characters
            // 'a' has ascii code of 97
            if (islower(text[i]))
            {
                // function to obtain the lower case character
                char lowerChar = get_lowerChar(argc, argv, text, i);
                printf("%c", lowerChar);
            }
        }
    }
    // print new line after printing all of the plaintext
    printf("\n");
    return SUCCESS;
}

char get_upperChar(int argc, string argv[], string text, int i)
{
    // shift text character by 65 as upper case alphabet was shifted to between 0-25 during substitution encryption
    // use toupper() and tolower() to perform case-insensitive search of index of character in key
    // strchr returns the pointer to the first occurrence, so to find the index, just take the offset with the starting pointer.
    // A variable that stores an address is called a pointer, which we can think of as a value that “points” to a location in memory.
    // We can use the * operator (in an unfortunately confusing way) to declare a variable that we want to be a pointer,
    // so position_pointer on line 156 onwards is a pointer that has the value of the address of variable text[i].
    char *position_pointer = strchr(argv[1], text[i]);
    if (position_pointer == NULL)
    {
        position_pointer = strchr(argv[1], tolower(text[i]));
    }
    // the string "argv[1]" is actually just a pointer with the address of the first character
    int position = position_pointer - argv[1];
    char upperChar = position + 65;
    return upperChar;
}

char get_lowerChar(int argc, string argv[], string text, int i)
{
    // shift text character by 97 as lower case alphabet was shifted to between 0-25 during substitution encryption
    // use toupper() and tolower() to perform case-insensitive search of index of character in key
    // strchr returns the pointer to the first occurrence, so to find the index, just take the offset with the starting pointer
    char *position_pointer = strchr(argv[1], text[i]);
    if (position_pointer == NULL)
    {
        position_pointer = strchr(argv[1], toupper(text[i]));
    }
    int position = position_pointer - argv[1];
    char lowerChar = position + 97;
    return lowerChar;
}