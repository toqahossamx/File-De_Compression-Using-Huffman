#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#define Max_Ascii 128


//A node that holds every occured character in the ASCII based text file and its frequency
//It represents both the priority queue nodes and the tree nodes
struct Node
{
	char character;
	int frequency;
	Node* next;
	Node* left;
	Node* right;
};


//A class that incapsulates all the methods related to the frequency table
class Frequency
{
public:

	//This function puts the frequencies in an array
	int GenerateFrequency(char filPath[], int frequency[Max_Ascii])
	{
		for (int i = 0; i < Max_Ascii; i++) //initialize the array if there was any previous values in it
			frequency[i] = 0;
		FILE* ASCIIText; //Variable for controlling the file
		ASCIIText = fopen(filPath, "r"); //Assigning the file to the variable
		if (!ASCIIText)
			return -1;
		char letter; //Variable to hold each character in the file
		while (fscanf(ASCIIText, "%c", &letter) != EOF) //Scaning each character & breaking the loop if it reached the end of the file
			frequency[letter]++; //The index of the array represents the ASCII number of the character and the value in it represents the character's frequency
		return 1;
	}

	void PrintFrequencyTable(int frequency[]) //This method prints the frequency table
	{
		for (int i = 0; i < Max_Ascii; i++) //For loop that passes through all the possible ASCII characters
		{
			if (frequency[i] == 0) //If the frequency is zero we don't need to print it
				continue;
			else
				printf("%c: %d\n", i, frequency[i]); //Print the character and the frequency
		}
	}
}; //End of the Frequency class




//A class that incapsulates all the methods related to the priority queue
class PriorityQueue
{
public:

	//A method that inserts nodes in the sorted priority queue based on their frequencies
	Node* Enqueue(Node* PQ_head, Node* node)
	{
		//If the queue is empty it will creat only one node
		if (PQ_head == NULL)
		{
			PQ_head = node;
			return PQ_head;
		}
		//If the frequency is bigger than the frequency of the first node it will be placed at the first
		else if (PQ_head->frequency > node->frequency)
		{
			node->next = PQ_head; //pointing its pointer to the head
			PQ_head = node; //setting the new head
			return PQ_head;
		}
		//General condition
		else
		{
			if ((PQ_head->next == NULL)) //if there is only 1 node in the priority queue the other node will be inserted at the end
			{
				PQ_head->next = node;
				return PQ_head;
			}
			else
			{
				Node* it = PQ_head; //A node that will iterate until it finds the proper position to insert
				while ((node->frequency >= it->next->frequency)) //Locating the node at the proper position to insert
				{
					it = it->next;
					if (it->next == NULL) //Breaking the loop if the iterator reached the end of the queue
						break;
				}
				if (it->next == NULL) //Poiting the pointer of the iterator to the new node if it was positioned at the end of the queue
					it->next = node;
				else
				{
					node->next = it->next; //Pointing the pointer of the tmp to the node infront of the iterator
					it->next = node; //Pointing the pointer of the iterator to the node
				}
				return PQ_head;
			}
		}
	}

	//This method Enqueue all the nodes automatically
	Node* EnqueueFrequencyTable(int frequency[], Node* PQ_head)
	{
		if (PQ_head != NULL) //initializing the priority queue
		{
			Node* it = PQ_head;
			while (it != NULL)
			{
				Node* tmp = it->next;
				free(it);
				it = tmp;
			}
			PQ_head = NULL;
		}
		//For loop that passes through all the possible ASCII characters
		for (int i = 0; i < Max_Ascii; i++)
		{
			//If the frequency is zero we don't need to insert it
			if (frequency[i] == 0)
				continue;
			else
			{
				Node* tmp = (Node*)malloc(sizeof(Node));
				tmp->left = NULL;
				tmp->right = NULL;
				tmp->next = NULL;
				tmp->frequency = frequency[i];
				tmp->character = i;
				PQ_head = Enqueue(PQ_head, tmp); //Insert a new node 
			}
		}
		return PQ_head;
	}

