#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The RGB values of a pixel. */
struct Pixel {
    unsigned int red, green, blue;
};

/* Comments from a PPM file stored in a linked list */
struct LinkedList {
    char *comment_node;
    struct LinkedList *next_node;
};

/* An image loaded from a PPM file. */
struct PPM {
    /* Question 1 */
    struct LinkedList *comments;
    unsigned int width, height, max;
    struct Pixel *pixel;
};

/* Reads an image from an open PPM file.
 * Returns a new struct PPM, or NULL if the image cannot be read. */
struct PPM *getPPM(FILE * f)
{
    /* Question 2a */

    /* Check if the input ppm file is in the P3 format */
    char ppmType[2];
    fscanf(f, "%2s\n", ppmType); // reads the first line of the file
    if(ppmType[0] != 'P' && ppmType[1] != '3') {
        fprintf(stderr, "PPM type needs to be P3");
        return NULL;
    }

	struct PPM *getPPMImg = (struct PPM*) malloc(sizeof(struct PPM));
    /* Return NULL if there is an issue with memory allocation */
    if(!getPPMImg) {
        return NULL;
    }

    /* Question 4 */
    /* Check for comments in the ppm file and store it in a linked list */
    struct LinkedList *currentNode = (struct LinkedList*) malloc(sizeof(struct LinkedList));
    getPPMImg->comments = currentNode; // head
    char character = getc(f); // get the first character of the line
    while(character == '#'){ 
        ungetc(character, f);
        char *comment = NULL;
        size_t commentLength;
        getline(&comment, &commentLength , f); // get the comment line from the file
        currentNode->comment_node = comment; // store the comment in the current node
        currentNode->next_node = (struct LinkedList*) malloc(sizeof(struct LinkedList));
        currentNode = currentNode->next_node; // set current node to next node
        currentNode->next_node = NULL; // set next node (possibly tail if its the end) to null
        character = getc(f); // get the first character of the next line
    }
    ungetc(character, f);

    /* Reads the next two lines and sets the width, height and max value accordingly */
	fscanf(f, "%i %i\n", &getPPMImg->width, &getPPMImg->height);	
	fscanf(f, "%i\n", &getPPMImg->max);

	getPPMImg->pixel = (struct Pixel*) malloc((getPPMImg->height * getPPMImg->width) * sizeof(struct Pixel));

    /* Reads every consequent line from the file with RGB values for a pixel and sets the image struct ppm RGB values accordingly for each pixel */
	for(int j = 0; j < (getPPMImg->height * getPPMImg->width); j++){
			fscanf(f, "%i %i %i", &getPPMImg->pixel[j].red, &getPPMImg->pixel[j].green, &getPPMImg->pixel[j].blue);		
	}

	return  getPPMImg;
}

/* Write img to stdout in PPM format. */
void showPPM(const struct PPM *img)
{
    /* Question 2b */

    // output the ppm type to the terminal
	printf("P3\n");

    /* Get the comments from the image ppm and loop until the tail has been reached */
    struct LinkedList *comment = img->comments;
    struct LinkedList *freeComment;
	while (comment->next_node != NULL) {
        // output the comment in each loop
        freeComment = comment; // assign current comment to a temporary variable
		printf("%s",comment->comment_node);
		comment = comment->next_node;
        free(freeComment); // deallocate the memory
	}

    /* Output the img ppm width, height and maximum value */
    printf("%i %i\n%i\n", img->width, img->height, img->max);

    /* Output each pixel's RGB values  */
	for(int j = 0; j < (img->height * img->width); j++){
			printf("%i %i %i\n", img->pixel[j].red, img->pixel[j].green, img->pixel[j].blue);	
	}
}

/* Opens and reads a PPM file, returning a pointer to a new struct PPM.
 * On error, prints an error message and returns NULL. */
struct PPM *readPPM(const char *filename)
{
    /* Open the file for reading */
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }

    /* Load the image using getPPM */
    struct PPM *img = getPPM(f);
    /* Close the file */
    fclose(f);

    if (img == NULL) {
        fprintf(stderr, "File %s could not be read.\n", filename);
        return NULL;
    }
    return img;
}

/* Helper function */
void minAndMax(int a, int b, int *min, int *max) {
    *max = (a > b) ? a : b;
    *min = (a > b) ? b : a;
}

/* Encode the string text into the red channel of image img.
 * Returns a new struct PPM, or NULL on error. */
struct PPM *encode(const char *text, const struct PPM *img)
{
    /* Question 2c */

	struct PPM *encodedImg = img;

    /* Return NULL if the new image ppm is faulty */
    if(!encodedImg) {
        return NULL;
    }

	unsigned int count, min, max;
    unsigned int randomNum = 0;
    
