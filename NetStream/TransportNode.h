#pragma once
#include "String.h"
#include "List.h"

class TransportNode;

class TransportEdge
{
public:
    size_t fromIndex;
    size_t toIndex;
    size_t currentFlow = 0;
    size_t maxFlow = 0;

    TransportEdge(size_t fromIndex, size_t toIndex, size_t maxFlow)
    {
        this->fromIndex = fromIndex;
        this->toIndex = toIndex;
        this->maxFlow = maxFlow;
    }
    size_t other(size_t v) const {
        return v == toIndex ? fromIndex : toIndex;
    }
    size_t capacityTo(size_t v) const {
        return v == toIndex ? maxFlow - currentFlow : currentFlow;
    }
    void addFlowTo(size_t v, size_t f) {
        currentFlow += (v == toIndex ? f : -f);
    }
};


class TransportNode
{
private:
    List<TransportEdge> connections;
public:
	String name;
    size_t searchDistance;
    TransportEdge* edgeFrom;
    TransportNode* pathFrom;

	TransportNode(const String& name)
	{
		this->name = name;
        searchDistance = 0;
        pathFrom = nullptr;
        edgeFrom = nullptr;
	}
    friend bool operator<(TransportNode& l, TransportNode& r)
    {
        return l.searchDistance < r.searchDistance;
    }
    friend bool operator>(TransportNode& l, TransportNode& r)
    {
        return l.searchDistance > r.searchDistance;
    }
    void AddConnection(TransportNode* neighbor, size_t maxFlow)
    {
        connections.Add(TransportEdge(neighbor, maxFlow));
    }
    TransportEdge& GetConnection(size_t index)
    {
        return connections[index];
    }
    size_t GetConnectionCount()
    {
        return connections.Count();
    }

    bool HasConnectionToNode(TransportNode* node)
    {
        for (size_t i = 0; i < connections.Count(); i++)
            if (connections[i].toNode == node)
                return true;

        return false;
    }
    bool operator==(TransportNode& toCompare)
    {
        return this->name == toCompare.name;
    }
    bool operator !=(TransportNode& toCompare)
    {
        return !(*this == toCompare);
    }
};

