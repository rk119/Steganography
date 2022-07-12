# Image Steganography in C.

## Program design

This program in steg.c serves the purpose of an image steganography by allowing the user to encode their private text message or information into an image with the ppm extension without making the hidden message obvious. The program also allows the user to decode the hidden message. The following are the descriptions of the methods implemented in the program:

- minAndMax(): This is a helper function which compares the first two integer arguments and assigns the maximum and minimum values to the last two arguments respectively.

- main(): This is where the program runs in the relevent order of the function calls along with the execution of other commands. It consists of conditions to check the 2nd argument. The 't' mode runs the condition for testing the getPPM() and showPPM() methods and check whether the image ppm file in the 3rd argument is copied into the output ppm where it is redirected into. The 'e' mode executes the code in the condition for encoding. The user is asked (mentions the limit of characters as well depending on the dimensions of the image) to type a message in the terminal and the program scans the message into a char array to store the string and pass it into the encode() method. The 'd' mode takes in two input ppm files to compare and extract the hidden message as an output onto the terminal. The method fprintf is used to output text onto the terminal as an error to avoid it from redirecting into another file. Errors are handled accordingly and terminates the program if encountered.

- encode(): This method has two parameters - the text to encode, and the image file to perform encoding in. The image ppm parameter is assigned to a new PPM struct 'encodedImg' which is declared first, else returns Null if the contents of the input ppm isnt copied. The number of iterations of the for loop is the total number of characters of the string which is its length. In each loop, a random number is generated between 1 to a limit (which is calculated using the ppm's width and height). This random number is added into the accumalator which acts like the index of the selected red pixel. This is followed by a condition to check if the red pixel value is not the same as the character ascii value. If not, the character value is assigned to the red pixel. Else if the red pixel and character values are the same, a new random number for the current character is generated and added to accumalator. Finally, the new encoded ppm image is returned.

- decode(): The parameters of this method is the original, non-tampered image ppm, and the new encoded image ppm. Null is returned if either one of them has faulty content or nothing in them. A temporary message character array is declared and malloc is used to allocate the dimension value (total number of pixels) into the memory. The loop iterates through every pixel and comapres the red pixels of the old image with the red pixels of the new image. If they are not the same, the red pixel value in the new image is assigned to the temporary message character-array in its repective position. A new string character array is declared as the 'decodedMessage' with malloc allocating the exact length of the message to the memory. The for loop iterates through each character in the temporary decoded message and assigns it with the same position in the decoded message. The memory of the temporary message is deallocated and the decoded message is returned.

- getPPM(): Takes in an input ppm file as its parameter. It first checks if the format of the file passed is in the P3 format else returns Null. Then a new 'getPPMImg' struct is declared to store the values of the input ppm file. Malloc is used by passing in the predefied PPM struct to allocate the memory for the variables which will store the data of the file accordingly, else returns Null if memory allocation fails. This is preceeded by the lookup for comments which is implemented by the LinkedList struct. Furthermore, the width, height and max values are read from the file and stored in getPPMImg struct variables. Malloc is used by passing in the predefied Pixel struct to allocate the memory for each pixel. A for loop is used to iterate through the pixels and store each pixel in a 1-dimentional array with its RGB values. Finally, the new PPM struct with the copied data from the file is returned.

- showPPM(): Takes in a PPM struct as its parameter. The ppm type is sent to the standard output. The nodes of the comments linked list is traversed through and in every loop the comment is printed as an output, along with deallocating the memory for each comment. The width, height and max values are printed on the terminal followed by a for loop accessing each pixel and printing its RGB values to the standard output.

## Choice of data structures and algorithms

- structs: 3 struct objects were used in this program. The first one is the Pixel struct which stores the RGB values of a pixel as positive integers. The second struct is the LinkedList struct which is used as a linked list to store comments and the third one is PPM struct used to create a new image ppm by storing its values in the struct's predefined variables.

- arrays: 1 dimensional array is used to store each pixel with its RGB values of the image ppm. An array of characters was used for storing the user's string message from the stdin, and by the decode() method to store the temporary and decoded message.

- LinkedList: Each comment line from the file is stored in a node with a pointer to the next node or Null if its the tail.
  Visual representation of the implemented linked list:

##### | Comment 1 | ---> | Comment 2 | ---> | NULL |

- The encode() and decode() methods both have the time complexity of O(N).
