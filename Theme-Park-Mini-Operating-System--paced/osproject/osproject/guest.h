#ifndef GUEST_H
#define GUEST_H

struct guest {
	int guestID;
	int arrivalTime;
	int burstTime;
	int remainingTime;
	int memoryRequirement;
	int priority;
	int rideRequest;
	char state;
};

//function prototypes
void initializeGuest(guest& g, int id, int arrival, int burst, int memory,
	int prio, int ride); //initializes guest with data

void displayGuest(const guest& g); //shows guest and data

void updateRemainingTime(guest& g);

#endif
