/*
 Name: Natnael Gebremariam
 Description: Using threads to do long addition
 Input: a set of integers 
 Output: product of the input integers
*/
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <sstream>
#include <iterator> 
#include <cmath>

using namespace std;

std:: mutex mtxLock; 

vector<int> num1, num2, sum, carryOver; 

vector<int> stringToint(vector<string>& inputs)
{
    vector<int> temp; //temp vec 
    //allocate space for temp
    temp.reserve(inputs.size()); 
    //use an iterator
    vector<string>::const_iterator it; 
    //fo through and assign each spot in inputs as a string
    for(it = inputs.begin(); it != inputs.end(); ++it)
    {   
        //assigne elemnt to be indvadual string
        const string& s = *it; 
        //use stoi to conver each element into int
        int number = stoi(s); 
        //numbers into temp 
        temp.push_back(number); 
    }
    return temp; 
}
vector<string> stringTochar(string inputs)
{   //temp value to hold indvidual string 
    vector<string> temp; 
    //go through each input, conv s to char 
    for(int i = 0; i < inputs.size(); ++i)
    {   //conv elemnt in string to char 
        char c = inputs[i]; 
        //assign to char to new string, as indvidual elements to add to vector 
        string s(1, c); 
        //then push each element into vector
        temp.push_back(s); 
    }
    return temp; 
}
void solve(int start, int end)
{
    //run from the start to the end of the vectors 
    for(int i = start; i <= end; i++)
    {
        //temp to hold product and rem to store carry over 
        //add two elements in the vector
        int temp= num1[i] + num2[i]; 
        int rem = 0; 
        //if number is greater then 10 save rem & mode by 10
        if(temp >=10)
        {
            rem = 1; 
            temp %=10; 
        }
        mtxLock.lock();
        carryOver[i]=rem; 
        sum[i] = temp;  
        //pushback the carryover first(1) & sum
        //carryOver.push_back(rem); 
        //sum.push_back(temp);
        mtxLock.unlock();
    }
}
void addCarryOver(int start, int SumEnd)
{
    //add the carried one to the sum
    for(int i = 0; i < SumEnd; i++)
    {   //add carryOver to sum one element over
        sum[i]+=carryOver[i+1]; 
    }
}

void edgeCase()
{
    //run through the array and see if we have sum > 10
   for (int i = 0; i < sum.size(); i++)
   {    //use refrence to current dig to update value
        //index is digit and if the digit is greater then 10
        int& digit = sum[i];
        while (digit >= 10) 
        {
            //cal pos using the address of the dig and first element
            int pos = &digit - &sum[0];
            if (pos == 0) 
            {
                //if its zero, insert i the beginning and break cus no more carry over 
                sum.insert(sum.begin(), 0);
                break;
            }
            //if not zero, then carryover is there, mod it and place the value and 1 is handled by carry over
            sum[pos - 1]++;
            digit %= 10;
        }
    }         
}
bool largeProduct()
{
    //loop through checking each digit in the array
    for (int digit : sum) 
    {
        //if greater then 10, large product found
        if (digit >= 10) 
        {
            return false;
        }
    }
    return true;
}
void carryCheck()
{
  //if any carryover is found, insert in begining of vector
  if(carryOver[0]==1)
  {
    //insert one at the begining of sum : carryover 
    sum.insert(sum.begin(), 1); 
  }
  //if value greater then 10 is found, call edge case
  while(!largeProduct()) 
  { 
    //functio to add carry value to next position or front
    edgeCase(); 
  }

} 
void print()
{   //print the result 
	cout << "Result = ";
	for (int i = 0; i < sum.size(); i++)
	{
		cout << sum[i];
	}
	cout << endl;
}
int main()
{
    //read in two inputs as a string and conver into an int 
    //pysh back into vector and they can be global 
    //input from user as string  
    vector<string> x; 
    vector<string> y; 
    string input1;
    string input2;
    
    int power =  0; 
    int maxThreads = 0; 
    //max thread number 
    while(true)
    {   
        if(thread::hardware_concurrency()>pow(2,power))
        {
            power++; 
        }
        else if(thread::hardware_concurrency()==pow(2,power))
        {
            maxThreads=pow(2,power);
            break;  
        }
        else if(thread::hardware_concurrency()<pow(2,power))
        {
            maxThreads=pow(2,--power); 
            break; 
        }
    }
    // int maxThreads = 1;
    // int numThreads = std::thread::hardware_concurrency();

    // for (int i = 1; i < numThreads; i *= 2) {
    //     maxThreads = i;
    // }

    // if (maxThreads < numThreads) {
    //     maxThreads *= 2;
    // }
    
    cout<<"Enter a number: ";
    cin >> input1; 
    //conv to vextor<string> by reading through each char 
    x = stringTochar(input1); 
    //conv string into an int to do arithmetics 
    num1 = stringToint(x); 

    //repeat for second input 
    cout <<"Enter a number: ";
    cin >> input2;
    y = stringTochar(input2); 
    num2 = stringToint(y); 
    
    //long addition 
    int numThread = num1.size() / maxThreads;    
    int start = 0; 
    int end = numThread-1;    //end of array 
    //resize vectors 
    sum.resize(num1.size());
    carryOver.resize(num1.size());
    //declare vector of threads  
    std::vector<thread> threadPool; 
    for(int i = 0; i < maxThreads; i++)
    {
        //call function to solve using peralla calc 
        threadPool.push_back(thread(solve,start,end));
        //inc to indcate where next thread should start
        start+=numThread;
        end +=numThread; 

    }
    //join all the threads
    for(int i =0; i <threadPool.size(); i++)
    {   
        threadPool[i].join();  
    }
    threadPool.clear(); 
    int sumStart = 0; 
    int sumEnd = sum.size()-1;  //end of sum array 
    addCarryOver(sumStart, sumEnd);//add the carry one
    carryCheck();               //check for edg cases, carryovers 
    print();                    //print result

    return 0;
}

//3377364598435445778809903612001058603382305856774546614102313493
//8080212596483814704504943572350673964488834590322416825886403781
//11457577194919260483314847184351732567871140447096963439988717274


//9999999999999999999999999999999999999999999999999999999999999999
//0000000000000000000000000000000000000000000000000000000000000001
//10000000000000000000000000000000000000000000000000000000000000000