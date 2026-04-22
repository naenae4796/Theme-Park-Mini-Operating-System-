#include <iostream>
#include "guest.h"

using namespace std;

void initializeGuest(guest& g, int id, int arrival, int burst, int memory, int prio, int ride) { //function definition for initializeGuest
	g.guestID = id;
	g.arrivalTime = arrival;
	g.burstTime = burst;
	g.remainingTime = burst;
	g.priority = prio;
	g.rideRequest = ride;
	g.state = 'R'; //R means ready
}

void displayGuest(const guest& g) {
	cout << "Guest info" << endl;
	cout << "-----------" << endl;
	cout << "Guest ID: " << endl;
	cout << "Arrival Time: " << g.arrivalTime << endl;
	cout << "Burst Time: " << g.burstTime << endl;
	cout << "Remaining Time: " << g.remainingTime << endl;
	cout << "Memory Requirement: " << g.memoryRequirement << endl;
	cout << "Priority: " << g.priority << endl;
	cout << "Ride Request: " << g.rideRequest << endl;
	cout << "State: " << g.state << endl;
	cout << "------------" << endl;
}

void updateRemainingTime(guest& g) {
	if (g.remainingTime > 0) {
		g.remainingTime--;

		if (g.remainingTime == 0) {
			g.state = 'T'; // T means terminated
		}
	}
}