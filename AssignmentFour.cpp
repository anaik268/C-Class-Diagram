//Abhishek Naik
//Programming Assignment 4
//may 11 2014

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

using namespace std;



string getParents(istream *);
void readFile(vector<string>&, string);
void getWords(string, vector<string>&);
bool checkLine(string, string);
void printClasses(string, string, string);
void printClasses2(string, string, string);
string subgraphCluster(string, string, string, int&);

vector<string> nestedClass(0);


vector<string> content(0);				//this will hold the content and its initial size is 1.
int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		cout << "You entered to many arguments" << endl;
		exit(0);
	}

	if (argc == 1)
	{
		cout << "you entered no command line arguments" << endl;
		exit(0);
	}
	string recognized;

	ifstream myFile;
	readFile(content, argv[1]);


	//first three lines of code
	cout << "digraph G" << endl;
	cout << "{" << endl;
	cout << "compound = true;" << endl;
	cout << "rankdir = BT;" << endl;

	for (int i = 0; i < content.size(); i++)
	{
		vector<string> words(0);
		getWords(content[i], words);
		/*	for (int k = 0; k < words.size(); k++)
		{
		cout << words[k] + " ";
		}*/
		//cout << endl;
		if (!(i + 2 > content.size()) && checkLine(content[i + 1], "nested-in"))
		{
			int tempIndex = i;
			nestedClass.push_back(words[1]);
			cout << subgraphCluster(words[1], words[1], words[1], i) << endl;
			for (int j = 1; j < words.size(); j++)
			{
				if (words[j] == "parent")
				{
					printClasses(words[1], words[j + 1], words[j + 2]);
				}
			}

		}
		else if (checkLine(content[i], "nested-in"))
		{
			nestedClass.push_back(words[3]);
			cout << subgraphCluster(words[3], words[3], words[3], i) << endl;
			for (int j = 1; j < words.size(); j++)
			{
				if (words[j] == "parent")
				{
					printClasses(words[1], words[j + 1], words[j + 2]);
				}
			}
		}
		else if (checkLine(content[i], "parent"))
		{
			for (int j = 1; j < words.size(); j++)
			{
				if (words[j] == "parent")
				{
					printClasses2(words[1], words[j + 1], words[j + 2]);
				}
			}
		}
		else
		{
			for (int j = 1; j < words.size(); j++)
			{
				if (words[j - 1] == "class")
				{
					cout << words[j] + ";" << endl;
				}

			}
		}
	}
	cout << "}" << endl;
	system("pause");
	return 0;

}



void readFile(vector<string>& content, string fileName)
{
	string line;
	fstream myFile;
	myFile.open(fileName.c_str());
	if (!myFile.is_open())
	{
		cout << "Error opening the file" << endl;
		exit(EXIT_FAILURE);
	}
	while (getline(myFile, line))
	{
		content.push_back(line);
	}
}

bool checkLine(string sent, string pat)
{
	if (pat.length() == 0)
		return true;
	while (sent.length() >= pat.length())
	{
		int x = 0;
		for (int i = 0; i < pat.length(); i++)
		{
			if (i == pat.length() - 1 && ((pat[i] == sent[i - x]) || pat[i] == '.'))
			{
				return true;
			}
			if (pat[i] == '.' || pat[i] == sent[i - x]) //check if its a period or the char matches
				continue;				//continue if it is
			else if (pat[i] == '\\' && pat[i + 1] == sent[i - x])
			{
				x++;
				continue;
			}
			else
				break;
		}
		sent = sent.substr(1, sent.length());
	}
	return false;
}

void getWords(string sent, vector<string>& word)
{
	string tempWord = "";
	for (int i = 0; i < sent.length(); i++)
	{
		if (i == sent.length() - 1)
		{
			word.push_back(tempWord + sent[i]);
			continue;
		}
		if (sent[i] == ' ')
		{
			word.push_back(tempWord);
			tempWord = "";
			continue;
		}
		tempWord += sent[i];
	}
}
void printClasses(string name, string name2, string label)
{
	cout << name << " -> " << name2 << "[label=\"" << label << "\", ltail=\"cluster" << name << "\"]" << endl;
}

void printClasses2(string name, string name2, string label)
{
	cout << name << " -> " << name2 << "[label=\"" << label << "\"]" << endl;
}

string subgraphCluster(string cName, string cNameLink, string cLink, int& index)
{
	if (index < content.size() - 1 && checkLine(content[index + 1], "nested-in"))
	{
		vector<string>tempWords(0);
		getWords(content[index + 1], tempWords);
		//if (tempWords[3] = nestedClass[nestedClass.size()-1])
		//{

		//	return "subgraph cluster" + cNameLink + " {\nlabel = \"" + cName + "\";\n\"" + cLink + "\" [shape = none, label = \"\"];\n\n}" + subgraphCluster(tempWords[1], cNameLink + "_" + tempWords[1], cLink + ":" + tempWords[1], ++index) + "\n";
		//}
//		else
		{
			nestedClass.push_back(tempWords[1]);
			return "subgraph cluster" + cNameLink + " {\nlabel = \"" + cName + "\";\n\"" + cLink + "\" [shape = none, label = \"\"];\n\n" + subgraphCluster(tempWords[1], cNameLink + "_" + tempWords[1], cLink + ":" + tempWords[1], ++index) + "\n}";
		}
	}
	else
	{
		return "subgraph cluster" + cNameLink + "{\nlabel = \"" + cName + "\";\n\"" + cLink + "\" [shape = none, label = \"\"];\n\n}";
	}
}