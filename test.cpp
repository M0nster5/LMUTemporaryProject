#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <utility>
#include <map>
#include "WalkerStruct.h"

int main(){
	//create walker
	Walker myWalk;
	//set the function to your contraints.
	myWalk.setFunction([](double a){return std::abs(a) < 2;});
	for (int i = 0; i < 10; i++){
		myWalk.addConnection(i,i);
	}
	//prints the system
	myWalk.printSystem();
	//add connection with n of -2 and deltaP of -2
	myWalk.addConnection(-2,-2);
	myWalk.printSystem();
	//moves the walker 2 to the right and changes connections
	myWalk.hop(2);
	myWalk.printSystem();
	//deletes connection with n of 3
	myWalk.deleteConnection(3);
	myWalk.printSystem();
	return 0;
}