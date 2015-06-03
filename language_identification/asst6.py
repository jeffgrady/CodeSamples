#!/usr/bin/python

# Email:  jeff.grady@xxxxxxxx.com
# Date:   2010/10/27

# regular expressions
import re
# standard math routines
import math

# this will be our global dictionary (hash) of learned character frequency
# statistics
LANGUAGES = {
    "english" : {},
    "french" : {},
    "german" : {},
    "italian" : {},
    "portuguese" : {},
    "spanish" : {}
    }

# Function: initialize_matrix
# Parameters:  none
# Returns: none
# Note:  Initialized global language matrix for one character look-ahead
def initialize_matrix():
    for lang in LANGUAGES.keys():
        for first in range(ord('a'), ord('z')+1) + [ord(' ')]:
            first = chr(first)
            LANGUAGES[lang][first] = {}
            for second in range(ord('a'), ord('z')+1) + [ord(' ')]:
                second = chr(second)
                # We use 0.1 in the *frequency* matrix, as specified
                # by the assignment
                LANGUAGES[lang][first][second] = 0.1

# Function: initialize_matrix_extension
# Parameters:  none
# Returns: none
# Note:  Initialized global language matrix for two character look-ahead
def initialize_matrix_extension():
    for lang in LANGUAGES.keys():
        for first in range(ord('a'), ord('z')+1) + [ord(' ')]:
            first = chr(first)
            LANGUAGES[lang][first] = {}
            for second in range(ord('a'), ord('z')+1) + [ord(' ')]:
                second = chr(second)
                LANGUAGES[lang][first][second] = {}
                for third in range(ord('a'), ord('z')+1) + [ord(' ')]:
                    third = chr(third)
                    # We use 0.1 in the *frequency* matrix, as specified
                    # by the assignment
                    LANGUAGES[lang][first][second][third] = 0.1

# Function: learn
# Parameters: a string representing the language to be learned
# Returns: nothing
# Note:  This function will populate LANGUAGES with statistics on the 
#   one look-ahead character frequency of the language passed in
def learn(language):
    sample = file("%s.txt" % (language))
    data = sample.read()
    size = len(data)
    # this compiled regex matches a character that is NOT a-z
    non_alpha_char = re.compile("[^a-z]")
    # number of relevant chars
    num_chars = 0
    for i in range(0, size-2):
        first = data[i].lower()
        second = data[i+1].lower()
        if non_alpha_char.match(first):
            first = " "
        if non_alpha_char.match(second):
            second = " "
        # Multiple space should be treated as a single space
        if (first == " ") and (second == " "):
            continue
        num_chars += 1
        LANGUAGES[language][first][second] += 1.0
    #print "num_chars is " + str(num_chars)
    for first in LANGUAGES[language].keys():
        for second in LANGUAGES[language][first].keys():
            LANGUAGES[language][first][second] = \
                math.log(LANGUAGES[language][first][second] / num_chars)
    sample.close()

# Function: learn_extension
# Parameters: a string representing the language to be learned
# Returns: nothing
# Note:  This function will populate LANGUAGES with statistics on the 
#   two look-ahead character frequency of the language passed in
def learn_extension(language):
    sample = file("extension_%s.txt" % (language))
    # data is utf-8, thus we should decode it as such.  I'm pretty sure
    # if we were using python 3, that wouldn't be necessary, but we're not
    data = strip_accents(sample.read().decode("utf-8"))
    size = len(data)
    # this compiled regex matches a character that is NOT a-z
    non_alpha_char = re.compile("[^a-z]")
    # number of relevant chars
    num_chars = 0
    for i in range(0, size-3):
        first = data[i].lower()
        second = data[i+1].lower()
        third = data[i+2].lower()
        if non_alpha_char.match(first):
            first = " "
        if non_alpha_char.match(second):
            second = " "
        if non_alpha_char.match(third):
            third = " "
        # Multiple space should be treated as a single space
        if (first == " ") and (second == " ") and (third == " "):
            continue
        num_chars += 1
        LANGUAGES[language][first][second][third] += 1.0
    for first in LANGUAGES[language].keys():
        for second in LANGUAGES[language][first].keys():
            for third in LANGUAGES[language][first][second].keys():
                # Take the natural log in an attempt to not blow out
                # the floating point number
                LANGUAGES[language][first][second][third] = \
                    math.log(LANGUAGES[language][first][second][third] / \
                                 num_chars)
    sample.close()

# Not my code and not presenting this as my own.  Borrowed from:
# http://stackoverflow.com/questions/517923/what-is-the-best-way-to-remove-accents-in-a-python-unicode-string
# This will strip accent characters from utf-8 data and convert them to their
# ascii equivalent
import unicodedata
def strip_accents(s):
   return ''.join((c for c in unicodedata.normalize('NFD', s) if unicodedata.category(c) != 'Mn'))

