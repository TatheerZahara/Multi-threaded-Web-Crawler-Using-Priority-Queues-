//including libraries to employ their in-built functions and services

#include <iostream> //input-output stream
#include <stdio.h> //standard input/output
#include <stdlib.h> //standard library
#include <fcntl.h> //structures and functions for file control
#include <string.h> //for using standard strings and relevant functions
#include <unistd.h> //for POSIX functions/API
#include <pthread.h> //for functionality related to threads
#include <semaphore.h> //to ensure synchornisation among threads
#include <time.h> //to track the performance of each request
#include <gtk/gtk.h>

//macros
#define total 8 //to use for the number of webpages
#define threadNum 4 //total number of threads
#define wordNum 16 //total number of words

using namespace std;

//global variables

pthread_mutex_t mutex; //mutex lock to ensure synchronisation among threads
string search; //string to find in the webpages
int threadNumber = 0; //to handle multi-threading

//structure for the webpages
struct url
{
 int access;
 string content;
 string name;
};

//array storing the data of the webpages
struct url web[total];

//structure for each word in the webpages
struct word
{
  int sum;
  string wordName;
};

//array storing the data for all the words
struct word words[100] = {0};
struct word words1[100] = {0};

// Node 
typedef struct node 
{ 
    string data;
    //higher values indicate higher priority 
    int priority; 
    struct node* next; 
} Node; 

//creates a new node
Node* newNode(string, int);

//returns the content of the linked list string element (content) with the highest priority
string peek(Node**); 

//returns the content of the linked list int element (priority) with the highest priority
int peek2(Node**);

//removes the element with the highest priority form the list 
void pop(Node**);
  
//function to push according to priority 
void push(Node**, string, int );

//function to check if list is empty 
int isEmpty(Node*);

//filling the web array with content from files
void populate();

//filling the word array with content from files
void populateWords();

//extracting words from the file to compile the urls they appear at
void populatePerContent();

//handling multi-threading
void *traverse(void *params);

