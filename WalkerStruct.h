#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <utility>
#include <map>


class Connection{
private:
	int n;
	double deltaP{0};
	bool withinConstraints{false};
public:
	Connection() = default;
	Connection(int n, double place,std::function<bool(double)> logic){
		this->n = n;
		deltaP=place;
		inBounds(logic);
	}
	void inBounds(std::function<bool(double position)> logic){
		withinConstraints = logic(deltaP);
	}
	void updateConnection(double dp,std::function<bool(double p)> myLogic){
		deltaP = dp;
		inBounds(myLogic);
	}
	bool getWithinConstraints(){
		return withinConstraints;
	}
	int getN(){return n;}
	void plusEqualsN(int increment){n+=increment;}
	void plusEqualsP(double increment){}
};

class Walker{
private: 
	int range;
	std::vector<int> links;
	std::vector<Connection> connections;
	std::vector<Connection> inBoundConnections;
	std::function<bool(double)> myConstraints;
	int currentPosition{0};
	
public:
	Walker() = default;
	void addConnection(int n, double deltaP){
		connections.emplace(std::lower_bound(connections.begin(),connections.end(),n,[&](Connection c){return c.getN()<n;}),deltaP,myConstraints);
		if (myConstraints(deltaP))
			inBoundConnections.emplace(std::lower_bound(inBoundConnections.begin(),inBoundConnections.end(),n,[&](Connection c){return c.getN()<n;}),deltaP,myConstraints);
	}
	void deleteConnection(int n){
		connections.erase(std::lower_bound(connections.begin(),connections.end(),n,[&](Connection c){return c.getN()<n;}));
		inBoundConnections.erase(std::lower_bound(inBoundConnections.begin(),inBoundConnections.end(),n,[&](Connection c){return c.getN()<n;}));

	}

	void updateBools(){
		for(int i = 0;i<connections.size(); i++){
			connections[i].inBounds(myConstraints);
			if (i<inBoundConnections.size()){
				inBoundConnections[i].inBounds(myConstraints);
				if (!inBoundConnections[i].getWithinConstraints())
					inBoundConnections.erase(inBoundConnections.begin()+i);
			}
		}
	}

	void hop(int increment,double posChange){
		currentPosition+=increment;
		for(int i = 0; i<connections.size();i++){
			connections[i].plusEqualsN(increment);
			connections[i].plusEqualsP(posChange);
			if (i<inBoundConnections.size()){
				inBoundConnections[i].plusEqualsN(increment);
				inBoundConnections[i].plusEqualsP(posChange);
			}
		}
		updateBools();

	}
	// void update

};