# Function:  print_result
# Parameters:  takes a dictionary as input.  Strings representing 
#  languages are the keys.  Values are the sum of the natural log of the 
#  probability that characters appear in the input sentence.
# Returns:  nothing
# Note:  prints an ordered list of languages that the input might belong
#  to starting with the most likely and indicating the relative/normalized
#  probability that the input belongs to each language.
def print_result(lang_samples):
    total = 0.0
    for lang in lang_samples:
        # these are stored as natural logs, so take the natural exponent
        total += math.exp(lang_samples[lang])
    i = 1
    for lang in sorted(lang_samples,
                       key=lambda lang: lang_samples[lang],
                       reverse=True):
        # Take the natural exponent of the stats, because before we took
        # the natural log, normalize it, and multiply by 100 to get a percent
        print "\t%s.) %s:  probability %3.10f%%" % \
            (i, lang, math.exp(lang_samples[lang]) / total * 100.0)
        i += 1

# Function:  test
# Parameters:  A string indicating the filename of the input data
# Returns: nothing
# Note:  Computes the relative probabilities that each line in the input
#  file belongs to a particular language and then outputs the results
def test(test_file):
    test_data = file(test_file, "r")
    # regex matching everything up to and including a period
    stuff_before_a_period = re.compile('(.*?\.)')
    # this compiled regex matches a character that is NOT a-z
    non_alpha_char = re.compile("[^a-z]")
    for line in test_data.readlines():
        matches = stuff_before_a_period.match(line)
        # If we got a blank line or something without a period, skip it
        if (not matches) or (len(matches.groups()) <= 0):
            continue
        # Grab the first match of everything up to and including the period
        sentence = matches.group(1)
        size = len(sentence)
        # this stores our statistics for each language for the given input
        # line
        lang_samples = {}
        for lang in LANGUAGES.keys():
            # Note that probabilities when we're taking the log and adding
            # values, rather than multiplying them should NOT be initialized
            # to 1.0 as stated in the assignment, but 0.0
            lang_samples[lang] = 0.0
        for i in range(0, size-2):
            first = sentence[i].lower()
            second = sentence[i+1].lower()
            if non_alpha_char.match(first):
                first = " "
            if non_alpha_char.match(second):
                second = " "
            # Multiple space should be treated as a single space
            if (first == " ") and (second == " "):
                continue
            for lang in LANGUAGES.keys():
                # Here we add natural logs, because we're dealing with
                # exponents, rather than multiplying non-exponents.
                # high school algebra stuff.
                lang_samples[lang] += LANGUAGES[lang][first][second]
        print sentence
        i = 1
        print_result(lang_samples)

# Function:  test_extension
# Parameters:  A string indicating the filename of the input data
# Returns: nothing
# Note:  Computes the relative probabilities that each line in the input
#  file belongs to a particular language and then outputs the results,
#  except this is for the two character look-ahead
def test_extension(test_file):
    test_data = file(test_file, "r")
    # regex matching everything up to and including a period
    stuff_before_a_period = re.compile('(.*?\.)')
    # this compiled regex matches a character that is NOT a-z
    non_alpha_char = re.compile("[^a-z]")
    for line in test_data.readlines():
        matches = stuff_before_a_period.match(line)
        # If we got a blank line or something without a period, skip it
        if (not matches) or (len(matches.groups()) <= 0):
            continue
        # Grab the first match of everything up to and including the period
        sentence = matches.group(1)
        size = len(sentence)
        lang_samples = {}
        for lang in LANGUAGES.keys():
            # Note that probabilities when we're taking the log and adding
            # values, rather than multiplying them should NOT be initialized
            # to 1.0 as stated in the assignment, but 0.0
            lang_samples[lang] = 0.0
        for i in range(0, size-3):
            first = sentence[i].lower()
            second = sentence[i+1].lower()
            third = sentence[i+2].lower()
            if non_alpha_char.match(first):
                first = " "
            if non_alpha_char.match(second):
                second = " "
            if non_alpha_char.match(third):
                third = " "
            # Multiple space should be treated as a single space
            if (first == " ") and (second == " ") and (third == " "):
                continue
            for lang in LANGUAGES.keys():
                # Here we add natural logs, because we're dealing with
                # exponents, rather than multiplying non-exponents.
                # high school algebra stuff.
                lang_samples[lang] += LANGUAGES[lang][first][second][third]
        print sentence
        print_result(lang_samples)

# Function:  learn_all_languages
# Parameters:  none
# Returns:  nothing
# Note:  Calls learn() on all available languages
def learn_all_languages():
    for language in LANGUAGES.keys():
        print "Learning:  %s" % (language)
        learn(language)

# Function:  learn_all_languages_extension
# Parameters:  none
# Returns:  nothing
# Note:  Calls learn_extension() on all available languages
def learn_all_languages_extension():
    for language in LANGUAGES.keys():
        print "Learning:  %s (extended)" % (language)
        learn_extension(language)

# Convenience function to print the LANGUAGES matrix
def print_matrix():
    print LANGUAGES

# Function:  main
# Parameters:  none
# Returns: nothing
# Note:  Runs through test data and extended test data using the
#  one character look-ahead method, then does the same thing with the
#  two character look-ahead.
def main():
    print "########################################"
    initialize_matrix()
    learn_all_languages()
    test("test-data.txt")
    test("test-data-extension.txt")

    print "################# EXTENSION #######################"
    initialize_matrix_extension()
    learn_all_languages_extension()
    test_extension("test-data.txt")
    test_extension("test-data-extension.txt")

main()
