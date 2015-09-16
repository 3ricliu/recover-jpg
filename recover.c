/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 *Author: Eric Liu
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void)
{
    //Open memory card
    FILE* infile = fopen("card.raw", "r");

    //Create new unsigned char 8 bit array, with space for 512 elements
    //Will be used to temporarily hold what is read from infile before deciding what to do with it
    uint8_t buffer[512];

    //Creating a .jpg number counter
    int jpg = 0; 
    
    //Create floodgate, currently set to close. Controls what's copied into new jpg, and current jpg file open
    int floodgate = 0;

    //Creating pointer to title, with a memory of 4 bytes
    char* title = malloc((sizeof(int)*3 + sizeof(char)*3));

    //Open new file, titled 000.jpg, for writing only
    FILE* img = fopen("000.jpg", "a");

    //While scanline can read buffer, keep reading
    while(fread(&buffer, sizeof(buffer), 1, infile)) 
    {
    	//If statement to test if buffer element is the beginning of a new jpg
    	if(buffer[0]==0xff && buffer[1]==0xd8 && buffer[2]==0xff && (buffer[3]==0xe0 || buffer[3]==0xe1))
    	{
    		if(floodgate ==1) //If there's already a jpg open
    		{
    			fclose(img); //Close current jpg
    			jpg++; //Increment to next file
    			//Next ifelse statement determines how to format name of new jpg to be opened
    			if(jpg<10)
    			{
    				sprintf(title, "00%d.jpg", jpg);
    			} 
    			else
    			{
    				sprintf(title, "0%d.jpg", jpg);
    			}

    			//Opens new jpg file
    			img = fopen(title,"a");
    		}

    		floodgate = 1; //opens the floodgates up after 1st jpg in the file is detected
    	}

    	//If floodgate is open, start writing into currently pened jpg
	    if(floodgate ==1)
	    {
	    	fwrite(&buffer, sizeof(buffer), 1, img);
	    }
    }

    //free up memory that was allocated during execution
    free(title);
    fclose(img);
    fclose(infile);

}
