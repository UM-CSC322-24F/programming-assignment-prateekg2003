//------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//------------------------------------------------------
#define MAX_STRING 128
typedef char String[MAX_STRING];
//------------------------------------------------------
typedef enum {
	slip,
	land,
	trailor,
	storage,
	error
}BoatPlace;
//------------------------------------------------------
typedef union {
	int SlipNumber;
	String BayLetter;
	String TrailorLicense;
	int StorageSpaceNumber;	
}ExtraInfo;
//------------------------------------------------------
typedef struct{
	String name;
	int Length;
	BoatPlace Location;
	ExtraInfo Information;
	float MoneyOwed;
}Boat;
//typedefs and structures to store values of boats and any other info
//------------------------------------------------------
BoatPlace InputBoatPlace(String Location){
//function converts input of BoatPlace typedef to enum type to be stored in boat structure
    if (!strcmp(Location,"slip")) {
        return(slip);
    }
    if (!strcmp(Location,"land")) {
        return(land);
    }
    if (!strcmp(Location,"trailor")) {
        return(trailor);
    }
    if (!strcmp(Location,"storage")) {
        return(storage);
    	}
    return(error);
}
//------------------------------------------------------
char* StringBoatPlace(BoatPlace Location){
//prints inventory of boats by returning a string value corresponding to Location value of boat it's passed in
	switch (Location) {
		case slip:
			return("slip");
       		     	break;
	        case land:
       		     	return("land");
            		break;
        	case trailor:
            		return("trailor");
			break;
   		case storage:
			return("storage");
            		break;
		default:
            		return("error");
            		break;
    	}	
}
//------------------------------------------------------
void ExtraInfoInput(Boat* boat, char* s){
//determines which type of union to use depending on which location inputed to boat
	ExtraInfo Info;
	
	if(boat->Location == slip){
	//checks to see if the location is a slip
		Info.SlipNumber = atoi(s);
		//converts the string inputted to an integer
		boat->Information = Info;
		//sets the boats info value to the value of the info union
	}
	//repeat the previous check for all the different boatplace types
	else if(boat->Location == land){
		strcpy(Info.BayLetter, s);
		boat->Information = Info;
	}
	else if(boat->Location == trailor){
		strcpy(Info.TrailorLicense, s);
		boat->Information = Info;
	}
	else if(boat->Location == storage){
		Info.StorageSpaceNumber = atoi(s);
		boat->Information = Info;
	}
}
//------------------------------------------------------
char* StringExtraInfo(BoatPlace Location, ExtraInfo Information){
//String to copy in t value of union and convert to a string as well so that it can be printed in printinventry
	static char buffer[MAX_STRING];
	
	switch(Location){
		case slip:
			snprintf(buffer, MAX_STRING, "#   %d", Information.SlipNumber);
			//return(buffer);
			break;
		case land:
			snprintf(buffer, MAX_STRING, "%s", Information.BayLetter);
			//return(buffer);
			break;
		case trailor:
			snprintf(buffer, MAX_STRING, "%s", Information.TrailorLicense);
			//return(buffer);
			break;
		case storage:
			snprintf(buffer, MAX_STRING, "#   %d", Information.StorageSpaceNumber);
			//return(buffer);
			break;
		default:
			strncpy(buffer, "error", MAX_STRING);
			//return()
			break;
	}
	return(buffer);
}
//------------------------------------------------------
int compareBoats(const void* a, const void* b) {
//function to be used in qsort throughout the program, namely in the printing and adding methods
	const Boat* boatA = *(const Boat**)a;
    	const Boat* boatB = *(const Boat**)b;

    	return strcasecmp(boatA->name, boatB->name);
}
//------------------------------------------------------
void stringToLower(char* Lower) {
//simple function to use the tolower on all of the characters in a string
    for (int i = 0; Lower[i]; i++) {
	Lower[i] = tolower(Lower[i]);
    }
}
//------------------------------------------------------
void PrintInventory(Boat** boats, int* numBoats){
//this function is just used to print out all of the boats in the array
	//printf("hello how do");
	qsort(boats, *numBoats, sizeof(Boat*), compareBoats);
	//sorts array first to make sure everything is correctly sorted
	
	for(int i = 0; i<*numBoats; i++){
	//iterates through all of the elements in the array
		Boat* boat = boats[i];
		//set a boat pointer equal to the value of the array at that point in the array
		printf("%-20s %-5d' %-10s %-8s Owes $%.2f\n", boat->name, boat->Length, StringBoatPlace(boat->Location), StringExtraInfo(boat->Location, boat->Information), boat->MoneyOwed);
		//prints out eleemtns of the boat with properly formatted to look good and like a table and it uses the stringboatplace and stringextrainfo in order to print out all of the enums and unions.
	}
}
//------------------------------------------------------
void FlushInputBuffer() {
//removes any extra characters after the input, prevents segfault without this when running the addboat function
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
//------------------------------------------------------
void AddBoat(Boat** boats, int* numBoats){
//takes in the pointer to the array of boats as access and the number of boats to know where to add the boat in the array
	
	String Input;
	Boat* newBoat;
	
	newBoat = (Boat *)malloc(sizeof(Boat));
	//mallocs memory for the new boat that was inputed
	
	if(newBoat == NULL){
	//checks to see if there is enough space for the new boat object
		printf("Error: malloc() failed to allocate memory\n");
		exit(EXIT_FAILURE);
	}

	printf("Please enter the boat data in CSV format                 : ");
	fgets(Input, MAX_STRING, stdin);
	//uses fgets in order to read in the boat information in csv format.
	
	strcpy(newBoat->name, strtok(Input, ","));
	//uses strtok in order to get the first part of the string, which is the name, and copies it into the value of name on the new boat
	newBoat->Length = atoi(strtok(NULL, ","));
	//uses strtok to get the next value, which is the length, and converts it from a string to a integer and sets the lengt of the new boat to that
	newBoat->Location = InputBoatPlace(strtok(NULL, ","));
	//gets the next input and uses the inputboatplace function to take in the string and convert it to the enumerated type of the location structure
	ExtraInfoInput(newBoat, strtok(NULL, ","));
	//similar to the location, uses the extrainfoinput function to take in the string value of the extrainfo and put it into the new boat's extrainfo union
	newBoat->MoneyOwed = atof(strtok(NULL, ","));
	//converts the string value to a float and stores it into the moneyowed variable in the boat structure
	
	boats[*numBoats] = newBoat;
	//sets the new boat as the most recent open spot of the array
	(*numBoats)++;
	//increments the array by one

	qsort(boats, *numBoats, sizeof(Boat*), compareBoats);
	//sorts the array
}
//------------------------------------------------------
void BoatPayment(Boat** boats, int* numBoats){

	String PayBoat;
	String PaymentInput;
	float Payment;
	int BoatFind = 0;

	printf("Please enter the boat name			: ");
	fgets(PayBoat, MAX_STRING, stdin);
	PayBoat[strcspn(PayBoat, "\n")] = 0;
	//removing the newline character at the end of the string
	stringToLower(PayBoat);
	//converts the payboat string to the lowercase version of it for case insensitivity

	for(int i = 0; i<*numBoats; i++){
	//loop through array
		String boatLower;
		//make a new string to hold the lowercase value of the boat in the array
		strcpy(boatLower, boats[i]->name);
		//copy in the name from the array to the new variable
		stringToLower(boatLower);
		//convert the name to lowercase
		if(strcmp(PayBoat, boatLower) == 0){
			
			BoatFind = 1;
			//set to 1 for a check at the end of the function

			printf("Please Enter the amount to be paid		: ");
			fgets(PaymentInput, MAX_STRING, stdin);
			//using fgets() and a string to take in the input as a string because I would get some inconsistencies while running the code by using scanf and a float
			Payment = atof(PaymentInput);
			//convert the input to a string variable
			
			if(Payment>boats[i]->MoneyOwed){
			//check if the input is greater than the input value
				printf("That is more than the amount owed, %.2f\n", boats[i]->MoneyOwed);
				//tell user they inputted a value too large
			}else{
				boats[i]->MoneyOwed -= Payment;
				//subtract the value input from the moneyowed part of the boat the user specified
			}
		}
	}
	if(!BoatFind){
	//if the boat was not found in the array, the BoatFind variable would be 0 and this tells the user there is no boat with the name they specified.
		printf("No boat with that name\n");
	}
}
//------------------------------------------------------
void Month(Boat** boats, int* numBoats){
//function to increase the payments by month of all of the boats in the array
	for(int i = 0; i<*numBoats; i++){
	//iterate through array
		switch(boats[i]->Location){
		//switch/case statement to check the location type of the boat for the calculation
			case slip:
				boats[i]->MoneyOwed += (12.5 * boats[i]->Length);
				//update the moneyowed based on the location of the boat
				break;
			case land:
				boats[i]->MoneyOwed += (14 * boats[i]->Length);
				break;
			case trailor:
				boats[i]->MoneyOwed += (25 * boats[i]->Length);
				break;
			case storage:
				boats[i]->MoneyOwed += (11.2 * boats[i]->Length);
				break;
			default:
				printf("idk how you got here but error");
				break;
		}
	}
}
//------------------------------------------------------
void RemoveBoat(Boat** boats, int* numBoats){
//function to remove a boat from array
	String removeThis;
	String boatLower;
	int boatFound = 0;

	printf("Please enter boat name			:");
	fgets(removeThis, MAX_STRING, stdin);
	//use fgets to read in the boat to be removed
	removeThis[strcspn(removeThis, "\n")] = 0;
	//remove the \n character from the end of the string
	stringToLower(removeThis);
	//convert the boat name to lowercase

	//printf("superhero");
	for(int i = 0; i<*numBoats; i++){
		//printf("pokemon music");
		strcpy(boatLower, boats[i]->name);
		//copy in the name from the array to a new string that will be lowercase
		stringToLower(boatLower);
		if(strcmp(removeThis, boatLower) == 0){
			boatFound = 1;
			//same as the payment method, checks if boat is found
			free(boats[i]);
			//free the memory that was taken up by that boat
			for(int j = i; j<*numBoats-1; j++){
				boats[j] = boats[j+1];
				//iterate through the array and shift everything down one
			}
			(*numBoats)--;
			//decrease the size of the array
			break;
		}
	}
	if (!boatFound){
		printf("No Boat with that name\n");
		//if boat isnt found in the array then user is notified that they did not input a correct value
	}
}
//------------------------------------------------------
void LoadCSV(Boat** boats, int* numBoats, String filename){
//function to lead in values from a file
	String Input;
	Boat* CSVBoat;

	FILE* file = fopen(filename, "r");
	//open a file for reading
	if (file == NULL) {
		//check if the file is null
        	printf("Error opening file: %s\n", filename);
        	return;
	}

	while(fgets(Input, MAX_STRING, file) != NULL){
	//iterate through the file until the next input is NULL
		CSVBoat = (Boat *)malloc(sizeof(Boat));
		//malloc memory for the new boat value
		
	        if(CSVBoat == NULL){
		//check if the was memory available to allocate for the boat
                	printf("Error: malloc() failed to allocate memory\n");
                	exit(1);
        	}

	        strcpy(CSVBoat->name, strtok(Input, ","));
        	CSVBoat->Length = atoi(strtok(NULL, ","));
        	CSVBoat->Location = InputBoatPlace(strtok(NULL, ","));
        	ExtraInfoInput(CSVBoat, strtok(NULL, ","));
        	CSVBoat->MoneyOwed = atof(strtok(NULL, ","));
		//similar to the process in the addboat function to get all of the information of the boat in the file

        	boats[*numBoats] = CSVBoat;
		//set the value of the array at the largest point as this new boat
        	(*numBoats)++;
		//increase the size of the array
	}
       	qsort(boats, *numBoats, sizeof(Boat*), compareBoats);
	//sort the boats here as well
}
//------------------------------------------------------
char* StringExtraInfoCSV(BoatPlace Location, ExtraInfo Information){
//this is a function similar to the stringextrainfo function but it is changed to add the numerical values correctly to the file because the other one puts a # infront of all of the values and reading it in after would set them to 0
	static char buffer[MAX_STRING];
	
	switch(Location){
		case slip:
			snprintf(buffer, MAX_STRING, "%d", Information.SlipNumber);
			break;
		case land:
			snprintf(buffer, MAX_STRING, "%s", Information.BayLetter);
			break;
		case trailor:
			snprintf(buffer, MAX_STRING, "%s", Information.TrailorLicense);
			break;
		case storage:
			snprintf(buffer, MAX_STRING, "%d", Information.StorageSpaceNumber);
			break;
		default:
			strncpy(buffer, "error", MAX_STRING);
			break;
	}
	return(buffer);
}
//------------------------------------------------------
void WritetoFile(Boat** boats,int* numBoats, String filename){
//function that writes all of the information in the array to the Boatdata.csv file.
	FILE* file = fopen(filename, "w");
	//opens a new file to write to
	if(file == NULL){
	//checks to see if the file was opened correctly
		printf("Error opening file");
		return;
	}

	Boat* byebyeBoat;
	//variable that is written into the file

	for(int i = 0; i<*numBoats; i++){
	//iterate through array
		byebyeBoat = boats[i];
		//set the variable to the boats array at that point in the array
		fprintf(file, "%s,%d,%s,%s,%.2f\n", byebyeBoat->name, byebyeBoat->Length, StringBoatPlace(byebyeBoat->Location), StringExtraInfoCSV(byebyeBoat->Location, byebyeBoat->Information), byebyeBoat->MoneyOwed);
		//use a fprintf to write in the values of the boat into the file by accessing each part of the struct and converting the union and enumerated types to strings
	}
	fclose(file);
	//close the file
}
//------------------------------------------------------
int main(int argc, char* argv[]){

	if(argc != 2){
	//checks if the arugment input is correct
		printf("ERROR: Arguements not Right");
		return(EXIT_FAILURE);
	}

	printf("Welcome to the Boat Management System\n");
	printf("-------------------------------------\n");
	//intro message
	char select = '\0';
	int numBoats = 0;
	Boat* boats[120];

	LoadCSV(boats, &numBoats, argv[1]);
	//call the function to load from the csv file

	do{
	//do while loop that continues until the user inputs x
		printf("(I)nventory, (A)dd, (R)emove, (P)ayment, (M)onth, e(X)it : ");
		scanf("%c", &select);
		//get the user input
		FlushInputBuffer();
		//call this function because i would get segfaults without it while running the add program
		select = tolower(select);
		//convert the input to lowercase
		switch(select){
		//switch/case statemtns to determine what the user selected in the menu
			case 'i':
				//printf("hello");
				PrintInventory(boats, &numBoats);
				//print the inventory if 'i'
				break;
			case 'a':
				//printf("money trees");
				AddBoat(boats, &numBoats);
				//add a boat if 'a'
				break;
			case 'r':
				RemoveBoat(boats, &numBoats);
				//remove a boat if 'r'
				break;
			case 'p':
				BoatPayment(boats, &numBoats);
				//input a payment for boat if 'p'
				break;
			case 'm':
				Month(boats, &numBoats);
				//if 'm' add to money owed
				break;
			case 'x':
				printf("Exiting Boat Management System");
				//exit program if 'x'
				break;
			default:
				printf("Invalid Option %c\n", select);
				//invalid option if else
				break;
		}
	}while(select != 'x');

	WritetoFile(boats, &numBoats, "BoatData.csv");
	//write the array to the file boatdata.csv

	for(int i = 0; i<numBoats; i++){
		free(boats[i]);
		//iterate through array and free all of the boats from the allocated memory
	}
	
	//WritetoFile(boats, &numBoats, "BoatData.csv");
	
	//printf("Exiting Boat Management System");

	return(EXIT_SUCCESS);
}

