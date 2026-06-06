#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

void listContacts(AddressBook *addressBook) 
{
    // Sort contacts based on the chosen criteria

    for(int i = 0; i < addressBook->contactCount - 1; i++)
    {
        for(int j = 0; j < addressBook->contactCount - 1 - i; j++)
        {
            if(strcasecmp(addressBook->contacts[j].name,addressBook->contacts[j+1].name)>0)
            {
                Contact temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j+1];
                addressBook->contacts[j+1] = temp;
            }
        }
    }

    printf(" Name                Mobile.no           mail.id\n");
    printf("-----------------------------------------------------------------\n");
    for (int  i = 0; i < addressBook->contactCount; i++)
    {            
        printf("%-20s %-15s %-30s\n",
              addressBook->contacts[i].name,
              addressBook->contacts[i].phone,
              addressBook->contacts[i].email);  
    }
     printf("-----------------------------------------------------------------\n");
}


void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    //populateAddressBook(addressBook);
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) 
{
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

//---------------------------------------------------------------Validation-----------------------------------------------------------------------//

int validate_name(char *name)
{
    int len = strlen(name);

    if(len < 4)
    {
        printf("Name must contain at least 4 characters\n");
        return 0;
    }

    for(int i = 0;  name[i] != '\0'; i++ )
    {


        if(!((name[i] >= 'A' && name[i] <= 'Z') || 
            (name[i] >= 'a' && name[i] <= 'z')))
        {
            printf("Name should contain only alphabets\n");
            return 0;
        }
    }
    //     check name[i] is not in the range of A to Z, a to z, ' ', '.'
    //         return 0; // notvalid
    // END LOOP

    return 1; // valid.
}

int validate_mobile(char *mobile, AddressBook *AddressBook)
{
    int len;
    len = strlen(mobile);

    if(len != 10)
    {
        printf("Invalid! Number must be 10 digits\n");
        return 0;
    }

    for (int  i = 0; mobile[i] != '\0'; i++)
    {
         if(!(mobile[i] >= '0' && mobile[i] <= '9'))
            {
                printf("Invalid! only digits are allowed\n");
                return 0;
            }
    }

    if(!(mobile[0] == '6' || mobile[0] == '7' || 
         mobile[0] == '8' || mobile[0] == '9'))
         {
            printf("Invalid must starts with 6/7/8/9\n");
            return 0;
         }
    
    for (int  i = 0; i < AddressBook->contactCount; i++)
    {
        if(strcmp(mobile, AddressBook->contacts[i].phone) == 0)
        {
            printf("Invalid - Phone number already exits\n");
            return 0;
        }
    }
    return 1;
   
}


int validate_mail(char *mail, AddressBook *addressBook)
{
    int atFound = 0;
    int dotFound = 0;
    int atIndex = -1;
    int dotIndex = -1;
    int len = strlen(mail);

    if(mail[0] == '@' || mail[0] == '.')
    {
        printf("Email cannot start with @ or . \n");
        return 0;
    }

    for (int  i = 0; mail[i] != '\0'; i++)
    {
        if(!((mail[i] >= 'a' && mail[i] <= 'z') ||
          (mail[i] >= 'A' && mail[i] <= 'Z') ||  
          (mail[i] >= '0' && mail[i] <= '9')||
          (mail[i] == '@' || mail[i] == '.')))
        {
            printf("Invalid symbol present in email\n");
            return 0;
        }
        if(mail[i] == '@')
        {
            atFound++;
            atIndex = i;

            if(mail[i+1] == '\0' || mail[i+1] == '.' || mail[i+1] == '@')
            {
                printf("Invalid input \n");
                return 0;  
            }
        }

        else if(mail[i] == '.')
        {
            dotFound++;
            dotIndex = i;
        }           
    }

    if(atFound != 1)
    {
        printf("Email must contain exactly one @\n");
        return 0;
    }

    if(dotFound < 1)
    {
        printf("Email must contain at least one '.'\n");
        return 0;
    }

    if(dotIndex < atIndex)
    {
        printf("'.' must come after @\n");
        return 0;
    }

    if(strstr(mail,"..") != NULL)
    {
        printf("Email cannot contain continous dots\n");
        return 0;
    }

    if(dotIndex == len - 1)
    {
        printf("mail extension is missing\n");
        return 0;
    }

    for(int  i = atIndex + 1; i < len; i++)
    {
        if(mail[i] == '.')
            continue;
        
        if(!((mail[i] >= 'a' && mail[i] <= 'z') ||
            (mail[i] >= 'A' && mail[i] <= 'Z')))
        {
            printf("Domain must contain only alphabets\n");
            return 0;
        }
    }
    
     for(int i = dotIndex + 1; i < len; i++)
    {
        if(!((mail[i] >= 'a' && mail[i] <= 'z') ||
             (mail[i] >= 'A' && mail[i] <= 'Z')))
        {
            printf("Invalid characters after domain\n");
            return 0;
        }
    }

    for (int  i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(mail,addressBook->contacts[i].email)== 0)
        {
            printf("mail id already exists\n");
            return 0;
        }
    }
    return 1;     
}
    
