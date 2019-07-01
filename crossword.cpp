/*    
    Running this program looks like:
        
        System: Windows 10,Dev-C++
        Author: Harsh Devprakash Gupta
       

        Welcome to the game of Crosswords, where you play against the clock
        to see how many words you can find using adjacent letters on the
        board.  Each letter can be used only once for a given word.
 
 
        The dictionary total number of words is: 263533
        Number of words of the right length is:  259709

        Some random characters are: a c r l n e a p p u

        Do word lookups for 10 seconds.

        Enter a word: fish
        fish is in the dictionary.
        There are 3 seconds left.

        Enter a word: cat
        cat is in the dictionary.
        There are 3 seconds left.

        Enter a word: dog
        dog is in the dictionary.
        There are 1 seconds left.

        Enter a word: entropy
        entropy is in the dictionary.
        There are -3 seconds left.
        I let you finish your last move. Time is up!
 
*/
#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <cctype>     // Allows using the tolower() function
#include <ctime>      // Timer functions
#include <cstring>    // For strlen
using namespace std;

// Global constants
const char DictionaryFileName[] = "dictionary.txt"; // Dictionary File Name
const int MaxNumberOfWords = 263533; 		        // Number of dictionary words
const int MinWordLength = 3;         				// Minimum dictionary word size to be stored
const int MaxWordLength = 16;	     				// Max word size.  Add 1 for null
const int MaxUserInputLength = 81;   				// Max user input length
const int NumberOfLetters = 26;      				// Letters in the alphabet
const int TotalSecondsToPlay = 60;   				// Total number of seconds to play per board
int offsets[]={-1,-7,-6,-5,1,7,6,5}; 				// The possible position of the Letters on the board
char **dictionary;                					// 2d array of dictionary words, dynamically allocated
long int numberOfWords;           					// how many words actually found in dictionary
string userWords[2000];								// to store User Words



// Display name and program information
void displayIdentifyingInformation()
{
    
    cout << "Author: Harsh Devprakash Gupta" << endl;
    
    cout << "System: C++ in Dev C++" << endl;
    cout<< endl;
}//end displayIdentifyingInformation()


//--------------------------------------------------------------------------------------------------------------------------------------------
// Display instructions
void displayInstructions()
{
    cout<<"Welcome to the game of Boggle, where you play against the clock   \n";
    cout<<"to see how many words you can find using adjacent letters on the  \n";
    cout<<"board.  Each letter can be used only once for a given word.       \n";
    cout<<"  \n";
    cout<<"When prompted to provide input you may also:                      \n";
    cout<<"     Enter 'r' to reset the board to user-defined values.         \n";
    cout<<"     Enter 's' to solve the board and display all possible words. \n";
    cout<<"     Enter 't' to toggle the timer on/off.                        \n";
    cout<<"     Enter 'x' to exit the program.                               \n";
    cout<<"  \n";
}//end displayInstructions()


//----------------------------------------------------------------------------------------------------------------------------------------------
// Read in dictionary
// First dynamically allocate space for the dictionary.  Then read in words
// from file.  Note that the '&' is needed so that the new array address is
// passed back as a reference parameter.
void readInDictionary(
          char ** &dictionary,                      // dictionary words
          long int &numberOfWords)                  // number of words stored
{
    // Allocate space for large array of C-style strings
    dictionary = new char*[ MaxNumberOfWords];
    
    // For each array entry, allocate space for the word (C-string) to be stored there
    for (int i=0; i < MaxNumberOfWords; i++) {
        dictionary[i] = new char[ MaxWordLength+1];
        // just to be safe, initialize C-strings to all null characters
        for (int j=0; j < MaxWordLength; j++) {
            dictionary[i][j] = '\0';
        }//end for (int j=0...
    }//end for (int i...
    
    // Now read in the words from the file
    ifstream inStream;                 // declare an input stream for my use
    numberOfWords = 0;                   // Row for the current word
    inStream.open( DictionaryFileName);
    assert( ! inStream.fail() );       // make sure file open was OK
    
    // Keep repeating while input from the file yields a word
    char theWord[ 81];    // declare input space to be clearly larger than largest word
    while( inStream >> theWord) {
        int wordLength = (int)strlen( theWord);
        if( wordLength >= MinWordLength && wordLength <= MaxWordLength) {
            strcpy( dictionary[ numberOfWords], theWord);
            // increment number of words
            numberOfWords++;
        }
    }//end while( inStream...
    
    cout << "The dictionary total number of words is: " << MaxNumberOfWords << endl;
    cout << "Number of words of the right length is:  " << numberOfWords << endl;
    
    // close the file
    inStream.close();
}//end readInDictionary()


