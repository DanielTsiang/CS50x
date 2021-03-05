from cs50 import get_float


# Get change owed input from user,
def get_change_owed():
    # repeats until valid float input provided
    while (True):
        # get_float forces the user to input a valid float
        dollars = get_float("Change owed: ")
        if dollars >= 0:
            break
        else:
            print("Invalid input, please enter an non-negative number.")

    return dollars


# Assume that only coins available are quarters (25¢), dimes (10¢), nickels (5¢), and pennies (1¢)
# Calculate minimum number of coins needed to reach change owed
def calculate_min_coins(dollars):
    # Convert cents to dollars and round to avoid floating-point imprecision
    cents = round(dollars * 100)
    print(cents)
    coins_used = 0

    while cents >= 25:
        cents -= 25
        coins_used += 1

    while cents >= 10:
        cents -= 10
        coins_used += 1

    while cents >= 5:
        cents -= 5
        coins_used += 1

    while cents > 0:
        cents -= 1
        coins_used += 1

    return coins_used


if __name__ == "__main__":
    dollars = get_change_owed()
    print(calculate_min_coins(dollars))
