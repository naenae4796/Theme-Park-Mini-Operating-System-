#include <iostream>
#include "guest.h"

using namespace std;

void initializeGuest(guest& g, int id, int arrival, int burst, int memory, int prio, int ride) { //function definition for initializeGuest
	g.PID = id;
	g.arrivalTime = arrival;
	g.burstTime = burst;
	g.remainingTime = burst;
	g.priority = prio;
	g.rideRequest = ride;
	g.state = 'R'; //R means ready
}