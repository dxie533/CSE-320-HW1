#include <stdlib.h>

#include "debug.h"
#include "hw1.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the content of three frames of audio data and
 * two annotation fields have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 1 if validation succeeds and 0 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variables "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 1 if validation succeeds and 0 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */

int correctBinaudible(char **argv);
int checkNumStrings(int argc);
int correctFlag(char **argv);
int correctOther(char **argv);
int factorOf(char **argv);
unsigned long validKey(char **argv);
int placeKey(char **argv);

int read_header(AUDIO_HEADER *hp);
int write_header(AUDIO_HEADER *hp);
int read_annotation(char *ap, unsigned int size);
int write_annotation(char *ap, unsigned int size);
int read_frame(int *fp, int channels, int bytes_per_sample);
int write_frame(int *fp, int channels, int bytes_per_sample);
int read_arguments(char *ap, char **argv);
int append_annotation(char *ap, unsigned int size, char *ap2, unsigned int size2);
int copy_frame(int *fp, int *fp2, int channels, int bytes_per_sample);
int slowDown_frame(int *fp, int *fp2, int *fp3, int channels, int bytes_per_sample, int factor);
int convertTwosComplement(int theChar, int bytes_per_sample);
int xor_frame(int *fp, int channels);

int theArgumentNumber = 0;

