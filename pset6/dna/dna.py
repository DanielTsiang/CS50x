# A program that identifies to whom a sequence of DNA belongs to

import csv
import sys
import re


def main():

    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Read people data and Short Tandem Repeat (STR) patterns into memory from file and store as list of dictionaries
    people, STR_patterns = read_people_data()

    # Read DNA sequence into memory from file as store as string
    filename = sys.argv[2]
    with open(filename, "r") as file:
        sequence = file.read().strip()

    # compute the longest run of consecutive repeats of the STR in the DNA sequence
    # store each STR count in a dictionary
    longest_STR_count = longest_STR(sequence, STR_patterns)

    # check if STR counts match exactly with any of the individuals
    check_match(people, longest_STR_count)


def read_people_data():
    people = []
    filename = sys.argv[1]
    with open(filename, "r") as file:
        reader = csv.DictReader(file)
        patterns = reader.fieldnames[1:]
        for person in reader:
            for pattern in patterns:
                person[pattern] = int(person[pattern])
            people.append(person)
    return people, patterns


def longest_STR(sequence, patterns):
    longest_STR_count = {}
    for pattern in patterns:
        # use ?: syntax for non-capturing group, so can backtrack to find longest non-overlapping match
        match = re.findall(f"(?:{pattern})+", sequence)
        if match:
            # last element in match is the longest match as sorted
            longest_STR = sorted(match)[-1]
            STR_length = len(longest_STR)/len(pattern)
            longest_STR_count[pattern] = STR_length
    return longest_STR_count


def check_match(people, longest_STR_count):
    for person in people:
        # print name of matching individual
        matching_individual = list(person.values())[1:] == list(longest_STR_count.values())
        if matching_individual:
            print(f"{person['name']}")
            return
    print("No match")


if __name__ == "__main__":
    main()