//--------------------------------------------------------------------------------------
// Use binary search to look up the search word in the dictionary array, returning index
// if found, -1 otherwise
int binarySearch( const char searchWord[ MaxWordLength+1], // word to be looked up
                  char **dictionary)               // the dictionary of words
{
    int low, mid, high;     // array indices for binary search
    int searchResult = -1;  // Stores index of word if search succeeded, else -1
    
    // Binary search for word
    low = 0;
    high = MaxNumberOfWords - 1;
    while ( low <= high)  {
        mid = (low + high) / 2;
        // searchResult negative value means word is to the left, positive value means
        // word is to the right, value of 0 means word was found
        searchResult = strcmp( searchWord, dictionary[ mid]);
        if ( searchResult == 0)  {
            // Word IS in dictionary, so return the index where the word was found
            return mid;
        }
        else if (searchResult < 0)  {
            high = mid - 1; // word should be located prior to mid location
        }
        else  {
            low = mid + 1; // word should be located after mid location
        }
    }
    
    // Word was not found
    return -1;
}//end binarySearch()


//---------------------------------------------------------------------------
// Get random character
// Find random character using a table of letter frequency counts.
// Iterate through the array and find the first position where the random number is
// less than the value stored.  The resulting index position corresponds to the
// letter to be generated (0='a', 1='b', etc.)
char getRandomCharacter()
{
    // The following table of values came from the frequency distribution of letters in the dictionary
    float letterPercentTotals[ NumberOfLetters] = {
                0.07680,  //  a
                0.09485,  //  b
                0.13527,  //  c
                0.16824,  //  d
                0.28129,  //  e
                0.29299,  //  f
                0.32033,  //  g
                0.34499,  //  h
                0.43625,  //  i
                0.43783,  //  j
                0.44627,  //  k
                0.49865,  //  l
                0.52743,  //  m
                0.59567,  //  n
                0.66222,  //  o
                0.69246,  //  p
                0.69246,  //  q
                0.76380,  //  r
                0.86042,  //  s
                0.92666,  //  t
                0.95963,  //  u
                0.96892,  //  v
                0.97616,  //  w
                0.97892,  //  x
                0.99510,  //  y
                1.000001}; //  z
    
    // generate a random number between 0..1
    // Multiply by 1.0 otherwise integer division truncates remainders
    float randomNumber = 1.0 * rand() / RAND_MAX;
    
    // Find the first position where our random number is less than the
    // value stored.  The index corresponds to the letter to be returned,
    // where 'a' is 0, 'b' is 1, and so on.
    for( int i=0; i<NumberOfLetters; i++) {
        if( randomNumber < letterPercentTotals[ i]) {
            // we found the spot.  Return the corresponding letter
            return (char) 'a' + i;
        }
    }
    
    // Sanity check
    cout << "No alphabetic character generated.  This should not have happened. Exiting program.\n";
    exit( -1);
    return ' ';   // should never get this
}//end getRandomCharacter