	//A method to remove the first Node and return it
	Node* Dequeue(Node* PQ_head, Node** dequeuedNode)
	{
		// If the queue is empty nothing will happen
		if (PQ_head == NULL)
			return NULL;
		//If there is only 1 node it will initialize the queue and return this node
		else if (PQ_head->next == NULL)
		{
			Node* dequeuedQ = PQ_head;
			PQ_head = NULL;
			*dequeuedNode = dequeuedQ;
			return PQ_head;
		}
		//In general condition it will return the first node and position the head to the node infront of it
		else
		{
			Node* dequeuedQ = PQ_head;
			PQ_head = PQ_head->next;
			dequeuedQ->next = NULL;
			*dequeuedNode = dequeuedQ;
			return PQ_head;
		}
	}

	//This methods prints all the nodes you have in the queue
	void PrintQueue(Node* PQ_head)
	{
		Node* it = PQ_head; // Node that passes through all the nodes you have in the queue
		//The loop will stop when it reaches the end of the queue
		while (it != NULL)
		{
			printf("%c\t%d\n", it->character, it->frequency); // Print the character and the frequency in the node
			it = it->next; //Positioning the iterator to the node infront of it
		}
	}
}; //End of the PriorityQueue class




//A class that incapsulates all the methods related to the Huffman tree
class Huffman
{
public:

	char BinaryCode[Max_Ascii][Max_Ascii] = { 0 }; // Array to store binary codes for each ASCII character
	int Character[Max_Ascii]; // Array to hold the current code being generated


	PriorityQueue pq;
	Node* HuffmanInternalNodesCreator(Node* q) //this method combines two nodes and create a new node with the sum of the frequencies of the two nodes
	{
		if (q->next == NULL)
			return q;
		else
		{
			Node* node1 = NULL;
			Node* node2 = NULL;

			q = pq.Dequeue(q, &node1); //getting the first node
			q = pq.Dequeue(q, &node2); //getting the second node

			Node* tmp = (Node*)malloc(sizeof(Node));

			tmp->frequency = node1->frequency + node2->frequency; //assigning the frequency to the new node
			tmp->left = node1; //setting the new node left pointer to the first node
			tmp->right = node2; //setting the new node left pointer to the second node
			tmp->next = NULL;
			tmp->character = NULL; //initializing the chracter since this is just an internal node

			q = pq.Enqueue(q, tmp); //inserting the new node to the priority queue

			q = HuffmanInternalNodesCreator(q);
		}
		return q;
	}//End of Function


	//Check if the  Node is a leaf or not
	int isLeaf(struct Node* root) {
		if (root->left == NULL && root->right == NULL)
			return 1;
	}


	// Function to print the binary code for each character in file and Cmd
	void printCodes(struct Node* root, int Binary[], int index, FILE* fp) {


		// Assign 0 to the left edge
		if (root->left) {
			Binary[index] = 0;
			printCodes(root->left, Binary, index + 1, fp);
		}

		// Assign 1 to right edge
		if (root->right) {
			Binary[index] = 1;
			printCodes(root->right, Binary, index + 1, fp);
		}

		//Check if The Node is a leaf then Generate its Binary Code
		if (isLeaf(root) == 1) {


			//printing the Character in file and screen
			fprintf(fp, "%c", root->character);
			printf("%c: ", root->character);

			for (int i = 0; i < index; i++) {

				fprintf(fp, "%d", Binary[i]);
				printf("%d", Binary[i]);

				BinaryCode[(unsigned char)root->character][i] = Binary[i] + '0'; // Store binary code as string

			}
			BinaryCode[(unsigned char)root->character][index] = '\0'; // Null-terminate the string

			fprintf(fp, "\n");
			printf("\n");
		}
	}
	//----------------------------------------------------------------------------------------------------
	//Compression

