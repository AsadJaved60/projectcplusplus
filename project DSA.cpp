#include <iostream> //for input output
#include <fstream> //for file handling
#include <cstring> //for string operations like substring
#include <sstream> //to convert string into integer
#include <list> //Standard Template Library (STL) of linklist
#include <cstdlib> //for random number (rand())
#include <ctime> //for different random number everytime (srand())
#include <queue> //Standard Template Library (STL) of Queue

/** We used many stl classes to keep code as short and readable as possible. 
Also it is said that there is no need to rebuild a thing if you can't make it any better.
*/

using namespace std;

/**
the parent class Questions have everything common in 3 MCQs, true/false and Short Answers. It is abstract class because
it contains pure virtual function view.
*/
class Questions
{
	virtual void view() = 0; //pure virtual function
	
	string question;
    string result;
    int marks;
    
    public:
    void setQuestion(string question)
    {
    	this->question=question;
	}
	
	void setResult(string result)
    {
    	this->result=result;
	}
	
	void setMarks(int marks)
    {
    	this->marks=marks;
	}
	
	string getQuestion()
	{
		return question;
	}
	
	string getResult()
	{
		return result;
	}
	
	int getMarks()
	{
		return marks;
	}
	
	
	
};

//MCQs class child of Question Class
class MCQ : public Questions {
	string option1, option2, option3, option4;
	
public:
	MCQ(string question, string b, string c, string d, string e, string result, int marks) {
        setQuestion(question);
        this->option1 = b;
        this->option2 = c;
        this->option3 = d;
        this->option4 = e;
        setResult(result);
        setMarks(marks);
    }

    
    void view()
    {
    	cout<<getQuestion()<<endl;
    	cout<<option1<<endl;
    	cout<<option2<<endl;
    	cout<<option3<<endl;
    	cout<<option4<<endl;
	}
};

//True/False class child of Question Class
class TF : public Questions {
public:

    TF(string question, string result, int marks) {
        setQuestion(question);
        setResult(result);
        setMarks(marks);
    }
    void view()
    {
    	cout<<getQuestion()<<endl;
	}
};

//Short Answers class child of Question Class
class SA : public Questions {
public:
	
    SA(string question, string result, int marks) {
        setQuestion(question);
        setResult(result);
        setMarks(marks);
    }
    
    void view()
    {
    	cout<<getQuestion()<<endl;
	}
};

//Function to check if str can be converted to integer. If not return false otherwise return true.
bool check_number(string str) 
{
   	for (int i = 0; i < str.length(); i++)
   	{
   	if (isdigit(str[i]) == false)
   	{
		return false;
   	}
   	}	
	return true;
}

//Function to print question number asked and number obtained by the user.
void showq(queue<int> qmarks)
{
    queue<int> u = qmarks;
    int i=1;
    cout<<"Question No"<<"\t"<<"Marks Obtained"<<endl;
    while (!u.empty()) 
	{
        cout<<i<<"\t\t"<<u.front()<<" marks"<<endl;
        u.pop();
        i++;
    }
    cout<<endl;
}

