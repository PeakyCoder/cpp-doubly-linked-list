#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>


using namespace std;

struct NodeInfo{

	string awayTeamName;
	int goalMinute;
	int matchID;

    NodeInfo* down; 
    NodeInfo* up; 

};

struct Node
{
	string footballerName;
	string teamName;
	
    Node* next;
   	Node* prev;
   	NodeInfo* down;
};

class LinkedList
{
public:
    int sizeOfList;
    Node* head;

    LinkedList();
    void add(string footballerName, string teamName, string awayTeamName, int goalMinute, int matchID);
};

LinkedList::LinkedList(){
    this->sizeOfList = 0;
    this->head = NULL;
}

void LinkedList::add(string footballerName, string teamName, string awayTeamName, int goalMinute, int matchID){
	
	
	int state = 0;
	if(head == NULL){
		Node* node = new Node();
		node->footballerName = footballerName;
		node->teamName = teamName;
		node->prev = NULL;
		node->next = NULL;
		node->down = new NodeInfo();
		node->down->awayTeamName = awayTeamName;
		node->down->goalMinute = goalMinute;
		node->down->matchID = matchID;
		node->down->up = NULL;
		node->down->down = NULL;
		this->head = node;
	}
	else{
		Node* iter = this->head;
		for(iter; iter != NULL; iter = iter->next){
			if(iter->footballerName.compare(footballerName) == 0){
				state = 1;
				NodeInfo* newInfoNode = new NodeInfo();
				newInfoNode->awayTeamName = awayTeamName;
				newInfoNode->goalMinute = goalMinute;
				newInfoNode->matchID = matchID;
				
				if(iter->down == NULL){
					newInfoNode->up = NULL;
					newInfoNode->down = NULL;	
					iter->down = newInfoNode;
				}
				else{
					NodeInfo* iterDown = iter->down;
					while(iterDown->down != NULL){
						iterDown = iterDown->down;
					}
					newInfoNode->up = iterDown;
					newInfoNode->down = NULL;
					iterDown->down = newInfoNode;
					
 				}
			}
		}	
		if(state == 0){
			Node* current = this->head; 
				
			Node* node = new Node();
			node->footballerName = footballerName;
			node->teamName = teamName;
			node->down = new NodeInfo();
			node->down->awayTeamName = awayTeamName;
			node->down->goalMinute = goalMinute;
			node->down->matchID = matchID;
			node->down->up = NULL;
			node->down->down = NULL;
			
	    	if (this->head->footballerName.compare(footballerName) > 0) { 
		        node->next = this->head; 
		        node->next->prev = node; 
		        this->head = node; 
		    } 
		    
		    else{
			   	
		  
		        // locate the node after which the new node 
		        // is to be inserted 
		        while (current->next != NULL && current->next->footballerName.compare(footballerName) < 0  ) {
		            current = current->next; 
		  		}
		        /* Make the appropriate links */
		        node->next = current->next; 
		  
		        // if the new node is not inserted 
		        // at the end of the list 
		        if (current->next != NULL){
		            node->next->prev = node; 
	  			}
		        current->next = node; 
		        node->prev = current; 
			    	
		    	
			}
		
			
		}
	}
	
	this->sizeOfList++;
		
}

void readInputFile(char fileName[], LinkedList* list){
	int minute, ID;
	
	std::ifstream inputFile;
	inputFile.open(fileName);
	
	
	std::string footballerInfo;
	std::vector<string> footballerList;

	while(!inputFile.eof() && inputFile.peek()!='\n') // To get you all the lines.
    {
        getline(inputFile,footballerInfo); // Saves the line in footballerInfo.
    	footballerList.push_back(footballerInfo);	 //Saves the footballerList       
	}
	    
    // Split "," and adding to Linked List
	for(int i = 0; i < footballerList.size(); i++ ){
		stringstream ss(footballerList.at(i));
		vector<string> result;
		while( ss.good() )
		{
		    string substr;
		    getline( ss, substr, ',' );
		    result.push_back(substr);
		}
		minute = atoi(result.at(3).c_str());
		ID = atoi(result.at(4).c_str());
	
		list->add(result.at(0), result.at(1), result.at(2), minute, ID);
	}
	inputFile.close();
}

void readOpFile(char fileName[], std::vector<string>& list){
	
	std::ifstream operationsFile;
	operationsFile.open(fileName);

	std::string footballers;
	std::vector<string> footballerList;
	while(!operationsFile.eof() && operationsFile.peek()!='\n') // To get you all the lines.
    {
        getline(operationsFile, footballers); // Saves the line in footballerInfo.
    	footballerList.push_back(footballers);	 //Saves the footballerList       
	}
	
    
    // Split "," and adding to Linked List
	for(int i = 0; i < footballerList.size(); i++ ){
		stringstream ss(footballerList.at(i));
		vector<string> result;
		while( ss.good() )
		{
		    string substr;
		    getline( ss, substr, ',' );
		    result.push_back(substr);
		    list.push_back(substr);
		}
	
	
		
	}
	operationsFile.close();
}