	// Helper function to pad binary data with zeros on the left
	void Binary(char Data_Path[], char Binary_Code_Path[])
	{
		FILE* DataF = fopen(Data_Path, "r");
		FILE* BinaryData = fopen(Binary_Code_Path, "w");
		
		char character;
		while (fscanf(DataF, "%c", &character) != EOF)
		{
			fprintf(BinaryData, "%s", BinaryCode[character]);
		}
		fclose(BinaryData);
	}

	void BitsNumber(char Binary_Code_Path[], int* bits)
	{
		FILE* binaryF = fopen(Binary_Code_Path, "r");
		int NumberOfBits = 0;
		char ch;
		while ((ch = fgetc(binaryF)) != EOF)
		{
			NumberOfBits++;
		}
		*bits = NumberOfBits;
		fclose(binaryF);
	}


	void BinaryPadded(char Data_Path[], char BinaryPadded_Path[], int bits, int* padz)
	{
		FILE* BinaryData = fopen(BinaryPadded_Path, "w");
		FILE* Data = fopen(Data_Path, "r");
		int rBits = 0;
		int pad = 0;
		rBits = bits % 8;

		if (rBits != 0)
		{
			for (int i = 0; i < 8 - rBits; i++)
			{
				fprintf(BinaryData, "%d", 0);
				pad++;
			}

		}
		*padz = pad;
		fclose(BinaryData);
		FILE* BinaryData_Pad = fopen(BinaryPadded_Path, "a");
		char character;
		while (fscanf(Data, "%c", &character) != EOF)
		{
			fprintf(BinaryData_Pad, "%s", BinaryCode[character]);
		}
		fclose(BinaryData_Pad);
		fclose(Data);
	}


	// Function to compress binary data and write it to a binary file
	int BinaryCompression(char BinaryPadded_Path[], char Compressed_File_Path[], int bits)
	{
		FILE* binaryPaddedFile = fopen(BinaryPadded_Path, "r");
		FILE* compressedFile = fopen(Compressed_File_Path, "wb");
		int* binaryData = (int*)malloc(bits * sizeof(int));

		char bit;
		int i = 0;
		while (fscanf(binaryPaddedFile, "%c", &bit) != EOF && i < bits) {
			if (bit == '0') {
				binaryData[i++] = 0;
			}
			else {
				binaryData[i++] = 1;
			}
		}
		fclose(binaryPaddedFile);

		int bufferIndex = 0;
		unsigned char buffer = 0; //               000000001

		for (int i = 0; i < bits; i++) {
			buffer |= binaryData[i] << (7 - bufferIndex);
			bufferIndex++;
			if (bufferIndex == 8) {
				fwrite(&buffer, sizeof(unsigned char), 1, compressedFile);
				buffer = 0;
				bufferIndex = 0;
			}
		}

		fclose(compressedFile);
		free(binaryData);
		return 1;
	}

	//---------------------------------------------------------
	//Decompression

