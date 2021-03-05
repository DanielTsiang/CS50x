#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// declare functions at the top that will be used later in main
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int get_index(int letters, int words, int sentences);

int main(void)
{
    // prompt user for text and store in a string variable called text
    string text = get_string("Text: ");

    // use custom-built functions to obtain number of letters, words and sentences
    int letters = count_letters(text);
    int words = count_words(text) + 1;
    int sentences = count_sentences(text);

    // use custom-built function to obtain reading grade and print grade
    int index = get_index(letters, words, sentences);
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// function to count number of letters in text
int count_letters(string text)
{
    int counter = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (islower(text[i]) || isupper(text[i]))
        {
            counter++;
        }
    }
    return counter;
}

// function to count number of words in text
int count_words(string text)
{
    int counter = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isspace(text[i]))
        {
            counter++;
        }
    }
    return counter;
}

// function to count number of sentences in text
int count_sentences(string text)
{
    int counter = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            counter++;
        }
    }
    return counter;
}

// function to get reading level of text
// Coleman-Liau index = 0.0588 * L - 0.296 * S - 15.8
int get_index(int letters, int words, int sentences)
{
    float L = (float) letters / (float) words * 100;
    float S = (float) sentences / (float) words * 100;
    float index = round(0.0588 * L - 0.296 * S - 15.8);
    return index;
}