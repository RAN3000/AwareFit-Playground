/*
 * C++ program for Kruskal's algorithm  to find Minimum
 * Spanning Tree of a given connected, undirected and weighted graph.
 * 
 * Kruskal's algorithm:
 *      1. Sort all the edges in non-decreasing order of their weight
 *      2. Pick the smallest edge. Check if it forms a cycle with the spanning tree formed
 *         so far. If a cycle is not formed, include this edge. Else, discard it.
 *      3. Repeat step 2 until there are (V-1) edges in the spanning tree.
 */
#include <stdio.h>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// shortcut for an integer pair
typedef pair<int, int> iPair;

// structure to represent a graph
struct Graph
{
    // number of vertices and edges
    int V, E;
    // inner iPair contains vertices of the edge, outer pair's int is the edge's weight
    vector<pair<int, iPair>> edges;

    // constructor
    Graph(int V, int E)
    {
        this->V = V;
        this->E = E;
    }

    // add an edge
    void addEdge(int u, int v, int weight)
    {
        edges.push_back({weight, {u, v}});
    }

    // Build the undirected graph from a image,
    // each image pixel correspond to a vertex,
    // The edge set is constructed by connecting pairs
    // of pixels that are neighbors in a 8-connected sense:
    //      o-o-o
    //      |X|X|
    //      o-o-o
    //      |X|X|
    //      o-o-o
    Graph(Mat &image);

    // function to find MST using Kruskal's
    // MST algorithm
    int kruskalMST();
};

// structure to represent disjoint sets
struct DisjointSets
{
    // Int is internal difference
    int *parent, *rank, *size, *Int;
    int n;
    int numTrees;

    // constructor
    DisjointSets(int n)
    {
        // allocate memory
        this->n = n;
        parent = new int[n + 1];
        rank = new int[n + 1];
        size = new int[n + 1];
        Int = new int[n + 1];
        numTrees = n;

        // initially, all vertices are in different sets and have rank 0
        for (int i = 0; i <= n; i++)
        {
            rank[i] = 0;
            // every element is parent of itself
            parent[i] = i;

            size[i] = 1;
            Int[i] = 0;
        }
    }

    // find the parent of node u, uses path compression
    int find(int u)
    {
        if (u != parent[u])
        {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    }

    // union by rank
    // w is the weight of the edge causing the merge
    void merge(int x, int y, int w = 0)
    {
        x = find(x), y = find(y);

        // make tree of smaller height a subtree of the other tree
        if (rank[x] > rank[y])
        {
            parent[y] = x;
            Int[x] = w;
            size[x] = size[x] + size[y];
        }
        else
        {
            parent[x] = y;
            Int[y] = w;
            size[y] = size[x] + size[y];
        }

        if (rank[x] == rank[y])
        {
            rank[y]++;
        }

        numTrees--;
    }
};

int vertexId(int x, int y, int width)
{
    return x + y * width;
}

Graph::Graph(Mat &image)
{
    // only mono channel images of unsigned char
    CV_Assert(image.channels() == 1);
    CV_Assert(image.depth() == CV_8U);

    // scan image

    int nRows = image.rows;
    int nCols = image.cols;

    int x, y, weight;
    uchar *rowPtr;
    uchar *prevRowPtr;
    for (y = 0; y < nRows; ++y)
    {
        rowPtr = image.ptr<uchar>(y);
        for (x = 0; x < nCols; ++x)
        {
            if (x > 0)
            {
                //   "-"
                weight = abs(rowPtr[x] - rowPtr[x - 1]);
                addEdge(vertexId(x, y, nCols), vertexId(x - 1, y, nCols), weight);
            }

            if (y > 0)
            {
                //   "|"
                weight = abs(rowPtr[x] - prevRowPtr[x]);
                addEdge(vertexId(x, y, nCols), vertexId(x, y - 1, nCols), weight);
            }

            if (x > 0 && y > 0)
            {
                //   "\"
                weight = abs(rowPtr[x] - prevRowPtr[x - 1]);
                addEdge(vertexId(x, y, nCols), vertexId(x - 1, y - 1, nCols), weight);

                //   "/"
                weight = abs(prevRowPtr[x] - rowPtr[x - 1]);
                addEdge(vertexId(x, y - 1, nCols), vertexId(x - 1, y, nCols), weight);
            }
        }
        prevRowPtr = rowPtr;
    }

    this->V = nRows * nCols;
    this->E = edges.size();
}

int Graph::kruskalMST()
{
    int mst_wt = 0;
    int spanning_tree_size = 0;

    // sort edges in non-decreasing order on basis of weight
    // by default the sort function sorts the vector elements on basis of the first element of pairs
    sort(edges.begin(), edges.end());

    // create disjoint sets
    DisjointSets ds(V);

    // iterate through all sorted edges
    vector<pair<int, iPair>>::iterator it;
    for (it = edges.begin(); it != edges.end() && spanning_tree_size <= V - 1; it++)
    {
        int u = it->second.first;
        int v = it->second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        // check if selected edge is creating a cycle or not
        // (cycle is created if u and v belong to the same set)
        if (set_u != set_v)
        {
            // current edge will be in the MST
            cout << u << " - " << v << endl;

            mst_wt += it->first;

            // merge two sets
            ds.merge(set_u, set_v);
            spanning_tree_size++;
        }
    }

    return mst_wt;
}

// will return true for pass, false for fail
bool testMST()
{
    int V = 9, E = 12;
    Graph g(V, E);

    g.addEdge(0, 1, 2);
    g.addEdge(1, 2, 1);
    g.addEdge(0, 3, 2);
    g.addEdge(1, 4, 1);
    g.addEdge(2, 5, 3);
    g.addEdge(3, 4, 1);
    g.addEdge(4, 5, 1);
    g.addEdge(3, 6, 1);
    g.addEdge(4, 7, 2);
    g.addEdge(5, 8, 3);
    g.addEdge(6, 7, 2);
    g.addEdge(7, 8, 1);

    cout << "Edges of MST are \n";
    int mst_wt = g.kruskalMST();

    cout << "\n Weight of MST is " << mst_wt << endl;
    return mst_wt == 10;
}