	void Decompression(const char* compressedFilePath, const char* mapFilePath, const char* outputFilePath) {
		// Open the map file
		FILE* mapFile = fopen(mapFilePath, "r");
		if (mapFilePath == NULL) {
			printf("Error: Unable to open compressed file.\n");
			return;
		}

		// Read the padding information from the map file
		fseek(mapFile, -1, SEEK_END); // Move to the last byte
		int pad = fgetc(mapFile) - '0'; // Read the padding value
		fseek(mapFile, 0, SEEK_SET);

		// Read the character to binary code mappings
		char map[128][128] = { 0 };
		char character;
		while (fscanf(mapFile, "%c", &character) != EOF) {
			int i = 0;
			while ((i < 127) && ((map[(unsigned char)character][i++] = fgetc(mapFile)) != '\n') && !feof(mapFile));
			map[(unsigned char)character][i - 1] = '\0';
		}
		fclose(mapFile);

		// Open the compressed file
		FILE* compressedFile = fopen(compressedFilePath, "rb");
		if (compressedFile == NULL) {
			printf("Error: Unable to open compressed file.\n");
			return;
		}

		// Open the file for writing decompressed data
		FILE* outputFile = fopen(outputFilePath, "w");
		if (outputFile == NULL) {
			printf("Error: Unable to create output file.\n");
			fclose(compressedFile);
			return;
		}

		// Read the compressed file and convert it to binary
		fseek(compressedFile, 0, SEEK_END);
		long fileSize = ftell(compressedFile);
		fseek(compressedFile, 0, SEEK_SET);
		unsigned char* compressedData = (unsigned char*)malloc(fileSize);
		if (!compressedData) {
			printf("Error: Memory allocation failed.\n");
			fclose(compressedFile);
			fclose(outputFile);
			return;
		}
		fread(compressedData, 1, fileSize, compressedFile);
		fclose(compressedFile);

		char* binaryString = (char*)malloc(fileSize * 8 + 1);
		int binaryIndex = 0;
		for (int i = 0; i < fileSize; i++) { 
			for (int b = 7; b >= 0; b--) {
				binaryString[binaryIndex++] = ((compressedData[i] >> b) & 1) ? '1' : '0'; // Convert bytes to bits and store it
			}
		}
		binaryString[binaryIndex] = '\0'; // Null-terminate the binary string
		free(compressedData);


		
		// Skip the padding at the beginning
		char* current = binaryString + pad;
		while (*current) {
			int found = 0;
				
			for (int j = 0; j < 128; j++) { // match bits to get character by comparing to map
				if (map[j][0] != '\0' && strncmp(current, map[j], strlen(map[j])) == 0) {
					fputc(j, outputFile);
					current += strlen(map[j]);
					found = 1; 
					break;
				}
			}
			if (!found) {
				current++;
			}
		}

		free(binaryString);
		fclose(outputFile);
	}

}; //End of the Huffman class


void displayWelcomeMessage() {
	printf("				**********************************************************\n");
	printf("				*                                                        *\n");
	printf("				*               Huffman Text File Compressor             *\n");
	printf("				*                                                        *\n");
	printf("				**********************************************************\n");
	printf("				*                                                        *\n");
	printf("				*  Welcome to the Huffman Text File Compressor!          *\n");
	printf("				*  This program will help you compress your text files   *\n");
	printf("				*  efficiently using Huffman coding algorithm.           *\n");
	printf("				*                                                        *\n");
	printf("				*  Please follow the on-screen instructions to compress  *\n");
	printf("				*  your text files and save space on your storage device.*\n");
	printf("				*                                                        *\n");
	printf("				**********************************************************\n");
	printf("\n");

	printf("\n");
}