//-------------------------------------------------------------------------------------------------------------------------------------------
// Display Boggle Grid
void displayGrid(char boggleArray[36]) // To display the Boggle Grid
{
	for( int i=7; i<29;i++) { // for loop to print the 16 character
		if (i == 11 || i == 17 || i== 23 || i== 29 ){ // to skip the stars '*' and print a new line
			cout << endl;
		}
		else if (i == 12 || i == 18 || i== 24){ // to skip the stars '*' 
			
		}
		else {
			cout << boggleArray[i] << " "; // to print the characters
		}
    } 
    cout << endl;
}//end displaygrid()


//--------------------------------------------------------------------------------------------------------------------------------------------
// Get Random Characters in the Board
void setRandomCharOnGrid(char boggleArray[36]) // To display the Boggle Grid
{
	for( int i=0; i<36;i++) { // for loop to print the 16 character
		if (i <= 6 || i >= 29){ // to set the stars '*' for edges
			boggleArray[i]= '*';
		}
		else if (i == 11 || i == 12 || i== 17 || i== 18 || i == 23 || i == 24){ // to set the stars '*' 
			boggleArray[i]= '*';
		}
		else {
			boggleArray[i] = getRandomCharacter(); // to add the random characters
		}
    } 
}//end setRandomCharOnGrid()


//--------------------------------------------------------------------------------------------------------------------------------------------
// Words in the Grid 
bool checkWords(char userInput[], char board[] , int positionOnBoard, int indexOfUserInput, int offsets[])
{ // To check whether the words is possible on the board or not
	
	bool found= false; // boolean Variable
	
	int lengthOfInput= strlen(userInput); // length of the User Input

	if (indexOfUserInput == lengthOfInput){ // when the condition is satisfied
		return true;
	}
	else{
	
		for (int j=0; j<8; j++){
			if( board[positionOnBoard+offsets[j]]== userInput[indexOfUserInput] ){
				char letterFound1= board[positionOnBoard+offsets[j]];
				board[positionOnBoard+offsets[j]]=' '; // blanking it out so that it is not used again
				found=checkWords(userInput, board, positionOnBoard+offsets[j],++indexOfUserInput, offsets); // Calling itself again
				board[positionOnBoard+offsets[j] ]=letterFound1; // adding the letter back to the array
				if(found){
					break;
				}
			}
		}
	}
	return found;
	
}//end checkWords()


//-----------------------------------------------------------------------------------------------------------------------------------------
// Words in the Grid along with edges 
bool checkWordsOnGrid(char userInput[], char boggleBoard[] , int offsets[])
{
	bool wordCheck= false;
	for (int i=0; i<36; i++){ // for loop for the edges as weel as the characters
        	
		if(boggleBoard[i]== userInput[0]){
			char letter= boggleBoard[i];
			boggleBoard[i]= ' '; // blanking it out so that it is not used again
			wordCheck=checkWords(userInput,boggleBoard,i,1, offsets); // calling the Recursive Function to Check the Word on the grid
			boggleBoard[i]=letter; // adding the letter back to the array
			if (wordCheck){
				break;
			}
									
		}
	}
	return wordCheck;
	
}//end checkWordsOnGrid()


//-----------------------------------------------------------------------------------------------------------------------------------------
// Points Calculator 
int calculatePoints(int length)
{	
	if (length<3){
		return 0;
	}
	else if (length==3){
		return 1;
	}
	else if (length==4){
		return 2;
	}
	else if (length==5){
		return 4;
	}
	else if (length>=6){
		return length;
	}
	
	return 0;  
}//end calculatePoints()

//-------------------------------------------------------------------------------------------------------------------------------------
// Reset Board 
void resetBoard(char resetArray[36])
{
	cout<< "Enter 16 characters to be used to set the board: ";
	cin >> resetArray[7] >> resetArray[8] >> resetArray[9] >> resetArray[10] >> resetArray[13] >> resetArray[14] >> resetArray[15] >> resetArray[16];
	cin >> resetArray[19] >> resetArray[20] >> resetArray[21] >> resetArray[22] >> resetArray[25] >> resetArray[26] >> resetArray[27] >> resetArray[28];
	cout << endl;
}//end resetBoard()

