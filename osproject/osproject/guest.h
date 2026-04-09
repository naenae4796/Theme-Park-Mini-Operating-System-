#define GUEST_H

struct guest {
	int PID;
	int arrivalTime;
	int burstTime;
	int remainingTime;
	int memoryRequirement;
	int priority;
	int rideRequest;
	char state;
};

#endif