int main() 
{
    string fname, lname;
	string filename;
	fstream file;
    string totFileQues;
    int totalq;
    int userQuestions;
    string answer;
    int totalscore=0;
    int ranNum;
    
	//three list for each type as asked in the question.
    list<MCQ*> QuestionMC;
    list<TF*> QuestionTF;
    list<SA*> QuestionSA;
    
    //queue containing points obtained by user for each question.
    queue<int> qmarks;
    
    cout << "What is your first name?" << endl;
    cin >> fname;
    cout << "What is your last name?" << endl;
    cin >> lname;
    cout << "What file stores your questions?" << endl;
    cin >> filename;
    
    file.open(filename.c_str(),ios::in);
    
    //reading total number of questions in the file
    getline(file, totFileQues);

	//User input validation
	while (true) {
		string temp;
        cout << "How many questions would you like (out of " <<totFileQues<<")?" << endl;
        cin >> temp;
        stringstream noq(temp);
        stringstream noOfQuestions(totFileQues);
        noq >> userQuestions;
        noOfQuestions >> totalq;
    	if(!check_number(temp))
		{
    		cout << "Sorry, that is not valid. " << endl;
		}
    	else if (userQuestions > totalq) 
		{
            cout << "Sorry, that is too many. " << endl;
        }
        else
        {
        	break;
		}
    }
    
    
    
	//Reading question, results, marks, and options from file and storing them in respective list.
    if (file.is_open()) {
        string typeOfQues;
        while (getline(file, typeOfQues)) 
		{
			//incase you have empty lines we don't have them but just in case.
			if(typeOfQues.size()==0)
			{
				cout<<"File Reading Completed. You have entered empty lines in the file."<<endl;
				break;
			}
			
			//Storing marks of question in marks.
            string mark = typeOfQues.substr(3);
            stringstream m(mark);
			int marks;
			m >> marks;
			
			//If question is a True/False type.
            if (typeOfQues[0] == 'T') {
                string question;
                getline(file, question);
                string result;
                getline(file, result);
                TF * tf = new TF(question, result, marks);
                QuestionTF.push_back(tf);
            } 
			else if (typeOfQues[0] == 'M') //if question is MCQs type. 4 Options are must in this type.
			{
                string question;
                getline(file, question);
                string op1;
                getline(file, op1);
                string op2;
                getline(file, op2);
                string op3;
                getline(file, op3);
                string op4;
                getline(file, op4);
                string result;
                getline(file, result);

                MCQ * mcq = new MCQ(question, op1, op2, op3, op4, result, marks);
                QuestionMC.push_back(mcq);
            } 
			else //If question is a Short Answer Type.
			{
                string question;
                getline(file, question);
                string result;
                getline(file, result);
                SA * sa = new SA(question, result, marks);
                QuestionSA.push_back(sa);
            }
        }
        file.close(); //File reading completed. So, closing the file.
    }


    srand(time(0)); //to get different random number everytime
    int q=0;
    
    //Quiz taking place. User is attempting quiz in this loop.
	while(q<userQuestions)
	{
		//if user skips a question he is asked another question but if all questions have been asked once this condition applies.
		if(QuestionMC.size()==0 && QuestionSA.size()==0 && QuestionTF.size()==0)
		{
			cout<<endl<<"Sorry, that's all. We have asked you all questions that are in our database."<<endl;
			break;
		}
		
		/**
		Creating random number everytime,
		which is greater than or equal to 1 and less than or equal to questions user wants to answer.
		*/
		ranNum = (rand() % userQuestions) + 1;

		//If random number is 1, MCQs question is asked.
		if(ranNum==1 && QuestionMC.size()!=0)
		{
			cout<<"\nPoints: "<<QuestionMC.back()->getMarks()<<endl;
			QuestionMC.back()->view(); //View Question
			cout<<"Your Answer: ";
			cin >> answer;

			
			if(answer==QuestionMC.back()->getResult()) //If answer is correct.
			{
				cout<<"Correct! You get "<<QuestionMC.back()->getMarks()<<" points. "<<endl;
				totalscore+=QuestionMC.back()->getMarks();
				qmarks.push(QuestionMC.back()->getMarks());
				q++;
			}
			else if(answer=="SKIP") //If question is skipped.
			{
				cout<<"You skipped this question no change in total points. "<<endl;		
			}
			else //If answer is incorrect.
			{
				cout<<"Incorrect, the answer was "<<QuestionMC.back()->getResult()<<". You lose "<<QuestionMC.back()->getMarks()<<" points."<<endl;
				totalscore-=QuestionMC.back()->getMarks();
				qmarks.push(-1*(QuestionMC.back()->getMarks()));
				q++;		
			}
			QuestionMC.pop_back();
		}
		else if(ranNum==2 && QuestionSA.size()!=0) //If random number is 2, Short Answer type question is asked.
		{
			cout<<"\nPoints: "<<QuestionSA.back()->getMarks()<<endl;
			QuestionSA.back()->view(); //View Question
			cout<<"Your Answer: ";
			cin >> answer;
			
			if(answer==QuestionSA.back()->getResult()) //If answer is correct.
			{
				cout<<"Correct! You get "<<QuestionSA.back()->getMarks()<<" points. "<<endl;
				totalscore+=QuestionSA.back()->getMarks();
				qmarks.push(QuestionSA.back()->getMarks());

				q++;
			}
			else if(answer=="SKIP") //If question is skipped.
			{
				cout<<"You skipped this question no change in total points. "<<endl;
			}
			else //If answer is incorrect.
			{
				cout<<"Incorrect, the answer was "<<QuestionSA.back()->getResult()<<". You lose "<<QuestionSA.back()->getMarks()<<" points."<<endl;
				totalscore-=QuestionSA.back()->getMarks();
				qmarks.push(-1*(QuestionSA.back()->getMarks()));
				q++;
			}
			QuestionSA.pop_back();
		}
		else if(ranNum==3 && QuestionTF.size()!=0) //If random number is 3, True/False question is asked.
		{
			cout<<"\nPoints: "<<QuestionTF.back()->getMarks()<<endl;
			QuestionTF.back()->view(); //View Question
			cout<<"Your Answer: ";
			cin >> answer;

			if(answer==QuestionTF.back()->getResult()) //If answer is correct.
			{
				cout<<"Correct! You get "<<QuestionTF.back()->getMarks()<<" points. "<<endl;
				totalscore+=QuestionTF.back()->getMarks();
				qmarks.push(QuestionTF.back()->getMarks());
				q++;
			}
			else if(answer=="SKIP") //If question is skipped.
			{
				cout<<"You skipped this question no change in total points. "<<endl;
			}
			else //If answer is incorrect.
			{
				cout<<"Incorrect, the answer was "<<QuestionTF.back()->getResult()<<". You lose "<<QuestionTF.back()->getMarks()<<" points."<<endl;
				totalscore-=QuestionTF.back()->getMarks();
				qmarks.push(-1*(QuestionTF.back()->getMarks()));
				q++;
			}
			QuestionTF.pop_back();
		}
		
	}
	
	//Showing user their result.
	cout<<endl<<endl<<"-----------**Result**-----------"<<endl<<endl;
	cout<<"Name: "<<fname<<" "<<lname<<endl<<endl;
	showq(qmarks);
	cout<<endl<<"Total Points Obtained: "<<totalscore;
}
