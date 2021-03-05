from cs50 import get_int


def print_pyramid():
    # Get height input from user
    while (True):
        # get_int forces user to input integer
        height = get_int("Pyramind height: ")
        if 1 <= height <= 8:
            break
        else:
            print("Invalid height, please enter an integer between 1 and 8 inclusive.")

    # Print out two half-pyramids with gap
    for i in range(1, height + 1):
        spaces = " " * (height - i)
        squares = "#" * i
        print(f"{spaces}{squares}  {squares}")


if __name__ == "__main__":
    print_pyramid()
