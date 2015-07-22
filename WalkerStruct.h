#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <utility>
#include <map>

//[&](Connection lhs, Connection rhs) -> bool { return lhs.getN() < rhs.getN(); }

//connection is a class that stores n it's unifor position, deltap - its distance
//from the walker and a boolean telling you if it is within the constraints
class Connection{
private:
	int n;
	double deltaP{0};
	bool withinConstraints{false};
public:
	Connection() = default;
	//initializes
	Connection(int n, double place,std::function<bool(double)> logic){
		this->n = n;
		deltaP=place;
		inBounds(logic);
	}
	//will update within constraints with the function entered
	void inBounds(std::function<bool(double position)> logic){
		withinConstraints = logic(deltaP);
	}
	//will update the connections deltaP with the new one and check if its withinConstraints
	void updateConnection(double dp,std::function<bool(double p)> myLogic){
		deltaP = dp;
		inBounds(myLogic);
	}
	//will tell you if it is within contraints
	bool getWithinConstraints(){
		return withinConstraints;
	}
	double getDeltaP(){return deltaP;}
	int getN(){return n;}
	//adds the increment value to the current value
	void plusEqualsN(int increment){n+=increment;}
	void plusEqualsP(double increment){deltaP+=increment;}
};
//this is used for the binarysearch that requires a compare operator
struct Comp {
    // important: we need two overloads, because the comparison
    // needs to be done both ways to check for equality

    bool operator()(Connection p, int s) const
    { return p.getN() < s; }
    bool operator()(int s, Connection p) const
    { return s < p.getN(); }
};

//Walker class stores its current connections
class Walker{
private: 
	int range;
	std::vector<Connection> connections{};
	std::vector<Connection> inBoundConnections{};
	std::function<bool(double)> myConstraints{};
	double currentPosition{0};
	
public: 
	Walker() = default;
	//adds connection to connection with a deltaP and n, It will add to inBoundConnections if it fits your constraints
	void addConnection(int n, double deltaP){
		connections.emplace(std::lower_bound(connections.begin(),connections.end(),n,[](Connection lhs, int rhs) -> bool { return lhs.getN() < rhs; }),n,deltaP,myConstraints);
		if (myConstraints(deltaP))
			inBoundConnections.emplace(std::lower_bound(inBoundConnections.begin(),inBoundConnections.end(),n,[](Connection lhs, int rhs) -> bool { return lhs.getN() < rhs; }),n,deltaP,myConstraints);
	}
	//deletes connection that has n, uses binarysearch
	void deleteConnection(int n){
		connections.erase(--std::lower_bound(connections.begin(),connections.end(),n,[](Connection lhs, int rhs) -> bool { return lhs.getN() < rhs; }));
		inBoundConnections.erase(--std::lower_bound(inBoundConnections.begin(),inBoundConnections.end(),n,[](Connection lhs, int rhs) -> bool { return lhs.getN() < rhs; }));

	}
	//updates the booleans of connections after a change
	void updateBools(){
		for(int i = 0;i<connections.size(); i++){
			connections[i].inBounds(myConstraints);
			if (i<inBoundConnections.size()){
				inBoundConnections[i].inBounds(myConstraints);
				if (!inBoundConnections[i].getWithinConstraints())
					inBoundConnections.erase(inBoundConnections.begin()+i);
			}
			else{
				if (!std::binary_search (inBoundConnections.begin(), inBoundConnections.end(), connections[i].getN(),Comp()) && myConstraints(connections[i].getDeltaP()))
					inBoundConnections.emplace(std::lower_bound(inBoundConnections.begin(),inBoundConnections.end(),connections[i].getN(),[](Connection lhs, int rhs) -> bool { return lhs.getN() < rhs; }),connections[i].getN(),connections[i].getDeltaP(),myConstraints);
			}
		}
	}
	//hops with a specific increment. positive is right and negative is left
	void hop(double posChange){
		currentPosition+=posChange;
		for(int i = 0; i<connections.size();i++){
			connections[i].plusEqualsP(-posChange);
			if (i<inBoundConnections.size()){
				inBoundConnections[i].plusEqualsP(-posChange);
			}
		}
		updateBools();
	}
	//good print system if integer values are entered
	void printSystem(){
		std::string lnOne;
		std::string lnTwo;
		std::string lnThree;
		std::string num;
		int count = connections[0].getN();
		int count2 = -1;
		for(int x = 0; x <connections.size();x++){
			// std::cout<<count<<" "<<connections[x].getN()-1<<"\n";
			while (count<connections[x].getN()-1){
				lnTwo+="___B";
				lnOne+="___W";
				if (count2>-1)
					lnThree+="___B";
				count++;
			}
			if (connections[x].getN()==inBoundConnections[0].getN()){
				count2++;
				if (count!=connections[0].getN())
					lnThree+="    ";
			}
			if(count2>-1&&count2<inBoundConnections.size()){
				lnThree+="___"+std::to_string((int)inBoundConnections[count2].getDeltaP());
				count2++;
			}
			else{
				lnThree+="   ";
			}
			count++;
			for(int i = 1;i<num.length();i++){
				lnOne+="_";
			}
			if (connections[x].getDeltaP()==0)
				lnOne+="___W";
			else
				lnOne+="___B";
			num = std::to_string((int)connections[x].getDeltaP());
			lnTwo+="___" + num;
		}			
		printInBounds();
		std::cout<<lnOne<<"\n"<<lnTwo<<"\n"<<lnThree;
		std::cout<<"\n\n";
	}
	//this initializes the function you will use for your contraints
	void setFunction(std::function<bool(double)> func){myConstraints=func;}
	//prints the inBoundConnections array.
	void printInBounds(){
		for (int x = 0; x<inBoundConnections.size();x++){
			std::cout<<inBoundConnections[x].getDeltaP()<<" ";
		}
		std::cout<<'\n';
	}
};