void scoreInWhichHalf(std::ofstream &outputFile, LinkedList* list){
	int firstHalf = 0, secondHalf = 0;
	
	Node* iter = list->head;
	
	
	for(iter; iter!= NULL; iter = iter->next){
		NodeInfo* iterDown = iter->down;
		while(iterDown != NULL){
			if(iterDown->goalMinute <= 45){
				firstHalf++;
			}
			else if(iterDown->goalMinute > 45){
				secondHalf++;
			}
			iterDown = iterDown->down;
		}
	}
	// Printing to Output File
	outputFile << "1)THE MOST SCORED HALF\n";
	if(firstHalf > secondHalf){
		outputFile << "0\n";
	}
	else{
		outputFile << "1\n";
	}
	
}

void topScorers(std::ofstream &outputFile, LinkedList* list){
	int topScorer = 0, goalCounter = 0;
	std::vector<string> topScorerList;
	
	Node* iter = list->head;
	for(iter; iter!= NULL; iter = iter->next){
		NodeInfo* iterDown = iter->down;
		while(iterDown != NULL){
			goalCounter++;
			iterDown = iterDown->down;
		}
		if(goalCounter > topScorer){
			topScorer = goalCounter; 
			topScorerList.clear();
			topScorerList.push_back(iter->footballerName);
		}
		else if(goalCounter == topScorer){
			topScorerList.push_back(iter->footballerName);
		}
		goalCounter = 0;
	}
	
	// Printing to Output File
	outputFile << "2)GOAL SCORER\n";
	for(int i = 0; i < topScorerList.size(); i++){
		outputFile << topScorerList.at(i) << "\n";	
	}

}

void hattricksPlayer(std::ofstream &outputFile, LinkedList* list){
	int goalCounter = 0;
	std::vector<string> topScorerList;
	
	Node* iter = list->head;
	int matchID;
	for(iter; iter!= NULL; iter = iter->next){
		NodeInfo* iterDown = iter->down;
		matchID = iterDown->matchID;
		while(iterDown != NULL){
			
			if(matchID == iterDown->matchID){	
				goalCounter++;
				if(goalCounter >= 3){
					topScorerList.push_back(iter->footballerName);
				}
				iterDown = iterDown->down;
			}
		
			else{
				matchID = iterDown->matchID;
				goalCounter = 1;
				iterDown = iterDown->down;
			}
	
		}
		
		goalCounter = 0;
	}
	
	// Printing to Output File
	outputFile << "3)THE NAMES OF FOOTBALLERS WHO SCORED HAT-TRICK\n";
	for(int i = 0; i < topScorerList.size(); i++){
		outputFile << topScorerList.at(i) << "\n";	
	}

}

void listOfTeams(std::ofstream &outputFile, LinkedList* list){
	Node* iter = list->head;
	
	// Printing to Output File
	outputFile << "4)LIST OF TEAMS\n";
	//All Team List
	std::vector<string> teamList;
	//Adding teams to list
	for(iter; iter!= NULL; iter = iter->next){
		bool isPresent = (std::find(teamList.begin(), teamList.end(), iter->teamName) != teamList.end());

		if(!isPresent){
			teamList.push_back(iter->teamName);
		}
	}
	
	for(int i = 0; i < teamList.size(); i++){
		outputFile << teamList.at(i) << "\n";
	}
}

void listOfFootballers(std::ofstream &outputFile, LinkedList* list){
	Node* iter = list->head;
	
	// Printing to Output File
	outputFile << "5)LIST OF FOOTBALLERS\n";
	
	for(iter; iter!= NULL; iter = iter->next){
		outputFile << iter->footballerName << "\n";
	}
}

void matchesOfGivenFootballer(string footballerName1, string footballerName2, std::ofstream &outputFile, LinkedList* list){
	
	// Printing to Output File
	outputFile << "6)MATCHES OF GIVEN FOOTBALLER\n";
	outputFile << "Matches of " << footballerName1 << "\n";
	
	Node* iter = list->head;
	for(iter; iter!= NULL; iter = iter->next){
		if(iter->footballerName.compare(footballerName1) == 0){
			NodeInfo* iterDown = iter->down;
			while(iterDown != NULL){
				outputFile << "Footballer Name: " << iter->footballerName << ",Away Team: " << iterDown->awayTeamName << ",Min of Goal: " << iterDown->goalMinute << ",Match ID: " << iterDown->matchID << "\n";
				iterDown = iterDown->down;
			}
		}
	
	}
	outputFile << "Matches of " << footballerName2 << "\n";
	iter = list->head;
	for(iter; iter!= NULL; iter = iter->next){
		if(iter->footballerName.compare(footballerName2) == 0){
			NodeInfo* iterDown = iter->down;
			while(iterDown != NULL){
				outputFile << "Footballer Name: " << iter->footballerName << ",Away Team: " << iterDown->awayTeamName << ",Min of Goal: " << iterDown->goalMinute << ",Match ID: " << iterDown->matchID << "\n";
				iterDown = iterDown->down;
			}
		}
	
	}

}

