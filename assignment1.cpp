#include <iostream>
#include <fstream>
using namespace std;

int main(){
	ifstream documents;
	ifstream queries;
	documents.open("dataset.txt");
	queries.open("queries.txt");
	cout << "HI" << endl;
	//last time!
	//test
	documents.close();
	queries.close();
	return 0;
}
