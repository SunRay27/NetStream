#pragma once
#include "String.h"
#include "List.h"

class FlowNode;

class FlowEdge
{
public:
    FlowNode* toNode;
    FlowNode* fromNode;
    FlowEdge* reverse;
    
    long initCapacity;
    long capacity;
   

    FlowEdge(FlowNode* toNode, FlowNode* fromNode, long initCapacity)
    {
        this->toNode = toNode;
        this->initCapacity = initCapacity;
        this->capacity = initCapacity;
        this->fromNode = fromNode;

        //created in transportNode object AddConnection()
        reverse = nullptr;
    }
};


class FlowNode
{
private:
    List<FlowEdge> connections;
public:
	String name;
    FlowEdge* edgeFrom;
    bool visited = false;

	FlowNode(const String& name)
	{
		this->name = name;
        //searchDistance = 0;
        edgeFrom = nullptr;
	}

    void AddConnection(FlowNode* neighbor, size_t initCapacity)
    {
        connections.Add(FlowEdge(neighbor, this, initCapacity));
        neighbor->connections.Add(FlowEdge(this, neighbor, 0));

        //because forward and reverse edges are copied to list...
        connections[connections.Count() - 1].reverse = &neighbor->connections[neighbor->connections.Count() - 1];
        neighbor->connections[neighbor->connections.Count() - 1].reverse = &connections[connections.Count() - 1];
    }
    FlowEdge* GetConnection(size_t index)
    {
        return &connections[index];
    }
    size_t GetConnectionCount()
    {
        return connections.Count();
    }

    bool HasConnectionToNode(FlowNode* node)
    {
        for (size_t i = 0; i < connections.Count(); i++)
            if (connections[i].toNode == node)
                return true;

        return false;
    }
    FlowEdge* GetConnectionToNode(FlowNode* node)
    {
        for (size_t i = 0; i < connections.Count(); i++)
            if (connections[i].toNode == node)
                return &connections[i];
        throw exception("Tried to get non-existent connection");
    }
    bool operator==(FlowNode& toCompare)
    {
        return this->name == toCompare.name;
    }
    bool operator !=(FlowNode& toCompare)
    {
        return !(*this == toCompare);
    }
};