//-------------------------------------------------------------------------------------------------------------------------------------
// Store the Words 
void displayWords(string arrayToStoreWords[2000], int flag)
{
	cout << "Words so far are: ";
	for (int i=0; i< flag; i++){
	 	cout << arrayToStoreWords[i] << " ";
	}
	cout << endl;
    
}//end displayWords()


//---------------------------------------------------------------------------
// Display Array of Users Words 
bool dispalyUserWords(long int numberOfWords, long int wordsPossible[], char userInput[81], int numOfMoves, int counter)
{
	//string userWords[2000];
	string userInputstr;
	bool wordRepeated= false;
	
	if (numOfMoves==1 || (userInput[0]== 'r' && strlen(userInput)==1) || counter == 1){ // Making all the valuse to default to avoid conflict
		for (int i=0; i<numberOfWords; i++){
		wordsPossible[i]= 1;
		} 	
	}

	for (int i=0; i<numberOfWords; i++){                                                //  Adding number 22 as the number where the words are same
		if (strcmp(dictionary [i],userInput)==0){                                       // comparing the words 
			if (wordsPossible[i]==1){													// if the word has not yet been guessed then it is printed
				wordsPossible[i]= 22;												
			}
			else{
				wordRepeated = true;													// else because the word has been repeated it returns true
			}		
		}	
	}  
	int minLength=MinWordLength;
	int maxLength= MaxWordLength;
	int flag=0;
	
	while (minLength<=maxLength){
		for (int i=0; i<numberOfWords; i++){										  // Adding the words into an array of string type for storing the words
			if (wordsPossible[i]==22){
				if (strlen(dictionary[i])==minLength){
					userInputstr= dictionary[i];
					userWords[0+flag]= userInputstr;
					flag++;
				}
			}
		}
	minLength= minLength+1;	
	}
	//displayWords(userWords, numOfMoves);
	return wordRepeated;
	
}//end displayUserWords()


//------------------------------------------------------------------------------------------------------------------------
// Dispaly All Possible Words 
void dispalyAllPossibleWords(long int numberOfWords, long int wordsPossible[], char boggleBoard[36])
{
	for (int i=0; i<numberOfWords; i++){ 										// Assigning 0 to make all the values as false
		wordsPossible[i]= 0;
	} 
	for (int i=0; i<numberOfWords; i++){
		wordsPossible[i]= checkWordsOnGrid(dictionary [i],boggleBoard, offsets); // If true it changes the value to 1
	}  
	int minLength =MinWordLength;
	int maxLength =MaxWordLength;
	cout << "Enter min and max word lengths to display: ";						// User Input for the words between the length 
	cin >> minLength;
	cin >> maxLength;
	cout << endl;
	cout << "Words between " << minLength << " and " << maxLength << " are: " << endl;
	while (minLength<=maxLength){
		for (int i=0; i<numberOfWords; i++){								// If the words is 1 which is true, print it
			if (wordsPossible[i]== 1){
				if (strlen(dictionary[i])==minLength){
					cout << dictionary[i] << " ";
				}
				
			}
		}
		minLength= minLength+1;	
	}
	cout << endl;
	cout << endl <<"Exiting the Program...." << endl;
	cout << endl <<"Thank you for playing......." << endl;
	exit(0);															// exiting after it has been printed
	
    
}//end dispalyAllPossibleWords()


