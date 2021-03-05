# Implementation Details

Write a program to run simulations of the FIFA World Cup.

Complete the implementation of tournament.py, such that it simulates a number of tournaments and outputs each team’s probability of winning.

* First, in main, read the team data from the CSV file into your program’s memory, and add each team to the list teams.
* Next, implement the simulate_tournament function. This function should accept as input a list of teams and should repeatedly simulate rounds until you’re left with one team. The function should the return the name of that team.
* Finally, back in the main function, run N tournament simulations, and keep track of how many times each team wins in the counts dictionary.
