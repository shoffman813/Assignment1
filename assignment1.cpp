#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int isDocNumber(string s,int count){
	/*int i;
	cout << "in isDocNumber function testing " << s << endl;
	cout << "size of string is " << s.size() << endl;
	for(i=0;i<s.size()-1;i++){
		if(!isdigit(s[i])){
			cout << "It's not a doc number" << endl;
			return 0;		
		}
	}
	if(s[s.size()] != '\n'){
		cout << "it's not a doc number" << endl;
		return 0;
	}
	else{
		cout << "It's a doc number" << endl;
		return 1;
	}*/
	int i;
	for(i=0;i<s.size();i++){
		if(!isdigit(s[i])){
			cout << "Not a number" << endl;
			return 0;
		}
	}
	if(count == stoi(s)){
		cout << "It is a doc number because count is " << count << " and s is " << s << endl;
		return 1;
	}
	else{
		cout << "not a doc number because count is " << count << " and s is " << s << endl;
		return 0;
	}
}
	
vector<vector<string> > readDocs(){
	int count = 2;
	ifstream documents;
	documents.open("dataset2.txt");
	vector<vector<string> > doc_vector;
	vector<string> doc;
	string doc_word;
	documents >> doc_word;
	doc.push_back(doc_word);
	documents >> doc_word;
	vector<string> temp;
	while(!documents.eof()){
		if(isDocNumber(doc_word,count)){
			temp = doc;
			doc_vector.push_back(temp);
			doc.clear();
			count++;
		}
		doc.push_back(doc_word);
		documents >> doc_word;
	}
	doc_vector.push_back(doc);
	documents.close();
	return doc_vector;
}

vector<vector<string> > readQueries(){
	int count = 2;
	ifstream queries;
	queries.open("queries2.txt");
	vector<vector<string> > query_vector;
	vector<string> query;
	string query_word;
	queries >> query_word;
	query.push_back(query_word);
	queries >> query_word;
	vector<string> temp;
	while(!queries.eof()){
		if(isDocNumber(query_word,count)){
			temp = query;
			query_vector.push_back(temp);
			query.clear();
			count++;
		}
		query.push_back(query_word);
		queries >> query_word;
	}
	query_vector.push_back(query);
	queries.close();
	return query_vector;
}

int main(){
	vector<vector<string> > doc_vector;
	vector<vector<string> > query_vector;
	vector<string> temp;
	doc_vector = readDocs();
	int i,j;
	cout << "master doc vector size is " << doc_vector.size() << endl;
	for(i=0;i<doc_vector.size();i++){
		temp = doc_vector.at(i);
		for(j=1;j<temp.size();j++){
			cout << temp.at(j) << " ";
		}
		cout << endl;
	}
	query_vector = readQueries();
	cout << "master query vector size is " << query_vector.size() << endl;
	for(i=0;i<query_vector.size();i++){
		temp = query_vector.at(i);
		for(j=1;j<temp.size();j++){
			cout << temp.at(j) << " ";
		}
		cout << endl;
	}	
	return 0;
}