void ascendingOrderFootballer(string footballerName1, string footballerName2, std::ofstream &outputFile, LinkedList* list){
	
	// Printing to Output File
	outputFile << "7)ASCENDING ORDER ACCORDING TO MATCH ID\n";
	
	Node* iter = list->head;
	std::vector<int> firstPlayerIDs;
	std::vector<int> secondPlayerIDs;

	
	for(iter; iter!= NULL; iter = iter->next){
		if(iter->footballerName.compare(footballerName1) == 0){
			NodeInfo* iterDown = iter->down;
			while(iterDown != NULL){
				firstPlayerIDs.push_back(iterDown->matchID);
				iterDown = iterDown->down;
			}
		}
	
	}
	iter = list->head;
	for(iter; iter!= NULL; iter = iter->next){
		if(iter->footballerName.compare(footballerName2) == 0){
			NodeInfo* iterDown = iter->down;
			while(iterDown != NULL){
				secondPlayerIDs.push_back(iterDown->matchID);
				iterDown = iterDown->down;
			}
		}
	}
	//Sorting IDs and delete duplicate elements
	sort( firstPlayerIDs.begin(), firstPlayerIDs.end() );
	firstPlayerIDs.erase( unique( firstPlayerIDs.begin(), firstPlayerIDs.end() ), firstPlayerIDs.end() );
	sort( secondPlayerIDs.begin(), secondPlayerIDs.end() );
	secondPlayerIDs.erase( unique( secondPlayerIDs.begin(), secondPlayerIDs.end() ), secondPlayerIDs.end() );
	
	for(int i = 0; i < firstPlayerIDs.size(); i++){
		outputFile << "footballer Name: " << footballerName1 << ",Match ID:" << firstPlayerIDs.at(i) << "\n";
	}
	for(int i = 0; i < secondPlayerIDs.size(); i++){
		outputFile << "footballer Name: " << footballerName2 << ",Match ID:" << secondPlayerIDs.at(i) << "\n";
	}

}

void descendingOrderFootballer(string footballerName1, string footballerName2, std::ofstream &outputFile, LinkedList* list){
	
	// Printing to Output File
	outputFile << "8)DESCENDING ORDER ACCORDING TO MATCH ID\n";
	
	Node* iter = list->head;
	std::vector<int> firstPlayerIDs;
	std::vector<int> secondPlayerIDs;

	
	for(iter; iter!= NULL; iter = iter->next){
		if(iter->footballerName.compare(footballerName1) == 0){
			NodeInfo* iterDown = iter->down;
			while(iterDown != NULL){
				firstPlayerIDs.push_back(iterDown->matchID);
				iterDown = iterDown->down;
			}
		}
	
	}
	iter = list->head;
	for(iter; iter!= NULL; iter = iter->next){
		if(iter->footballerName.compare(footballerName2) == 0){
			NodeInfo* iterDown = iter->down;
			while(iterDown != NULL){
				secondPlayerIDs.push_back(iterDown->matchID);
				iterDown = iterDown->down;
			}
		}
	}
	//Sorting IDs and delete duplicate elements
	std::sort(firstPlayerIDs.rbegin(), firstPlayerIDs.rend());
	firstPlayerIDs.erase( unique( firstPlayerIDs.begin(), firstPlayerIDs.end() ), firstPlayerIDs.end() );
	std::sort(secondPlayerIDs.rbegin(), secondPlayerIDs.rend());
	secondPlayerIDs.erase( unique( secondPlayerIDs.begin(), secondPlayerIDs.end() ), secondPlayerIDs.end() );
	
	for(int i = 0; i < firstPlayerIDs.size(); i++){
		outputFile << "footballer Name: " << footballerName1 << ",Match ID:" << firstPlayerIDs.at(i) << "\n";
	}
	for(int i = 0; i < secondPlayerIDs.size(); i++){
		outputFile << "footballer Name: " << footballerName2 << ",Match ID:" << secondPlayerIDs.at(i) << "\n";
	}

}

main(int argc, char **argv){

	//Create Output File
	std::ofstream outputFile;
	outputFile.open(argv[3]);
	
	//Create Linked List
	LinkedList* list = new LinkedList();

	
	//Reading Input File and Adding to Linked List
	readInputFile(argv[1], list);

	
	//Reading Operations File and Adding to Vector List
	std::vector<string> operationsList;
	readOpFile(argv[2], operationsList);
	
	//Output Functions
	//---------------------------------
	scoreInWhichHalf(outputFile, list);
	
	topScorers(outputFile, list);
	
	hattricksPlayer(outputFile, list);
	
	listOfTeams(outputFile, list);
	
	listOfFootballers(outputFile, list);
	
	matchesOfGivenFootballer(operationsList.at(0), operationsList.at(1), outputFile, list);

	ascendingOrderFootballer(operationsList.at(2), operationsList.at(3), outputFile, list);
	
	descendingOrderFootballer(operationsList.at(4), operationsList.at(5), outputFile, list);
	//-------------------------------------------------------------------------------------------
	
	
}