//-----------------------------------------------------------------Create Contact-----------------------------------------------------------------//
void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */

    char name[20];
    int valid;

    while(1)
    {
        printf("Enter the name : ");
        scanf(" %[^\n]", name);
        valid = validate_name(name);
        if(valid == 1)// valid
            break;
        printf("Try again\n");
    }

     char mobile[11];
     int valid_mobile;

      while(1)
     {
         printf("Enter mobile number : ");
         scanf(" %[^\n]", mobile);
         valid_mobile = validate_mobile(mobile,addressBook);
         if(valid_mobile == 1)// valid
             break;
        printf("Try again\n");
     }

     char mail[30];
     int valid_mail;

     while(1)
     {
         printf("Enter your mail id :");
         scanf(" %[^\n]",mail);
         valid_mail = validate_mail(mail,addressBook);
         if(valid_mail == 1)// valid
             break;
         printf("Try again\n");

     }

     int i = addressBook->contactCount;
     strcpy(addressBook->contacts[i].name,name);
     strcpy(addressBook->contacts[i].phone,mobile);
     strcpy(addressBook->contacts[i].email,mail);
     addressBook->contactCount++;

     printf("Contact created successfully\n");
    
}
//---------------------------------------------------------------Search Contact-------------------------------------------------------------------------// 

int search_by_name(AddressBook *addressBook)
{
    char name[50];
    printf("Enter name:");
    scanf(" %[^\n]",name);

    int name_count = 0;
    int index[100];

    printf("\nSearching.... \n");

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if(strcasestr(addressBook->contacts[i].name,name) != NULL )
        {
            index[name_count] = i;
            name_count++;
            // printf("Matching Contacts Found\n");
        
            printf("%-5d %-20s %-15s %-25s\n",name_count,
                  addressBook->contacts[i].name,
                  addressBook->contacts[i].phone,
                  addressBook->contacts[i].email);
        }
    }
    
    if (name_count == 0)
    {
        printf("No matches found\n");
        return -1;
    }

    if (name_count == 1)
    {
        return index[0];
    }

    printf("%d matches found\n",name_count);
    
    int choice;
    printf("Select the contact uisng serial no.: ");
    scanf("%d",&choice);

    if (choice < 1 || choice > name_count)
    {
        printf("Invalid choice \n");
        return -1;
    }

    return index[choice - 1];
    
}

int search_by_phone(AddressBook *addressBook)
{
    char mobile[20];
    printf("Enter the phone number:");
    scanf(" %[^\n]",mobile);

    printf("\nSearching.... \n");

    for (int  i = 0; i < addressBook->contactCount; i++)
    {
        if(strcmp(mobile,addressBook->contacts[i].phone) == 0)
        {
            return i;
        }
    }
    printf("No matches found!\n");
    return -1;
}


int search_by_mail(AddressBook *addressBook)
{
    char mail[30];
    printf("Enter the mail: ");
    scanf(" %[^\n]",mail);

    printf("\nSearching.... \n");

    for (int  i = 0; i < addressBook->contactCount; i++)
    {
        
        if(strcmp(mail,addressBook->contacts[i].email) == 0)
        {
            return i;
        }
    }
    printf("No matches found!\n");
    return -1;

}



 int searchContact(AddressBook *addressBook) 
 {
     /* Define the logic for search */

    printf("Search contact\n");
    int choice;
    int index = -1;

    printf("1. Name \n2. Phone\n3. Mail\n4. Exit\n");
    printf("Enter your choice: ");

    if(scanf("%d",&choice) != 1)
    {
        printf("Invalid input\n");
        while(getchar() != '\n');
        return -1;
    }

    if(choice == 4)
    {
        return -1;
    }

    switch (choice)
    {
    case 1:
       index =  search_by_name(addressBook);
        break;

    case 2:
       index =  search_by_phone(addressBook);
        break;
    
    case 3:
       index =  search_by_mail(addressBook);
        break;
    
    default:
        printf("Invalid choice\n");
        break;
    }

    if(index != -1)
    {
       printf("Selected contact\n");
            printf(" Name                Mobile.no           mail.id\n");
            printf("-----------------------------------------------------------------\n");
            
            printf("%-20s %-15s %-30s\n",
               addressBook->contacts[index].name,
               addressBook->contacts[index].phone,
               addressBook->contacts[index].email);
            printf("-----------------------------------------------------------------\n");  
    }
    return index;
 }


