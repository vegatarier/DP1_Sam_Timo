#ifndef _NODE_H
#define _NODE_H

#include "Component.h"
#include "Edge.h"

class Edge;
class Node : public Component 
{
protected:
	std::string Identifier;
	// er zit een std::vector in component hoeft geen array te maken
	//Edge* pEdges[2];
public:
	Node();
	~Node();

	void CallNext();
	virtual void ExecuteOperation() = 0;
	void WaitForValues();
};

#endif _NODE_H