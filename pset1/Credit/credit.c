#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // declare cardNum
    long cardNum;
    // prompt user for valid long input
    do
    {
        cardNum = get_long("What is your card number?\n");
    }
    while (cardNum < 1);
    // Steps to separate out cardNum into digits and store into array1:
    // initialise length
    int length = 0;
    // copy cardNum to getLength variable
    long getLength = cardNum;
    // start division by 10 and increment counter for each iteration of while loop to obtain number of digits i.e. length of getLength
    while (getLength != 0)
    {
        getLength /= 10;
        length++;
    }
    // initialise empty array1 with a length equal to "length" variable
    int array1[length];
    // copy cardNum to getArray1 variable
    long getArray1 = cardNum;
    // initalise array1Length
    int array1Length = 0;
    // separate out getArray1 into digits and store into array1
    // note that the digits are stored in the correct order in array1
    for (int i = length - 1; getArray1 > 0; i--)
    {
        array1[i] = getArray1 % 10;
        getArray1 /= 10;
        array1Length++;
    }
    // multiple every other digit in array1 by 2 starting from 2nd last digit of cardNum,
    // i.e. starting with array1[array1Length - 2] as digits are stored in correct order,
    // then add them together.
    // initialise array1Sum
    int array1Sum = 0;
    for (int i = array1Length - 2; i >= 0; i -= 2)
    {
        if (array1[i] * 2 < 10)
        {
            array1Sum += array1[i] * 2;
        }
        else
        {
            array1Sum += (array1[i] * 2) % 10;
            array1Sum += ((array1[i] * 2) / 10) % 10;
        }
    }
    // Now let’s add that array1Sum to the sum of the digits that weren’t multiplied by 2
    // starting from the end of cardNum i.e. end of array1:
    for (int i = array1Length - 1; i >= 0; i -= 2)
    {
        array1Sum += array1[i];
    }
    // check if credit card number is valid, if valid then print card type
    if (array1Sum % 10 == 0)
    {
        if (length == 15 && array1[0] == 3 && (array1[1] == 4 || array1[1] == 7))
        {
            printf("AMEX\n");
        }
        else if (length == 16 && array1[0] == 5 && (array1[1] == 1 || array1[1] == 2 || array1[1] == 3 || array1[1] == 4 || array1[1] == 5))
        {
            printf("MASTERCARD\n");
        }
        else if ((length == 13 || length == 16) && array1[0] == 4)
        {
            printf("VISA\n");
        }
        // else print invalid as card number is invalid
        else
        {
            printf("INVALID\n");
        }
    }
    // else print invalid as card number is invalid
    else
    {
        printf("INVALID\n");
    }
}