//-------------------------------------------------------------Edit Contact-------------------------------------------------------------//

 void editContact(AddressBook *addressBook)
 {
 	/* Define the logic for Editcontact */
    
    int choice;
    int index = -1;

    printf("Search by\n");
    printf("1. Name \n2. Mobile \n3. Mail\n");
    printf("Enter your choice: ");

    if(scanf("%d",&choice) != 1)
    {
        printf("Invalid input\n");
        while(getchar() != '\n');
        return ;
    }

    switch (choice)
    {
    case 1:
       index =  search_by_name(addressBook);
        break;

    case 2:
       index =  search_by_phone(addressBook);
        break;
    
    case 3:
       index =  search_by_mail(addressBook);
        break;
    
    default:
        break;
    }

    if (index == -1)
    {
        return;
    }

    printf("Selected contact\n");
            printf(" Name                Mobile.no           mail.id\n");
            printf("-----------------------------------------------------------------\n");
            
            printf("%-20s %-15s %-30s\n",
               addressBook->contacts[index].name,
               addressBook->contacts[index].phone,
               addressBook->contacts[index].email);
            printf("-----------------------------------------------------------------\n");  


    
    char edit[50];
    int edit_choice;

    printf("Edit contact\n");
    printf("Select the field that you want to edit\n");
    printf("1. Name \n2. Mobile \n3. Mail\n4. Exit\n");
    printf("Enter your choice: ");

    if(scanf("%d",&edit_choice) != 1)
    {
        printf("Invalid input\n");
        while(getchar() != '\n');
        return;
    }

    switch (edit_choice)
    {
    case 1:
        while (1)
        {
            printf("Enter new name:");
            scanf(" %[^\n]",edit);

            int valid = validate_name(edit);

            if(valid)
            {
                strcpy(addressBook->contacts[index].name,edit);
                printf("Name Updated Successfully\n");

            printf("\nUpdated  Contact Details\n");
                printf(" Name                Mobile.no           mail.id\n");
                printf("-----------------------------------------------------------------\n");
                
                printf("%-20s %-15s %-30s\n",
                addressBook->contacts[index].name,
                addressBook->contacts[index].phone,
                addressBook->contacts[index].email);
                printf("-----------------------------------------------------------------\n");  
                break;
            }
            printf("Invalid name. Try again\n");
        }
    break;
    

    case 2:
        while (1)
        {
            printf("Enter new mobile no.:");
            scanf(" %[^\n]",edit);

            int valid = validate_mobile(edit,addressBook);

            if(valid)
            {
                strcpy(addressBook->contacts[index].phone,edit);
                printf("Phone Number Updated Successfully\n");

            printf("\nUpdated  Contact Details\n");
                printf(" Name                Mobile.no           mail.id\n");
                printf("-----------------------------------------------------------------\n");
                
                printf("%-20s %-15s %-30s\n",
                addressBook->contacts[index].name,
                addressBook->contacts[index].phone,
                addressBook->contacts[index].email);
                printf("-----------------------------------------------------------------\n");  
                break;
            }
        }
    break;

     case 3:
     while (1)
     {
        printf("Enter new mail id:");
        scanf(" %[^\n]",edit);

        int valid = validate_mail(edit,addressBook);

        if(valid)
        {
            strcpy(addressBook->contacts[index].email,edit);
            printf("Mail Id Updated Successfully\n");

           printf("\nUpdated  Contact Details\n");
            printf(" Name                Mobile.no           mail.id\n");
            printf("-----------------------------------------------------------------\n");
            
            printf("%-20s %-15s %-30s\n",
               addressBook->contacts[index].name,
               addressBook->contacts[index].phone,
               addressBook->contacts[index].email);
            printf("-----------------------------------------------------------------\n");  
            break;
        }
        printf("Invalid mail id . Try again\n");
     }

    break;

    case 4:
        return;

    default:
        break;
    }


     


 }

 void deleteContact(AddressBook *addressBook)
 {
 	/* Define the logic for deletecontact */
    int index = searchContact(addressBook);

    if(index == -1)
    {
        return;
    }
    char ch;
    printf("Are you sure to delete the contact(y/n):");
    scanf(" %c",&ch);

    if (ch == 'y' || ch == 'Y')
    {
        for (int  i = index; i < addressBook->contactCount-1; i++)
        {
            strcpy(addressBook->contacts[i].name,addressBook->contacts[i+1].name);
            strcpy(addressBook->contacts[i].phone,addressBook->contacts[i+1].phone);
            strcpy(addressBook->contacts[i].email,addressBook->contacts[i+1].email);
        }
        addressBook->contactCount--;
        printf("Contact deleted successfully\n");   
    }
    else
        printf("Contact not deleted\n"); 
 }
