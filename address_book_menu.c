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

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */ 

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

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}