int validargs(int argc, char **argv)
{
    theArgumentNumber = argc;
    int argvCounter = 0;
    if(correctBinaudible(argv) == 1)//passes in the pointer
    {
        if(checkNumStrings(argc) == 1)// still another string
        {
            argc--;//decrement a string
            argv++;// go to next string
            argvCounter++;
            int flagOne = correctFlag(argv);
            if(flagOne == 1)
            {
                global_options = 1;
                global_options = global_options << 63;
                argv = argv - argvCounter;
                return 1;
            }
            if(flagOne == 2)// it is -u
            {
                if(checkNumStrings(argc) == 1)// still another string
                {
                    argc--;//decrement a string
                    argv++;// go to next string
                    argvCounter++;
                    int otherFlagOne = correctOther(argv);
                    if(otherFlagOne == 1)// its is -f
                    {
                        //followed by factor and then maybe -p
                        if(checkNumStrings(argc) == 1)// still another string
                        {
                            argc--;//decrement a string
                            argv++;// go to next string
                            argvCounter++;
                            int factor = factorOf(argv);//get the factor
                            if(factor != 0)//correct factor
                            {
                                if(checkNumStrings(argc) == 1)// still another string
                                {
                                    argc--;//decrement a string
                                    argv++;// go to next string
                                    argvCounter++;
                                    int otherFlagTwo = correctOther(argv);
                                    if(otherFlagTwo == 2)
                                    {
                                        global_options = 1;
                                        global_options = global_options << 62;
                                        unsigned long globalP = 1;
                                        globalP = globalP << 59;
                                        global_options = global_options + globalP;
                                        unsigned long globalFactor = factor - 1;
                                        globalFactor = globalFactor << 48;
                                        global_options = global_options + globalFactor;
                                        argv = argv - argvCounter;
                                        return 1;// bin/audible -u -f validfactor -p
                                    }
                                    else
                                    {
                                        return 0;// bin/audible -u -f validfactor wrongflag
                                    }
                                }
                                else
                                {
                                    global_options = 1;
                                    global_options = global_options << 62;
                                    unsigned long globalFactor = factor - 1;
                                    globalFactor = globalFactor << 48;
                                    global_options = global_options + globalFactor;
                                    argv = argv - argvCounter;
                                    return 1;// bin/audible -u -f validfactor
                                }
                            }
                            else
                            {
                                return 0; //bin/audible -u -f wrongfactor
                            }
                        }
                        else
                        {
                            return 0; //bin/audible -u -f
                        }
                    }
                    if(otherFlagOne == 2)// its is -p
                    {
                        if(checkNumStrings(argc) == 1)// still another string
                        {
                            argc--;//decrement a string
                            argv++;// go to next string
                            argvCounter++;
                            int otherFlagTwo = correctOther(argv);
                            if(otherFlagTwo == 1)// It is -f
                            {
                                if(checkNumStrings(argc) == 1)// still another string
                                {
                                    argc--;//decrement a string
                                    argv++;// go to next string
                                    argvCounter++;
                                    int factor = factorOf(argv);//get the factor
                                    if(factor != 0)//correct factor
                                    {
                                        if(checkNumStrings(argc) == 1)// still another string
                                        {
                                            return 0;// bin/audible -u -p -f -validfactor ...
                                        }
                                        else
                                        {
                                            global_options = 1;
                                            global_options = global_options << 62;
                                            unsigned long globalP = 1;
                                            globalP = globalP << 59;
                                            global_options = global_options + globalP;
                                            unsigned long globalFactor = factor - 1;
                                            globalFactor = globalFactor << 48;
                                            global_options = global_options + globalFactor;
                                            argv = argv - argvCounter;
                                            return 1;// bin/duaible -u -p -f validfactor
                                        }
                                    }
                                    else
                                    {
                                        return 0; //bin/audible -u -p -f wrongfactor
                                    }
                                }
                                else
                                {
                                    return 0; //bin/audible -u -p -f
                                }
                            }
                            else
                            {
                                return 0; // bin/audible -u -p wrongstring
                            }
                        }
                        else
                        {
                            global_options = 1;
                            global_options = global_options << 62;
                            unsigned long globalP = 1;
                            globalP = globalP << 59;
                            global_options = global_options + globalP;
                            argv = argv - argvCounter;
                            return 1;//bin/audible -u -p
                        }
                        // followed by maybe factor
                    }
                    return 0;
                }
                else
                {
                    global_options = 1;
                    global_options = global_options << 62;
                    argv = argv - argvCounter;
                    return 1; //bin/audible -u
                }

            }
            if(flagOne == 3)// it is -d
            {
                if(checkNumStrings(argc) == 1)// still another string
                {
                    argc--;//decrement a string
                    argv++;// go to next string
                    argvCounter++;
                    int otherFlagOne = correctOther(argv);
                    if(otherFlagOne == 1)// its is -f
                    {
                        //followed by factor and then maybe -p
                        if(checkNumStrings(argc) == 1)// still another string
                        {
                            argc--;//decrement a string
                            argv++;// go to next string
                            argvCounter++;
                            int factor = factorOf(argv);//get the factor
                            if(factor != 0)//correct factor
                            {
                                if(checkNumStrings(argc) == 1)// still another string
                                {
                                    argc--;//decrement a string
                                    argv++;// go to next string
                                    argvCounter++;
                                    int otherFlagTwo = correctOther(argv);
                                    if(otherFlagTwo == 2)
                                    {
                                        global_options = 1;
                                        global_options = global_options << 61;
                                        unsigned long globalP = 1;
                                        globalP = globalP << 59;
                                        global_options = global_options + globalP;
                                        unsigned long globalFactor = factor - 1;
                                        globalFactor = globalFactor << 48;
                                        global_options = global_options + globalFactor;
                                        argv = argv - argvCounter;
                                        return 1;// bin/audible -d -f validfactor -p
                                    }
                                    else
                                    {
                                        return 0;// bin/audible -d -f validfactor wrongflag
                                    }
                                }
                                else
                                {
                                    global_options = 1;
                                    global_options = global_options << 61;
                                    unsigned long globalFactor = factor - 1;
                                    globalFactor = globalFactor << 48;
                                    global_options = global_options + globalFactor;
                                    argv = argv - argvCounter;
                                    return 1;// bin/audible -d -f validfactor
                                }
                            }
                            else
                            {
                                return 0; //bin/audible -d -f wrongfactor
                            }
                        }
                        else
                        {
                            return 0; //bin/audible -d -f
                        }
                    }
                    if(otherFlagOne == 2)// its is -p
                    {
                        if(checkNumStrings(argc) == 1)// still another string
                        {
                            argc--;//decrement a string
                            argv++;// go to next string
                            argvCounter++;
                            int otherFlagTwo = correctOther(argv);
                            if(otherFlagTwo == 1)// It is -f
                            {
                                if(checkNumStrings(argc) == 1)// still another string
                                {
                                    argc--;//decrement a string
                                    argv++;// go to next string
                                    argvCounter++;
                                    int factor = factorOf(argv);//get the factor
                                    if(factor != 0)//correct factor
                                    {
                                        if(checkNumStrings(argc) == 1)// still another string
                                        {
                                            return 0;// bin/audible -d -p -f -validfactor ...
                                        }
                                        else
                                        {
                                            global_options = 1;
                                            global_options = global_options << 61;
                                            unsigned long globalP = 1;
                                            globalP = globalP << 59;
                                            global_options = global_options + globalP;
                                            unsigned long globalFactor = factor - 1;
                                            globalFactor = globalFactor << 48;
                                            global_options = global_options + globalFactor;
                                            return 1;// bin/duaible -d -p -f validfactor
                                        }
                                    }
                                    else
                                    {
                                        return 0; //bin/audible -d -p -f wrongfactor
                                    }
                                }
                                else
                                {
                                    return 0; //bin/audible -d -p -f
                                }
                            }
                            else
                            {
                                return 0; // bin/audible -d -p wrongstring
                            }
                        }
                        else
                        {
                            global_options = 1;
                            global_options = global_options << 61;
                            unsigned long globalP = 1;
                            globalP = globalP << 59;
                            global_options = global_options + globalP;
                            argv = argv - argvCounter;
                            return 1;//bin/audible -d -p
                        }
                        // followed by maybe factor
                    }
                    return 0;
                }
                else
                {
                    global_options = 1;
                    global_options = global_options << 61;
                    argv = argv - argvCounter;
                    return 1; //bin/audible -d
                }

            }
            if(flagOne == 4)// it is -c
            {
                if(checkNumStrings(argc) == 1)// still another string
                {
                    argc--;//decrement a string
                    argv++;// go to next string
                    argvCounter++;
                    int otherFlagOne = correctOther(argv);
                    if(otherFlagOne == 3)// its is -k
                    {
                        if(checkNumStrings(argc) == 1)// still another string
                        {
                            argc--;//decrement a string
                            argv++;// go to next string
                            argvCounter++;
                            unsigned long key = validKey(argv);
                            if(key != 0)
                            {
                                if(checkNumStrings(argc) == 1)// still another string
                                {
                                    argc--;//decrement a string
                                    argv++;// go to next string
                                    argvCounter++;
                                    int otherFlagTwo = correctOther(argv);
                                    if(otherFlagTwo == 2)// it is -p
                                    {
                                        global_options = 1;
                                        global_options = global_options << 60;
                                        unsigned long globalP = 1;
                                        globalP = globalP << 59;
                                        global_options = global_options + globalP;
                                        global_options = global_options + key;
                                        argv = argv - argvCounter;
                                        return 1;// bin/audible -c -k validkey -p
                                    }
                                    else
                                    {
                                        return 0;// bin/audible -c -k validkey ...
                                    }
                                }
                                else
                                {
                                    global_options = 1;
                                    global_options = global_options << 60;
                                    global_options = global_options + key;
                                    argv = argv - argvCounter;
                                    return 1; // bin/audible -c -k validkey
                                }
                            }
                            else
                            {
                                return 0;// bin/audible -c -k wrongkey
                            }
                        }
                        else
                        {
                            return 0;// bin/audible -c -k
                        }
                    }
                    if(otherFlagOne == 2)// its is -p
                    {
                        if(checkNumStrings(argc) == 1)// still another string
                        {
                            argc--;//decrement a string
                            argv++;// go to next string
                            argvCounter++;
                            int otherFlagTwo = correctOther(argv);
                            if(otherFlagTwo == 3)// its is -k
                            {
                                if(checkNumStrings(argc) == 1)// still another string
                                {
                                    argc--;//decrement a string
                                    argv++;// go to next string
                                    argvCounter++;
                                    unsigned long key = validKey(argv);
                                    if(key != 0)
                                    {
                                        if(checkNumStrings(argc) == 1)// still another string
                                        {
                                            return 0;// bin/audible -c -p -k validkey ...
                                        }
                                        else
                                        {
                                            global_options = 1;
                                            global_options = global_options << 60;
                                            unsigned long globalP = 1;
                                            globalP = globalP << 59;
                                            global_options = global_options + globalP;
                                            global_options = global_options + key;
                                            argv = argv - argvCounter;
                                            return 1;// bin/audible -c -p -k validkey
                                        }
                                    }
                                    else
                                    {
                                        return 0;// bin/audible -c -p -k wrongkey
                                    }
                                }
                                else
                                {
                                    return 0;// bin/audible -c -p -k
                                }
                            }
                            else
                            {
                                return 0;// bin/audible -c -p wrongflag
                            }
                        }
                        else
                        {
                            return 0;// bin/audible -c -p
                        }
                        //followed by key
                    }
                    else
                    {
                        return 0;
                    }

                }
                else
                {
                    return 0;//wrong, must have a key
                }

            }
            else
            {
                return 0;// Is not -h, -u, -d, -c
            }
        }
        return 0;
    }
    return 0;
}

int checkNumStrings(int argc)
{
    if(argc > 1)
    {
        return 1;
    }
    return 0;
}

