#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // declare dollars
    float dollars;
    // prompt user for valid dollars input
    do
    {
        dollars = get_float("How much change in dollars do you need?\n");
    }
    while (dollars < 0);
    // convert cents to dollars and round to avoid floating-point imprecision
    int cents = round(dollars * 100);
    int counter = 0;
    // start substraction using next highest coin and increment counter for each iteration of while loop
    while (cents >= 25)
    {
        cents -= 25;
        counter++;
    }
    while (cents >= 10)
    {
        cents -= 10;
        counter++;
    }
    while (cents >= 5)
    {
        cents -= 5;
        counter++;
    }
    while (cents >= 5)
    {
        cents -= 5;
        counter++;
    }
    // while loop will stop once cents = 1 - 1 = 0 is reached
    while (cents > 0)
    {
        cents -= 1;
        counter++;
    }
    printf("%i\n", counter);
}
