#pragma once

class FPReservationStation
{
public:
	int maxEntries;
	int freeEntries;
	bool isFull();
	void insert();
	FPReservationStation(int entries);
};

class AddReservationStation
{
public:
	int maxEntries;
	int freeEntries;
	bool isFull();
	void insert();
	AddReservationStation(int entries);
};