//returns 0 if bin/audible is not there, 1 if bin/audible is there
int correctBinaudible(char **argv)
{
    int i = 0;
    while(**argv != '\0')
    {
        if(i == 0 && **argv != 'b')
        {
            return 0;
        }
        if(i == 1 && **argv != 'i')
        {
            return 0;
        }
        if(i == 2 && **argv != 'n')
        {
            return 0;
        }
        if(i == 3 && **argv != '/')
        {
            return 0;
        }
        if(i == 4 && **argv != 'a')
        {
            return 0;
        }
        if(i == 5 && **argv != 'u')
        {
            return 0;
        }
        if(i == 6 && **argv != 'd')
        {
            return 0;
        }
        if(i == 7 && **argv != 'i')
        {
            return 0;
        }
        if(i == 8 && **argv != 'b')
        {
            return 0;
        }
        if(i == 9 && **argv != 'l')
        {
            return 0;
        }
        if(i == 10 && **argv != 'e')
        {
            return 0;
        }
        if(i > 10)
        {
            return 0; //any more chars is wrong
        }
        *argv = *argv + 1;
        i++;
    }
    *argv = *argv - i;
    return 1;
}

int correctFlag(char **argv)
{
    int i = 0;
    while(**argv != '\0')
    {
        *argv = *argv + 1;
        i++;
    }
    if(i > 2)
    {
        return 0;// too long
    }
    *argv = *argv - i;
    i = 0;
    while(**argv != '\0')
    {
        if(i == 0 && **argv != '-')
        {
            return 0;
        }
        if(i == 1 && **argv == 'h')
        {
            *argv = *argv - i;
            return 1;// is h
        }
        if(i == 1 && **argv == 'u')
        {
            *argv = *argv - i;
            return 2;// is u
        }
        if(i == 1 && **argv == 'd')
        {
            *argv = *argv - i;
            return 3;// is d
        }
        if(i == 1 && **argv == 'c')
        {
            *argv = *argv - i;
            return 4;// is c
        }
        if((i == 1 && **argv != 'h') || (i == 1 && **argv != 'u') || (i == 1 && **argv != 'd') || (i == 1 && **argv != 'c'))
        {
            *argv = *argv - i;
            return 0;//wrong letter
        }
        *argv = *argv + 1;
        i++;
    }
    return 0;
}

int correctOther(char **argv)
{
    int i = 0;
    while(**argv != '\0')
    {
        *argv = *argv + 1;
        i++;
    }
    if(i > 2)
    {
        return 0;// too long
    }
    *argv = *argv - i;
    i = 0;
    while(**argv != '\0')
    {
        if(i == 0 && **argv != '-')
        {
            return 0;
        }
        if(i == 1 && **argv == 'f')
        {
            *argv = *argv - i;
            return 1;// is f
        }
        if(i == 1 && **argv == 'p')
        {
            *argv = *argv - i;
            return 2;// is p
        }
        if(i == 1 && **argv == 'k')
        {
            *argv = *argv - i;
            return 3;// is k
        }
        if((i == 1 && **argv != 'f') || (i == 1 && **argv != 'p') || (i == 1 && **argv != 'k'))
        {
            return 0;//wrong letter
        }
        *argv = *argv + 1;
        i++;
    }
    return 0;
}
// returns 0 if factor is wrong, otherwise returns the factor
int factorOf(char **argv)
{
    int i = 0;
    while(**argv != '\0')
    {
        if(**argv < '0' || **argv > '9')
        {
            return 0;
        }
        *argv = *argv + 1;
        i++;
    }
    if(i > 4)
    {
        return 0;// too long
    }
    int argvCounter = i;
    *argv = *argv - i;
    int total = 0;
    while(**argv != '\0')
    {
        i--;
        int sum = 0;
        int number = **argv - '0';
        if(i == 0)
        {
            sum = 1;
        }
        if(i == 1)
        {
            sum = 10;
        }
        if(i == 2)
        {
            sum = 100;
        }
        if(i == 3)
        {
            sum = 1000;
        }
        total = total + number * sum;
        *argv = *argv + 1;
    }
    if(total > 1024)
    {
        return 0;
    }
    if(total == 0)
    {
        return 0;
    }
    *argv = *argv - argvCounter;
    return total;
}

unsigned long validKey(char **argv)
{
    int i = 0;
    while(**argv != '\0')
    {
        if((**argv >= '0' && **argv <= '9') || (**argv >= 'a' && **argv <='f') || (**argv >= 'A' && **argv <='F'))
        {
            *argv = *argv + 1;
            i++;
        }
        else
        {
            return 0;
        }
    }
    if(i > 8)
    {
        return 0;// too long
    }
    *argv = *argv - i;
    unsigned long key = 0;
    while(**argv != '\0')
    {
        key = key << 4;
        unsigned long theChar = 0;
        if ('0' <= **argv && **argv <= '9')
        {
            theChar = **argv - '0';
        }
        if ('a' <= **argv && **argv <= 'f')
        {
            theChar = **argv - 'a' + 10;
        }
        if ('A' <= **argv && **argv <= 'F')
        {
            theChar = **argv - 'A' + 10;
        }
        key = key + theChar;
        *argv = *argv + 1;
        //i--;
    }
    *argv = *argv - i;
    return key;
}

int placeKey(char **argv)
{
    int i = 0;
    while(**argv != '\0')
    {
        i++;
    }
    int argvCounter = i;
    *argv = *argv - i;
    int total = 0;
    while(**argv != '\0')
    {
        i--;
        int sum = 0;
        int number = **argv - '0';
        if(i == 0)
        {
            sum = 1;
        }
        if(i == 1)
        {
            sum = 10;
        }
        if(i == 2)
        {
            sum = 100;
        }
        if(i == 3)
        {
            sum = 1000;
        }
        if(i == 4)
        {
            sum = 10000;
        }
        if(i == 5)
        {
            sum = 100000;
        }
        if(i == 6)
        {
            sum = 1000000;
        }
        if(i == 7)
        {
            sum = 10000000;
        }
        if(i == 8)
        {
            sum = 100000000;
        }
        total = total + number * sum;
        *argv = *argv + 1;
    }
    *argv = *argv - argvCounter;
    return total;
}

/**
 * @brief  Recodes a Sun audio (.au) format audio stream, reading the stream
 * from standard input and writing the recoded stream to standard output.
 * @details  This function reads a sequence of bytes from the standard
 * input and interprets it as digital audio according to the Sun audio
 * (.au) format.  A selected transformation (determined by the global variable
 * "global_options") is applied to the audio stream and the transformed stream
 * is written to the standard output, again according to Sun audio format.
 *
 * @param  argv  Command-line arguments, for constructing modified annotation.
 * @return 1 if the recoding completed successfully, 0 otherwise.
 */
