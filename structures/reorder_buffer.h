#pragma once


class ReorderBuffer
{
public:
	int entries;
	int freeEntries;
	ReorderBuffer(int entries);

	void insert();
	bool isFull() const;
};

class ReservationStation
{
public:
	int entries;
	int freeEntries;
	void insert();
	bool isFull() const;
};


class FPReservationStation : public ReservationStation
{
	FPReservationStation(int entries)
	{
		entries = entries;
		freeEntries = entries;
	}

	void insert()
	{
		freeEntries--;
	}

	bool isFull() const
	{
		return (freeEntries == 0);
	}
};

class IntReservationStation : public ReservationStation
{
	IntReservationStation(int entries)
	{
		entries = entries;
		freeEntries = entries;
	}

	void insert()
	{
		freeEntries--;
	}

	bool isFull() const
	{
		return (freeEntries == 0);
	}
};


