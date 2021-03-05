#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("height: ");
    }
    while (height < 1 || height > 8);
    // loop for rows
    for (int i = 0; i < height; i++)
    {
        // loop for leading spaces
        for (int j = i + 1; j < height; j++)
        {
            printf(" ");
        }

        // loop for LHS hashes
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }

        // loop for middle gap
        for (int l = 0; l < 2; l++)
        {
            printf(" ");
        }

        // loop for RHS hashes
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }
        // start new row for next iteration
        printf("\n");
    }
}