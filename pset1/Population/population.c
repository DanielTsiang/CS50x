#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // prompt for start size, valid if start size > 9
    int startSize;
    do
    {
        startSize = get_int("Start size: ");
    }
    while (startSize < 9);

    // prompt for end size, valid if end size > start size
    int endSize;
    do
    {
        endSize = get_int("End size: ");
    }
    while (endSize < startSize);

    // calculate number of years until we reach threshold
    int years;
    for (years = 0; startSize < endSize; years++)
    {
        startSize = startSize + startSize / 3 - startSize / 4;
    }

    // print number of years
    printf("Years: %i\n", years);
}
