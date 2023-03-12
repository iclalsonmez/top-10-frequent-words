#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include "hashMap.h"
using namespace std;
auto start = chrono::steady_clock::now();
string lower(string s) {
	for(int i = 0; i < s.size(); i++) {
		s.at(i) = tolower(s.at(i)); 
	}
	return s; 
}

bool isSuitable(string s) {
	if(s.length()<3){
		return false;
	}
	for (int i=1;i<s.length()-1;i++){
		if(!isalpha(s[i]))
			return false;
	}
	return true; 
}


int main() {
    ifstream file("PublicationsDataSet.txt");
	//ifstream file("ornek.txt");
    ifstream file1("stopwords.txt");
    std::string word;
	HashMap stopwords(1001);
    while(!file1.eof()) {
		file1 >> word; 
		stopwords.insert(word, 1);
    }
    
    HashMap wordsMap(100001); 
    string contents((std::istreambuf_iterator<char>(file)), 
    std::istreambuf_iterator<char>());

	
	string parsedContents = "";
	bool insideQuotation = false;
	for(int i = 0; i < contents.size(); i++) {
		if(contents.at(i) == '"'){
			int j = 1; 
			while(contents.at(i - j) == '\\') j++; 
			if(j % 2 == 1) insideQuotation = !insideQuotation; 
		}
		
		if(!insideQuotation && (contents.at(i) == '{' || contents.at(i) == '}')) {
			parsedContents += "\n";
			if(contents.at(i) == '}') {
				parsedContents += contents.at(i); 
				parsedContents += "\n";
			}
		}
		else if(!insideQuotation && contents.at(i) == ','){
			parsedContents += "\n"; 
		}
		else{
			parsedContents += contents.at(i); 
		}
	}

	std::istringstream stream(parsedContents);

	std::string line;
	bool inside = false; 
	while (getline(stream, line))
	{ 	
		if(line == "\"unigramCount\":") {
			inside = true; 
		}
		else if(line == "}") {
			inside = false; 
		}
		else if(inside) {
			size_t quotation = line.find_last_of("\""); 
			string word = lower(line.substr(1, quotation - 1));
			if (!stopwords.search(word) && isSuitable(word)) {
				int count = stoi(line.substr(quotation + 2, line.size() - quotation + 1));
				if (wordsMap.search(word)){
					int prevCount = wordsMap.retrieve(word); 
					wordsMap.update(word, count + prevCount); 
				}
				else{
					wordsMap.insert(word, count); 
				}
			}
		}
	}

	HashNode** allEntries = wordsMap.getAllEntries();
	HashNode** topTenEntries = new HashNode*[10]; 
	for (int i=0;i<10;i++){
		topTenEntries[i]=new HashNode("",0);
	}


	for (int i=0;i<wordsMap.size;i++){
		if(allEntries[i]->value>topTenEntries[9]->value){
			topTenEntries[9]=allEntries[i];
			int j=9;
			while(j > 0 && topTenEntries[j]->value>topTenEntries[j-1]->value){
				swap(topTenEntries[j],topTenEntries[j-1]);
				j--;
			}
		}
	}
	for (int i=0;i<10;i++){
		cout<<i+1<<". "<<topTenEntries[i]->key<<":  "<<topTenEntries[i]->value << "\n";
	}

    
    file.close();
	auto end = chrono::steady_clock::now();
	cout << "Total Elapsed Time: "<< chrono::duration_cast<chrono::seconds>(end - start).count() << " seconds " << chrono::duration_cast<chrono::milliseconds>(end - start).count() % 1000 << " miliseconds" << endl;
	return 0;
}
