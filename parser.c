//parses the input file into Process and event
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define _CRT_SECURE_NO_WARNINGS

/*

CHANGE fp1 AND fp2 IN BOTH PARSER() AND MAIN() ACCORDING TO
THE INPUT FILE THAT YOU WANT TO USE

In the past lab exercise, we had events in the text file that triggered a swap 
such as P15 is swapped out; but in this Lab the threshold triggers the swap.

Processes are swapped in when other Processes terminate, therefore, 
there will be no event such as P11 is swapped in; in your input text file. 
Instead, a process terminating and moving to the Exit state triggers a swap into 
memory.

Will need a blocked tracker, keeping track of the number of blocked processes
in relation to the total processes to check if it hit the threshold
*/

struct Process {
	char pid[8];
	char state[30];
	char queue[15];
};

char temp_arr[20][5];


void parser() {
	int i;
	char* rch;
	char str[200];
	char LineInFile[40][300];
	int lineP, lineQ;
	char* sch;
	char tokenizedLine[10][10];

	FILE* fp1;
	FILE* fp2;
	fp1 = fopen("inp2.txt", "r");			//open the original input file
	fp2 = fopen("inp2_parsed.txt", "w");	//output the Process ID and event to another file. 
	//You can store in variables instead of printing to file

	lineP = 0;
	i = 0;

	printf("Started parsing...\n");

	//copy first line of the original file to the new file
	if (fgets(str, sizeof(str), fp1) != NULL)
		fprintf(fp2, "%s", str);

	//parse each remaining line into Process event
	//while loop with fgets reads each line
	while (fgets(str, sizeof(str), fp1) != NULL)
	{
		lineP = 0;
		rch = strtok(str, ":;.");					// use strtok to break up the line by : or . or ; This would separate each line into the different events
		while (rch != NULL)
		{
			strcpy(LineInFile[lineP], rch);			//copy the events into an array of strings
			lineP++;								//keep track of how many events are in that line
			rch = strtok(NULL, ":;.");				//needed for strtok to continue in the while loop
		}


		//for each event (e.g. Time slice for P7 expires) pull out process number and event
		for (i = 1; i < lineP - 1; i++)
		{
			lineQ = 0;
			sch = strtok(LineInFile[i], " ");
			while (sch != NULL)
			{
				strcpy(tokenizedLine[lineQ], sch);		//use strtok to break up each line into separate words and put the words in the array of strings
				lineQ++;								//count number of valid elements
				sch = strtok(NULL, " ");
			}

			//tokenizedLine has the event separated by spaces (e.g. Time slice for P7 expires)
			if (strcmp(tokenizedLine[1], "requests") == 0)						//Process requests an I/O device
			{

				fprintf(fp2, "%s %s %s ", tokenizedLine[0], tokenizedLine[1], tokenizedLine[3]);
				//fprintf(fp2, "%s %s %s ", tokenizedLine[0], tokenizedLine[1], tokenizedLine[3]);
			}
			else if ((strcmp(tokenizedLine[2], "dispatched") == 0))				//Process is dispatched
			{
				fprintf(fp2, "%s %s ", tokenizedLine[0], tokenizedLine[2]);
			}
			else if (strcmp(tokenizedLine[0], "Time") == 0)						//Process has timed off
			{
				fprintf(fp2, "%s %s ", tokenizedLine[3], tokenizedLine[4]);
			}
			else if (strcmp(tokenizedLine[3], "out") == 0)						//Process is swapped out
			{
				fprintf(fp2, "%s %s ", tokenizedLine[0], tokenizedLine[3]);
			}
			else if (strcmp(tokenizedLine[3], "in") == 0)						//Process is swapped in
			{
				fprintf(fp2, "%s %s ", tokenizedLine[0], tokenizedLine[3]);
			}
			else if (strcmp(tokenizedLine[1], "interrupt") == 0)				//An interrupt has occured
			{
				fprintf(fp2, "%s %s ", tokenizedLine[4], tokenizedLine[1]);
			}
			else																//Process has been terminated
			{
				fprintf(fp2, "%s %s ", tokenizedLine[0], tokenizedLine[2]);
			}

		}
		fprintf(fp2, "\n");
	}

	printf("Parsing complete\n\n");

	fclose(fp1);
	fclose(fp2);

}

