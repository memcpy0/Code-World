/*Given a directed graph, check whether the graph contains a cycle or not.
Your function should return true if the given graph contains
at least one cycle, else return false.
Input: n = 4, e = 6
0 -> 1, 0 -> 2, 1 -> 2, 2 -> 0, 2 -> 3, 3 -> 3
Output: Yes
	1 ---> 2 ---> 3 ----
	^    ^ |      ^    |
	|   /  |      |____|
	|  /   |
	| /    |
	0 <-----
The diagram clearly shows a cycle 0 -> 2 -> 0
	
Input:n = 4, e = 3
0 -> 1, 0 -> 2, 1 -> 2, 2 -> 3
Output:No
	1 ---> 2 ---> 3
	^     ^
	|     |
	|     |
	0 -----

https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
*/
/*Solution using Depth First Search or DFS
DFS判断有向图的环（和无向图DFS判断环差不多的做法）

    Approach: Depth First Traversal can be used to detect a cycle in a Graph.
	DFS for a connected graph produces a tree.
	There is a cycle in a graph only if there is a back edge present in the graph.
	A back edge is an edge that is from a node to itself (self-loop) or
	one of its ancestors in the tree produced by DFS.
	In the following graph, there are 3 back edges, marked with a cross sign.
	We can observe that these 3 back edges indicate 3 cycles present in the graph.
https://media.geeksforgeeks.org/wp-content/uploads/cycle.png

For a disconnected graph, Get the DFS forest as output.
To detect cycle, check for a cycle in individual trees by checking back edges.

To detect a back edge, keep track of vertices currently in the recursion stack of function for DFS traversal.
If a vertex is reached that is already in the recursion stack, then there is a cycle in the tree.
The edge that connects the current vertex to the vertex in the recursion stack is a back edge.
Use recStack[] array to keep track of vertices in the recursion stack.

Dry run of the above approach:
https://media.geeksforgeeks.org/wp-content/cdn-uploads/20190704130006/DetectCycleInaDirectedGraph.png

Algorithm:
    Create the graph using the given number of edges and vertices.
    Create a recursive function that initializes the current index or vertex, visited, and recursion stack.
    Mark the current node as visited and also mark the index in recursion stack.
    Find all the vertices which are not visited and are adjacent to the current node. Recursively
call the function for those vertices, If the recursive function returns true, return true.
    If the adjacent vertices are already marked in the recursion stack then return true.
    Create a wrapper class, that calls the recursive function for all the vertices and if any
function returns true return true. Else if for all vertices the function returns false return false.

Output:

Graph contains cycle

Complexity Analysis:

    Time Complexity: O(V+E).
    Time Complexity of this method is same as time complexity of DFS traversal which is O(V+E).
    Space Complexity: O(V).
    To store the visited and recursion stack O(V) space is needed.

*/

// A C++ Program to detect cycle in a graph
#include<bits/stdc++.h>
using namespace std;

class Graph
{
    int V;    // No. of vertices
    list<int> *adj;    // Pointer to an array containing adjacency lists
    bool isCyclicUtil(int v, bool visited[], bool *rs);  // used by isCyclic()
public:
    Graph(int V);   // Constructor
    void addEdge(int v, int w);   // to add an edge to graph
    bool isCyclic();    // returns true if there is a cycle in this graph
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v’s list.
}

// This function is a variation of DFSUtil() in https://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclicUtil(int v, bool visited[], bool *recStack) {
    if(visited[v] == false) {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;   //已经访问过
        recStack[v] = true;  //当前顶点加入栈中

        // Recur for all the vertices adjacent to this vertex
        list<int>::iterator i;
        for(i = adj[v].begin(); i != adj[v].end(); ++i) {
            if (!visited[*i] && isCyclicUtil(*i, visited, recStack)) return true;
            else if (recStack[*i]) //注意：和无向图DFS(只用parent)判断环存在的异同
                return true;  //访问过且在栈内部
				// | 没有访问、isCycleUtil返回false
        }
    }
    recStack[v] = false;  //当前顶点移出栈 // remove the vertex from recursion stack
    return false;         //没有环
}

// Returns true if the graph contains a cycle, else false.
// This function is a variation of DFS() in https://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclic() {
    // Mark all the vertices as not visited and not part of recursion
    // stack
    bool *visited = new bool[V];
    bool *recStack = new bool[V];
    for(int i = 0; i < V; i++) {
        visited[i] = false;
        recStack[i] = false;
    }

    // Call the recursive helper function to detect cycle in different
    // DFS trees
    for(int i = 0; i < V; i++)
        if (isCyclicUtil(i, visited, recStack))
            return true;

    return false;
}

int main() {
    // Create a graph given in the above diagram
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);

    if(g.isCyclic()) cout << "Graph contains cycle";
    else cout << "Graph doesn't contain cycle";
    return 0;
}
//----------------------------------------------------------------------------------------------------------
//有向图 普通DFS判断环
class Solution {
private:
    bool isCyclic(const vector<vector<int>>& G, int u, vector<bool>& visited, vector<bool>& recStack) {
        visited[u] = true;  //已经访问过
        recStack[u] = true; //当前顶点加入递归栈中
        for (const int &v : G[u]) {
            if (visited[v] == false) { //没有访问过
                if (isCyclic(G, v, visited, recStack)) //存在环
                    return true;
            } else if (recStack[v]) return true; //访问过v且v在此时的递归栈中,存在回边
        }
        recStack[u] = false; //当前顶点移出递归栈
        return false; //没有环
    }
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> G(numCourses);
        //vector<int> in(numCourses, 0);
        for (const vector<int>& v : prerequisites) {
            //++in[v[0]]; //v[0] <- v[1]
            G[v[1]].push_back(v[0]);
        }
        vector<bool> visited(numCourses), recStack(numCourses);
        for (int i = 0; i < numCourses; ++i) {
            if (visited[i] == false)
                if (isCyclic(G, i, visited, recStack)) //存在环
                    return false; //无法完成课程
        }
        return true; //不存在环,可以完成课程
    }
};
//----------------------------------------------------------------------------------------------------------
//有向图 BFS拓扑排序判断环
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        vector<int> adjList[numCourses], indegrees(numCourses, 0);
        for (int i = 0; i < prerequisites.size(); ++i) {
            ++indegrees[prerequisites[i][0]];
            adjList[prerequisites[i][1]].push_back(prerequisites[i][0]);
        }
        queue<int> q;
        for (int i = 0; i < numCourses; ++i)
            if (!indegrees[i]) q.push(i);
        int count = 0; //拓扑序列中课程的数量
        while (!q.empty()) {
            int t = q.front(); q.pop();
            ++count;
            for (int i = 0; i < adjList[t].size(); ++i) {
                int v = adjList[t][i];
                if (!(--indegrees[v])) q.push(v);
            }
        }
        return count == numCourses;
    }
};
