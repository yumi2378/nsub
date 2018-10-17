#include <stdio.h>
#include <string.h>

#define true 1
#define false 0

//Workaround for characters not displaying properly in terminal
int isRedirected()
{
	int isatty();
	if (!isatty(fileno(stdout)))
    {
        return 1;
    }
    return 0;
}
void encryption(int nybbles[], int keyNybbles[], int bytes_read)
{
/*Encrypts the text. Nybbles and keyNybbles contain both the message and the key split into nybbles rather than bytes. The decimal value of the nybble from the text is "swapped" with the value at that index of the keyNybbles array.*/
	
	int encryptedText[2*bytes_read];
	for(int i=0; i < (2*bytes_read); i++)
 	{
 		int n = nybbles[i];
 		int value = keyNybbles[n]; 
 		
 		encryptedText[i] = value; 				
 	}
 	
 	   
    //Splice nibbles back together
    for(int i=0; i < 2*bytes_read-1; i+=2)
    {
    	//even index is upper nybble, odd is lower
    	int temp0 = encryptedText[i];
    	int temp1 = encryptedText[i+1];
    	
    	temp0 = temp0 << 4;
    	int valueX = temp0|temp1;
    	if(isRedirected() == 1)
    	{
    		printf("%c", valueX);
    	}
    	else
    	{
    		printf("%02x ", valueX);
    	}
    }
    if(isRedirected() == 0)
    {
 		printf("\n");
 	}
}  
void decryption(int nybbles[], int keyNybbles[], int bytes_read)
{
	int decryptedText[2*bytes_read];
	
	for(int i=0; i < 16; i++)
	{
		int temp = keyNybbles[i];
		for(int j=0; j < 2*bytes_read; j++)
		{
			if(nybbles[j] == temp)
			{
				decryptedText[j] = i;
			}
		}
	}
	//Splice nybbles back together
    for(int i=0; i < 2*bytes_read-1; i+=2)
    {
    	//even index is upper nybble, odd is lower
    	int temp0 = decryptedText[i];
    	int temp1 = decryptedText[i+1];
    	
    	temp0 = temp0 << 4;
    	int valueX = temp0|temp1;
    	if(isRedirected() == 1)
    	{
    		printf("%c", valueX);
    	}
    	else
    	{
    		printf("%c", valueX);
    	}
    }
    if(isRedirected() == 0)
    {
 		printf("\n");
 	}
}

//Borrowed from the provided "disp" source code for use when converting hex to binary. Not my code.
int convertASCIIHexToInt(char c) { 
    if(c >=65 && c <= 70) { // c is a uppercase letter
        return c-55;
    } else if(c >=97 && c <= 102) { // c is a lowercase letter
        return c-87;
    } else if(c >=48 && c <= 57) { // c is a number
        return c-48;
    } else { // Not a hex digit
        return -1;
    }
}	

int convertEncryptedText(int n)
{
	switch(n)
	{
		case 48: return 0;
		case 49: return 1;
		case 50: return 2;
		case 51: return 3;
		case 52: return 4;
		case 53: return 5;
		case 54: return 6;
		case 55: return 7;
		case 56: return 8;
		case 57: return 9;
		case 97: return 10;
		case 98: return 11;
		case 99: return 12;
		case 100: return 13;
		case 101: return 14;
		case 102: return 15;
		default: return -1;
	}
}
	
