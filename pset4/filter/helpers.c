#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get the original values of each color in the image,
            // so can use original values without them changing between calculating colors.
            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;
            int red = image[i][j].rgbtRed;

            // set sepia value for blue pixel, using original colors
            int sepiaBlue = round(0.131 * blue + 0.534 * green + 0.272 * red);
            image[i][j].rgbtBlue = (sepiaBlue > 255) ? 255 : sepiaBlue;

            // set sepia value for green pixel, using original colors
            int sepiaGreen = round(0.168 * blue + 0.686 * green + 0.349 * red);
            image[i][j].rgbtGreen = (sepiaGreen > 255) ? 255 : sepiaGreen;

            // set sepia value for red pixel, using original colors
            int sepiaRed = round(0.189 * blue + 0.769 * green + 0.393 * red);
            image[i][j].rgbtRed = (sepiaRed > 255) ? 255 : sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // temp is empty, will use temp to store each blurred pixel from image
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initialise values, or reset values to 0 for each grid of surrounding pixels being iterated over
            float sumBlue = 0;
            float sumGreen = 0;
            float sumRed = 0;
            float counter = 0;

            // obtain sum of RGB colours and number of valid pixels in grid of surrounding pixels being iterated over
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    if (i + h < 0 || i + h > height - 1 || j + w < 0 || j + w > width - 1)
                    {
                        // skip to next iteration of inner for loop to avoid segmentation fault
                        continue;
                    }
                    // sum of RGB values for valid pixels in grid of surrounding pixels
                    sumBlue += image[i + h][j + w].rgbtBlue;
                    sumGreen += image[i + h][j + w].rgbtGreen;
                    sumRed += image[i + h][j + w].rgbtRed;

                    // increase counter for each valid pixel in grid of surrounding pixels
                    counter++;
                }
            }

            // for each pixel in image, calculate average RGB values in grid of surrounding pixels and store in temp
            // orignal RGB values for pixels in image remain unaltered so will not affect subsequent calculations of average RGB values
            temp[i][j].rgbtBlue = round(sumBlue / counter);
            temp[i][j].rgbtGreen = round(sumGreen / counter);
            temp[i][j].rgbtRed = round(sumRed / counter);
        }
    }

    // copy the values stored in temp back to image by iterating over each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // initialise Gx convolutional matrix as float so possible to round up later
    float Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    //// initialise Gy convolutional matrix as float so possible to round up later
    float Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // temp is empty, will use temp to store each edge detection pixel from image
    // increase both height and width by 1 each to store black border
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initialise values, or reset values to 0 for each grid of surrounding pixels being iterated over
            float sumBlueGx = 0;
            float sumBlueGy = 0;
            float sumGreenGx = 0;
            float sumGreenGy = 0;
            float sumRedGx = 0;
            float sumRedGy = 0;

            // apply Sobel operator onto pixel
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    if (i + h < 0 || i + h > height - 1 || j + w < 0 || j + w > width - 1)
                    {
                        // skip to next iteration of inner for loop to avoid segmentation fault
                        continue;
                    }

                    // calculate edge pixel value for blue
                    sumBlueGx += image[i + h][j + w].rgbtBlue * Gx[h + 1][w + 1];
                    sumBlueGy += image[i + h][j + w].rgbtBlue * Gy[h + 1][w + 1];
                    int sobelValB = round(sqrt(pow(sumBlueGx, 2) + pow(sumBlueGy, 2)));
                    int edgeValB = (sobelValB > 255) ? 255 : sobelValB;
                    temp[i][j].rgbtBlue = edgeValB;

                    // calculate edge pixel value for green
                    sumGreenGx += image[i + h][j + w].rgbtGreen * Gx[h + 1][w + 1];
                    sumGreenGy += image[i + h][j + w].rgbtGreen * Gy[h + 1][w + 1];
                    int sobelValG = round(sqrt(pow(sumGreenGx, 2) + pow(sumGreenGy, 2)));
                    int edgeValG = (sobelValG > 255) ? 255 : sobelValG;
                    temp[i][j].rgbtGreen = edgeValG;

                    // calculate edge pixel value for red
                    sumRedGx += image[i + h][j + w].rgbtRed * Gx[h + 1][w + 1];
                    sumRedGy += image[i + h][j + w].rgbtRed * Gy[h + 1][w + 1];
                    int sobelValR = round(sqrt(pow(sumRedGx, 2) + pow(sumRedGy, 2)));
                    int edgeValR = (sobelValR > 255) ? 255 : sobelValR;
                    temp[i][j].rgbtRed = edgeValR;
                }
            }
        }
    }

    // copy the values stored in temp back to image by iterating over each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}