int recode(char **argv) {

    int theOption = global_options >> 60;
    if(theOption == 4)//speed up
    {
        int theAnnotationBit = global_options >> 59;
        theAnnotationBit = theAnnotationBit & 1;

        int theFactor = global_options >> 48;
        theFactor = theFactor & 1023;
        theFactor = theFactor + 1; //account for the -1 we did

        if(theAnnotationBit == 1)//annotations left unmodified
        {
            AUDIO_HEADER theHeader;
            AUDIO_HEADER *ahp = &theHeader;
            if(read_header(ahp) == 0)// read header
            {
                return 0;
            }
            char *inputAnnotationP = input_annotation;
            //int *inputAnnotationIntP = (int*)inputAnnotationP;
            int *inputFrameIntP = (int*)input_frame;
            unsigned int annotationSize = (*ahp).data_offset - 24;//size of annotation = dataoffset - headersize(always 24)
            //read the annotation
            if(read_annotation(inputAnnotationP, annotationSize) == 0)
            {
                return 0;
            }
            //check bytes
            int audioDataSize = (*ahp).data_size;// number of bytes of audio data
            if(audioDataSize == 0xffffffff)//make sure there is no indefinite duration
            {
                return 0;
            }
            //get the bytes_per_sample
            int bytesASample = 0;
            if(((*ahp).encoding) == PCM8_ENCODING)
            {
                bytesASample = 1;
            }
            else
            {
                if(((*ahp).encoding) == PCM16_ENCODING)
                {
                    bytesASample = 2;
                }
                else
                {
                    if(((*ahp).encoding) == PCM24_ENCODING)
                    {
                        bytesASample = 3;
                    }
                    else
                   {
                        if(((*ahp).encoding) == PCM32_ENCODING)
                        {
                            bytesASample = 4;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            int frameSize = bytesASample * (*ahp).channels; //bytes in a frame
            if(audioDataSize % frameSize != 0)
            {
                return 0; // not multiple of frame size
            }
            int amountOfFrames = audioDataSize/frameSize;
            //calculate new datasize
            unsigned int newDataSizeCounter = 0;
            unsigned int dataSizeThrowableCounter = 0;
            while(dataSizeThrowableCounter < amountOfFrames)
            {
                if(dataSizeThrowableCounter % theFactor == 0)
                {
                    newDataSizeCounter++;
                }
                dataSizeThrowableCounter++;
            }
            (*ahp).data_size = newDataSizeCounter * frameSize;
            //write header
            if(write_header(ahp) == 0)// read header
            {
                return 0;
            }
            //write annotation
            if(write_annotation(inputAnnotationP, annotationSize) == 0)// write header
            {
                return 0;
            }
            //read frames
            int readFrameCounter = 0;
            while(readFrameCounter < amountOfFrames)
            {
                if(readFrameCounter % theFactor == 0)
                {
                    if(read_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                    {
                        return 0;
                    }
                    if(write_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                    {
                        return 0;
                    }
                }
                else
                {
                    if(read_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                    {
                        return 0;
                    }
                }
                readFrameCounter++;
            }
            //END OF FUNCTION
            return 1;
        }
        else
        {
            //-p bit is not set
            AUDIO_HEADER theHeader;
            AUDIO_HEADER *ahp = &theHeader;
            if(read_header(ahp) == 0)// read header
            {
                return 0;
            }
            char *inputAnnotationP = input_annotation;
            char *outputAnnotationP = output_annotation;
            int *inputFrameIntP = (int*)input_frame;
            unsigned int annotationSize = (*ahp).data_offset - 24;//size of annotation = dataoffset - headersize(always 24)
            //read the annotation
            if(read_annotation(inputAnnotationP, annotationSize) == 0)
            {
                return 0;
            }
            int argumentsBytes = read_arguments(outputAnnotationP, argv);
            int newAnnotationSize = append_annotation(inputAnnotationP, annotationSize, outputAnnotationP, argumentsBytes);
            //set the annotation in the header
            (*ahp).data_offset = newAnnotationSize + 24;
            //check bytes
            int audioDataSize = (*ahp).data_size;// number of bytes of audio data
            if(audioDataSize == 0xffffffff)//make sure there is no indefinite duration
            {
                return 0;
            }
            //get the bytes_per_sample
            int bytesASample = 0;
            if(((*ahp).encoding) == PCM8_ENCODING)
            {
                bytesASample = 1;
            }
            else
            {
                if(((*ahp).encoding) == PCM16_ENCODING)
                {
                    bytesASample = 2;
                }
                else
                {
                    if(((*ahp).encoding) == PCM24_ENCODING)
                    {
                        bytesASample = 3;
                    }
                    else
                   {
                        if(((*ahp).encoding) == PCM32_ENCODING)
                        {
                            bytesASample = 4;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            int frameSize = bytesASample * (*ahp).channels; //bytes in a frame
            if(audioDataSize % frameSize != 0)
            {
                return 0; // not multiple of frame size
            }
            int amountOfFrames = audioDataSize/frameSize;
            //calculate new datasize
            unsigned int newDataSizeCounter = 0;
            unsigned int dataSizeThrowableCounter = 0;
            while(dataSizeThrowableCounter < amountOfFrames)
            {
                if(dataSizeThrowableCounter % theFactor == 0)
                {
                    newDataSizeCounter++;
                }
                dataSizeThrowableCounter++;
            }
            (*ahp).data_size = newDataSizeCounter * frameSize;
            //write header
            if(write_header(ahp) == 0)// read header
            {
                return 0;
            }
            //write annotation
            if(write_annotation(outputAnnotationP,newAnnotationSize) == 0)// write header
            {
                return 0;
            }
            //read frames
            int readFrameCounter = 0;
            while(readFrameCounter < amountOfFrames)
            {
                if(readFrameCounter % theFactor == 0)
                {
                    if(read_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                    {
                        return 0;
                    }
                    if(write_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                    {
                        return 0;
                    }
                }
                else
                {
                    if(read_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                    {
                        return 0;
                    }
                }
                readFrameCounter++;
            }
            //END OF FUNCTION
            return 1;
        }
    }
    if(theOption == 2)//slow down
    {
        int theAnnotationBit = global_options >> 59;
        theAnnotationBit = theAnnotationBit & 1;

        int theFactor = global_options >> 48;
        theFactor = theFactor & 1023;
        theFactor = theFactor + 1; //account for the -1 we did

        if(theAnnotationBit == 1)//annotations left unmodified
        {
            AUDIO_HEADER theHeader;
            AUDIO_HEADER *ahp = &theHeader;
            if(read_header(ahp) == 0)// read header
            {
                return 0;
            }
            char *inputAnnotationP = input_annotation;
            int *previousFrameIntP = (int*)previous_frame;
            int *inputFrameIntP = (int*)input_frame;
            int *outputFrameIntP = (int*)output_frame;
            //int *inputAnnotationIntP = (int*)inputAnnotationP;
            unsigned int annotationSize = (*ahp).data_offset - 24;//size of annotation = dataoffset - headersize(always 24)
            //read the annotation
            if(read_annotation(inputAnnotationP, annotationSize) == 0)
            {
                return 0;
            }
            //check bytes
            int audioDataSize = (*ahp).data_size;// number of bytes of audio data
            if(audioDataSize == 0xffffffff)//make sure there is no indefinite duration
            {
                return 0;
            }
            //get the bytes_per_sample
            int bytesASample = 0;
            if(((*ahp).encoding) == PCM8_ENCODING)
            {
                bytesASample = 1;
            }
            else
            {
                if(((*ahp).encoding) == PCM16_ENCODING)
                {
                    bytesASample = 2;
                }
                else
                {
                    if(((*ahp).encoding) == PCM24_ENCODING)
                    {
                        bytesASample = 3;
                    }
                    else
                   {
                        if(((*ahp).encoding) == PCM32_ENCODING)
                        {
                            bytesASample = 4;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            int frameSize = bytesASample * (*ahp).channels; //bytes in a frame
            if(audioDataSize % frameSize != 0)
            {
                return 0; // not multiple of frame size
            }
            int amountOfFrames = audioDataSize/frameSize;
            int newAmountOfFrames = (amountOfFrames - 1) * (theFactor - 1);
            newAmountOfFrames = newAmountOfFrames + amountOfFrames;
            (*ahp).data_size = newAmountOfFrames * frameSize;
            //write header
            if(write_header(ahp) == 0)// read header
            {
                return 0;
            }
            //write annotation
            if(write_annotation(inputAnnotationP, annotationSize) == 0)// write header
            {
                return 0;
            }
            //read frames
            int readFrameCounter = 0;
            if(read_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
            {
                return 0;
            }
            while(readFrameCounter < amountOfFrames - 1)
            {
                //read_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding);
                if(read_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                {
                    return 0;
                }
                if(write_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                {
                    return 0;
                }
                if(slowDown_frame(previousFrameIntP, inputFrameIntP, outputFrameIntP, (*ahp).channels, (*ahp).encoding, theFactor) == 0)
                {
                    return 0;
                }
                copy_frame(inputFrameIntP, previousFrameIntP, (*ahp).channels, (*ahp).encoding);
                //write_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding);
                readFrameCounter++;
            }
            //read and write the last frame
            read_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding);
            write_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding);
            //END OF FUNCTION
            return 1;
        }
        else
        {
            //-p bit is not set
            AUDIO_HEADER theHeader;
            AUDIO_HEADER *ahp = &theHeader;
            if(read_header(ahp) == 0)// read header
            {
                return 0;
            }
            char *inputAnnotationP = input_annotation;
            char *outputAnnotationP = output_annotation;
            int *previousFrameIntP = (int*)previous_frame;
            int *inputFrameIntP = (int*)input_frame;
            int *outputFrameIntP = (int*)output_frame;
            unsigned int annotationSize = (*ahp).data_offset - 24;//size of annotation = dataoffset - headersize(always 24)
            //read the annotation
            if(read_annotation(inputAnnotationP, annotationSize) == 0)
            {
                return 0;
            }
            int argumentsBytes = read_arguments(outputAnnotationP, argv);
            int newAnnotationSize = append_annotation(inputAnnotationP, annotationSize, outputAnnotationP, argumentsBytes);
            //set the annotation in the header
            (*ahp).data_offset = newAnnotationSize + 24;
            //check bytes
            int audioDataSize = (*ahp).data_size;// number of bytes of audio data
            if(audioDataSize == 0xffffffff)//make sure there is no indefinite duration
            {
                return 0;
            }
            //get the bytes_per_sample
            int bytesASample = 0;
            if(((*ahp).encoding) == PCM8_ENCODING)
            {
                bytesASample = 1;
            }
            else
            {
                if(((*ahp).encoding) == PCM16_ENCODING)
                {
                    bytesASample = 2;
                }
                else
                {
                    if(((*ahp).encoding) == PCM24_ENCODING)
                    {
                        bytesASample = 3;
                    }
                    else
                   {
                        if(((*ahp).encoding) == PCM32_ENCODING)
                        {
                            bytesASample = 4;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            int frameSize = bytesASample * (*ahp).channels; //bytes in a frame
            if(audioDataSize % frameSize != 0)
            {
                return 0; // not multiple of frame size
            }
            int amountOfFrames = audioDataSize/frameSize;
            int newAmountOfFrames = (amountOfFrames - 1) * (theFactor - 1);
            newAmountOfFrames = newAmountOfFrames + amountOfFrames;
            (*ahp).data_size = newAmountOfFrames * frameSize;
            //write header
            if(write_header(ahp) == 0)// read header
            {
                return 0;
            }
            //write annotation
            if(write_annotation(outputAnnotationP,newAnnotationSize) == 0)// write header
            {
                return 0;
            }
            //read frames
            int readFrameCounter = 0;
            if(read_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
            {
                return 0;
            }
            while(readFrameCounter < amountOfFrames - 1)
            {
                //read_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding);
                if(read_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                {
                    return 0;
                }
                if(write_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                {
                    return 0;
                }
                if(slowDown_frame(previousFrameIntP, inputFrameIntP, outputFrameIntP, (*ahp).channels, (*ahp).encoding, theFactor) == 0)
                {
                    return 0;
                }
                copy_frame(inputFrameIntP, previousFrameIntP, (*ahp).channels, (*ahp).encoding);
                //write_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding);
                readFrameCounter++;
            }
            //read and write the last frame
            read_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding);
            write_frame(previousFrameIntP, (*ahp).channels, (*ahp).encoding);
            //END OF FUNCTION
            return 1;
        }
    }
    if(theOption == 1)//crypt
    {
        int theAnnotationBit = global_options >> 59;
        theAnnotationBit = theAnnotationBit & 1;
        int theKeyMask = 0;
        for(int i = 0; i < 32; i++)
        {
            theKeyMask = theKeyMask << 1;
            theKeyMask = theKeyMask + 1;
        }
        int theKey = global_options & theKeyMask;
        mysrand(theKey);

        if(theAnnotationBit == 1)//annotations left unmodified
        {
            AUDIO_HEADER theHeader;
            AUDIO_HEADER *ahp = &theHeader;
            if(read_header(ahp) == 0)// read header
            {
                return 0;
            }
            char *inputAnnotationP = input_annotation;
            //int *inputAnnotationIntP = (int*)inputAnnotationP;
            int *inputFrameIntP = (int*)input_frame;
            unsigned int annotationSize = (*ahp).data_offset - 24;//size of annotation = dataoffset - headersize(always 24)
            //read the annotation
            if(read_annotation(inputAnnotationP, annotationSize) == 0)
            {
                return 0;
            }
            //check bytes
            int audioDataSize = (*ahp).data_size;// number of bytes of audio data
            if(audioDataSize == 0xffffffff)//make sure there is no indefinite duration
            {
                return 0;
            }
            //get the bytes_per_sample
            int bytesASample = 0;
            if(((*ahp).encoding) == PCM8_ENCODING)
            {
                bytesASample = 1;
            }
            else
            {
                if(((*ahp).encoding) == PCM16_ENCODING)
                {
                    bytesASample = 2;
                }
                else
                {
                    if(((*ahp).encoding) == PCM24_ENCODING)
                    {
                        bytesASample = 3;
                    }
                    else
                   {
                        if(((*ahp).encoding) == PCM32_ENCODING)
                        {
                            bytesASample = 4;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            int frameSize = bytesASample * (*ahp).channels; //bytes in a frame
            if(audioDataSize % frameSize != 0)
            {
                return 0; // not multiple of frame size
            }
            int amountOfFrames = audioDataSize/frameSize;
            //write header
            if(write_header(ahp) == 0)// read header
            {
                return 0;
            }
            //write annotation
            if(write_annotation(inputAnnotationP, annotationSize) == 0)// write header
            {
                return 0;
            }
            //read frames
            int readFrameCounter = 0;
            while(readFrameCounter < amountOfFrames)
            {
                if(read_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                {
                    return 0;
                }
                xor_frame(inputFrameIntP, (*ahp).channels);
                if(write_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                {
                    return 0;
                }
                readFrameCounter++;
            }
            //END OF FUNCTION
            return 1;
        }
        else
        {
            //-p bit is not set
            AUDIO_HEADER theHeader;
            AUDIO_HEADER *ahp = &theHeader;
            if(read_header(ahp) == 0)// read header
            {
                return 0;
            }
            char *inputAnnotationP = input_annotation;
            char *outputAnnotationP = output_annotation;
            int *inputFrameIntP = (int*)input_frame;
            unsigned int annotationSize = (*ahp).data_offset - 24;//size of annotation = dataoffset - headersize(always 24)
            //read the annotation
            if(read_annotation(inputAnnotationP, annotationSize) == 0)
            {
                return 0;
            }
            int argumentsBytes = read_arguments(outputAnnotationP, argv);
            int newAnnotationSize = append_annotation(inputAnnotationP, annotationSize, outputAnnotationP, argumentsBytes);
            //set the annotation in the header
            (*ahp).data_offset = newAnnotationSize + 24;
            //check bytes
            int audioDataSize = (*ahp).data_size;// number of bytes of audio data
            if(audioDataSize == 0xffffffff)//make sure there is no indefinite duration
            {
                return 0;
            }
            //get the bytes_per_sample
            int bytesASample = 0;
            if(((*ahp).encoding) == PCM8_ENCODING)
            {
                bytesASample = 1;
            }
            else
            {
                if(((*ahp).encoding) == PCM16_ENCODING)
                {
                    bytesASample = 2;
                }
                else
                {
                    if(((*ahp).encoding) == PCM24_ENCODING)
                    {
                        bytesASample = 3;
                    }
                    else
                   {
                        if(((*ahp).encoding) == PCM32_ENCODING)
                        {
                            bytesASample = 4;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
            int frameSize = bytesASample * (*ahp).channels; //bytes in a frame
            if(audioDataSize % frameSize != 0)
            {
                return 0; // not multiple of frame size
            }
            int amountOfFrames = audioDataSize/frameSize;
            //write header
            if(write_header(ahp) == 0)// read header
            {
                return 0;
            }
            //write annotation
            if(write_annotation(outputAnnotationP,newAnnotationSize) == 0)// write header
            {
                return 0;
            }
            //read frames
            int readFrameCounter = 0;
            while(readFrameCounter < amountOfFrames)
            {
                if(read_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                {
                    return 0;
                }
                xor_frame(inputFrameIntP, (*ahp).channels);
                if(write_frame(inputFrameIntP, (*ahp).channels, (*ahp).encoding) == 0)
                {
                    return 0;
                }
                readFrameCounter++;
            }
            //END OF FUNCTION
            return 1;
        }
    }
    return 0;//not any of he available options
}

int read_header(AUDIO_HEADER *hp)
{
    unsigned int firstFieldTest = 0;
    int i = 1;
    while(i <= 4)
    {
        unsigned int theChar = getchar();
        if(theChar == EOF)
        {
            return 0;//endoffile
        }
        else
        {
            firstFieldTest = firstFieldTest << 8;
            firstFieldTest = firstFieldTest + theChar;
            i++;
        }
    }
    if(firstFieldTest == AUDIO_MAGIC) //first field correct
    {
        (*hp).magic_number = firstFieldTest;//set the magic-number
    }
    else
    {
        return 0;
    }
    firstFieldTest = 0;
    i = 1;
    while(i <= 4)
    {
        unsigned int theChar = getchar();
        if(theChar == EOF)
        {
            return 0;
        }
        else
        {
            firstFieldTest = firstFieldTest << 8;
            firstFieldTest = firstFieldTest + theChar;
            i++;
        }
    }
    if(firstFieldTest % 8 == 0) //second field correct
    {
        (*hp).data_offset = firstFieldTest;//set the data offset
    }
    else
    {
        return 0;
    }
    firstFieldTest = 0;
    i = 1;
    while(i <= 4)
    {
        unsigned int theChar = getchar();
        if(theChar == EOF)
        {
            return 0;
        }
        else
        {
            firstFieldTest = firstFieldTest << 8;
            firstFieldTest = firstFieldTest + theChar;
            i++;
        }
    }
    (*hp).data_size = firstFieldTest;//set the data offset
    firstFieldTest = 0;
    i = 1;
    while(i <= 4)
    {
        unsigned int theChar = getchar();
        if(theChar == EOF)
        {
            return 0;
        }
        else
        {
            firstFieldTest = firstFieldTest << 8;
            firstFieldTest = firstFieldTest + theChar;
            i++;
        }
    }
    if(firstFieldTest == PCM8_ENCODING) //third field correct
    {
        (*hp).encoding = PCM8_ENCODING;//set the enocoding
    }
    else
    {
        if(firstFieldTest == PCM16_ENCODING) //third field correct
        {
            (*hp).encoding = PCM16_ENCODING;//set the enocoding
        }
        else
        {
            if(firstFieldTest == PCM24_ENCODING) //third field correct
            {
                (*hp).encoding = PCM24_ENCODING;//set the enocoding
            }
            else
            {
                if(firstFieldTest == PCM32_ENCODING) //third field correct
                {
                    (*hp).encoding = PCM32_ENCODING;//set the enocoding
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    firstFieldTest = 0;
    i = 1;
    while(i <= 4)
    {
        unsigned int theChar = getchar();
        if(theChar == EOF)
        {
            return 0;
        }
        else
        {
            firstFieldTest = firstFieldTest << 8;
            firstFieldTest = firstFieldTest + theChar;
            i++;
        }
    }
    (*hp).sample_rate = firstFieldTest;//set the sample rate

    firstFieldTest = 0;
    i = 1;
    while(i <= 4)
    {
        unsigned int theChar = getchar();
        if(theChar == EOF)
        {
            return 0;
        }
        else
        {
            firstFieldTest = firstFieldTest << 8;
            firstFieldTest = firstFieldTest + theChar;
            i++;
        }
    }
    if(firstFieldTest == 1)
    {
        (*hp).channels = 1;//set the channel
    }
    else
    {
        if(firstFieldTest == 2)
        {
            (*hp).channels = 2;//set the channel
        }
        else
        {
            return 0; //invalid channel
        }
    }

return 1;
}

int write_header(AUDIO_HEADER *hp)
{
    int theChar = 0;
    while(theChar != EOF)
    {
        theChar = putchar((*hp).magic_number >> 24);
        theChar = putchar((*hp).magic_number >> 16);
        theChar = putchar((*hp).magic_number >> 8);
        theChar = putchar((*hp).magic_number);

        theChar = putchar((*hp).data_offset >> 24);
        theChar = putchar((*hp).data_offset >> 16);
        theChar = putchar((*hp).data_offset >> 8);
        theChar = putchar((*hp).data_offset);

        theChar = putchar((*hp).data_size >> 24);
        theChar = putchar((*hp).data_size >> 16);
        theChar = putchar((*hp).data_size >> 8);
        theChar = putchar((*hp).data_size);

        theChar = putchar((*hp).encoding >> 24);
        theChar = putchar((*hp).encoding >> 16);
        theChar = putchar((*hp).encoding >> 8);
        theChar = putchar((*hp).encoding);

        theChar = putchar((*hp).sample_rate >> 24);
        theChar = putchar((*hp).sample_rate >> 16);
        theChar = putchar((*hp).sample_rate >> 8);
        theChar = putchar((*hp).sample_rate);

        theChar = putchar((*hp).channels >> 24);
        theChar = putchar((*hp).channels >> 16);
        theChar = putchar((*hp).channels >> 8);
        theChar = putchar((*hp).channels);
        return 1;
    }
    return 0;
}

int read_arguments(char *ap, char **argv)
{
    unsigned int amountOfBytes = 0;
    int argvCounter = 0;
    int stringCounter = 0;
    while(theArgumentNumber > 0)
    {
        while(**argv != '\0')
        {
            *ap = **argv;//place char in array
            ap++; //move array up
            amountOfBytes++;//increment bytes
            *argv = *argv + 1;//go to next character
            argvCounter++;
        }
        if(theArgumentNumber != 1)
        {
            *ap = ' ' + 0;
            ap++;
            amountOfBytes++;
        }
        argv++;//go to next string
        stringCounter++;
        theArgumentNumber--;
    }
    *ap = '\n' + 0;
    ap++;
    amountOfBytes++;
    argv = argv - stringCounter;
    *ap = *ap - amountOfBytes;
    return amountOfBytes;
}

int read_annotation(char *ap, unsigned int size)
{
    int nullterminatorBool = 0;
    int i = 0;
    if(size == 0)
    {
        return 1;
    }
    while(i < size)
    {
        unsigned int theChar = getchar();
        if(theChar != EOF)
        {
            if(theChar != '\0' && nullterminatorBool == 0)
            {
                *ap = theChar;
                ap++;
                i++;
            }
            if(theChar == '\0' && nullterminatorBool == 1)
            {
                *ap = theChar;
                ap++;
                i++;
            }
            if(theChar != '\0' && nullterminatorBool == 1)
            {
                return 0;
            }
            if(theChar == '\0' && nullterminatorBool == 0)
            {
                nullterminatorBool = 1;
                *ap = theChar;
                ap++;
                i++;
            }
        }
        else
        {
            return 0;//end of file
        }
    }
    if(nullterminatorBool == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//append the first array ot the second
int append_annotation(char *ap, unsigned int size, char *ap2, unsigned int size2)
{
    ap2 = ap2 + size2;
    if(size != 0)
    {
        int i = 0;
        while(*ap != '\0')
        {
            *ap2 = *ap;
            ap2++;
            ap++;
            i++;
        }
        *ap2 = '\0';
        ap2++;
        i++;
        int newAnnotationSize = size2 + i;
        while(newAnnotationSize % 8 != 0)
        {
            *ap2 = '\0';
            ap2++;
            newAnnotationSize++;
        }
        *ap2 = *ap2 - newAnnotationSize;
        return newAnnotationSize;
    }
    else
    {
        *ap2 = '\0';
        ap2++;
        int newAnnotationSize = size2 + 1;
        while(newAnnotationSize % 8 != 0)
        {
            *ap2 = '\0';
            ap2++;
            newAnnotationSize++;
        }
        *ap2 = *ap2 - newAnnotationSize;
        return newAnnotationSize;
    }
}

int write_annotation(char *ap, unsigned int size)
{
    int theChar = 0;
    while(theChar != EOF && size > 0)
    {
        theChar = putchar(*ap);
        ap++;
        size--;
    }
    if(size == 0 && theChar != EOF)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int read_frame(int *fp, int channels, int bytes_per_sample)
{
    //bytes_per_sample = bytes_per_sample - 1;//get the REAL bytes_per_sample
    if(bytes_per_sample == PCM8_ENCODING)
    {
        bytes_per_sample = 1;
    }
    else
    {
        if(bytes_per_sample == PCM16_ENCODING)
        {
            bytes_per_sample = 2;
        }
        else
        {
            if(bytes_per_sample == PCM24_ENCODING)
            {
                bytes_per_sample = 3;
            }
            else
            {
                if(bytes_per_sample == PCM32_ENCODING)
                {
                    bytes_per_sample = 4;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    for(int i = 0; i < channels; i++)
    {
        int readFrameIn = 0;
        for(int j = 0; j < bytes_per_sample; j++)
        {
            int theChar = getchar();
            if(theChar == EOF)
            {
                return 0;//end of file
            }
            readFrameIn = readFrameIn << 8;
            readFrameIn = readFrameIn + theChar;
        }
        *fp = readFrameIn;
        fp++;
    }
    return 1;
}

int write_frame(int *fp, int channels, int bytes_per_sample)
{
    //bytes_per_sample = bytes_per_sample - 1;//get the REAL bytes_per_sample
    if(bytes_per_sample == PCM8_ENCODING)
    {
        bytes_per_sample = 1;
    }
    else
    {
        if(bytes_per_sample == PCM16_ENCODING)
        {
            bytes_per_sample = 2;
        }
        else
        {
            if(bytes_per_sample == PCM24_ENCODING)
            {
                bytes_per_sample = 3;
            }
            else
            {
                if(bytes_per_sample == PCM32_ENCODING)
                {
                    bytes_per_sample = 4;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    for(int i = 0; i < channels; i++)
    {
        for(int j = bytes_per_sample - 1; j >= 0; j--)
        {
            int theChar = putchar(*fp >> (8*j));
            if(theChar == EOF)
            {
                return 0;//end of file
            }
        }
        fp++;
    }
    return 1;
}

int xor_frame(int *fp, int channels)
{
    for(int i = 0; i < channels; i++)
    {
        int theObfuscater = myrand32();
        *fp = *fp ^ theObfuscater;
        fp++;
    }
    return 1;
}
// fp = previous frame fp2 = inputframe fp3 = outputframe
int slowDown_frame(int *fp, int *fp2, int *fp3, int channels, int bytes_per_sample, int factor)
{
        for(int j = 1; j < factor; j++)
        {
            for(int k = 0; k < channels; k++)
            {
                //convert to twos complement
                int frameSampleOne = *fp;
                int frameSampleTwo = *fp2;
                if(convertTwosComplement(frameSampleOne, bytes_per_sample) == 1)
                {
                    frameSampleOne = ~frameSampleOne;
                    frameSampleOne = frameSampleOne + 1;
                    int example = 0;
                    int exCounter = 0;
                    if(bytes_per_sample == PCM8_ENCODING)
                    {
                        while(exCounter < 8)
                        {
                            example = example << 1;
                            example = example + 1;
                            exCounter++;
                        }
                    }
                    else
                    {
                        if(bytes_per_sample == PCM16_ENCODING)
                        {
                            while(exCounter < 16)
                        {
                            example = example << 1;
                            example = example + 1;
                            exCounter++;
                        }
                        }
                        else
                        {
                            if(bytes_per_sample == PCM24_ENCODING)
                            {
                                while(exCounter < 24)
                                {
                                    example = example << 1;
                                    example = example + 1;
                                    exCounter++;
                                }
                            }
                            else
                            {
                                if(bytes_per_sample == PCM32_ENCODING)
                                {
                                    while(exCounter < 32)
                                    {
                                        example = example << 1;
                                        example = example + 1;
                                        exCounter++;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                    frameSampleOne = frameSampleOne & example;
                    frameSampleOne = frameSampleOne * -1;
                }

                if(convertTwosComplement(frameSampleTwo, bytes_per_sample) == 1)
                {
                    frameSampleTwo = ~frameSampleTwo;
                    frameSampleTwo = frameSampleTwo + 1;
                    int example = 0;
                    int exCounter = 0;
                    if(bytes_per_sample == PCM8_ENCODING)
                    {
                        while(exCounter < 8)
                        {
                            example = example << 1;
                            example = example + 1;
                            exCounter++;
                        }
                    }
                    else
                    {
                        if(bytes_per_sample == PCM16_ENCODING)
                        {
                            while(exCounter < 16)
                        {
                            example = example << 1;
                            example = example + 1;
                            exCounter++;
                        }
                        }
                        else
                        {
                            if(bytes_per_sample == PCM24_ENCODING)
                            {
                                while(exCounter < 24)
                                {
                                    example = example << 1;
                                    example = example + 1;
                                    exCounter++;
                                }
                            }
                            else
                            {
                                if(bytes_per_sample == PCM32_ENCODING)
                                {
                                    while(exCounter < 32)
                                    {
                                        example = example << 1;
                                        example = example + 1;
                                        exCounter++;
                                    }
                                }
                                else
                                {
                                    return 0;
                                }
                            }
                        }
                    }
                    frameSampleTwo = frameSampleTwo & example;
                    frameSampleTwo = frameSampleTwo * -1;
                }
                *fp3 = frameSampleOne + ((frameSampleTwo - frameSampleOne) * j)/factor;
                fp++;
                fp2++;
                fp3++;
            }
            fp3 = fp3 - 2;
            if(write_frame(fp3, channels, bytes_per_sample) == 0)
            {
                return 0;
            }
            fp = fp - 2;
            fp2 = fp2 - 2;
        }
        return 1;
}

//moves content of *fp to *fp2
int copy_frame(int *fp, int *fp2, int channels, int bytes_per_sample)
{
    if(bytes_per_sample == PCM8_ENCODING)
    {
        bytes_per_sample = 1;
    }
    else
    {
        if(bytes_per_sample == PCM16_ENCODING)
        {
            bytes_per_sample = 2;
        }
        else
        {
            if(bytes_per_sample == PCM24_ENCODING)
            {
                bytes_per_sample = 3;
            }
            else
            {
                if(bytes_per_sample == PCM32_ENCODING)
                {
                    bytes_per_sample = 4;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    for(int i = 0; i < channels; i++)
    {
        int readFrameIn = 0;
        for(int j = 0; j < bytes_per_sample; j++)
        {
            readFrameIn = readFrameIn << 8;
            readFrameIn = readFrameIn + *fp;
        }
        *fp2 = readFrameIn;
        fp2++;
        fp++;
    }
    return 1;
}

int convertTwosComplement(int theChar, int bytes_per_sample)
{
    //int mostSignificantBit = 1;
    if(bytes_per_sample == PCM8_ENCODING)
    {
        theChar = theChar >> 7;
        theChar = theChar & 1;
    }
    else
    {
        if(bytes_per_sample == PCM16_ENCODING)
        {
            theChar = theChar >> 15;
            theChar = theChar & 1;
        }
        else
        {
            if(bytes_per_sample == PCM24_ENCODING)
            {
                theChar = theChar >> 23;
                theChar = theChar & 1;
            }
            else
            {
                if(bytes_per_sample == PCM32_ENCODING)
                {
                    theChar = theChar >> 31;
                    theChar = theChar & 1;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    if(theChar == 1)
    {
        return 1;
    }
    return 0;
}

