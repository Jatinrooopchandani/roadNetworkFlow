#pragma once
#include<iostream>
#include<queue>
#include<algorithm>
#include<vector>

#include "edge.h"

using namespace std;

class NetworkFlow {
	
public:
	vector<int> path;
	int maxFlow = 0;

    int fordFulkerson(std::vector<std::vector<int>>& graph, int source, int sink) {
        int n = graph.size();
        std::vector<std::vector<int>> residualGraph(n, std::vector<int>(n, 0));

        // Copy the original graph to the residual graph
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                residualGraph[i][j] = graph[i][j];

        // Array to store the path taken from source to sink
        std::vector<int> parent(n);

        // Augment the flow while there is a path from source to sink
        while (true) {
            std::fill(parent.begin(), parent.end(), -1);
            std::queue<int> q;
            q.push(source);
            parent[source] = -2;

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int v = 0; v < n; ++v) {
                    if (parent[v] == -1 && residualGraph[u][v] > 0) {
                        parent[v] = u;
                        q.push(v);
                    }
                }
            }

            // If there is no augmenting path, break
            if (parent[sink] == -1)
                break;

            int pathFlow = INT_MAX;
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = min(pathFlow, residualGraph[u][v]);
            }

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                residualGraph[u][v] -= pathFlow;
                residualGraph[v][u] += pathFlow;
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }

    int findWeightTill(vector<vector<Edge*>>& edgeMap, vector<int> &parent, int source, int target)
    {
        int res = 0;

        while (target != source)
        {
            res += edgeMap[parent[target]][target]->weight;
            target = parent[target];
        }

        return res;
    }

    vector<int> optimizedPath(vector<Edge>& edges, int source, int sink, int n)
    {
        Edge* NULLEDGE = new Edge();
        vector<Edge*> NullEdgeVector(edges.size(), NULLEDGE);
        vector<vector<Edge*>> edgeMap(edges.size(), NullEdgeVector);


        for (int i = 0; i < edges.size(); i++)
        {
            edgeMap[edges[i].from][edges[i].to] = &(edges[i]);
        }

        vector<int> parent(n, -1);
        queue<int> q;
        q.push(source);

        while (!q.empty())
        {
            int current = q.front();
            q.pop();

            int weightTillCurrent = findWeightTill(edgeMap, parent, source, current);
            
            for (auto i : edgeMap[current])
            {
                if (i != NULLEDGE)
                {
                    if (parent[i->to] == -1 || weightTillCurrent + edgeMap[current][i->to]->weight < findWeightTill(edgeMap, parent, source, i->to))
                    {
                        parent[i->to] = current;
                        q.push(i->to);
                    }
                }
            }
        }

        // Reconstruct the path from source to sink
        vector<int> path;
        for (int at = sink; at != source; at = parent[at])
            path.push_back(at);
        path.push_back(source);
        reverse(path.begin(), path.end());

        return path;
    }


	

};