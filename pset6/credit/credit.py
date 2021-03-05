from cs50 import get_int


def get_card_number():
    # get_int forces the user to input a valid integer
    card_num = get_int("What is your card number? ")
    length = len(str(card_num))

    # convert card_num into list of digits
    card_digits = list(map(int, str(card_num)))

    return [length, card_digits]


def card_algorithm(length, card_digits):
    """multiple every other digit in list by 2 starting from 2nd last digit of card_digits,
    i.e. starting with card_digits[length - 2], then add digits together"""
    card_sum = 0
    for i in range(length - 2, -1, -2):
        doubled_digit = card_digits[i] * 2
        if (doubled_digit < 9):
            card_sum += doubled_digit
        else:
            # doubled_digit is > 10 so need to split into individual digits again
            digits = list(map(int, str(doubled_digit)))
            card_sum += digits[0] + digits[1]

    """Now let’s add that card_sum to the sum of the digits that weren’t multiplied by 2,
    starting from the end of card_digits list"""
    for i in range(length - 1, -1, -2):
        card_sum += card_digits[i]

    # check last digit of card_sum
    sum_last_digit = card_sum % 10

    return sum_last_digit


def check_card(length, card_digits, sum_last_digit):
    # check if card number is valid
    if sum_last_digit == 0:
        if length == 15 and card_digits[0] == 3 and (card_digits[1] in [4, 7]):
            print("AMEX")
        elif length == 16 and card_digits[0] == 5 and (card_digits[1] in [1, 2, 3, 4, 5]):
            print("MASTERCARD")
        elif length in [13, 16] and card_digits[0] == 4:
            print("VISA")
        else:
            print("INVALID")
    # last digit of card number is not equal to 0 so must be invalid
    else:
        print("INVALID")


if __name__ == "__main__":
    length, card_digits = get_card_number()
    sum_last_digit = card_algorithm(length, card_digits)
    check_card(length, card_digits, sum_last_digit)