void queue_parser(char* queue) {
	char* temp_parse = strtok(queue, " ");
	int k = 0;
	// clean out temp_arr each time queue parser is called
	
	for (int j = 0; j < 20; j++) {
		strcpy(temp_arr[j], "");
	}
	while (temp_parse != NULL)
	{
		strcpy(temp_arr[k], temp_parse);		//use strtok to break up each line into separate words and put the words in the array of strings
		k++;									//count number of valid elements
		temp_parse = strtok(NULL, " ");
	}
}

int main()
{
	parser(); // run parser and stored parsed information in inp1_parsed.txt
	char buffer[2];
	int usr_choice;
	printf("Choose the swapping threshold:\n");
	printf("1. 80%%\n");
	printf("2. 90%%\n");
	printf("3. 100%%\n");
	// Ask the user to choose the threshold
	if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Save the user choice in integer form
		usr_choice = atoi(buffer);
    }
	FILE* fp1;
	FILE* fp2;
	fp1 = fopen("inp2.txt", "r");			//open the original input file
	fp2 = fopen("inp2_parsed.txt", "r");	//output the Process ID and event to another file. 
	char str[200];
	char str1[200];
	const char delim[] = " ";
	int curr_processes[20];
	int first = 0;

	struct Process processes[20];			// Create an array of 20 processes
	// dynamically allocate memory for each q in case it overflows
	char* printer_q=(char*)malloc(10 * sizeof(char));
	char* disk_q = (char*)malloc(10 * sizeof(char));
	char* kb_q = (char*)malloc(10 * sizeof(char));

	strcpy(disk_q, "");						// initialize memory location for queues
	strcpy(printer_q, "");
	strcpy(kb_q, "");

	int prev_id[20];
	int prev_track = 0;

	for (int i = 0; i < 20; i++) {			// Populate each process with respective ID and Ready state and default values for prev_id
		char str[3];
		char str1[5] = "P";
		sprintf(str, "%d", i + 1);
		strcat(str1, str);
		strcpy(processes[i].pid, str1);
		strcpy(processes[i].queue, "");
		prev_id[i] = 100;
	}
	while (fgets(str, sizeof(str), fp1) != NULL)	// loops through the file and retrieves each line
	{
		printf("%s\n", str);	// prints the line
		fgets(str1, sizeof(str1), fp2);	// gets the first line of parsed file
		char* token = strtok(str1, delim);
		char* token_next;
		while (token != NULL)			// separate lines of parsed file by spaces; token is the tokenized parts of the string
		{
			if (first == 0) {			// check if first line, go to next if so
				char temp[200];
				strcpy(temp, str);
				char* temp_cp = strtok(temp, delim);
				int i = 0;
				while (strcmp(temp_cp, "end\n") != 0) {
					if (temp_cp[0] == 'P') {
						curr_processes[i] = atoi(temp_cp + 1);
						i++;
					}
					else {
						strcpy(processes[curr_processes[i-1] - 1].state, temp_cp);
					}
					temp_cp = strtok(NULL, delim);
				}
				break;
			}
			//printf("%s\n", token);
			if (token[0] == 'P') {
				token_next = strtok(NULL, delim);
				//printf("The operation for %s is %s\n", token, token_next); // states the operation for current process
				int temp_id;
				if (strlen(token) > 2) {
					char temp_str[5];
					strcpy(temp_str, token + 1);
					temp_id = atoi(temp_str);
				}
				else {
					temp_id = token[1] - '0';
				}
				//printf("token_next is (%s)\n", token_next);
				if (strcmp(token_next, "requests") == 0) {				   // compares value at memory addr of token_next with corresponding state
					token_next = strtok(NULL, delim);
					if (strcmp(token_next, "disk") == 0) {
						strcpy(processes[temp_id - 1].queue, "disk");
						if (strcmp(disk_q, "") != 0) {
							strcat(disk_q, " ");
							strcat(disk_q, token);
						}
						else {
							strcat(disk_q, token);
						}
					}
					else if (strcmp(token_next, "printer") == 0) {
						strcpy(processes[temp_id - 1].queue, "printer");
						if (strcmp(printer_q, "") != 0) {
							strcat(printer_q, " ");
							strcat(printer_q, token);
						}
						else {
							strcat(printer_q, token);
						}
					}
					else {
						strcpy(processes[temp_id - 1].queue, "keyboard");
						if (strcmp(kb_q, "") != 0) {
							strcat(kb_q, " ");
							strcat(kb_q, token);
						}
						else {
							strcat(kb_q, token);
						}
					}
					strcpy(processes[temp_id - 1].state, "Blocked*");
					prev_id[prev_track] = temp_id - 1;
					prev_track++;
				}
				else if (strcmp(token_next, "admitted") == 0) {
					strcpy(processes[temp_id - 1].state, "Ready*");
					prev_id[prev_track] = temp_id - 1;
					prev_track++;
				}
				else if (strcmp(token_next, "dispatched") == 0) {
					strcpy(processes[temp_id - 1].state, "Running*");
					prev_id[prev_track] = temp_id - 1;
					prev_track++;
				}
				else if (strcmp(token_next, "expires") == 0) {
					strcpy(processes[temp_id - 1].state, "Ready*");
					prev_id[prev_track] = temp_id - 1;
					prev_track++;
				}
				else if (strcmp(token_next, "terminated") == 0) {
					strcpy(processes[temp_id - 1].state, "Exit*");
					prev_id[prev_track] = temp_id - 1;
					prev_track++;
				}
				else if (strcmp(token_next, "out") == 0) {
					// check if running
					if ((strcmp(processes[temp_id - 1].state, "Running") == 0) || (strcmp(processes[temp_id - 1].state, "Running*") == 0)) {
						strcpy(processes[temp_id - 1].state, "Ready/Suspended*");
						prev_id[prev_track] = temp_id - 1;
						prev_track++;
					}
					else {
						strcat(processes[temp_id - 1].state, "/Suspended*");	// append Suspended state to end of state
						prev_id[prev_track] = temp_id - 1;
						prev_track++;
					}
				}
				else if (strcmp(token_next, "in") == 0) {
					size_t temp_size = strlen(processes[temp_id - 1].state);
					if (processes[temp_id - 1].state[temp_size-1] == '*') {
						processes[temp_id - 1].state[temp_size - 11] = '\0';
					}
					else {
						processes[temp_id - 1].state[temp_size - 10] = '\0';
					}
					strcat(processes[temp_id - 1].state, "*");
					prev_id[prev_track] = temp_id - 1;
					prev_track++;
				}
				else if (strcmp(token_next, "interrupt") == 0) {
					if (strcmp(processes[temp_id - 1].state, "Blocked") == 0) {
						strcpy(processes[temp_id - 1].state, "Ready*");
						if (strcmp(processes[temp_id - 1].queue, "disk") == 0) {
							if (strcmp(processes[temp_id - 1].pid, disk_q) == 0) {
								disk_q[0] = '\0';
								prev_id[prev_track] = temp_id - 1;
								prev_track++;
								token = strtok(NULL, delim);
								continue;
							}
							int i = 0;
							queue_parser(disk_q);
							while (strcmp(temp_arr[i], "") != 0) {
								if (strcmp(temp_arr[i], processes[temp_id - 1].pid) != 0) {
									// Code to take out PID from queue
									// Assuming LIFO, delete from top of the stack
									size_t temp_size = strlen(disk_q);
									disk_q[temp_size - 3] = '\0';
								}
								i++;
							}
						}
						else if (strcmp(processes[temp_id - 1].queue, "printer") == 0) {
							if (strcmp(processes[temp_id - 1].pid, printer_q) == 0) {
								printer_q[0] = '\0';
								prev_id[prev_track] = temp_id - 1;
								prev_track++;
								token = strtok(NULL, delim);
								continue;
							}
							int i = 0;
							queue_parser(printer_q);
							while (strcmp(temp_arr[i], "") != 0) {
								if (strcmp(temp_arr[i], processes[temp_id - 1].pid) != 0) {
									// Code to take out PID from queue
									// Assuming LIFO, delete from top of the stack
									size_t temp_size = strlen(printer_q);
									printer_q[temp_size - 3] = '\0';
								}
								i++;
							}
						}
						else if(strcmp(processes[temp_id - 1].queue, "keyboard") == 0){
							if (strcmp(processes[temp_id - 1].pid, kb_q) == 0) {
								kb_q[0] = '\0';
								prev_id[prev_track] = temp_id - 1;
								prev_track++;
								token = strtok(NULL, delim);
								continue;
							}
							int i = 0;
							queue_parser(kb_q);
							while (strcmp(temp_arr[i], "") != 0) {
								if (strcmp(temp_arr[i], processes[temp_id - 1].pid) != 0) {
									// Code to take out PID from queue
									// Assuming LIFO, delete from top of the stack
									size_t temp_size = strlen(kb_q);
									kb_q[temp_size - 3] = '\0';
								}
								i++;
							}
						}
					}
					// do the same for Blocked/Suspended state as well
					else if (strcmp(processes[temp_id - 1].state, "Blocked/Suspended") == 0) {
						strcpy(processes[temp_id - 1].state, "Ready/Suspended*");
						if (strcmp(processes[temp_id - 1].queue, "disk") == 0) {
							if (strcmp(processes[temp_id - 1].pid, disk_q) == 0) {
								disk_q[0] = '\0';
								prev_id[prev_track] = temp_id - 1;
								prev_track++;
								token = strtok(NULL, delim);
								continue;
							}
							int i = 0;
							queue_parser(disk_q);
							while (strcmp(temp_arr[i], "") != 0) {
								if (strcmp(temp_arr[i], processes[temp_id - 1].pid) != 0) {
									// Code to take out PID from queue
									// Assuming LIFO, delete from top of the stack
									size_t temp_size = strlen(disk_q);
									disk_q[temp_size - 3] = '\0';
								}
								i++;
							}
						}
						else if (strcmp(processes[temp_id - 1].queue, "printer") == 0) {
							if (strcmp(processes[temp_id - 1].pid, printer_q) == 0) {
								printer_q[0] = '\0';
								prev_id[prev_track] = temp_id - 1;
								prev_track++;
								token = strtok(NULL, delim);
								continue;
							}
							int i = 0;
							queue_parser(printer_q);
							while (strcmp(temp_arr[i], "") != 0) {
								if (strcmp(temp_arr[i], processes[temp_id - 1].pid) != 0) {
									// Code to take out PID from queue
									// Assuming LIFO, delete from top of the stack
									size_t temp_size = strlen(printer_q);
									printer_q[temp_size - 3] = '\0';
								}
								i++;
							}
						}
						else if (strcmp(processes[temp_id - 1].queue, "keyboard") == 0) {
							if (strcmp(processes[temp_id - 1].pid, kb_q) == 0) {
								kb_q[0] = '\0';
								prev_id[prev_track] = temp_id - 1;
								prev_track++;
								token = strtok(NULL, delim);
								continue;
							}
							int i = 0;
							queue_parser(kb_q);
							while (strcmp(temp_arr[i], "") != 0) {
								if (strcmp(temp_arr[i], processes[temp_id - 1].pid) != 0) {
									// Code to take out PID from queue
									// Assuming LIFO, delete from top of the stack
									size_t temp_size = strlen(kb_q);
									kb_q[temp_size - 3] = '\0';
								}
								i++;
							}
						}
					}
					prev_id[prev_track] = temp_id - 1;
					prev_track++;
				}

			}
			token = strtok(NULL, delim);
		}
		if (first == 0) {
			first = 1;
			continue;
		}
		printf("Current states are: "); // Prints the current states
		for (int i = 0; i < 20; i++) {
			if (curr_processes[i] < -1) {
				break;
			}
			
			printf("%s %s ", processes[curr_processes[i] - 1].pid, processes[curr_processes[i] - 1].state);
		}
		printf("\n");
		printf("disk queue: %s\n", disk_q);
		printf("printer queue: %s\n", printer_q);
		printf("keyboard queue: %s\n", kb_q);
		printf("\n");
		for (int i = 0; i < 20; i++) {		// searches for previous updates and takes out '*'
			if (prev_id[i] != 100) {
				size_t temp_size = strlen(processes[prev_id[i]].state);
				processes[prev_id[i]].state[temp_size - 1] = '\0';	// use end null character to cut off the last character
				prev_id[i] = 100;
			}
		}
		prev_track = 0;
	}
	return 0;
}