//--------------------------------------------------------------------------------------------------------------------------------
int main()
{
    // declare variables
    
    char userInput[ MaxUserInputLength];		// to store the User Input.
    int numOfMoves=1;							// number Of Moves used. 
    int totalPoints=0;							// to store the total score.
    int lengthInput=0;							// to store length of User Input.
    int counter=0;								// to handle an error that is if the user enters a word not on board it is still found.
    //srand(time(NULL));							// to make the board unpredictable everytime the program is run.
    srand(1);									// to make it the same everytime.
    bool timerDisable= false; 					// boolean variable to store the toggle for timer.
    char boggleBoard[36];						// the array for storing boggle board along with the edges.
     
    // Declare a variable to hold a time, and get the current time.
    time_t startTime = time( NULL);
    int timePaused;
    // Display Author Information
    displayIdentifyingInformation();
    // Display Instructions for the game.
    displayInstructions();
    
    // read in dictionary.  numberOfWords returns the actual number of words found.
    readInDictionary( dictionary, numberOfWords);
    cout << endl;
    // Declare an array to store the boolean values for the words Possible Array.
    long int wordsPossible[numberOfWords];
    
    cout << "  60 seconds remaining" << endl;
	// Add the random Characters by the getRandomCharacter() along with edges.			
    setRandomCharOnGrid(boggleBoard);
 
    // Loop to while there is still time left
    int elapsedSeconds = 0; // to store elapsed seconds since the game started start
    
    while( elapsedSeconds < TotalSecondsToPlay) {
        
        // Displaying the grid
		displayGrid(boggleBoard);
		
		// Display Score
		cout << "   Score: "<< totalPoints << endl;
		
		// Prompt for and get user input
        cout << numOfMoves << ". "<<"Enter a word: ";
        cin >> userInput;
        
        // the Length of User Input
        lengthInput=strlen( userInput); 
		 
        if (userInput[0]=='r'&& lengthInput==1){	// If user enters 'r' for resetting the board.
        	resetBoard(boggleBoard);
        	cout <<endl << " "<< TotalSecondsToPlay - elapsedSeconds << " seconds remaining" << endl;
        	continue;
		}
		if (userInput[0]=='s'&& lengthInput==1){	// If user enters 's' for solutions of the board that is the words possible on the board.
        	dispalyAllPossibleWords(numberOfWords,wordsPossible, boggleBoard);
		}
		if (userInput[0]=='x'&& lengthInput==1){	// If user enters 'x' for exiting the game.
        	cout << "Thank you for playing......" << endl;
        	cout << endl;
        	cout << "Exiting the Game...." << endl;
			exit(0);
		}
      
        if( binarySearch( userInput, dictionary) != -1) { // search whether the word is in the dictionary or not.
           if (checkWordsOnGrid(userInput,boggleBoard, offsets)){ 	// If the word is on the board then check whether the word is repeated.
	           	if (dispalyUserWords(numberOfWords,wordsPossible, userInput, numOfMoves, counter)){ // If the word has not been repeated then give the user points.
	           		cout << "Sorry, that word was already previously found." << endl;
	           		displayWords(userWords, numOfMoves);
	           		numOfMoves++;
					cout <<endl << " "<< TotalSecondsToPlay - elapsedSeconds << " seconds remaining" << endl; // print the time remaining
	           		continue;
				}
			counter= counter+2;		// handling an error
           	cout << "   Worth " << calculatePoints(lengthInput) << " points." << endl; 		// print the score that the word is worth of
           	totalPoints=totalPoints+calculatePoints(lengthInput); 							// Increase the score and add it to the total score
           	displayWords(userWords, numOfMoves);											// Display the words found till now
		   }
		   else{
		   	cout << " cannot be formed on this board." << endl;
		   	if (numOfMoves==1){
		   		counter=1;
			   }  
		   	displayWords(userWords, numOfMoves);
		   }  
    	}
        else {
            cout << userInput << " was not found in the dictionary." << endl;
        }
        // Calculate how many seconds have elapsed since we started the timer.
        
        elapsedSeconds = difftime( time( NULL), startTime);
        cout <<endl << " "<< TotalSecondsToPlay - elapsedSeconds << " seconds remaining" << endl;
		numOfMoves++; // Increase the number of Moves
        
    }// end while 
    return 0;   
}//end main()
