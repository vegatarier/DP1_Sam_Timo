#ifndef _CIRCUIT_H
#ifndef _CIRCUIT_H
#define _CIRCUIT_H

#include <vector>
#include "Component.h"
#include "Output.h"

class Circuit
{
protected:
	Output *pOutput;
	std::vector<Component *> Components;
	
	int ErrorFound();
public:
	Circuit();
	~Circuit();

	int CreateNodes();
	int CreateEdges();
	int LinkAll();

	void Start();

};

#endif