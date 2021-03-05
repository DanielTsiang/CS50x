// Recovers JPEGs from a forensic image

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

// Number of bytes in JPEG block
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // initialise variables
    int counter = 0;
    // array of size 8 as need to store /0 also
    char filename[8] = "000.jpg";

    // Open files
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // create file with name equal to filename
    FILE *img = fopen(filename, "w");
    if (img == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Read block of 512 bytes from memory card file
    // “buffer” is a variable in which to store blocks read from the memory card file
    BYTE buffer[BLOCK_SIZE];
    // Read from memory card file, 512 bytes at a time, via a while loop, until end of memory card
    while (fread(&buffer, sizeof(BYTE), BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        // Check if start of JPEG:
        // bitwise operator (&) is used to check buffer[3], (0xf0 = 1111 0000, 0xe0 = 1110 0000, 0xef = 1110 1111)
        // using & 0xf0, the first 4 values is 1111 so using it with any hex value will set the the last 4 characters to 0,
        // i.e. use this to ignore the last 4 digits.
        // for buffer[4], it can be 0xe0 to 0xef i.e. first 4 digits will always be 1110
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            if (counter == 0)
            {
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, img);
                counter++;
            }
            else
            {
                // close previous JPEG img file so can start writing to a new JPEG file
                fclose(img);
                // update name of filename variable
                sprintf(filename, "%03i.jpg", counter);
                // create file with name equal to filename, i.e. change file pointer (called img) to point to new file
                img = fopen(filename, "w");
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, img);
                counter++;
            }
        }

        else
        {
            // if already found JPEG
            if (counter >= 1)
            {
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, img);
            }
        }
    }

    // Close files
    fclose(input);
    fclose(img);

    return 0;
}