#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

int isDocNumber(string s,int count){	//function detects if a document number is encountered --> start a new vector
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

int seenPreviously(string word, vector<string> seenWords){	//function detects if term was in one of the previous documents or if it is a new term
	int i;
	for(i=0;i<seenWords.size();i++){
		if(seenWords.at(i).compare(word) == 0){
			return i;
		}
	}
	return -1;
}	
	
vector<vector<int> > readDocs(vector<string> &seenWords){
	
	//declarations

	int count = 2;		//keeps track of what number document we're on (disregards the first one because we handle it differently)
	ifstream documents;
	documents.open("dataset.txt");
	vector<int> doc_freq;	//each doc will have a frequency vector containing term frequencies
	vector<vector<int> > doc_matrix;	//matrix of dimensions #documents by #terms seen in corpus
	string doc_word;	//stores each word read in from file
	int i;
	vector<int> temp;
	vector<int> temp2;

	//start reading
	
	documents >> doc_word;	//just the first doc number (1)
	documents >> doc_word;	//go ahead and read the first word in the first document
	while(!documents.eof()){	//keep reading until end of file
		if(isDocNumber(doc_word,count)){	//check to see if we found a document number
			temp = doc_freq;		//if so, add the current document's frequency vector to the matrix
			doc_matrix.push_back(temp);
			for(i=0;i<seenWords.size();i++){	//get the frequency vector ready for the next document by setting it to all zeros
				doc_freq.at(i) = 0;
			}
			count++;	//increment count because we're moving on to the next document
		}
		else{		//it is not a document number, it is a term within a document
			int index = seenPreviously(doc_word,seenWords);	//check to see if we have already seen the word before
			if(index >= 0){	//we have seen the word before and know it's index number in the frequency vector
				doc_freq.at(index) += 1;	//add one to that term's frequency
			}
			else{		//we have not seen the word before
				seenWords.push_back(doc_word);	//add it to the seenWords vector for the future
					for(i=0;i<count-2;i++){		//since we've added a column to this frequency vector, 
						doc_matrix.at(i).push_back(0);	//go back to all the previous rows and add a 0 to make the dimensions match
					}
				doc_freq.push_back(1);	//because it is a new term, we know the frequency is only 1, add this to the end of the vector
			}
		}
		documents >> doc_word;	//read the next term in
	}
	doc_matrix.push_back(doc_freq);	//add the final document's frequency vector to the matrix
	documents.close();
	return doc_matrix;
}

vector<vector<string> > readQueries(){	//function does not create a frequency vector for each query; rather, it stores each query in a vector

	//declarations

	int count = 2;	//keeps track of what number query we're on (disregards the first one since it is handled differently)
	ifstream queries;
	queries.open("queries.txt");
	vector<vector<string> > query_vector;
	vector<string> query;
	string query_word;
	
	//start reading

	queries >> query_word;		//reads query number in
	query.push_back(query_word);	//adds this to vector (the number is needed later)
	queries >> query_word;
	vector<string> temp;
	while(!queries.eof()){
		if(isDocNumber(query_word,count)){	//uses this function to tell if term is a query number or not
			temp = query;
			query_vector.push_back(temp);	//if so, add this query to the vector of queries and start reading the next one
			query.clear();			//clear the vector variable to start the new one
			count++;
		}
		query.push_back(query_word);		//add the number or term to the vector
		queries >> query_word;			//read the next number or term in
	}
	query_vector.push_back(query);		//add the final query's vector to the vector of all queries
	queries.close();
	return query_vector;
}

int getWordIndex(string word,vector<string> wordList){	//takes the vector containing ALL seen words in the corpus and returns the index of string passed to the function
	int i;
	for(i=0;i<wordList.size();i++){
		if(word.compare(wordList.at(i)) == 0)
			return i;
	}
	return -1;
}

void insertRanking(vector<vector<double> > &ranking,vector<double> pair){	//inserts the document in the correct location in the ranking vector 
										//according to it's cosine similarity value
	int i;
	for(i=0;i<ranking.size();i++){
		if(pair.at(0) > ranking.at(i).at(0)){
			ranking.insert(ranking.begin()+i,pair);
			return;
		}
	}
	ranking.push_back(pair);	//the document with the smallest cosine similarity value gets added to the back
	return;
}

vector<int> computeQueryFrequencyVector(vector<string> query, vector<string> words){
	vector<int> frequency_vector;
	int i;
	for(i=0;i<words.size();i++){	//initialize frequency vector to all zeros
		frequency_vector.push_back(0);
	}
	for(i=1;i<query.size();i++){                   //build query frequency vector so you can use this to do the dot product later
        	int index = seenPreviously(query.at(i),words);     //checks to see if the term has been seen in any previous documents or queries
        	if(index >= 0){         //it has been seen before
                	frequency_vector.at(index) += 1;      //increment the value at that index by 1
                }
                else{           //it has not been seen before
                	frequency_vector.push_back(1);        //add a 1 to the end of the query frequency vector
                        words.push_back(query.at(i));  //add that word to the end of the list of seen words
        	}
	}
	return frequency_vector;
}

double computeBinaryCosineSimilarity(vector<int> document,vector<int> query){
	int doc_size = 0;   //stores value for ||d|| in cosine similarity equation
        int numerator = 0;	//stores numerator in cosine similarity equation
	int query_size = 0;	//stores value for ||q|| in cosine similarity equation
        int i;
	for(i=0;i<document.size();i++){         //for each term frequency in the vector
        	if(query.at(i) > 0 && document.at(i) > 0)  //if both the query's freq vector and document's freq vector are > 0 at the ith index
                	numerator += 1;        //calculate dot product the binary way by just adding 1 to the numerator (1 * 1 = 1)
        }
        for(i=0;i<document.size();i++){ //calculate the ||d|| in cosine similarity equation
        	if(document.at(i) > 0)
                	doc_size += 1;  
        }
	for(i=0;i<query.size();i++){
		if(query.at(i) > 0)
			query_size += 1;
	}
        double denominator = sqrt(query_size) * sqrt(doc_size);       //calculate denominator of cosine similarity equation
        double cosine = numerator/denominator;
	return cosine;
}

double computeCountCosineSimilarity(vector<int> document,vector<int> query){	//same function as above but for count term frequency
	int doc_size = 0;  
        int numerator = 0;
	int query_size = 0;
        int i;
	for(i=0;i<document.size();i++){         //for each term frequency in the vector
        	numerator += query.at(i) * document.at(i);        //calculate the dot product the count way
        }
        for(i=0;i<document.size();i++){ //calculate the ||d|| in cosine similarity equation
        	doc_size += (document.at(i) * document.at(i));  //the count way
        }
	for(i=0;i<query.size();i++){
		query_size += (query.at(i) * query.at(i));
	}
        double denominator = sqrt(query_size) * sqrt(doc_size);       //calculate denominator of cosine similarity equation the count way
        double cosine = numerator/denominator;
	return cosine;
}

void printResults(int option, vector<vector<double> > ranking1, vector<vector<double> > ranking2){
	cout << "Document ranking using binary term frequency: ";
        int i;
	if(option == 1){        //want to see all of the ranked documents
        	for(i=0;i<ranking1.size();i++){
                	cout << ranking1.at(i).at(1) << " ";
                }
                cout << endl << "Document ranking using count of term frequencies: ";
                for(i=0;i<ranking2.size();i++){
                	cout << ranking2.at(i).at(1) << " ";
                }
        }
        if(option == 2){        //only want to see the top 10 documents
        	for(i=0;i<10;i++){
                	cout << ranking1.at(i).at(1) << " " << ranking1.at(i).at(0) << " ";
                }
                cout << endl << "Document ranking using count of term frequencies: ";
                for(i=0;i<10;i++){
                	cout << ranking2.at(i).at(1) << " " << ranking2.at(i).at(0) << " ";
                }
        }
	return;

}

/*Assignment 2 Functions*/

void randomizeCentroids(int classNum, int wordsSize, vector<vector<double> > &cMatrix) { //Generates initial matrix of randomized centroids
	double a = 0;		
	srand(time(NULL));
	for(int i=0; i < classNum; i++) {
		for(int j = 0; j< wordsSize; j++) {
			a = rand()%10;
			a = a/10.0;
			cMatrix.at(i).push_back(a);
		}
	}
	return;
}

//version of computeCountCosineSimilarity with doubles instead of ints
double computeCountCosineSimilarity2(vector<double> document,vector<double> query, int size){    //same function as above but for count term frequency
        double doc_size = 0;
        double numerator = 0;
        double query_size = 0;
        int i;
        for(i=0;i<size;i++){         //for each term frequency in the vector
                numerator += query.at(i) * document.at(i);        //calculate the dot product the count way
        }
        for(i=0;i<size;i++){ //calculate the ||d|| in cosine similarity equation
                doc_size += (document.at(i) * document.at(i));  //the count way
        }
        for(i=0;i<size;i++){
                query_size += (query.at(i) * query.at(i));
        }
        double denominator = sqrt(query_size) * sqrt(doc_size);       //calculate denominator of cosine similarity equation the count way
        double cosine = numerator/denominator;
	return cosine;
}

vector<double> addVectors(vector<double> v1, vector<double> v2, int size){
	for(int i = 0;i<size;i++){
		v1.at(i) += v2.at(i);
	}
	return v1;
}

//Recomputes a centroid for a given class and saves it to the centroid matrix
void recomputeCentroid(vector<double> allDocClass, vector<vector<double> > &centroidMatrix, vector<vector<double> > matrix) {
	int i;
	for(i=0;i<centroidMatrix.size();i++){
		vector<int> docClass;
		for(int j=0;j<allDocClass.size();j++){
			if(allDocClass.at(j) == i+1){
				docClass.push_back(j+1);
			}
		}
		double dc = docClass.size(); //Number of documents in cluster
		if(dc == 0) continue;
		int docLength = matrix.at(0).size(); //length of each document vector
		vector<double> newCentroid(docLength); //new centroid
		vector<double> sum(docLength); //sum of document vectors
		for(int i = 0;i<docLength;i++){
			sum.at(i) = 0;
		}
		for(int a = 0; a < dc ; a++) { //finding the sum of vectors in cluster
			int docNum = docClass.at(a);
			for(int b = 0; b < docLength; b++) {
				sum.at(b) = sum.at(b) +  matrix.at(docNum-1).at(b); //adding each column from a single document vector
			}
		}
	
		for(int c = 0; c < docLength; c++) { //dividing each number in vector by the number of documents in the cluster
			if(sum.at(c) == 0) {
				newCentroid.at(c) = 0;
			}
			else{
				newCentroid.at(c) = sum.at(c) / dc;
			}
		}
	
		for(int d = 0; d < docLength; d++) { //Adding new centroid to centroid matrix
			centroidMatrix.at(i).at(d) = newCentroid.at(d);
		}
		newCentroid.clear();
		docClass.clear();
		sum.clear();
	}
	return;
}

int main(){
	
	//declarations
	
	vector<vector<int> > matrix;	//matrix where each row is a document frequency vector and each column represents a word seen in the corpus
	
	//FOR EXAMPLE, if a corpus contains 4000 unique words, there will be 4000 columns
	//if "the" is the first word seen, then column 0 represents the term frequency of "the" for all documents in the corpus
	
	vector<vector<string> > query_vector;	//stores the query that the user wants to search
	vector<string> words;			//contains a list of all words seen in the corpus, used to find index of a word in the matrix
	int i,j;	//iterators
	int query_number;	//stores query number the user wants to search
	vector<double> pair(2);	//holds the cosine similarity value and its associated document number
	vector<vector<double> > doc_rank1; 	//holds document relevance ranking list based on binary frequency
	vector<vector<double> > doc_rank2;	//holds document relevance ranking list based on count frequency
	vector<int> query_freq;	//frequency vector for the user's query
	
	double classNumber = 0; //Number of classes for Rocchio, entered by user
	double maxCosClass = 0; //For saving the class number of the maximum cosine value
	double maxCos = 0;	

	//processing
	matrix = readDocs(words);
	query_vector = readQueries();
	vector<string> temp_q;		//holds the vector containing the requested query
	/*cout << "Enter 1 if you would like to see the entire list of ranked documents for each query, enter 2 if you would only like to see the top 10." << endl;
	int option;
	cin >> option;
	if(option != 1 && option != 2){		//error checking
		cout << "Invalid input" << endl;
		return 0;
	}*/
	cout << "Enter the number of the query you'd like to search, a number between 1 and 225, or enter 0 when you want to quit" << endl;
	cin >> query_number;
	if(query_number < 0 || query_number > 225){	//error checking
		cout << "Invalid input" << endl;
		return 0;
	}
	while(query_number > 0){
		temp_q = query_vector.at(query_number-1); 	//stores the correct query in temp_q
		query_freq = computeQueryFrequencyVector(temp_q,words);	
		/*for(j=0;j<matrix.size();j++){			//for each document in the corpus, calculate two similarities
			pair.at(0) = computeBinaryCosineSimilarity(matrix.at(j),query_freq);
			pair.at(1) = j+1;
			insertRanking(doc_rank1,pair);
			pair.at(0) = computeCountCosineSimilarity(matrix.at(j),query_freq);
			pair.at(1) = j+1;
			insertRanking(doc_rank2,pair);
		}*/
		
		//printResults(option, doc_rank1, doc_rank2);
		temp_q.clear();
		



		cout << endl << "Enter the desired number of classes" << endl;
		cin >> classNumber;		
		if (classNumber <= 0 || classNumber > 1400) { //error checking
			cout << "Invalid input" << endl;
			return 0;
		}
		vector<double> documentClass(matrix.size());	//vector that holds the assigned class for each document where index 0 represents document 1
		for(i=0;i<documentClass.size();i++){
			documentClass.at(i) = 0;		//initialize vector to all zeros
		}
		vector<vector<double > > centroidMatrix(classNumber);	//matrix to hold each centroid vector
			
		randomizeCentroids(classNumber, words.size(), centroidMatrix);	//randomly place centroids to begin 
		
		vector<vector<double> > matrix2;	//changes matrix of ints to matrix of doubles
		matrix2.reserve(matrix.size());
		for (auto&& v : matrix) matrix2.emplace_back(begin(v), end(v));	
		
		//normalize each vector in matrix
		double denominator;
		for(i=0;i<matrix2.size();i++){
			denominator = 0;
			for(j=0;j<words.size();j++){
				denominator += (matrix2.at(i).at(j) * matrix2.at(i).at(j));
			}
			denominator = sqrt(denominator);
			for(j=0;j<words.size();j++){
				if(denominator != 0)
					matrix2.at(i).at(j) = matrix2.at(i).at(j)/denominator;
			}
		}
		int classChange = 1;	//flag that tells if a document had to change classes or not
		double temp_cos;
		vector<int> documentsThatChanged;
		while(classChange == 1){	//keep assigning documents to a class until there is no change
			classChange = 0;	//initialize to no class change
			for(int a = 0; a < matrix2.size(); a++) {	//assign each document to a class
				maxCos = 0;
				for(int b = 0; b < centroidMatrix.size(); b++) {	//compute the cosine similarity between that document and every centroid
					temp_cos = computeCountCosineSimilarity2(matrix2.at(a), centroidMatrix.at(b),words.size()); //computer cosine similarity for each doc/centroid pair
					if(temp_cos > maxCos){
						maxCosClass = b+1;	//store the class number of the centroid closest to the document
						maxCos = temp_cos;	//update the max value
					}
				}
				if(maxCosClass != documentClass.at(a)){	//if the class number is different from the previous class number of the document
					documentsThatChanged.push_back(a+1);	
					classChange = 1;		//set the class change flag
					documentClass.at(a) = maxCosClass;	//change that document's class number to reflect the new class
				}
				cout << "Max class for document " << a+1 << " is " << maxCosClass << endl;
			}
			documentsThatChanged.clear();
			if(classChange == 0) break;	//if no documents were reassigned, no need to recompute centroids, so break out of loop
			cout << "Class changed, recomputing all centroids" << endl;
			recomputeCentroid(documentClass, centroidMatrix, matrix2);	//at least one document changed classes, so recompute centroids
		}
		for(i=0;i<classNumber;i++){			//prints out the documents in each class
			cout << "Class: " << i+1 << endl;
			for(j=0;j<documentClass.size();j++){
				if(documentClass.at(j) == i+1)
					cout << j+1 << " ";
			}
			cout << endl;
		}
		maxCos = 0;
		denominator = 0;
		vector<double> query_freq2(query_freq.size());	//change query frequency vector to doubles and normalize
		for(i=0;i<query_freq2.size();i++){
			query_freq2.at(i) = query_freq.at(i) * 1.0;
			denominator += query_freq2.at(i) * query_freq2.at(i);	
		}
		denominator = sqrt(denominator);
		if(denominator != 0){
			for(i=0;i<query_freq2.size();i++){
				query_freq2.at(i) = query_freq2.at(i)/denominator;
			}
		}
		for(i=0;i<classNumber;i++){ 	//determine which centroid is closest to query and return that class of documents
			double query_centroid_sim = computeCountCosineSimilarity2(query_freq2,centroidMatrix.at(i),words.size());
			if(query_centroid_sim > maxCos){
				maxCosClass = i+1;	//store the class number of the centroid closest to the query vector
				maxCos = query_centroid_sim;	//update the max value
			}
		}
		cout << "ROCCHIO RESULTS: " << endl;
		cout << "Going to return class " << maxCosClass << endl;
		for(i=0;i<documentClass.size();i++){
			if(documentClass.at(i) == maxCosClass)	//print out the document number of each document with the assigned class equal to maxCosClass
				cout << i+1 << " ";
		}

		/*Rocchio ends here*/
		centroidMatrix.clear();
		doc_rank1.clear();
		doc_rank2.clear();
		cout << endl << "Enter the number of the query you'd like to search, a number between 1 and 225, or enter 0 when you want to quit" << endl;
        	cin >> query_number;
		if(query_number < 0 || query_number > 225){	//error checking
			cout << "Invalid input" << endl;
			return 0;
		}
		query_freq.clear();
	}
	return 0;
}
