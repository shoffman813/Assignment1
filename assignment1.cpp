#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int isDocNumber(string s,int count){
	int i;
	for(i=0;i<s.size();i++){
		if(!isdigit(s[i])){
			return 0;
		}
	}
	if(count == stoi(s)){
		return 1;
	}
	else{
		return 0;
	}
}

int seenPreviously(string word, vector<string> seenWords){
	int i;
	for(i=0;i<seenWords.size();i++){
		if(seenWords.at(i).compare(word) == 0){
			cout << "seen the word before at index " << i << endl;
			return i;
		}
	}
	cout << "never seen that word before" << endl;
	return -1;
}	
	
vector<vector<int> > readDocs(vector<string> &seenWords){
	cout << "In function" << endl;
	int count = 2;
	ifstream documents;
	documents.open("dataset2.txt");
	vector<int> doc_freq;	//each doc will have a frequency vector
	vector<vector<int> > doc_matrix;
	string doc_word;
	int i;
	vector<int> temp;
	vector<int> temp2;
	documents >> doc_word;	//just the first doc number
	documents >> doc_word;
	while(!documents.eof()){
		cout << "On word: " << doc_word << endl;
		if(isDocNumber(doc_word,count)){
			cout << "it's a doc number" << endl;
			temp = doc_freq;
			doc_matrix.push_back(temp);
			for(i=0;i<seenWords.size();i++){
				doc_freq.at(i) = 0;
			}
			count++;
		}
		else{
			int index = seenPreviously(doc_word,seenWords);
			if(index >= 0){
				doc_freq.at(index) += 1;
			}
			else{
				seenWords.push_back(doc_word);
				cout << "Added " << doc_word << " to seenWords" << endl;
				if(count > 2){
					cout << "Count is " << count << endl;
					for(i=0;i<count-2;i++){
						doc_matrix.at(i).push_back(0);
					}
				}
				doc_freq.push_back(1);
			}
		}
		documents >> doc_word;
	}
	doc_matrix.push_back(doc_freq);
	documents.close();
	return doc_matrix;
}

vector<vector<string> > readQueries(){
	int count = 2;
	ifstream queries;
	queries.open("queries.txt");
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
	vector<vector<int> > matrix;
	vector<vector<string> > query_vector;
	vector<string> words;
	cout << "Calling readDocs" << endl;
	matrix = readDocs(words);
	int i,j;
	vector<int> freq_vector;
	for(i=0;i<matrix.size();i++){
		freq_vector = matrix.at(i);
		for(j=0;j<freq_vector.size();j++){
			cout << freq_vector.at(j);
		}
		cout << endl;
	}
	cout << "Seen words are: " << endl;
	for(i=0;i<words.size();i++){
		cout << words.at(i) << " ";
	}
	//query_vector = readQueries();
	int similarity1;
	int similarity2;
	string doc_word, q_word;
	int k;
	return 0;
}