    minAndMax(img->height, img->width, &min, &max);

    for(count = 0; count < strlen(text); count++) {
        randomNum = randomNum + rand()%((max/2) - (min/2)+1)+1; // accumalator
        // condition to check if the red pixel value is not the same as the character value
        if(encodedImg->pixel[randomNum].red != text[count]) {
            // encode the character into the red pixel
            encodedImg->pixel[randomNum].red = text[count];
        } else {
            // else repeat the loop for the character value and get a new red pixel to encode in
            count--;
        }
    }
    return encodedImg;
}

/* Extract the string encoded in the red channel of newimg, by comparing it
 * with oldimg. The two images must have the same size.
 * Returns a new C string, or NULL on error. */
char *decode(const struct PPM *oldimg, const struct PPM *newimg)
{
    /* Question 2d */

    /* Return NULL if the original input ppm or the encoded input ppm is faulty */
    if(!oldimg || !newimg) {
        return NULL;
    }

	unsigned int dimensions = newimg->height * newimg->width;
    char *tempMessage = (char*) malloc(dimensions * sizeof(char));
    unsigned int charPos = 0; // character position

    /* Iterate through the pixels of the image to decode the message */
    for(int count = 0; count < dimensions; count++){
        // condition to find a character in the red pixel
        if(oldimg->pixel[count].red != newimg->pixel[count].red){
            tempMessage[charPos] = newimg->pixel[count].red; // assign the character from the pixel to the temperory message holder
            charPos++;
        }
    }

    // memory allocation of the length of the string
    char *decodedMessage = (char*) malloc(charPos *sizeof(char));
    // assigns the characters of the decoded message
    for(int count=0; count<charPos; count++) {
        decodedMessage[count] = tempMessage[count];
    }

    free(tempMessage);
    decodedMessage[charPos] = '\0'; // indicating the end of the string
    return decodedMessage;
}

/* Question 3 */
int main(int argc, char *argv[])
{
    /* Initialise the random number generator, using the time as the seed */
    srand(time(NULL));

    /* Parse command-line arguments */
    if (argc == 3 && strcmp(argv[1], "t") == 0) {
        /* Mode "t" - test PPM reading and writing */

        struct PPM *img = readPPM(argv[2]);

        // exit the program if any error occured in reading the input ppm file
        if(img == NULL) {
            return 0;
        }

        // writes the image ppm to stdout
        showPPM(img);

    } else if (argc == 3 && strcmp(argv[1], "e") == 0) {
        /* Mode "e" - encode PPM */

        struct PPM *oldimg = readPPM(argv[2]);

        // exit the program if any error occured in reading the input ppm file
        if(oldimg == NULL) {
            return 0;
        }

        /* Prompt for a message from the user, and read it into a string */
        unsigned int min, max;
        minAndMax(oldimg->height, oldimg->width, &min, &max);
		char *message = (char*) malloc((max*min)/((max/2) - (min/2)+1) * sizeof(char));
        fprintf(stderr, "Type a message you would like to encode not more than %i characters:\n-> ", ((max*min)/((max/2) - (min/2)+1)-min));
		fgets(message, (max*min)/((max/2) - (min/2)+1), stdin);

        /* Encode the text into the image with encode(), and assign it to newimg */
        struct PPM *newimg = encode(message, oldimg);

        // output an error message if any error occured while encoding and exit the program
        if(newimg == NULL) {
            fprintf(stderr, "Message cannot be encoded");
            return 0;
        }

        // write the image to stdout with showPPM
        showPPM(newimg);
        free(message);

    } else if (argc == 4 && strcmp(argv[1], "d") == 0) {
        /* Mode "d" - decode PPM */

        /* Get original file filename from argv, load it with
           readPPM, then assign to oldimg */
        struct PPM *oldimg = readPPM(argv[2]);

        // exit the program if any error occured in reading the original input ppm file
        if(oldimg == NULL) {
            return 0;
        }
        
        /* Get encoded file filename from argv, load it with
           readPPM, then assign to newimg */
        struct PPM *newimg = readPPM(argv[3]);

        // exit the program if any error occured in reading the encoded input ppm file
        if(newimg == NULL) {
            return 0;
        }

        /* Decode the encodedPPM with the comparisonPPM and assign to message */
        char *message = decode(oldimg, newimg);

        // output an error if any error occured in decoding the message and exit the program
        if(message == NULL) {
            fprintf(stderr, "Message cannot be decoded");
            return 0;
        }

        // print the decoded message to stdout
        fprintf(stderr, "Your message:\n");
        fprintf(stderr, "-> %s\n", message);
        free(message);

    } else {
        fprintf(stderr, "Unrecognised or incomplete command line.\n");
        return 1;
    }
    return 0;
}