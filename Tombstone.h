#pragma once

#include <iostream>


template <class T>
class Tombstone
{
public:
	T* pointer;
	int refCount;
	bool RIP;

	Tombstone<T>() { RIP = false; refCount = 1; pointer = nullptr; }

	Tombstone<T>(T* target)
	{
		RIP = false;

		if (target != nullptr)
		{
			refCount = 1;
			pointer = target;
		}
		else
		{
			refCount = 0;
			pointer = nullptr;
		}
	}

	~Tombstone<T>()
	{
		std::cerr << "Tombstoned!!!\n";
		if (pointer != nullptr)
		{
			delete pointer;
			pointer = nullptr;
		}
	}
};

template <class T>
class Pointer {

public:
	Tombstone<T>* tombstone;

public:
	// default constructor
	Pointer<T>()
	{
		//std::cerr << " Default Constructor\n";
		tombstone = new Tombstone<T>();	
	}

	// copy constructor
	Pointer<T>(Pointer<T>& ptarget)
	{
		//std::cerr << "copy constructor\n";
		if ( ptarget.tombstone != nullptr && ptarget.tombstone->RIP != true)
		{
			this->tombstone = ptarget.tombstone;
			this->tombstone->refCount++;
		}
		else
		{
			this->tombstone = nullptr;
		}
	}

	// bootstrapping constructor
	// The parameter for the bootstrapping constructor should always be
	// a call to operator new.
	Pointer<T>(T* target)
	{
		//std::cerr << "bootstrapping constructor\n";

		if (target != nullptr)
		{
			this->tombstone = new Tombstone<T>(target);
		}
		else
		{
			this->tombstone = nullptr;
		}
	}

	// destructor
	~Pointer<T>()
	{		
		// std::cerr << "Destructor\n";
		if (this->tombstone != nullptr)
		{
			this->tombstone->refCount--;
			if (this->tombstone->refCount <= 0)
			{
				delete tombstone;
				tombstone = nullptr;
			}
		}			
	}

	// dereferencing
	T& operator*() const
	{		
		if (this->tombstone->pointer != nullptr)
		{
			return *(this->tombstone->pointer);
		}
		else
		{
			std::cerr << "WARNING: Dangling pointer error" << std::endl;
			std::cerr << "         Exiting the diver program" << std::endl;
			exit(1);
		}
	}

	// field dereferencing
	T* operator->() const
	{
		return this->tombstone->pointer;
	}

	// assignment
	Pointer<T>& operator=(const Pointer<T>& target)
	{
		// std::cout << "Assignment operator\n";
		if (this != &target && target.tombstone->RIP != true )
		{
			this->tombstone = target.tombstone;
			this->tombstone->refCount++;
		}		
		return *this;
	}

	// delete pointed-at object
	// This is essentially the inverse of the new inside the call to
	// the bootstrapping constructor. It should delete the pointed-to
	// object (which should in turn call its destructor).
	friend void free(Pointer<T>& target)
	{
		if (target.tombstone != nullptr)
		{
			if (target.tombstone->pointer != nullptr)
			{
				delete target.tombstone->pointer;
				target.tombstone->pointer = nullptr;
			}						
			target.tombstone->refCount--;
			target.tombstone->RIP = true;
		}
	}

	// equality comparisons:
	bool operator==(const Pointer<T>& target) const
	{
		// std::cout << "== operator Pointer\n";
		if (this->tombstone == target.tombstone)
		{
			return true;
		}

		return false;
	}

	bool operator!=(const Pointer<T>& target) const
	{
		// std::cout << "!= operator Pointer\n";
		if (this->tombstone == target.tombstone)
		{
			return false;
		}

		return true;
	}

	// true iff Pointer is null and int is zero
	bool operator==(const int target) const
	{
		// std::cout << "== operator Int\n";
		if (target == 0 && this->tombstone == nullptr)
		{
			return true;
		}
		return false;
	}

	// false iff Pointer is null and int is zero
	bool operator!=(const int target) const
	{
		// std::cout << "!= operator Int\n";
		if (target == 0 && this->tombstone == nullptr)
		{
			return false;
		}
		return true;
	}
};