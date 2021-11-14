#include "reservation_station.h"

AddReservationStation::AddReservationStation(int entries)
{
	maxEntries= entries;
	freeEntries = entries;
}

bool AddReservationStation::isFull()
{
	return (freeEntries == 0);
}

void AddReservationStation::insert()
{
	freeEntries--;
}

FPReservationStation::FPReservationStation(int entries)
{
	maxEntries = entries;
	freeEntries = entries;
}

bool FPReservationStation::isFull()
{
	return (freeEntries == 0);
}

void FPReservationStation::insert()
{
	freeEntries--;
}

