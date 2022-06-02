#pragma once
#include "Collections.h"
#include "FileParser.h"
#include "FlowNode.h"
class FlowNetwork
{
private:
	List<FlowNode> network;
	size_t sourceIndex = SIZE_MAX;
	size_t destinationIndex = SIZE_MAX;
	List<FlowEdge*> currentPath;

	void RegisterNodes(FlowNode& source, FlowNode& dest, size_t initCapacity)
	{
		int targetIndex = network.IndexOf(source);
		int neighborIndex = network.IndexOf(dest);

		if (targetIndex != -1)
		{
			if (neighborIndex == -1)
			{
				network.Add(dest);
				neighborIndex = network.Count() - 1;
			}
		}
		else
		{
			//source node is not present, create it
			network.Add(source);
			targetIndex = network.Count() - 1;

			if (neighborIndex == -1)
			{
				network.Add(dest);
				neighborIndex = network.Count() - 1;
			}
		}
		if (!network[targetIndex].HasConnectionToNode(&network[neighborIndex]))
		{
			network[targetIndex].AddConnection(&network[neighborIndex], initCapacity);
		}
		else
		{
			//we already have this connection, and we assume it was created by reverse assignment
			//so we need to set its initCapacity and capacity according to input
			auto connection = network[targetIndex].GetConnectionToNode(&network[neighborIndex]);
			connection->capacity = connection->initCapacity = initCapacity;

			//else it is just a repeating element, so nothing changes...
		}

		if (source.name == "S")
			sourceIndex = targetIndex;
		else if (dest.name == "T")
			destinationIndex = neighborIndex;
	}

	void ParseLines(List<String>& lines)
	{

		for (size_t i = 0; i < lines.Count(); i++)
		{
			auto splitted = String::LazySplit(lines[i], ' ');
			if (splitted.Count() != 3)
				throw exception("invalid token count");

			FlowNode sourceNode(splitted[0]);
			FlowNode destNode(splitted[1]);

			//TODO: filter same positions?

			RegisterNodes(sourceNode, destNode, ParseMaxFlow(splitted[2]));
		}
	}
	long ParseMaxFlow(String& stringToParse)
	{
		long result = 0;
		if (sscanf_s(stringToParse.GetArrayPointer(), "%ld", &result) == 1)
		{
			if(result < 0)
				throw exception("Max flow can't be less than zero");

			return result;
		}
		throw exception("Can't parse 3th token");
	}


public:
	FlowNetwork(const String& fileName)
	{
		List<String> lines = FileParser::GetLines(fileName);
		ParseLines(lines);
	}
	FlowNetwork(List<String>& lines)
	{
		ParseLines(lines);
	}
	void DrawNetwork()
	{
		for (size_t i = 0; i < network.Count(); i++)
		{
			cout << network[i].name << ':' << endl;
			for (size_t j = 0; j < network[i].GetConnectionCount(); j++)
			{
				cout << "-->" << network[i].GetConnection(j)->toNode->name << " " << network[i].GetConnection(j)->initCapacity - network[i].GetConnection(j)->capacity << "/" << network[i].GetConnection(j)->initCapacity << endl;
			}
			cout << endl;
		}
	}

	long GetMaxFlow()
	{
		//reset current flow in network
		for (size_t i = 0; i < network.Count(); i++)
			for (size_t j = 0; j < network[i].GetConnectionCount(); j++)
				network[i].GetConnection(j)->capacity = network[i].GetConnection(j)->initCapacity;

		//find shortest path
		size_t totalFlow = 0;
		FindShortestPath(&network[sourceIndex], &network[destinationIndex]);
		while(!currentPath.IsEmpty())
		{
			PrintCurrentPath();

			//find min capacity on path
			size_t minCapacity = SIZE_MAX;
			for (size_t i = 0; i < currentPath.Count(); i++)
				if (currentPath[i]->capacity < minCapacity)
					minCapacity = currentPath[i]->capacity;

			totalFlow += minCapacity;
			
			//augment path nodes by minCapacity
			for (size_t i = 0; i < currentPath.Count(); i++)
			{
				auto forward = currentPath[i];
				auto reverse = forward->reverse;
				forward->capacity -= minCapacity;
				reverse->capacity += minCapacity;
			}
		

			FindShortestPath(&network[sourceIndex], &network[destinationIndex]);
		} 
		
		return totalFlow;
	}

	//BFS search
	void FindShortestPath(FlowNode* from, FlowNode* to)
	{
		currentPath.Clear();
		//reset connections
		for (size_t i = 0; i < network.Count(); i++)
				network[i].visited = false;

		Queue<FlowNode*> queue;
		queue.Enqueue(from);
		bool exitFlag = false;
		//do BFS stuff
		while (!queue.IsEmpty())
		{
			FlowNode* current = queue.Dequeue();
			current->visited = true;

			//if (current == to)
			//	break;

			for (size_t i = 0; i < current->GetConnectionCount(); i++)
			{
				FlowEdge* edge = current->GetConnection(i);

				if ((edge->capacity) > 0 && !edge->toNode->visited)
				{
					queue.Enqueue(edge->toNode);
					edge->toNode->edgeFrom = edge;
					if (edge->toNode == to)
					{
						exitFlag = true;
						break;
					}
				}

			}
			if (exitFlag)
				break;
		}

		//restore path to target node and write it to <currentPath>
		if (!queue.IsEmpty())
		{
			FlowEdge* currentNode = to->edgeFrom;
			while (currentNode != nullptr)
			{
				currentPath.Add(currentNode);
				currentNode = currentNode->fromNode->edgeFrom;
			}
			currentPath.Reverse();
		}
	}

	void PrintCurrentPath()
	{
		for (size_t i = 0; i < currentPath.Count(); i++)
		{
			cout << currentPath[i]->fromNode->name << " -> " << currentPath[i]->toNode->name << "   ";
		}
		cout << endl;
	}
};