int main(int argc, char **argv)
{
	int encrypt = true;
	int decrypt = false;
	int hex = false;
	int bin = true;
	char *keyfile = "NULL";
	void exit();

	for (int i = 1; i < argc; i++)
	{
		//Checking for decrypt or hex
		if(strcmp(argv[i], "-d") == 0) 
		{
			decrypt = true;
			encrypt = false;
		}
		if(strcmp(argv[i], "-x") == 0) 
		{
			hex = true;
			bin = false;
		}
		if(strcmp(argv[i], "-k") == 0)
		{
			if(i+1 > argc-1)
			{
				printf("Error, missing keyfile.\n");
				exit(1);
			}
			else
			{
				keyfile = argv[i+1];
			}
		}
	}
	
	if(strcmp(keyfile, "NULL") == 0)
	{
		printf("Error, missing keyfile.\n");
		exit(1);
	}
	
	//Processing the text file
	freopen(NULL, "rb", stdin);
	unsigned char buf[102400]; // Unsigned char buffer to prevent problems with printf
    size_t bytes_read = fread(buf, 1, sizeof(buf), stdin); // Read the file from stdin
    int nybbles[2*bytes_read]; //Store the text in Nybbles
    int j = 0;

 	for(int i=0; i < bytes_read; i++) 
  	{
   		int temp = buf[i];
   		//Split the bytes into nybbles
   		int lower = (temp >>  0) & 15;
   		int upper = (temp >>  4) & 15;
		nybbles[j] = upper;
		nybbles[j+1] = lower;
		j = j+2;    	 		    		
   	}
 
    unsigned char buffer[102400];
    int keyNybbles[16]; //Stores the key in Nybbles
    //If the key is in binary
    if(bin == true)
    {
    	//Read the key
    	FILE *key;
    	key = fopen(keyfile, "rb");
    	
    	if(!key)
    	{
    		printf("Key file not found.\n");
    		exit(1);
    	}
    	
    	size_t key_size = fread(buffer, 1, sizeof(buffer), key);
    	
    	if(key_size > 8)
    	{
    		printf("Error, invalid key.\n");
    		exit(1);
    	}
    	
    	//Split bytes into nybbles
    	j = 0;
    	for(int i = 0; i < 8; i++)
    	{
    		int temp = buffer[i];
    		int upper = (temp >>  4) & 15;
    		int lower = (temp >>  0) & 15;
    		keyNybbles[j] = upper;
    		keyNybbles[j+1] = lower;
    		j = j+2;
    	}	 
    }
    
    //If the key is in ASCII coded hex
    else if(hex == true)
    {
    	//Read the key
    	int hexKey[8]; //For storage of converted hex values
    	FILE *key;
    	key = fopen(keyfile, "r");
    	
    	if(!key)
    	{
    		printf("Key file not found.\n");
    		exit(1);
    	}
    	size_t key_size = fread(buffer, 1, sizeof(buffer), key);
    	int ascii_coded_hex = true;
    	int k = 0;

    	if(key_size > 24)
    	{
    		printf("Error, invalid key.\n");
    		exit(1);
    	}

    	/*I do not claim credit for the following code. It was provided with the materials for 			the assignment in the "disp" source code. Since it was provided with the assignment, I 			assumed it would be acceptable to edit it and use it in my program.*/
    	
    	//Check that file contains only ASCII characters
        for(int i=0; i < bytes_read; i++)
        {
            if(buffer[i] > 102) 
            { 
                ascii_coded_hex = false;
                break;
            } 
            else if(buffer[i] > 70) 
            { 
                if(buffer[i] < 97) 
                {
                    ascii_coded_hex = false;
                    break;
                }
            } 
            else if(buffer[i] > 57) 
            { 
                if(buffer[i] < 65) 
                { 
                    ascii_coded_hex = false;
                    break;
                }
            } 
            else if(buffer[i] < 48 && buffer[i] != 32 && buffer[i] != 10) 
            { 
                ascii_coded_hex = false;
                break;
            }
        }
    	if(ascii_coded_hex == true)
    	{
    		for(int i=0; i < 22; i++) 
    		{
        	    int done = false;
        	    // Check for whitespace or newline, and skip it until a hex digit is found
        	    while(buffer[i] == 32 || buffer[i] == '\n') 
        	    {
        	        i++;
        	        if(i > 24) 
        	        {
        	            done = true;
        	            break;
        	        }
        	    }
        	    if(done) // No more things in the buffer to look at
        	    {
        	        continue;
        	    }
        	    // Get first hex digit and convert it to an integer, and move onto the next hex digit	
        	    int high = convertASCIIHexToInt(buffer[i++]);
	
        	    int onlyHigh = false;
        	    if(i > 24) 
        	    {
        	        onlyHigh = true;
        	    }
	
        	    // Check for whitespace or newline, and skip it until a hex digit is found
        	    while(buffer[i] == 32 || buffer[i] == '\n') 
        	    {
        	        i++;
        	        if(i > 24)
        	        {
        	            onlyHigh = true;
        	            break;
        	        }
        	    }
        	    // Get second hex digit and convert it to an integer
        	    int low;
        	    if(!onlyHigh) 
        	    {
        	        low = convertASCIIHexToInt(buffer[i]);
        	    }
	
        	    // In integer form, the binary number will be high*16 + low, print this character
        	          
        	    if(!onlyHigh) 
        	    {
        	       	int binVal = high*16 + low;
        	       	hexKey[k] = binVal;//Add the converted value to an array for later use
        	       	k++;
        	    } 
        	    else 
        	    {   // Assume the low is 0
        	       	int binVal = high*16;
        	       	hexKey[k] = binVal;
        	       	k++;
        	    }
        	}
        }
        else if(ascii_coded_hex == false)
        {
        	printf("Error, key not ascii coded hex.\n");
        	exit(1);
        }
        	 
                    
        j = 0;
        //Split bytes into nybbles
    	for(int i = 0; i < 8; i++)
    	{
    		int temp = hexKey[i];
    		int upper = (temp >>  4) & 15;
    		int lower = (temp >>  0) & 15;
    		keyNybbles[j] = upper;
    		keyNybbles[j+1] = lower;
    		j = j+2;
    	}
    }
        
    //Encrypt
    if(encrypt == true)
    {
 		encryption(nybbles, keyNybbles, bytes_read);
 	}	
 	//Decrypt
 	else if(decrypt == true)
 	{
 		decryption(nybbles, keyNybbles, bytes_read);
 	}	
}
	
	
	
			
