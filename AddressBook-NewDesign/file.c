#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) {

    FILE *fptr = fopen("contacts.txt","w+");
    fprintf(fptr,"# %d\n",addressBook->contactCount);

    for (int  i = 0; i < addressBook->contactCount; i++)
        fprintf(fptr, "%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    fclose(fptr);
    printf("Contacts Saved Successfully\n");
    
    
}

void loadContactsFromFile(AddressBook *addressBook) {

    FILE *fptr = fopen("contacts.txt","r");

    if(fptr == NULL)
    {
        printf("File doesn't exist\n");
        return;
    }
    int count = 0;

    fscanf(fptr, "# %d\n", &count);

    for (int  i = 0; i < count; i++)
    {
        fscanf(fptr,"%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    addressBook->contactCount = count;

    printf("Contacts loaded: %d\n", count);

    fclose(fptr);    
}
