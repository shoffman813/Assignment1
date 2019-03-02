#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
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
			return i;
		}
	}
	return -1;
}	
	
vector<vector<int> > readDocs(vector<string> &seenWords){
	int count = 2;
	ifstream documents;
	documents.open("dataset.txt");
	vector<int> doc_freq;	//each doc will have a frequency vector
	vector<vector<int> > doc_matrix;
	string doc_word;
	int i;
	vector<int> temp;
	vector<int> temp2;
	documents >> doc_word;	//just the first doc number
	documents >> doc_word;
	while(!documents.eof()){
		if(isDocNumber(doc_word,count)){
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
				if(count > 2){
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

int getWordIndex(string word,vector<string> wordList){
	int i;
	for(i=0;i<wordList.size();i++){
		if(word.compare(wordList.at(i)) == 0)
			return i;
	}
	return -1;
}

void insertRanking(vector<vector<double> > &ranking,vector<double> pair){
	int i;
	for(i=0;i<ranking.size();i++){
		if(pair.at(0) > ranking.at(i).at(0)){
			ranking.insert(ranking.begin()+i,pair);
			return;
		}
	}
	ranking.push_back(pair);
	return;
}
	
int main(){
	vector<vector<int> > matrix;
	vector<vector<string> > query_vector;
	vector<string> words;
	matrix = readDocs(words);
	int i,j;
	query_vector = readQueries();
	int numerator1 = 0;
	int numerator2 = 0;
	int query_number;
	vector<double> pair(2);	//holds the similarity value and document number
	vector<vector<double> > doc_rank1; 	//holds document relevance ranking based on binary frequency
	vector<vector<double> > doc_rank2;	//holds document relevance ranking based on count frequency
	int index;
	double cosine1,cosine2;
	double size_query_vector1, size_query_vector2, size_doc_vector1, size_doc_vector2;
	double denominator1,denominator2;
	vector<int> query_freq;	//frequency vector for the user's query
	for(i=0;i<words.size();i++)
		query_freq.push_back(0); 	//initialize the frequency vector to zeros
	vector<string> temp_q;		//holds the vector containing the requested query
	cout << "Enter the number of the query you'd like to search, enter 0 when you want to quit" << endl;
	cin >> query_number;
	while(query_number > 0){
		size_query_vector1 = 0;
		size_query_vector2 = 0;
		temp_q = query_vector.at(query_number-1); 	//stores the correct query in temp_q
		for(i=1;i<temp_q.size();i++){			//build query frequency vector so you can use this to do the dot product later
			index = seenPreviously(temp_q.at(i),words);	
			if(index >= 0){
				query_freq.at(index) += 1;
			}
			else{
				query_freq.push_back(1);
				words.push_back(temp_q.at(i));
			}
		}
		for(i=0;i<query_freq.size();i++){	//calculate the ||q|| in cosine similarity equation
                	if(query_freq.at(i) > 0)	//the binary way
                        	size_query_vector1 += 1;
                        size_query_vector2 += (query_freq.at(i) * query_freq.at(i)); //the count way
                }
		for(j=0;j<matrix.size();j++){			//for each document in the corpus, calculate two similarities
			size_doc_vector1 = 0;
			size_doc_vector2 = 0;
			numerator1 = 0;
			numerator2 = 0;
			vector<int> document = matrix.at(j);
			for(i=0;i<document.size();i++){
				if(query_freq.at(i) > 0 && document.at(i) > 0)
					numerator1 += 1;	//calculate dot product the binary way
				numerator2 += query_freq.at(i) * document.at(i);	//calculate the dot product the count way
			}
			for(i=0;i<document.size();i++){	//calculate the ||d|| in cosine similarity equation
				if(document.at(i) > 0)
					size_doc_vector1 += 1;	//the binary way
				size_doc_vector2 += (document.at(i) * document.at(i));	//the count way
			}
			denominator1 = sqrt(size_query_vector1) * sqrt(size_doc_vector1);	//calculate denominator of cosine similarity equation the binary way
			denominator2 = sqrt(size_query_vector2) * sqrt(size_doc_vector2);	//calculate denominator of cosine similarity equation the count way
			cosine1 = numerator1/denominator1;
			cosine2 = numerator2/denominator2;
			pair.at(0) = cosine1;	//create pair holding the similarity value first, the document number second
			pair.at(1) = j+1;
			insertRanking(doc_rank1,pair);	//insert it into a vector containing the ranking the binary way
			pair.at(0) = cosine2;
			pair.at(1) = j+1;
			insertRanking(doc_rank2,pair);	//insert it into a vector containing the ranking the count way
		}
		cout << "Document ranking using binary term frequency: ";
		for(i=0;i<doc_rank1.size();i++){
			cout << doc_rank1.at(i).at(1) << " ";
		}
		cout << endl << "Document ranking using count of term frequencies: ";
		for(i=0;i<doc_rank2.size();i++){
			cout << doc_rank2.at(i).at(1) << " ";
		}
		doc_rank1.clear();
		doc_rank2.clear();
		cout << endl << "Enter the number of the query you'd like to search, enter 0 when you want to quit" << endl;
        	cin >> query_number;
		query_freq.clear();
		for(i=0;i<words.size();i++){
			query_freq.push_back(0);
		}
	}
	string doc_word, q_word;
	int k;
	return 0;
}
