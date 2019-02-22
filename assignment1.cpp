#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int isDocNumber(string s){
	int i;
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
	}
}
	

int main(){
	ifstream documents;
	ifstream queries;
	documents.open("dataset2.txt");
	queries.open("queries.txt");
	vector<vector<string> > doc_vector;
	vector<vector<string> > query_vector;
	vector<string> doc;
	vector<string> query;
	string doc_word;
	string line;
	//some get line function to check if it's a doc number
	documents >> doc_word;
	doc.push_back(doc_word);
	documents >> doc_word;
	vector<string> temp;
	while(!documents.eof()){
	//	cout << "doc_word is " << doc_word << endl;
		if(isDocNumber(doc_word)){
		//	cout << "doc_word " << doc_word << " is a document number" << endl;
			temp = doc;
			doc_vector.push_back(temp);
		//	cout << "Pushed previous vector on master vector" << endl;
				
			doc.clear();
		//	cout << "Cleared doc vector for start of new doc" << endl;
		}
		doc.push_back(doc_word);
		//cout << "Pushed " << doc_word << "  on doc vector" << endl;		
		documents >> doc_word;
	}
	doc_vector.push_back(doc);
	int i,j;
	cout << "master doc vector size is " << doc_vector.size() << endl;
	for(i=0;i<doc_vector.size();i++){
		temp = doc_vector.at(i);
		//cout << "current doc has " << temp.size() << " words in it" << endl;
		for(j=1;j<temp.size();j++){
			cout << temp.at(j) << " ";
		}
		cout << endl;
	}
	documents.close();
	queries.close();
	return 0;
}
