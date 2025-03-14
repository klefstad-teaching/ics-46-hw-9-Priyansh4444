#include "dijkstras.h"

vector<int> dijkstra_shortest_path(const Graph &G, int source, vector<int> &previous)
{
    int n = G.numVertices;
    // Graph Distances
    vector<int> distances(G.numVertices, INF);
    previous.assign(G.numVertices, 0);
    distances[source] = 0;
    vector<bool> visited(G.numVertices, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, source});
    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
        if (visited[u])
            continue;
        visited[u] = true;
        for (const Edge &e : G[u])
        {
            if (distances[e.dst] > distances[u] + e.weight)
            {
                distances[e.dst] = distances[u] + e.weight;
                previous[e.dst] = u;
                pq.push({distances[e.dst], e.dst});
            }
        }
    }
    return distances;
}

vector<int> extract_shortest_path(const vector<int> &distances, const vector<int> &previous, int destination)
{
    vector<int> path;
    int v = destination;
    while (distances[v] != 0) {
        path.push_back(v);
        v = previous[v];
    }
    path.push_back(v);  // Add the source vertex
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int> &v, int total)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        cout << v[i];
        cout << " ";
    }
    cout << "\nTotal cost is " << total << endl;
}