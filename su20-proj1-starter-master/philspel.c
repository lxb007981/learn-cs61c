/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philspel.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Specify a dictionary\n");
        return 0;
    }
    /*
     * Allocate a hash table to store the dictionary.
     */
    fprintf(stderr, "Creating hashtable\n");
    dictionary = createHashTable(2255, &stringHash, &stringEquals);

    fprintf(stderr, "Loading dictionary %s\n", argv[1]);
    readDictionary(argv[1]);
    fprintf(stderr, "Dictionary loaded\n");

    fprintf(stderr, "Processing stdin\n");
    processInput();

    /*
     * The MAIN function in C should always return 0 as a way of telling
     * whatever program invoked this that everything went OK.
     */
    return 0;
}

/*
 * This should hash a string to a bucket index.  Void *s can be safely cast
 * to a char * (null terminated string) and is already done for you here 
 * for convenience.
 */
unsigned int stringHash(void *s)
{
    char *string = (char *)s;
    // -- TODO --
    unsigned int hash = 5381;
    int c;
    while ((c = *(string++)))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % dictionary->size;
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2)
{
    char *string1 = (char *)s1;
    char *string2 = (char *)s2;
    // -- TODO --
    if (strcmp(string1, string2) == 0) //equal
        return 1;
    else
        return 0;
}

/*
 * This function should read in every word from the dictionary and
 * store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(1)
 * to cleanly exit the program.
 *
 * Since the format is one word at a time, with new lines in between,
 * you can safely use fscanf() to read in the strings until you want to handle
 * arbitrarily long dictionary chacaters.
 */
void readDictionary(char *dictName)
{
    FILE *fp;
    fp = fopen(dictName, "r");
    char *str1 = (char *)malloc(70);
    int c;
    int i = 0;
    int total = 70;
    if (fp == NULL)
    {
        fprintf(stderr, "Error in file opening\n");
        exit(1);
    }
    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
        {
            str1[i] = '\0';
            char *key = (char *)malloc((strlen(str1) + 1) * sizeof(char));
            strcpy(key, str1);
            if (findData(dictionary, key) == NULL)
            {
                insertData(dictionary, key, key);
            }
            i = 0;
            // total = 70;
            memset(str1, 0, strlen(str1));
            continue;
        }
        if (i == total)
        {
            str1 = (char *)realloc(str1, total *= 2);
        }
        str1[i] = (char)c;
        i++;
    }
    free(str1);
    fclose(fp);
}

void readDictionary_mysol(char *dictName)
{
    // -- TODO --
    FILE *fp;
    char str[9999];

    /* opening file for reading */
    fp = fopen(dictName, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }
    while (fgets(str, 9999, fp) != NULL)
    {
        /* writing content to stdout */
        int length = strlen(str);
        char *string = malloc(length);
        memcpy(string, str, length - 1);
        void *word = (void *)string;
        insertData(dictionary, word, word);
    }
    fclose(fp);
}

/*
 * This should process standard input (stdin) and copy it to standard
 * output (stdout) as specified in the spec (e.g., if a standard 
 * dictionary was used and the string "this is a taest of  this-proGram" 
 * was given to stdin, the output to stdout should be 
 * "this is a teast [sic] of  this-proGram").  All words should be checked
 * against the dictionary as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not found by appending " [sic]" after the error.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final 20% of your grade, you cannot assume words have a bounded length.
 */
void processInput()
{
    // -- TODO --
    char *str1 = (char *)malloc(70);
    char *str2 = (char *)malloc(70);
    char *str3 = (char *)malloc(70);
    int c = 0;
    int i = 0;
    int total = 70;

    while ((c = fgetc(stdin)) != EOF)
    {

        if (isalpha(c) != 0)
        {
            if (i == total)
            {
                str1 = (char *)realloc(str1, total * 2);
                str2 = (char *)realloc(str2, total * 2);
                str3 = (char *)realloc(str3, total * 2);
                total = total * 2;
            }
            str1[i] = (char)c;
            if (i == 0)
            {
                str2[i] = (char)c;
            }
            else
            {
                str2[i] = (char)tolower(c);
            }
            str3[i] = (char)tolower(c);
            i++;
        }
        else
        {
            if (isalpha(str1[0]))
            {
                str1[i] = '\0';
                str2[i] = '\0';
                str3[i] = '\0';
                if (findData(dictionary, str1) == NULL && findData(dictionary, str2) == NULL && findData(dictionary, str3) == NULL)
                {
                    fprintf(stdout, "%s [sic]%c", str1, c);
                }
                else
                {
                    fprintf(stdout, "%s%c", str1, c);
                }
            }
            else
            {
                fprintf(stdout, "%c", c);
            }
            i = 0;
            memset(str1, 0, strlen(str1));
            memset(str2, 0, strlen(str2));
            memset(str3, 0, strlen(str3));
        }
    }
    if (isalpha(str1[0]))
    {
        str1[i] = '\0';
        str2[i] = '\0';
        str3[i] = '\0';
        if (findData(dictionary, str1) == NULL && findData(dictionary, str2) == NULL && findData(dictionary, str3) == NULL)
        {
            fprintf(stdout, "%s [sic]", str1);
        }
        else
        {
            if (strlen(str1) != 0)
            {
                fprintf(stdout, "%s", str1);
            }
        }
    }
    free(str1);
    free(str2);
    free(str3);
}

void processInput_mysol()
{
    // -- TODO --
    char c[99999];
    FILE *fp;
    fp = fopen("file.txt", "r");
    while (fgets(c, 99999, stdin))
    {
        //char word[9999];
        int i = 0;
        int foundStart = 0;
        int wordStart = 0;
        int wordEnd;
        int newLine = c[strlen(c) - 1];
        for (; c[i] != 10 && c[i] != 0; ++i)
        {
            if ((c[i] >= 97 && c[i] <= 122) || (c[i] >= 65 && c[i] <= 90))
            {
                if (foundStart == 0)
                {
                    foundStart = 1;
                    wordStart = i;
                }
            }
            else
            {
                if (foundStart == 1)
                {
                    foundStart = 0;
                    wordEnd = i - 1;
                    int found = findWord(wordStart, wordEnd, c);
                    if (!found)
                    {
                        printf(" [sic]");
                    }
                }
            }
            printf("%c", c[i]);
        }
        if (foundStart == 1)
        {
            wordEnd = i - 1;
            int found = findWord(wordStart, wordEnd, c);
            if (!found)
            {
                printf(" [sic]");
            }
        }
        if (newLine == 10)
        {
            printf("%c", newLine);
        }
    }
}

int findWord(int wordStart, int wordEnd, const char *c)
{
    int wordLength = wordEnd - wordStart + 1;

    char *word = malloc(wordLength + 1);
    char *wordLower = malloc(wordLength + 1);
    char *wordPartialLower = malloc(wordLength + 1);

    for (int j = 0; j < wordLength; ++j)
    {
        word[j] = c[wordStart + j];
    }
    for (int j = 0; j < wordLength; ++j)
    {
        wordLower[j] = word[j];
        if (isupper(wordLower[j]))
        {
            wordLower[j] = tolower(wordLower[j]);
        }
    }
    memcpy(wordPartialLower, wordLower, wordLength);
    wordPartialLower[0] = word[0];
    int x = findData(dictionary, word);
    int y = findData(dictionary, wordLower);
    int z = findData(dictionary, wordPartialLower);
    free(word);
    free(wordLower);
    free(wordPartialLower);

    return (x || y || z);
}