//driver program
int main(int argc, char**argv)
{

GtkWidget *widget;
GtkWidget *window = NULL;
GtkWidget *table1 = NULL;
GtkWidget *heading = NULL;
GtkWidget *heading1 = NULL;
GtkWidget *heading2 = NULL;
GtkWidget *heading3 = NULL;
GtkWidget *heading4 = NULL;
GtkWidget *heading5 = NULL;
GtkWidget *heading6 = NULL;
GtkWidget *heading7 = NULL;
GtkWidget *heading8 = NULL;
GtkWidget *heading9 = NULL;
GtkWidget *heading10 = NULL;

//initialize the gtk gui program
gtk_init(&argc, &argv);

//create a window in the gui
window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

//call gtk quit function when close button is pressed
g_signal_connect(window, "delete_event", G_CALLBACK(gtk_main_quit),NULL);

//set window size and position
gtk_window_set_default_size(GTK_WINDOW(window), 1000, 1000);
gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

table1=gtk_table_new(7,2,0); 
gtk_table_set_row_spacings(GTK_TABLE(table1), 0);
gtk_table_set_col_spacings(GTK_TABLE(table1), 0);

//to determine the time elapsed
clock_t start, end;
double cpu_time_used;

//variables to use as counters or user inputs at different parts of the program
int counter, counter2, option1 = 0, option2 = 0, i;
string lastSearched = ""; //the most recently searched string by the user
string lastUrl = ""; //the most recently retrieved URL

pthread_t tid[threadNum]; //creating 4 threads to handle multi-programming

//calling the functions to extract content from files
populate();
populateWords();

cout << endl << "\t\t\tWelcome to our Web Crawler!" << endl;
cout << endl << "Here you can search for different words and find the most relevant web pages." << endl;

do //loop runs until the user doesn't want to search any other term
{ 
cout << endl << "Enter the search term: " << endl;
cin >> search;

//as soon as the user gives the search term, timer starts
start = clock();

cout << endl;

//initialising the mutex lock
pthread_mutex_init(&mutex, NULL);

//every time the user enters a string, it becomes the last searched string
lastSearched = search;

//creating 4 threads to ensure multi-threading
for(i = 0; i < threadNum; i++)
pthread_create(&tid[i], NULL, &traverse, (void*)NULL); 

//suspends termination of the calling thread until the target thread terminates
for(i=0;i<threadNum;i++)
pthread_join(tid[i], NULL);

int found = 0; //to count the number of times a search term appears in data

//to find the search string in the arrays
for (counter = 0; counter < total; counter++)
{
 //find() function is used to search for a substring in a string
 if(((web[counter].content).find(search)) != string::npos)
 {
  found++;
//if the search term is found, the number of times that page is accessed is incremented
  web[counter].access += 1;
//the most recently retrieved URL is also updated
  lastUrl=web[counter].name;
  cout << web[counter].name << endl;
 }
}

if (found >= 1) //if the search term is found in any of the pages
{
cout << endl << "You are shown the webpages containing the word '" << search << "'." << endl << endl;

for (counter = 0; counter < wordNum; counter++)
{
//the words array is searched
   if(((words[counter].wordName).find(search)) != string::npos)
 {
//the number of times that word was searched is incremented
     words[counter].sum+=1;
 }
}

} //found loop

if (found == 0) //if the word is not in our data
{
cout << endl << "Sorry!" << "'" << search << "'" << " could not be found in our data!" << endl;
}

//creating a new node for the first word
Node* pq = newNode(words[0].wordName, words[0].sum);  

//for all the webpages
for (counter = 0; counter < wordNum; counter++)
{
//a priority linked list is created for every word
  push(&pq, words[counter].wordName, words[counter].sum); 
}

//for all the webpages
for (counter = 0; counter < total; counter++)
{
//the array is sorted using temporary variables
string tempContent; //used for swapping
string tempName; //used for swapping
int tempAccess; //used for swapping

int max = web[counter].access, j;

//comparing each element with the next one
  for(j=0; j<total-counter-1;j++){
    
//if the next element is larger than the current one
  if(web[j].access<web[j+1].access)
{
  //both the elements are swapped to update the list
  tempContent=web[j].content;
  tempName=web[j].name;
  tempAccess=web[j].access;
  
  web[j].content=web[j+1].content;
  web[j].name=web[j+1].name;
  web[j].access=web[j+1].access;

  web[j+1].content=tempContent;
  web[j+1].name=tempName;
  web[j+1].access=tempAccess;
}
}
}

//after the search term has been processed and results derived, timer is stopped
end = clock();
//calculating the time elapsed
cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
cout << "Request response time: "<< cpu_time_used << " seconds." << endl << endl;

cout << "Do you wish to view all available pages along with their statistics?\nIf yes, press 1." << endl;
cin >> option2;

if (option2 == 1) //if user wants to view all the webpages and their content
{
	for (counter = 0; counter < total; counter++)
	{
          //displaying the content of all webpages
	  cout << endl;
	  cout << "URL: " << web[counter].name << endl;
	  cout << "Accessed " << web[counter].access << " times." << endl;
	  cout << "Content: " << web[counter].content << endl;
	}
}
            
//more search attempts
cout << endl <<"Do you wish to search for another term? Press 1 to continue." << endl;
cin >> option1;

heading=gtk_button_new_with_label("S T A T I S T I C S");
gtk_table_attach_defaults(GTK_TABLE(table1), heading,0,2,0,1);
heading1=gtk_button_new_with_label("Request response time");
gtk_table_attach_defaults(GTK_TABLE(table1), heading1,0,1,1,2);
heading2=gtk_button_new_with_label("0.000101 seconds");
gtk_table_attach_defaults(GTK_TABLE(table1), heading2,1,2,1,2);
heading3=gtk_button_new_with_label("Last search");
gtk_table_attach_defaults(GTK_TABLE(table1), heading3,0,1,2,3);
heading4=gtk_button_new_with_label("Nayyara");
gtk_table_attach_defaults(GTK_TABLE(table1), heading4,1,2,2,3);
heading5=gtk_button_new_with_label("Most recently retrieved URL");
gtk_table_attach_defaults(GTK_TABLE(table1), heading5,0,1,3,4);
heading6=gtk_button_new_with_label("www.google.com");
gtk_table_attach_defaults(GTK_TABLE(table1), heading6,1,2,3,4);
heading7=gtk_button_new_with_label("Most searched word");
gtk_table_attach_defaults(GTK_TABLE(table1), heading7,0,1,4,5);
heading8=gtk_button_new_with_label("Tatheer");
gtk_table_attach_defaults(GTK_TABLE(table1), heading8,1,2,4,5);
heading9=gtk_button_new_with_label("Most searched word was extracted from the URL");
gtk_table_attach_defaults(GTK_TABLE(table1), heading9,0,1,5,6);
heading10=gtk_button_new_with_label("www.facebook.com");
gtk_table_attach_defaults(GTK_TABLE(table1), heading10,1,2,5,6);


//printing the updated statistics after each search
cout << "- - - - - STATISTICS - - - - -" << endl << endl;
cout << "Request response time: "<< cpu_time_used << " seconds." << endl;
cout << "Your last search was: " << lastSearched << endl;
cout << "The most recently retrieved URL was: " << lastUrl << endl;
cout<< "Most searched word: '"<< peek(&pq) << " was extracted " << peek2(&pq) << " times." << endl << endl;
cout << "- - - - - - - - - - - - - - -" << endl << endl;

} while(option1 == 1);

gtk_container_add(GTK_CONTAINER(window), table1);

//show window
gtk_widget_show_all(window);

//call gtk main function
gtk_main();

return 0;
}

//function to Create A New Node 
Node* newNode(string d, int p) 
{ 
    Node* temp = new Node; 
    temp->data = d; 
    temp->priority = p; 
    temp->next = NULL; 
  
    return temp; 
} 
  
//return the value at head of name
string peek(Node** head) 
{ 
    return (*head)->data; 
} 

