// This is program to generate dot code from flow file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct object{
	struct object *next; // Next element in the list
	struct object *prev; // Previous element in the list
	int spaceCout; // How many space - lewel in graph
	char name[100]; // Function name
};

struct object *head; // Head of the list
struct object *tail; // Tail fo teh list

// DEBUG function
// Printing all elements in list
void print(struct object **start, struct object **end){
	struct object *jumper;
	jumper = *start;
	while(jumper != NULL){
		printf("%i ", jumper->spaceCout);
		printf("%s \n", jumper->name);
		jumper = jumper->next;
	}
	printf("\n");
}

// Inserting new object into list
void insert(struct object **start, struct object **end, char name[], int spaceCout){
	int i = 0;

	struct object *newObject = NULL;
	newObject = (struct object *)malloc(sizeof(struct object));
	newObject->next = NULL;
	newObject->prev = NULL;

	newObject->spaceCout = spaceCout;
	while(name[i] != 40) { // 40 = '('
		newObject->name[i] = name[i];
		i++;
	}

	//if list is empty
	if(*start == NULL){
		*start = newObject;
		(*start)->next = NULL;
		(*start)->prev = NULL;
		*end = *start;
		(*end)->next = NULL;
		(*end)->prev = NULL;
		return;
	} else {
		(*end)->next = newObject;
		newObject->prev = (*end);
		(*end) = newObject;
		return;
	}	
}


// Creating *.dot file
void createDotFile(struct object **start, struct object **end, FILE*dotFile){
	struct object *jumper; //parent
	struct object *jumper2; //childrens
	jumper = *start;
	
	fprintf(dotFile, "strict graph {\n");

	while(jumper->next != NULL){
		jumper2 = jumper->next;
		while(jumper->spaceCout + 4 == jumper2->spaceCout && jumper2->next != NULL){
			fprintf(dotFile, "%s -- %s\n", jumper->name, jumper2->name);
			jumper2 = jumper2->next;
		}

		jumper = jumper->next;
	}

	fprintf(dotFile, "}");
}

void prepareData(FILE *cflowFile){
	char arr[100];
	int spaceCout = 0;
	int iterator = 0;
	char name[100];
	int i;

	while(fgets(arr, 100, cflowFile) != NULL){
		
		// Counting space char
		while(arr[iterator] == ' '){
			spaceCout++;
			iterator++;
		}

		i = iterator;

		while(arr[i] != 41){ // 41 = ')'
			name[i - iterator] = arr[i];
			i++;
		}

    	insert(&head, &tail, name, spaceCout);
    	spaceCout = 0;
    	iterator = 0;
	}
}

int main(int argc, char *argv[]){
	FILE *cflowFile;
	FILE *dotFile;

	// Catching errors
	if (argc < 2) {
      printf ("Usage: %s CFLOW file name\n", argv[0]);
      exit (1);
    }

    // Opening needed files
    cflowFile = fopen (argv[1], "r");
   	dotFile = fopen("out.dot", "w+");

   	// Catching errors
    if (cflowFile == NULL || dotFile == NULL) {
      perror("Error: ");
      exit (1);
    }

    prepareData(cflowFile);

    // DEBUG
    print(&head, &tail);
    
    createDotFile(&head, &tail, dotFile);

    // Closing opened files
    fclose(cflowFile);
	fclose(dotFile);
	return 0;
}