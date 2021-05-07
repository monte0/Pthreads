#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <time.h>

using namespace std;

//Global Variables queue, run time, producer and consumer count
queue<int> myQ;
int run_program = 0;
int producer_count = 0;
int consumer_count = 0;

//max size of buffer(queue)
int buffer_size = 10;

//functions
void printIntro();
void printError();
void printQueue(queue <int> q);
void* addNumber(void*);
void* takeNumber(void*);

//initialize global mutex and conditional variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

int main(int argc, char* argv[])
{
  printIntro();
  if(argc < 4)
    {
      //if not enough info then print error and exit
      printError();
      exit(1);
    }
  
  run_program = atoi(argv[1]);//time main thread will run for
  producer_count = atoi(argv[2]);//the number of producer threads to make
  consumer_count = atoi(argv[3]);//the number of consumer threads to make
  pthread_t producer[producer_count];//array of producers
  pthread_t consumer[consumer_count];//array of consumers
  int p_num[producer_count];//integers for each producer created
  int c_num[consumer_count];//integers for each consumer created

  for(int i = 0; i <producer_count; i++)
    {
      //create producers
      p_num[i] = pthread_create(&producer[i], NULL, addNumber, NULL);
    }

  for(int i = 0; i < consumer_count; i++)
    {
      //create consumers
      c_num[i] = pthread_create(&consumer[i], NULL, takeNumber, NULL);
    }
  
  sleep(run_program);//main thread will sleep then exit
  exit(1);

  return 0;
}
//this function prints the intro of the program
void printIntro()
{
  cout << "CS 433 Programming Assignment 4" << endl;
  cout << "Author: Miguel Cea" << endl;
  cout << "Date: 04/20/2020" << endl;
  cout << "Course: CS433 (Operating Systems)" << endl;
  cout << "Description: Program to implement the Producer-Consumer problem using pthreads" << endl;
  cout << "=================================" << endl;
}
//this function prints the error message if not enough parameters
void printError()
{
  cout << "You have entered too few parameters to run the program. You must enter ";
  cout << "three command-line arguments:" << endl;
  cout << " - amount of time to run the program (in seconds, positive integer";
  cout << " that is nonzero)" << endl;
  cout << " - number of producer threads to create (positive integer or zero)" << endl;
  cout << " - number of consumer threads to create (positive integer or zero)" << endl;
  exit(1);
}
//prints the contents of a queue
void printQueue(queue <int> q)
{
  int tmp = 0;
  cout << "The current content of the buffer is [";
  for(int i = 0; i < q.size(); i++)
    {
      if(q.size() == 1)//if only one number
	{
	  tmp = q.front();
	  q.pop();
	  q.push(tmp);
	  cout << tmp;
	}
      else if(i == q.size() - 1)//the last number
	{
	  tmp = q.front();
          q.pop();
          q.push(tmp);
          cout << tmp;
	}
      else//middle numbers
	{
	  tmp = q.front();
	  q.pop();
	  q.push(tmp);
	  cout << tmp;
	  cout << ", ";
	}
    }
  cout << "]" << endl << endl;
}
//function for the producers to run
void* addNumber(void*)
{
  srand(time(NULL));//random number
  double num = 0;//holds random time the producer will sleep 
  int random_number = 0;//random number to add to queue

  while(1)
    {
      num = rand()%1000000;//generate random number to sleep for
      usleep(num);

      pthread_mutex_lock(&mutex);//lock before going into critical section
      while(myQ.size() == buffer_size)//buffer is full
	{
	  pthread_cond_wait(&full, &mutex);//wait and give back lock and wait for condition
	}
      //there is room in queue
      random_number = rand() % 100;//generate random number from 1 - 100
      myQ.push(random_number);//add to queue
      cout << "item " << random_number << " inserted by a producer. " << endl;
      printQueue(myQ);//print queue
      pthread_cond_broadcast(&empty);//signal all waiting threads
      pthread_mutex_unlock(&mutex);//give up lock
    }
}
//function for the consumers to run
void* takeNumber(void*)
{
  srand(time(NULL));
  double num = 0;//random number to sleep for
  int number = 0;//holds number taken from queue

  while(1)
    {
      num = rand()%1000000;//generate random number to sleep for
      usleep(num);

      pthread_mutex_lock(&mutex);//lock before going into critical section
      while(myQ.size() == 0)//its empty
	{
	  pthread_cond_wait(&empty, &mutex);//wait and give up lock
	}
      //there are numbers in the queue
      number = myQ.front();//get number in front
      myQ.pop();//reduce queue
      cout << "Item " << number << " removed by a consumer." << endl; 
      printQueue(myQ);//print queue
      pthread_cond_broadcast(&full);//signal all waiting threads
      pthread_mutex_unlock(&mutex);//give up lock
    }
}
