#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "address_book_fops.h"
#include "address_book_menu.h"
#include "address_book.h"

int get_option(int type, const char *msg)
{
	/*
	 * Mutilfuction user intractions like
	 * Just an enter key detection
	 * Read an number
	 * Read a charcter
	 */ 

	/* Fill the code to add above functionality */
	if (type == CHAR) // for csv file "save" feature, read character
	{
		return fgetc(stdin);
	}
	else if (type == NUM)
	{
		int on;
		scanf("%i", &on); // User Input, read number
		return on;
	}
	else
	{
		fgetc(stdin);
		return -1;
	}
}

MenuFeatures menu_option() // Feature Menu for Corressponding Number Selection
{
	switch (get_option(NUM, "")) // Menu Option Input from integer input to associated function
	{
	case 0:
		return e_exit;
	case 1:
		return e_add_contact;
	case 2: 
		return e_search_contact;
	case 3:
		return e_edit_contact;
	case 4:
		return e_delete_contact;
	case 5:
		return e_list_contacts;
	case 6:
		return e_save;
	}
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
		option = 'N';
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

Status list_contacts(AddressBook *address_book, const char *title, int index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */ 

	int option;

	menu_header("All Contacts: ");
	printf("(Page [%d] of [%d]):\n", (index+1), (address_book->count));

		printf("===============================================================================================================\n");
		printf(": S.No  : Name                            : Phone No                        : Email ID                        :\n");
		printf("===============================================================================================================\n");

		printf(": %d", address_book->list[index].si_no);
		if (address_book->list[index].si_no < 10) 
		{
			for (int i = 0; i < 5; i++)
				printf(" ");
		} 
		else
		{
			for (int i = 0; i < 4; i++)
				printf(" ");
		}
		printf(": %s", &address_book->list[index].name[0][0]);
		for (int i = 32 - strlen(&address_book->list[index].name[0][0]); i > 0; i--) 
		{
			printf(" ");
		}

		int PhoneNum = sizeof address_book->list[index].phone_numbers / sizeof *address_book->list[index].phone_numbers;
		int phonesPrinted = 0;
		int EmailNum = sizeof address_book->list[index].email_addresses / sizeof *address_book->list[index].email_addresses;
		int emailsPrinted = 0;
	
		if (PhoneNum > 0) //more than 1 phone
		{
			printf(": %s", &address_book->list[index].phone_numbers[0][0]);
			phonesPrinted++;
			for (int i = 32 - strlen(&address_book->list[index].phone_numbers[0][0]); i > 0; i--)
				printf(" ");
		} 
		else //online 1 phone
		{	
			for (int i = 32 ; i > 0; i--) 
			{
				printf(" ");
			}
		}

		if (EmailNum > 0) //more than 1 email
		{
			printf(": %s", &address_book->list[index].email_addresses[0][0]);
			emailsPrinted++;
			for (int i = 32 - strlen(&address_book->list[index].email_addresses[0][0]); i > 0; i--)
				printf(" ");
		} 
		else //only 1 email
		{ 
			for (int i = 32 ; i > 0; i--) 
			{
				printf(" ");
			}
		}
		printf(":\n");

		for (int i = 0; i < 4; i++)
		{
			printf(":       :                                  ");

			if (phonesPrinted < PhoneNum) {
				printf(":%s", &address_book->list[index].phone_numbers[phonesPrinted][0]);
				for (int m = 32 - strlen(&address_book->list[index].phone_numbers[phonesPrinted][0]); m > 0; m--){
					printf(" ");
				}
				phonesPrinted++;
			} 
			else{
				for (int n = 32 ; n > 0; n--){
					printf(" ");
				}	
			}
				
			if (emailsPrinted < EmailNum) 
			{
				printf(": %s", &address_book->list[index].email_addresses[emailsPrinted][0]);
				for (int l = 32 - strlen(&address_book->list[index].email_addresses[emailsPrinted][0]); l > 0; l--){
					printf(" ");
				}
				emailsPrinted++;
			} 
			else{
				for (int p = 32 ; p > 0; p--){
					printf(" ");
				}
			}	
			printf(":\n");
		}

		printf("===============================================================================================================\n");

	int pageMove = 0;
	do {
		printf("Exit: [0] | Previous Page: [1] | Next Page: [2]:   ");
		option = get_option(NUM, "");
		printf(" %d\n", option);

		if(option == 0) 
		{
			menu(address_book);
		} 
		else if(option == 2) 
		{
			if(index < (address_book->count-1)) 
			{
				index++;
				list_contacts(address_book, "", index, "", e_list_contacts);
			} 
			else 
			{
				printf("Already on last page.\n");
			}
		} 
		else if(option == 1)
		{
			if (index > 0) 
			{
				index--;
				list_contacts(address_book, "", index, "", e_list_contacts);
			} 
			else 
			{
				printf("Already on first page.\n");
			}
		}
	} while(pageMove == 0);

	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("cls");

	printf("#######  Address Book  #######\n");
	if (*str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

extern Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = menu_option();

		if ((address_book-> count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}
//		option = e_exit;

		switch (option)
		{
			case e_add_contact:
				/* Add your implementation to call add_contacts function here */
				add_contacts(address_book);
				break;
			case e_search_contact:
				search_contact(address_book);
				break;
			case e_edit_contact:
				edit_contact(address_book);
				break;
			case e_delete_contact:
				delete_contact(address_book);
				break;
			case e_list_contacts:
				list_contacts(address_book, "", 0, "", e_list_contacts);				
				break;
				/* Add your implementation to call list_contacts function here */
			case e_save:
				save_file(address_book);
				break;
			case e_exit:
				break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
	/* Add the functionality for adding contacts here */
	int opt = 0;
	int phn = 0; 
	int eml = 0;
	ContactInfo ci;
	strncpy(*ci.email_addresses, " ", sizeof(ci.email_addresses));
	do
	{
		menu_header("Add Contact:\n");
		printf("0. Back");
		printf("\n");
		printf("1. Name		: %s", ci.name[0]);
		printf("\n2. Phone No 1	: %s", ci.phone_numbers[0]);
		printf("\n3. Email ID 1	: %s", ci.email_addresses[0]);
		printf("\n\nPlease select an option: ");
		opt = get_option(NUM, "");
		switch (opt)
		{
			case e_first_opt: // Back Option
				menu(address_book); // Back to Menu
				break;
			case e_second_opt: // Name Option
				printf("Enter name: ");
				scanf("%s", &ci.name[0][0]); // Name Input
				break;
			case e_third_opt: // Phone Number Option
//				printf("AT PHONE NUMBER");
				if (phn < PHONE_NUMBER_COUNT)
				{
					printf("Enter Phone Number %d: [Please reenter the same option of alternate Phone Number]: ", phn + 1);
					scanf("%s", &ci.phone_numbers[phn][0]); // Phone Number Input
					phn++;
				}
				else
				{
					printf("Maximum Phone Number Amount. Press ENTER: ");
					getchar();
				}
				break;
			case e_fourth_opt: // Email Option
// 				printf("AT EMAIL");
				if (eml < EMAIL_ID_COUNT)
				{
					printf("Enter Email ID %d: [Please reenter the same option of the alternate Email ID]: ", eml + 1);
					scanf("%s", &ci.email_addresses[eml][0]); // Email Input
					eml++;
				}
				else
				{
					printf("Maximum Email Amount. Press Enter: ");
					getchar();
				}
				break;
			}
			getchar();
		} while (opt != 0);
		ci.si_no = address_book->count + 1;
		address_book->list[address_book->count] = ci;
		address_book->count++;
		return e_success;
}

Status search(const char *str, AddressBook *address_book, int serialNo, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
	ContactInfo contact;
	int found = 0;
	
	switch(field)
	{
		case 1://search by name
			for(int i = 0; i < address_book->count; i++)
			{
				contact = address_book->list[i];

				for(int k = 0; k < NAME_COUNT; k++)
				{
					if(strcmp(str, &contact.name[k][0]) == 0)// check name
					{
						found = 1;
						k = NAME_COUNT;
						i = address_book->count;
						break;
					}
				}
			}
			break;

		case 2://search by phone
			for(int i = 0; i < address_book->count; i++)
			{
				contact = address_book->list[i];

				for(int k = 0; k < PHONE_NUMBER_COUNT; k++)
				{
					if(strcmp(str, &contact.phone_numbers[k][0]) == 0)// check phone number
					{
						found = 1;
						k = NAME_COUNT;
						i = address_book->count;
						break;
					}
				}
			}
			break;

		case 3://search by email
			for(int i = 0; i < address_book->count; i++)
			{
				contact = address_book->list[i];
				for(int k = 0; k < EMAIL_ID_COUNT; k++){
					if(strcmp(str, &contact.email_addresses[k][0]) == 0)// check email id
					{
						found = 1;
						k = NAME_COUNT;
						i = address_book->count;
						break;
					}
				}
			}
			break;
		case 4://search by serial number
			serialNo = atoi(str);
			for(int i = 0; i < address_book->count; i++)
			{
				contact = address_book->list[i];
				if(serialNo == contact.si_no) //check serial number
				{
					found = 1;
					i = address_book->count;
					break;
				}
			}
			break;
		default:
			break;
	}

	//output
	menu_header("Search Results: ");
	if(found == 0){
		printf("\nContact was not found.\n");
		return e_no_match;
	}
	else{
		printf("===============================================================================================================\n");
		printf(": S.No  : Name                            : Phone No                        : Email ID                        :\n");
		printf("===============================================================================================================\n");
		printf(": %d", contact.si_no);
		if (contact.si_no < 10)
		{
			for (int i = 0; i < 5; i++)
				printf(" ");
		}
		else
		{
			for (int i = 0; i < 4; i++)
				printf(" ");
		}
		printf(": %s", &contact.name[0][0]);
		for (int i = 32 - strlen(&contact.name[0][0]); i > 0; i--)
		{
			printf(" ");
		}

		int PhoneNum = sizeof contact.phone_numbers / sizeof *contact.phone_numbers;
		int EmailNum = sizeof contact.email_addresses / sizeof *contact.email_addresses;
		int phonesPrinted = 0;
		int emailsPrinted = 0;

		if (PhoneNum > 0)
		{
			printf(": %s", &contact.phone_numbers[0][0]);
			phonesPrinted++;
			for (int i = 32 - strlen(&contact.phone_numbers[0][0]); i > 0; i--)
				printf(" ");
		}
		else
			for (int i = 32 ; i > 0; i--)
				printf(" ");

		if (EmailNum > 0)
		{
			printf(": %s", &contact.email_addresses[0][0]);
			emailsPrinted++;
			for (int i = 32 - strlen(&contact.email_addresses[0][0]); i > 0; i--)
				printf(" ");
		}
		else
			for (int i = 32 ; i > 0; i--)
				printf(" ");
		printf(":\n"); 

		for (int k = 0; k < 4; k++)
		{
			printf(":       :                                ");

			if (phonesPrinted < PhoneNum)
			{
				printf(": %s", &contact.phone_numbers[phonesPrinted][0]);
				for (int m = 32 - strlen(&contact.phone_numbers[phonesPrinted][0]); m > 0; m--)
					printf(" ");
				phonesPrinted++;
			}
			else
				for (int n = 32 ; n > 0; n--)
					printf(" ");

			if (emailsPrinted < EmailNum)
			{
				printf(": %s", &contact.email_addresses[emailsPrinted][0]);
				for (int l = 32 - strlen(&contact.email_addresses[emailsPrinted][0]); l > 0; l--)
					printf(" ");
				emailsPrinted++;
			}
			else
				for (int p = 32 ; p > 0; p--)
					printf(" ");
			printf(":\n");
		}

		printf("===========================================================================================================\n");

		char exit;
		do{//stays till user exits
			printf("Quit: [q]: ");
			exit = getchar();
		}while(exit != 'q');
		return e_success;
	}
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
	int option;
	char searchWord[NAME_LEN];

	do{
		menu_header("Search Contact by: ");
		printf("0. Back\n");
		printf("1. Name\n");
		printf("2. Phone No\n");
		printf("3. Email ID\n");
		printf("4. Serial No\n");
		printf("\n");
		printf("Please select an option: ");
		
		option = get_option(NUM, "");
		if((address_book-> count == 0) && (option != e_first_opt))
		{
			get_option(NONE, "No entries found!");
		}

		switch(option){
			case e_back://back
				option = e_exit;
				break;
			case e_second_opt://name
				printf("Enter the Name: ");
				scanf("%s", searchWord);
				search(searchWord, address_book, address_book-> count, 1, "", e_search);
				break;
			case e_third_opt://phone number
				printf("Enter the Phone Number: ");
				scanf("%s", searchWord);
				search(searchWord, address_book, address_book-> count, 2, "", e_search);
				break;
			case e_fourth_opt://email ID
				printf("Enter the Email ID: ");
				scanf("%s", searchWord);
				search(searchWord, address_book, address_book-> count, 3, "", e_search);
				break;
			case e_fifth_opt://Serial number
				printf("Enter the Serial No: ");
				scanf("%s", searchWord);
				search(searchWord, address_book, address_book-> count, 4, "", e_search);
				break;
			case e_no_opt://no option
				break;

			default:
				printf("\nIncorrect input.");
				return e_back;
				break;
		}

	}while(option != e_exit);

	return e_success;
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
	int option;
	char searchWord[32];
    char confirmation;
    ContactInfo contact;


	do
	{
        menu_header("Search Contact to Edit by: \n");
	    printf("0. Back\n");
	    printf("1. Name\n");
	    printf("2. Phone Number\n");
	    printf("3. Email ID\n");
	    printf("4. Serial No.\n");
	    printf("\n");
	    printf("Please select an option: ");

		option = get_option(NUM, "");
		if((address_book-> count == 0) && (option != e_back))
		{
			get_option(NONE, "No entries found!");
		}

		switch(option)
		{
			//Back
			case e_first_opt:
                menu(address_book);
			    break;
			//Name
			case e_second_opt:
				printf("Enter the Name: ");
				scanf("%s", searchWord);
				search(searchWord, address_book, address_book-> count, 1, searchWord, e_edit);
                printf("\nIs this the correct contact? y/n ");
                scanf("%s", &confirmation);
                confirmation = tolower(confirmation);
                if(confirmation == 'y'){
                    //getting info for contact variable -- from search function
                    int j;
                    for(int i = 0; i < address_book->count; i++){
				        contact = address_book->list[i];

				        for(int k = 0; k < NAME_COUNT; k++){
					        if(strcmp(searchWord, &contact.name[k][0]) == 0)// check name
					        {
						        k = NAME_COUNT;
                                j = i;
						        i = address_book->count;
						        break;
					        }
				        }
			        }

                    int opt1;
                    menu_header("Please select what you would like to change: \n");
                    printf("1. Name\n");
                    printf("2. Phone Number\n");
                    printf("3. Email ID\n");
                    printf("\n");
	                printf("Please select an option: \n");
                    scanf("%i", &opt1);
                    if(opt1 == 1){
                        char newName[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newName);
                        strcpy(address_book->list[j].name[0],newName);
                    }
                    else if(opt1 == 2){
                        int opt2;
                        menu_header("Please select which you would like to change: \n");
                        printf("1. %s\n", &contact.phone_numbers[0]);
                        printf("2. %s\n", &contact.phone_numbers[1]);
                        printf("3. %s\n", &contact.phone_numbers[2]);
                        printf("4. %s\n", &contact.phone_numbers[3]);
                        printf("5. %s\n", &contact.phone_numbers[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        char newPhone[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newPhone);
                        strcpy(address_book->list[j].phone_numbers[opt2-1],newPhone);
                    }
                    else if(opt1 == 3){
                        int opt2;
                        menu_header("Please select which you would like to change: \n");
                        printf("1. %s\n", &contact.email_addresses[0]);
                        printf("2. %s\n", &contact.email_addresses[1]);
                        printf("3. %s\n", &contact.email_addresses[2]);
                        printf("4. %s\n", &contact.email_addresses[3]);
                        printf("5. %s\n", &contact.email_addresses[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        char newEmail[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newEmail);
                        strcpy(address_book->list[j].email_addresses[opt2-1],newEmail);
                    }
                    else{
                        printf("No selection chosen, returning to menu.");
                    }

                }
                else if(confirmation == 'n'){
                    printf("Entry could not be found, returning to menu.");
                }
                else{
                    printf("Invalid response, returning to menu.");
                }
			    break;
			//Phone No
			case e_third_opt:
				printf("Enter the Phone Number: ");
				scanf("%s", searchWord);
				search(searchWord, address_book, address_book-> count, 2, searchWord, e_edit);
				printf("\nIs this the correct contact? y/n ");
                scanf("%s", confirmation);
                confirmation = tolower(confirmation);
                if(confirmation == 'y'){
                    //getting info for contact variable -- from search function
                    int j;
                    for(int i = 0; i < address_book->count; i++){
				        contact = address_book->list[i];

				        for(int k = 0; k < PHONE_NUMBER_COUNT; k++){
					        if(strcmp(searchWord, &contact.phone_numbers[k][0]) == 0)// check phone
					        {
						        k = PHONE_NUMBER_COUNT;
                                j = i;
						        i = address_book->count;
						        break;
					        }
				        }
			        }

                    int opt1;
                    menu_header("Please select what you would like to change: \n");
                    printf("1. Name\n");
                    printf("2. Phone Number\n");
                    printf("3. Email ID\n");
                    printf("\n");
	                printf("Please select an option: \n");
                    scanf("%i", &opt1);
                    if(opt1 == 1){
                        char newName[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newName);
                        strcpy(address_book->list[j].name[0],newName);
                    }
                    else if(opt1 == 2){
                        int opt2;
                        menu_header("Please select which you would like to change: \n");
                        printf("1. %s\n", &contact.phone_numbers[0]);
                        printf("2. %s\n", &contact.phone_numbers[1]);
                        printf("3. %s\n", &contact.phone_numbers[2]);
                        printf("4. %s\n", &contact.phone_numbers[3]);
                        printf("5. %s\n", &contact.phone_numbers[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        char newPhone[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newPhone);
                        strcpy(address_book->list[j].phone_numbers[opt2-1],newPhone);
                    }
                    else if(opt1 == 3){
                        int opt2;
                        menu_header("Please select which you would like to change: \n");
                        printf("1. %s\n", &contact.email_addresses[0]);
                        printf("2. %s\n", &contact.email_addresses[1]);
                        printf("3. %s\n", &contact.email_addresses[2]);
                        printf("4. %s\n", &contact.email_addresses[3]);
                        printf("5. %s\n", &contact.email_addresses[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        char newEmail[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newEmail);
                        strcpy(address_book->list[j].email_addresses[opt2-1],newEmail);
                    }
                    else{
                        printf("No selection chosen, returning to menu.");
                    }

                }
                else if(confirmation == 'n'){
                    printf("Entry could not be found, returning to menu.");
                }
                else{
                    printf("Invalid response, returning to menu.");
                }
			    break;
			//Email ID
			case e_fourth_opt:
				printf("Enter the Email ID: ");
				scanf("%s", searchWord);
				search(searchWord, address_book, address_book-> count, 3, searchWord, e_edit);
				printf("\nIs this the correct contact? y/n ");
                scanf("%s", confirmation);
                confirmation = tolower(confirmation);
                if(confirmation == 'y'){
                    //getting info for contact variable -- from search function
                    int j;
                    for(int i = 0; i < address_book->count; i++){
				        contact = address_book->list[i];

				        for(int k = 0; k < EMAIL_ID_COUNT; k++){
					        if(strcmp(searchWord, &contact.email_addresses[k][0]) == 0)// check eamil
					        {
						        k = EMAIL_ID_COUNT;
                                j = i;
						        i = address_book->count;
						        break;
					        }
				        }
			        }

                    int opt1;
                    menu_header("Please select what you would like to change: \n");
                    printf("1. Name\n");
                    printf("2. Phone Number\n");
                    printf("3. Email ID\n");
                    printf("\n");
	                printf("Please select an option: \n");
                    scanf("%i", &opt1);
                    if(opt1 == 1){
                        char newName[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newName);
                        strcpy(address_book->list[j].name[0],newName);
                    }
                    else if(opt1 == 2){
                        int opt2;
                        menu_header("Please select which you would like to change: \n");
                        printf("1. %s\n", &contact.phone_numbers[0]);
                        printf("2. %s\n", &contact.phone_numbers[1]);
                        printf("3. %s\n", &contact.phone_numbers[2]);
                        printf("4. %s\n", &contact.phone_numbers[3]);
                        printf("5. %s\n", &contact.phone_numbers[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        char newPhone[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newPhone);
                        strcpy(address_book->list[j].phone_numbers[opt2-1],newPhone);
                    }
                    else if(opt1 == 3){
                        int opt2;
                        menu_header("Please select which you would like to change: \n");
                        printf("1. %s\n", &contact.email_addresses[0]);
                        printf("2. %s\n", &contact.email_addresses[1]);
                        printf("3. %s\n", &contact.email_addresses[2]);
                        printf("4. %s\n", &contact.email_addresses[3]);
                        printf("5. %s\n", &contact.email_addresses[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        char newEmail[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newEmail);
                        strcpy(address_book->list[j].email_addresses[opt2-1],newEmail);
                    }
                    else{
                        printf("No selection chosen, returning to menu.");
                    }

                }
                else if(confirmation == 'n'){
                    printf("Entry could not be found, returning to menu.");
                }
                else{
                    printf("Invalid response, returning to menu.");
                }
			    break;
			//Serial No
			case e_fifth_opt:
				printf("Enter the Serial No: ");
				scanf("%s", searchWord);
                int sino = atoi(searchWord);
				search(searchWord, address_book, address_book-> count, 4, searchWord, e_edit);
				printf("\nIs this the correct contact? y/n ");
                scanf("%s", confirmation);
                confirmation = tolower(confirmation);
                if(confirmation == 'y'){
                    //enter code here to edit by serial
                    int j;
                    for(int i = 0; i < address_book->count; i++){
				        contact = address_book->list[i];

					    if(sino == contact.si_no) //check serial number
					    {
                            j = i;
						    i = address_book->count;
						    break;
					    }
			        }

                    int opt1;
                    menu_header("Please select what you would like to change: \n");
                    printf("1. Name\n");
                    printf("2. Phone Number\n");
                    printf("3. Email ID\n");
                    printf("\n");
	                printf("Please select an option: \n");
                    scanf("%i", &opt1);
                    if(opt1 == 1){
                        char newName[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newName);
                        strcpy(address_book->list[j].name[0],newName);
                    }
                    else if(opt1 == 2){
                        int opt2;
                        menu_header("Please select which you would like to change: \n");
                        printf("1. %s\n", &contact.phone_numbers[0]);
                        printf("2. %s\n", &contact.phone_numbers[1]);
                        printf("3. %s\n", &contact.phone_numbers[2]);
                        printf("4. %s\n", &contact.phone_numbers[3]);
                        printf("5. %s\n", &contact.phone_numbers[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        char newPhone[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newPhone);
                        strcpy(address_book->list[j].phone_numbers[opt2-1],newPhone);
                    }
                    else if(opt1 == 3){
                        int opt2;
                        menu_header("Please select which you would like to change: \n");
                        printf("1. %s\n", &contact.email_addresses[0]);
                        printf("2. %s\n", &contact.email_addresses[1]);
                        printf("3. %s\n", &contact.email_addresses[2]);
                        printf("4. %s\n", &contact.email_addresses[3]);
                        printf("5. %s\n", &contact.email_addresses[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        char newEmail[32];
                        printf("What would you like to change it to: \n");
                        scanf("%s", &newEmail);
                        strcpy(address_book->list[j].email_addresses[opt2-1],newEmail);
                    }
                    else{
                        printf("No selection chosen, returning to menu.");
                    }
                }
                else if(confirmation == 'n'){
                    printf("Entry could not be found, returning to menu.");
                }
                else{
                    printf("Invalid response, returning to menu.");
                }
			    break;
			//No Option
			case e_no_opt:
			    break;
		}

	} while (option != e_exit);
	
	return e_success;
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
	int option;
	char searchWord[32];
    char empty[32];
    char confirmation;
    ContactInfo contact;
    ContactInfo emptyContact;

	do
	{
        menu_header("Search Contact to Delete by: \n");
	    printf("0. Back\n");
	    printf("1. Name\n");
	    printf("2. Phone Number\n");
	    printf("3. Email ID\n");
	    printf("4. Serial No.\n");
	    printf("\n");
	    printf("Please select an option: ");
        
		option = get_option(NUM, "");
		if((address_book-> count == 0) && (option != e_back))
		{
			get_option(NONE, "No entries found!");
		}

		switch(option)
		{
			//Back
			case e_first_opt:
                menu(address_book);
			    break;
			//Name
			case e_second_opt:
				printf("Enter the Name: ");
				scanf("%s", searchWord);
				search(searchWord, address_book, address_book-> count, 1, searchWord, e_edit);
                printf("\nIs this the correct contact? y/n ");
                scanf("%s", &confirmation);
                confirmation = tolower(confirmation);
                if(confirmation == 'y'){
                    //getting info for contact variable -- from search function
                    int j;
                    for(int i = 0; i < address_book->count; i++){
				        contact = address_book->list[i];

				        for(int k = 0; k < NAME_COUNT; k++){
					        if(strcmp(searchWord, &contact.name[k][0]) == 0)// check name
					        {
						        k = NAME_COUNT;
                                j = i;
						        i = address_book->count;
						        break;
					        }
				        }
			        }

                    int opt1;
                    menu_header("Please select what you would like to delete: \n");
                    printf("1. Name\n");
                    printf("2. Phone Number\n");
                    printf("3. Email ID\n");
                    printf("\n");
	                printf("Please select an option: \n");
                    scanf("%i", &opt1);
                    if(opt1 == 1){
                        for(int i=j; i<(address_book->count-1); i++){
                            address_book->list[i] = address_book->list[i+1];
                        }
                        address_book->list[address_book->count-1] = emptyContact;
                    }
                    else if(opt1 == 2){
                        int opt2;
                        menu_header("Please select which you would like to delete: \n");
                        printf("1. %s\n", &contact.phone_numbers[0]);
                        printf("2. %s\n", &contact.phone_numbers[1]);
                        printf("3. %s\n", &contact.phone_numbers[2]);
                        printf("4. %s\n", &contact.phone_numbers[3]);
                        printf("5. %s\n", &contact.phone_numbers[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        strcpy(address_book->list[j].phone_numbers[opt2-1],empty);
                    }
                    else if(opt1 == 3){
                        int opt2;
                        menu_header("Please select which you would like to delete: \n");
                        printf("1. %s\n", &contact.email_addresses[0]);
                        printf("2. %s\n", &contact.email_addresses[1]);
                        printf("3. %s\n", &contact.email_addresses[2]);
                        printf("4. %s\n", &contact.email_addresses[3]);
                        printf("5. %s\n", &contact.email_addresses[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        strcpy(address_book->list[j].email_addresses[opt2-1], empty);
                    }
                    else{
                        printf("No selection chosen, returning to menu.");
                    }

                }
                else if(confirmation == 'n'){
                    printf("Entry could not be found, returning to menu.");
                }
                else{
                    printf("Invalid response, returning to menu.");
                }
			    break;
			//Phone No
			case e_third_opt:
				printf("Enter the Phone Number: ");
				scanf("%s", searchWord);
				search(searchWord, address_book, address_book-> count, 2, searchWord, e_edit);
				printf("\nIs this the correct contact? y/n ");
                scanf("%s", confirmation);
                confirmation = tolower(confirmation);
                if(confirmation == 'y'){
                    //getting info for contact variable -- from search function
                    int j;
                    for(int i = 0; i < address_book->count; i++){
				        contact = address_book->list[i];

				        for(int k = 0; k < PHONE_NUMBER_COUNT; k++){
					        if(strcmp(searchWord, &contact.phone_numbers[k][0]) == 0)// check phone
					        {
						        k = PHONE_NUMBER_COUNT;
                                j = i;
						        i = address_book->count;
						        break;
					        }
				        }
			        }

                    int opt1;
                    menu_header("Please select what you would like to delete: \n");
                    printf("1. Name\n");
                    printf("2. Phone Number\n");
                    printf("3. Email ID\n");
                    printf("\n");
	                printf("Please select an option: \n");
                    scanf("%i", &opt1);
                    if(opt1 == 1){
                        for(int i=j; i<(address_book->count-1); i++){
                            address_book->list[i] = address_book->list[i+1];
                        }
                        address_book->list[address_book->count-1] = emptyContact;
                    }
                    else if(opt1 == 2){
                        int opt2;
                        menu_header("Please select which you would like to delete: \n");
                        printf("1. %s\n", &contact.phone_numbers[0]);
                        printf("2. %s\n", &contact.phone_numbers[1]);
                        printf("3. %s\n", &contact.phone_numbers[2]);
                        printf("4. %s\n", &contact.phone_numbers[3]);
                        printf("5. %s\n", &contact.phone_numbers[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        strcpy(address_book->list[j].phone_numbers[opt2-1],empty);
                    }
                    else if(opt1 == 3){
                        int opt2;
                        menu_header("Please select which you would like to delete: \n");
                        printf("1. %s\n", &contact.email_addresses[0]);
                        printf("2. %s\n", &contact.email_addresses[1]);
                        printf("3. %s\n", &contact.email_addresses[2]);
                        printf("4. %s\n", &contact.email_addresses[3]);
                        printf("5. %s\n", &contact.email_addresses[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        strcpy(address_book->list[j].email_addresses[opt2-1], empty);
                    }
                    else{
                        printf("No selection chosen, returning to menu.");
                    }

                }
                else if(confirmation == 'n'){
                    printf("Entry could not be found, returning to menu.");
                }
                else{
                    printf("Invalid response, returning to menu.");
                }
			    break;
			//Email ID
			case e_fourth_opt:
				printf("Enter the Email ID: ");
				scanf("%s", searchWord);
				search(searchWord, address_book, address_book-> count, 3, searchWord, e_edit);
				printf("\nIs this the correct contact? y/n ");
                scanf("%s", confirmation);
                confirmation = tolower(confirmation);
                if(confirmation == 'y'){
                    //getting info for contact variable -- from search function
                    int j;
                    for(int i = 0; i < address_book->count; i++){
				        contact = address_book->list[i];

				        for(int k = 0; k < EMAIL_ID_COUNT; k++){
					        if(strcmp(searchWord, &contact.email_addresses[k][0]) == 0)// check email
					        {
						        k = EMAIL_ID_COUNT;
                                j = i;
						        i = address_book->count;
						        break;
					        }
				        }
			        }

                    int opt1;
                    menu_header("Please select what you would like to delete: \n");
                    printf("1. Name\n");
                    printf("2. Phone Number\n");
                    printf("3. Email ID\n");
                    printf("\n");
	                printf("Please select an option: \n");
                    scanf("%i", &opt1);
                    if(opt1 == 1){
                        for(int i=j; i<(address_book->count-1); i++){
                            address_book->list[i] = address_book->list[i+1];
                        }
                        address_book->list[address_book->count-1] = emptyContact;
                    }
                    else if(opt1 == 2){
                        int opt2;
                        menu_header("Please select which you would like to delete: \n");
                        printf("1. %s\n", &contact.phone_numbers[0]);
                        printf("2. %s\n", &contact.phone_numbers[1]);
                        printf("3. %s\n", &contact.phone_numbers[2]);
                        printf("4. %s\n", &contact.phone_numbers[3]);
                        printf("5. %s\n", &contact.phone_numbers[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        strcpy(address_book->list[j].phone_numbers[opt2-1],empty);
                    }
                    else if(opt1 == 3){
                        int opt2;
                        menu_header("Please select which you would like to delete: \n");
                        printf("1. %s\n", &contact.email_addresses[0]);
                        printf("2. %s\n", &contact.email_addresses[1]);
                        printf("3. %s\n", &contact.email_addresses[2]);
                        printf("4. %s\n", &contact.email_addresses[3]);
                        printf("5. %s\n", &contact.email_addresses[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        strcpy(address_book->list[j].email_addresses[opt2-1], empty);
                    }
                    else{
                        printf("No selection chosen, returning to menu.");
                    }

                }
                else if(confirmation == 'n'){
                    printf("Entry could not be found, returning to menu.");
                }
                else{
                    printf("Invalid response, returning to menu.");
                }
			    break;
			//Serial No
			case e_fifth_opt:
				printf("Enter the Serial No: ");
				scanf("%s", searchWord);
                int sino = atoi(searchWord);
				search(searchWord, address_book, address_book-> count, 4, searchWord, e_edit);
				printf("\nIs this the correct contact? y/n ");
                scanf("%s", confirmation);
                confirmation = tolower(confirmation);
                if(confirmation == 'y'){
                    //enter code here to edit by serial
                    int j;
                    for(int i = 0; i < address_book->count; i++){
				        contact = address_book->list[i];

					    if(sino == contact.si_no) //check serial number
					    {
                            j = i;
						    i = address_book->count;
						    break;
					    }
			        }

                    int opt1;
                    menu_header("Please select what you would like to delete: \n");
                    printf("1. Name\n");
                    printf("2. Phone Number\n");
                    printf("3. Email ID\n");
                    printf("\n");
	                printf("Please select an option: \n");
                    scanf("%i", &opt1);
                    if(opt1 == 1){
                        for(int i=j; i<(address_book->count-1); i++){
                            address_book->list[i] = address_book->list[i+1];
                        }
                        address_book->list[address_book->count-1] = emptyContact;
                    }
                    else if(opt1 == 2){
                        int opt2;
                        menu_header("Please select which you would like to delete: \n");
                        printf("1. %s\n", &contact.phone_numbers[0]);
                        printf("2. %s\n", &contact.phone_numbers[1]);
                        printf("3. %s\n", &contact.phone_numbers[2]);
                        printf("4. %s\n", &contact.phone_numbers[3]);
                        printf("5. %s\n", &contact.phone_numbers[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        strcpy(address_book->list[j].phone_numbers[opt2-1],empty);
                    }
                    else if(opt1 == 3){
                        int opt2;
                        menu_header("Please select which you would like to delete: \n");
                        printf("1. %s\n", &contact.email_addresses[0]);
                        printf("2. %s\n", &contact.email_addresses[1]);
                        printf("3. %s\n", &contact.email_addresses[2]);
                        printf("4. %s\n", &contact.email_addresses[3]);
                        printf("5. %s\n", &contact.email_addresses[4]);
                        printf("\n");
	                    printf("Please select an option: \n");
                        scanf("%i", &opt2);
                        strcpy(address_book->list[j].email_addresses[opt2-1], empty);
                    }
                    else{
                        printf("No selection chosen, returning to menu.");
                    }
                }
                else if(confirmation == 'n'){
                    printf("Entry could not be found, returning to menu.");
                }
                else{
                    printf("Invalid response, returning to menu.");
                }
			    break;
			//No Option
			case e_no_opt:
			    break;
		}

	} while (option != e_exit);
	
	return e_success;
}
