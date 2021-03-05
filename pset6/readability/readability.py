from cs50 import get_string


def main():
    # get_string gets string input from user
    string = get_string("Text: ")

    # calculate number of letters and words in string
    letters = sum(char.isalpha() for char in string)
    spaces = sum(char.isspace() for char in string)
    words = spaces + 1

    # calculate number of sentend in string
    sentences = 0
    for char in string:
        if char in [".", "?", "!"]:
            sentences += 1

    # calculate Coleman-Liau index
    index = get_index(letters, words, sentences)

    # print reading grade
    print_index(index)


def get_index(letters, words, sentences):
    # Reading level is from Coleman-Liau index = 0.0588 * L - 0.296 * S - 15.8
    L = (letters / words) * 100
    S = (sentences / words) * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)
    return index


def print_index(index):
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


if __name__ == "__main__":
    main()