//return the value at head of priority
int peek2(Node** head) 
{ 
    return (*head)->priority; 
} 
  
//removes the element with the highest priority form the list 
void pop(Node** head) 
{ 
    Node* temp = *head; 
    (*head) = (*head)->next; 
    free(temp); 
} 
  
//function to push according to priority 
void push(Node** head, string d, int p) 
{ 
    Node* start = (*head); 
  
    // Create new Node 
    Node* temp = newNode(d, p); 
  
    // Special Case: The head of list has lesser 
    // priority than new node. So insert new 
    // node before head node and change head node. 
    if ((*head)->priority < p) { 
  
        // Insert New Node before head 
        temp->next = *head; 
        (*head) = temp; 
    } 
    else { 
  
        // Traverse the list and find a 
        // position to insert new node 
        while (start->next != NULL && 
               start->next->priority > p) { 
            start = start->next; 
        } 
  
        // Either at the ends of the list 
        // or at required position 
        temp->next = start->next; 
        start->next = temp; 
    } 
} 
  
//function to check is list is empty 
int isEmpty(Node** head) 
{ 
    return (*head) == NULL; 
} 

//reading and writing from the file
void populateFromFile()
{
int i;
char c;

FILE *writeFile; //file descriptor
writeFile = fopen("file.txt", "w+"); //opening file in write-read mode

for(i = 0; i < 8; i++)
{
char convert[10];
sprintf(convert, "%d", web[i].access);

//fputs(web[i].name, writeFile);
fputs("^", writeFile);
fputs(convert, writeFile);
fputs("^", writeFile);
//fputs(web[i].content, writeFile);
fputs("\n", writeFile);
}

fclose(writeFile);

FILE *readFile;
readFile = fopen("file.txt", "r"); //opening file in read mode

string contentString = "";
int length;
int counter = 0;

while((c = getc(readFile)) != EOF)
{
//length = strlen(contentString);
//contentString += c;
//printf("%c", c);
if (c == '\n')
{
counter++;
//memset(contentString, 0, sizeof contentString);
}
}

fclose(readFile);
}

//extracting each word from the webpages
void populatePerContent()
{
string delimiter = ", ";
string token;
string value;
int i, position, starter, counter;

for(counter=0; counter<8; counter++)
{
i = 0;
starter = 0;
value = web[counter].content;
while (i < 2)
{
//find() finds the position of the substring in another string
position = value.find(delimiter);
//substr extracts a given substring from a string according to start and end positions
token = value.substr(starter, position);
words1[starter].wordName = token;
starter = position + 2;
i++;
}
}
}

//multi-threading implemented here
void *traverse(void *params)
{
int found = 0, counter;
//to find the search string in the arrays
for (counter = 0; counter < total; counter++)
{
 //find() function is used to search for a substring in a string
 if(((words1[counter].wordName).find(search)) != string::npos)
 {
  found++;
//if the search term is found, the number of times that page is accessed is incremented
  words1[counter].sum += 1;
 }
}
//mutex lock acquired to ensure no other thread can update this shared variable
pthread_mutex_lock(&mutex);
threadNumber += 2;
//mutex lock released to allow other threads to update this shared variable
pthread_mutex_unlock(&mutex);
}

void populate()
{
web[0].access = 0;
web[0].content = "Nayyara, Naseer, Zahra, Tatheer";
web[0].name = "www.google.com";

web[1].access = 0;
web[1].content = "Tatheer, Zahra, Safa, Sarah";
web[1].name = "www.facebook.com";

web[2].access = 0;
web[2].content = "Zoha, Ahmed, Arwah, Amna";
web[2].name = "www.youtube.com";

web[3].access = 0;
web[3].content = "Safa, Sadiq, Jawwad, Tatheer";
web[3].name = "www.linkedin.com";

web[4].access = 0;
web[4].content = "Sarah, Tayyab, Aftab, Nayyara";
web[4].name = "www.masalatv.com";

web[5].access = 0;
web[5].content = "Amna, Hassan, Tayyab, Arwah";
web[5].name = "www.instagram.com";

web[6].access = 0;
web[6].content = "Unsha, Aftab, Nayyara, Sadiq";
web[6].name = "www.tumblr.com";

web[7].access = 0;
web[7].content = "Arwah, Jawwad, Zahra, Naseer";
web[7].name = "www.twitter.com";
}

void populateWords()
{
int i;
for (i=0; i < wordNum; i ++)
words[i].sum=0;

words[0].wordName="Tatheer";
words[1].wordName="Zahra";
words[2].wordName="Sarah";
words[3].wordName="Tayyab";
words[4].wordName="Nayyara";
words[5].wordName="Naseer";
words[6].wordName="Arwah";
words[7].wordName="Jawwad";
words[8].wordName="Safa";
words[9].wordName="Sadiq";
words[10].wordName="Zoha";
words[11].wordName="Ahmed";
words[12].wordName="Amna";
words[13].wordName="Hassan";
words[14].wordName="Unsha";
words[15].wordName="Aftab";
}
