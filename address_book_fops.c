#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#include "address_book.h"

Status load_file(AddressBook *address_book)
{

	int ret = 0;
	int entries = 0;
	int column = 0;
	int currentEntry = 0;
	int phoneNumbers = 0;
	int emailAddresses = 0;
	int i;
	int j;

	char c;
	char fileInput[32] = "";

	address_book->list = (ContactInfo*)malloc(sizeof(ContactInfo));

	address_book->fp = fopen(DEFAULT_FILE, "r");

	if (address_book->fp == NULL)
	{

		ret = 1;

	}

	if (ret == 1)
	{

		printf("No address book exists yet. Creating a new one.\n");

		address_book->fp = fopen(DEFAULT_FILE, "wt+");
		
	}
	else{

		c = getc(address_book->fp);

		if (c == EOF){

			goto EntrySet;

		}

		for (c = getc(address_book->fp); c != EOF; c = getc(address_book->fp)){

        	if (c == '\n'){

				c = getc(address_book->fp);

				if (c != ','){

					entries++;

				}
            	

			}
		
		}

		fclose(address_book->fp);

		entries++;

		EntrySet:

		address_book->count = (entries);

		address_book->fp = fopen(DEFAULT_FILE, "r");

		address_book->list = malloc((entries + 1) * sizeof(ContactInfo));

		for (i = 0; i < entries; i++){

			strcpy(address_book->list[i].name[0], "");

			for (j = 0; j < 5; j++){

				strcpy(address_book->list[i].phone_numbers[j], "");

			}

			for (j = 0; j < 5; j++){

				strcpy(address_book->list[i].email_addresses[j], "");

			}


		}

		for (c = getc(address_book->fp); c != EOF; c = getc(address_book->fp)){

			switch (column){

				case 0:
					switch (c){

						case ',':
								
							if (fileInput[0] != '\0'){

								currentEntry++;

								phoneNumbers = 0;

								emailAddresses = 0;

								address_book->list[currentEntry - 1].si_no = currentEntry;
								
								strcpy(address_book->list[currentEntry - 1].name[0], fileInput);
								
								strcpy(fileInput, "");

							}
							
							column++;
							break;

						default:
							strncat(fileInput, &c, 1);
							break;

					}

					break;
				
				case 1:
					switch (c){

						case ',':
							
							if (fileInput[0] != '\0'){
								
								phoneNumbers++;
								
								if (phoneNumbers <= 5){

									strcpy(address_book->list[currentEntry - 1].phone_numbers[phoneNumbers - 1], fileInput);	
									
								}

								strcpy(fileInput, "");

							}

							column++;
							break;

						default:
							strncat(fileInput, &c, 1);
							break;

					}

					break;

				case 2:
					switch (c){

						case '\n':
							if (fileInput[0] != '\0'){

								emailAddresses++;

								if (emailAddresses <= 5){

									strcpy(address_book->list[currentEntry - 1].email_addresses[emailAddresses - 1], fileInput);	
									
								}

								strcpy(fileInput, "");

							}

							column = 0;
							break;

						default:
							strncat(fileInput, &c, 1);
							break;

					}

				break;
			}

		}
        
	}

	if (fileInput[0] != '\0'){

		emailAddresses++;

		if (emailAddresses <= 5){

			strcpy(address_book->list[currentEntry - 1].email_addresses[emailAddresses - 1], fileInput);	
	
		}

		strcpy(fileInput, "");

	}

	fclose(address_book->fp);
	printf("Successfully loaded file.\n");

	return e_success;
}

Status save_file(AddressBook *address_book)
{

	int *phoneNumbers;
	int *emailAddresses;
	int entries = 0;
	int phoneNumbersTraversed = 0;
	int emailAddressesTraversed = 0;
	int maxDataCount = 0;
	int i;
	int j;

	entries = address_book->count;

	phoneNumbers = malloc((entries + 1) * sizeof(int));
	emailAddresses = malloc((entries + 1) * sizeof(int));

	for (i = 0; i < entries; i++){

		phoneNumbers[i] = 0;

	}

	for (i = 0; i < entries; i++){

		emailAddresses[i] = 0;

	}



	for (i = 0; i < entries; i++){

		for (j = 0; address_book->list[i].phone_numbers[j][0] != '\0'; j++){

			phoneNumbers[i] += 1;

		}

		for (j = 0; address_book->list[i].email_addresses[j][0] != '\0'; j++){

			emailAddresses[i] += 1;

		}

	}

	address_book->fp = fopen(DEFAULT_FILE, "w");

	if (address_book->fp == NULL)
	{
		return e_fail;
	}

	for (i = 0; i < entries; i++){

		if (phoneNumbers[i] >= emailAddresses[i]){

			maxDataCount = phoneNumbers[i];

		}
		else{

			maxDataCount = emailAddresses[i];

		}

		if (maxDataCount < 1){

			maxDataCount = 1;

		}

		for (j = 0; j < maxDataCount; j++){

			if (j == 0){

				fputs(address_book->list[i].name[0], address_book->fp);

			}

			fputc(',', address_book->fp);

			if(phoneNumbers[i] > 0){

				fputs(address_book->list[i].phone_numbers[j], address_book->fp);

			}

			fputc(',', address_book->fp);

			if(emailAddresses[i] > 0){

				fputs(address_book->list[i].email_addresses[j], address_book->fp);

			}

			if (i != (entries - 1)){

				fputc('\n', address_book->fp);

			}

			phoneNumbers[i]--;
			emailAddresses[i]--;

		}


		


	}


	fclose(address_book->fp);

	free(phoneNumbers);
	free(emailAddresses);

	return e_success;
}