# Implementation Details

Complete the implementation of volume.c, such that it changes the volume of a sound file by a given factor.

* The program accepts three command-line arguments: input represents the name of the original audio file, output represents the name of the new audio file that should be generated, and factor is the amount by which the volume of the original audio file should be scaled.
    * For example, if factor is 2.0, then your program should double the volume of the audio file in input and save the newly generated audio file in output.
* Your program should first read the header from the input file and write the header to the output file. Recall that this header is always exactly 44 bytes long.
    * Note that volume.c already defines a variable for you called HEADER_SIZE, equal to the number of bytes in the header.
* Your program should then read the rest of the data from the WAV file, one 16-bit (2-byte) sample at a time. Your program should multiply each sample by the factor and write the new sample to the output file.
    * You may assume that the WAV file will use 16-bit signed values as samples. In practice, WAV files can have varying numbers of bits per sample, but we’ll assume 16-bit samples for this lab.
* Your program, if it uses malloc, must not leak any memory.