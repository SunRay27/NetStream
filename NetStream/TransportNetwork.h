#pragma once
#include "Collections.h"
#include "FileParser.h"
#include "TransportNode.h"
class TransportNetwork
{
private:
	List<TransportNode> network;
	size_t sourceIndex;
	size_t destinationIndex;


	void RegisterNodes(TransportNode& source, TransportNode& dest, size_t maxFlow)
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
			network[targetIndex].AddConnection(&network[neighborIndex], maxFlow);

		if (source.name == "S")
			sourceIndex = targetIndex;
		else if (dest.name == "T")
			destinationIndex = neighborIndex;
	}

	void ParseFile(const String& fileName)
	{
		List<String> lines = FileParser::GetLines(fileName);

		for (size_t i = 0; i < lines.Count(); i++)
		{
			auto splitted = String::LazySplit(lines[i], ' ');
			if (splitted.Count() != 3)
				throw exception("invalid token count");

			TransportNode sourceNode(splitted[0]);
			TransportNode destNode(splitted[1]);

			RegisterNodes(sourceNode, destNode, ParseMaxFlow(splitted[2]));


		}
	}
    size_t ParseMaxFlow(String& stringToParse)
	{
		size_t result = 0;
		if (sscanf_s(stringToParse.GetArrayPointer(), "%zu", &result) == 1)
		{
			return result;
		}
		throw exception("Can't parse 3th token");
	}


public:
	TransportNetwork(const String& fileName)
	{
		ParseFile(fileName);
	}

	void DrawNetwork()
	{
		for (size_t i = 0; i < network.Count(); i++)
		{
			cout << network[i].name << ':' << endl;
			for (size_t j = 0; j < network[i].GetConnectionCount(); j++)
			{
				cout << "----->" << network[i].GetConnection(j).toNode->name << " " << network[i].GetConnection(j).currentFlow << "/" << network[i].GetConnection(j).maxFlow << endl;
			}
			cout << endl;
		}
	}

	void CalculateMaxFlow()
	{
		//reset current flow in network
		for (size_t i = 0; i < network.Count(); i++)
			for (size_t j = 0; j < network[i].GetConnectionCount(); j++)
				network[i].GetConnection(j).currentFlow = 0;

		List<TransportEdge*>* currentPath;
		do
		{
			currentPath = FindPath(&network[sourceIndex], &network[destinationIndex]);
			size_t increment = GetPathIncrement(currentPath);
			for (size_t i = 0; i < currentPath->Count(); i++)
			{
				size_t a = currentPath->At(i)->currentFlow;
				currentPath->At(i)->currentFlow += increment;
				size_t b = currentPath->At(i)->currentFlow;



				int c = 1;
			}
		} while (!currentPath->IsEmpty());
		

			
		


	}

	List<TransportEdge*>* FindPath(TransportNode* from, TransportNode* to)
	{
		//init distances
		for (size_t i = 0; i < network.Count(); i++)
			network[i].searchDistance = SIZE_MAX;


		//init start node
		from->searchDistance = 0;
		from->pathFrom = nullptr;
		from->edgeFrom = nullptr;

		//some kind of priority queue
		//but in list
		List<TransportNode*> searchFronier;
		searchFronier.Add(from);

		while (searchFronier.Count() > 0)
		{
			TransportNode* current = searchFronier[0];
			searchFronier.RemoveAt(0);

			long minCost = SIZE_MAX;
			for (size_t i = 0; i < current->GetConnectionCount(); i++)
			{
				TransportEdge edge = current->GetConnection(i);
				TransportNode* neighbor = edge.toNode;

				if (edge.currentFlow == edge.maxFlow)
					continue;

				size_t newDistance = current->searchDistance + edge.currentFlow;

				//update distances
				if (neighbor->searchDistance == SIZE_MAX)
				{
					neighbor->searchDistance = newDistance;
					neighbor->edgeFrom = &edge;
					neighbor->pathFrom = current;
					searchFronier.Add(neighbor);
				}
				else if (newDistance < neighbor->searchDistance)
				{
					neighbor->searchDistance = newDistance;
					neighbor->edgeFrom = &edge;
					neighbor->pathFrom = current;
				}
			}

			//sort pointers by value's distance
			searchFronier.SortPtr(true);
		}

		if (from->searchDistance != SIZE_MAX)
		{
			cout << "Calculated path distance from " << from->name << " to " << to->name << " is " << to->searchDistance << endl;
			//PrintPathForNode(toIndex);
			return GetPathForNode(to);
		}
		else
		{
			cout << "Path from " << from->name << " to " << to->name << " doen't exist " << endl << endl;
			return new List<TransportEdge*>();
		}


	}
	List<TransportEdge*>* GetPathForNode(TransportNode* node)
	{
		TransportNode* currentNode = node;
		List<TransportNode*> pathList;
		List<TransportEdge*>* edgeList = new List<TransportEdge*>();
		while (currentNode != nullptr)
		{
			pathList.Add(currentNode);
			currentNode = currentNode->pathFrom;
		}
		pathList.Reverse();

		for (size_t i = 0; i < pathList.Count(); i++)
		{
			if (pathList[i]->edgeFrom != nullptr)
				edgeList->Add(pathList[i]->edgeFrom);
		}
		//now we have to get connections


		for (size_t i = 0; i < pathList.Count(); i++)
		{
			if (i != pathList.Count() - 1)
				cout << pathList[i]->name << " " << pathList[i]->searchDistance << " ---> ";
			else
				cout << pathList[i]->name << " " << pathList[i]->searchDistance << endl;
		}
		cout << endl;
		return edgeList;
	}
	size_t GetPathIncrement(List<TransportEdge*>* path)
	{
		size_t maxAvailableIncrement = 0;
		for (size_t i = 0; i < path->Count(); i++)
		{
			size_t availableFlow = path->At(i)->maxFlow - path->At(i)->currentFlow;
			if (availableFlow > maxAvailableIncrement)
			{
				maxAvailableIncrement = availableFlow;
			}
		}
		return maxAvailableIncrement;
	}

};