int main()
{

	displayWelcomeMessage();

	int operation;
	int Escape = 1;

	while (Escape != 0)
	{
		printf("\nHello, Choose the operation from the following\n");
		printf("1- Compress File \n2- Decompress File \n");
		scanf("%d", &operation);


		//Generating Binary Code
		int binary[100];
		int index = 0;
		int pad = 0;
		int NumberOfBits = 0;
		int NumberOfBits_padded = 0;
		int frequency[Max_Ascii] = { 0 }; // Initialing The Frequency 


		char Data_Path[] = "Text\\Data.txt"; // The User's File Path
		char CharacterCode_Path[] = "Text\\Map.cod";
		char BinaryCode_Path[] = "Text\\Bcode.cod";
		char BinaryPadded_Path[] = "Text\\Bcode_Pad.cod";
		char Compressed_Path[] = "Text\\Compressed.com";
		char Decompressed_Path[] = "Text\\Decompressed.cod";
		char Output_Path[] = "Text\\Output.txt"; // The Output File Path




		Frequency FrequencyTable; //Instance of The Frequency Class
		PriorityQueue Priority_Queue; //Instance  of The Priority Queue Class
		Huffman tree; //Instance  of The Huffman Tree Class
		Node* TreeNode = NULL; //Root of The Huffman Tree





		if (operation == 1)
		{

			//Declaring the File Path of The Data
			printf("\n\n");
			printf("Default file path is : %s\n\n", Data_Path);

			printf("Enter file path : ");
			scanf("%s", &Data_Path);

			FILE* CodesF = fopen(CharacterCode_Path, "w");
			FILE* CompressedF = fopen(Compressed_Path, "w");

			int succesful = 0; //Variable to Store the Return Value
			succesful = FrequencyTable.GenerateFrequency(Data_Path, frequency); // Assigning The Frequencies Values To The Array

			//Check If It's The Right Path
			if (succesful == -1)
			{
				printf("\nWrong file Path\n\n");
				return false;
			}

			//Printing Frequency Table
			printf("the frequency table:\n");
			FrequencyTable.PrintFrequencyTable(frequency);


			//Creating Priority Queue
			TreeNode = Priority_Queue.EnqueueFrequencyTable(frequency, TreeNode); //Inserting The Frequency Table To The Priority Queue


			//Printing The Priority Queue
			printf("The priority queue:\n");
			Priority_Queue.PrintQueue(TreeNode);


			//Building Huffman Tree
			TreeNode = tree.HuffmanInternalNodesCreator(TreeNode); //Returning The Root of The Huffman Tree

			//Generating Binary Codes
			printf("The Generated Binary Code: \n");
			tree.printCodes(TreeNode, binary, index, CodesF);

			tree.Binary(Data_Path, BinaryCode_Path);
			tree.BitsNumber(BinaryCode_Path, &NumberOfBits);
			tree.BinaryPadded(Data_Path, BinaryPadded_Path, NumberOfBits, &pad);

			//Adding Number of Pads
			fprintf(CodesF, "%d", pad);

			int l = 0;
			tree.BitsNumber(BinaryPadded_Path, &NumberOfBits_padded);
			tree.BinaryCompression(BinaryPadded_Path, Compressed_Path, NumberOfBits_padded);
			printf("Compressed File Path : %s \n", Compressed_Path);

			fclose(CompressedF);
			fclose(CodesF);

			printf("Do you want to Decompress the compressed file ? \n");
			printf("press 1) Continue \n");
			printf("press 0) Exit \n");
			if (scanf("%d", &l) == 1)
			{
				FILE* CodesF = fopen(CharacterCode_Path, "r");
				FILE* CompressedF = fopen(Compressed_Path, "r");
				tree.Decompression(Compressed_Path, CharacterCode_Path, Output_Path);
				printf("Decompressed File Path : %s \n", Output_Path);
				printf("\nFile decompressed\n\n");
				fclose(CompressedF);
				fclose(CodesF);
				
			}
			else
				continue;
		}

		else if (operation == 2)
		{
			FILE* CodesF = fopen(CharacterCode_Path, "r");
			FILE* CompressedF = fopen(Compressed_Path, "r");

			printf("Note that the padded bits must be on the end of The .cod Map file. \n\n");
			printf("Enter Compressed File Path : \n");
			scanf("%s", Compressed_Path);
			printf("Enter The Map.cod File Path :\n");
			scanf("%s", CharacterCode_Path);
			printf("Enter The Output File Path :\n");
			scanf("%s", Output_Path);

			tree.Decompression(Compressed_Path, CharacterCode_Path, Output_Path);
			printf("\nDecompressed File Path : %s \n", Output_Path);
			fclose(CompressedF);
			fclose(CodesF);
		}
		else
		{
			printf("Invalid Operation \n");
		}



		//Another Operation?
		printf("Press 1 if you want to continue \n");
		printf("Press 0 if you want to terminate \n");
		scanf("%d", &Escape);

	}

	
	return 0;
}