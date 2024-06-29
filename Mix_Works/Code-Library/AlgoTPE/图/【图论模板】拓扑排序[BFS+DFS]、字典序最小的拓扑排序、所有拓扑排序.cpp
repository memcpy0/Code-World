/*
Topological sorting for Directed Acyclic Graph (DAG) is a linear ordering of vertices
such that for every directed edge u v, vertex u comes before v in the ordering.
Topological Sorting for a graph is not possible if the graph is not a DAG.

Algorithm to find Topological Sorting:

We recommend to first see the implementation of DFS.
We can modify DFS to find Topological Sorting of a graph.
In DFS, we start from a vertex, we first print it and then recursively call DFS
for its adjacent vertices. In topological sorting, we use a temporary stack.
We don’t print the vertex immediately, we first recursively call topological sorting
for all its adjacent vertices, then push it to a stack.
Finally, print contents of the stack.
Note that a vertex is pushed to stack only when all of its adjacent vertices
(and their adjacent vertices and so on) are already in the stack.

Below image is an illustration of the above approach:
https://media.geeksforgeeks.org/wp-content/uploads/20200818211917/Topological-Sorting-1.png

Complexity Analysis:

    Time Complexity: O(V+E).
    The above algorithm is simply DFS with an extra stack. So time complexity is the same as DFS which is.
    Auxiliary space: O(V).
    The extra space is needed for the stack.

*/

class Solution { //DFS 判断环, 生成拓扑序列
private:
    bool isCyclic(const vector<vector<int>>& G, int u, vector<bool>& visited, vector<bool>& recStack, stack<int>& topStack) {
        visited[u] = true;  //已经访问过
        recStack[u] = true; //当前顶点加入递归栈中
        for (const int &v : G[u]) {
            if (visited[v] == false) { //没有访问过
                if (isCyclic(G, v, visited, recStack, topStack)) //存在环
                    return true;
            } else if (recStack[v]) return true; //访问过v且v在此时的递归栈中,存在回边
        }
        recStack[u] = false; //当前顶点移出递归栈
        topStack.push(u); //访问完当前顶点的所有邻接点后入栈当前顶点
        return false; //没有环
    }
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> G(numCourses);
        vector<int> in(numCourses, 0);
        for (const vector<int>& v: prerequisites) {
            ++in[v[0]];
            G[v[1]].push_back(v[0]);
        }
        vector<bool> visited(numCourses), recStack(numCourses);
        stack<int> st;
        for (int i = 0; i < numCourses; ++i) {
            if (visited[i] == false)
                if (isCyclic(G, i, visited, recStack, st)) //存在环
                    return {}; //无法完成课程
        }
        vector<int> ans;
        while (!st.empty()) { //不断弹出栈中的顶点
            ans.push_back(st.top()); //得到拓扑序列
            st.pop();
        }
        return ans; //不存在环,可以完成课程
    }
};

// A C++ program to print topological sorting of a DAG DFS 没有判断环！！！
#include <iostream>
#include <list>
#include <stack>
using namespace std;

// Class to represent a graph
class Graph {
    // No. of vertices'
    int V;
    // Pointer to an array containing adjacency listsList
    list<int>* adj;
    // A function used by topologicalSort
    void topologicalSortUtil(int v, bool visited[], stack<int>& Stack);
public:
    // Constructor
    Graph(int V);
    // function to add an edge to graph
    void addEdge(int v, int w);
    // prints a Topological Sort of
    // the complete graph
    void topologicalSort();
};
Graph::Graph(int V) {
    this->V = V;
    adj = new list<int>[V];
}
void Graph::addEdge(int v, int w) {
    // Add w to v’s list.
    adj[v].push_back(w);
}

// A recursive function used by topologicalSort
void Graph::topologicalSortUtil(int v, bool visited[], stack<int>& Stack) {
    // Mark the current node as visited.
    visited[v] = true;
    // Recur for all the vertices
    // adjacent to this vertex
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            topologicalSortUtil(*i, visited, Stack);
    // Push current vertex to stack
    // which stores result
    Stack.push(v);
}
// The function to do Topological Sort.
// It uses recursive topologicalSortUtil()
void Graph::topologicalSort() {
    stack<int> Stack;
    // Mark all the vertices as not visited
    bool* visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;
    // Call the recursive helper function
    // to store Topological
    // Sort starting from all
    // vertices one by one
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            topologicalSortUtil(i, visited, Stack);
    // Print contents of stack
    while (Stack.empty() == false) {
        cout << Stack.top() << " ";
        Stack.pop();
    }
}

// Driver Code
int main() {
    // Create a graph given in the above diagram
    Graph g(6);
    g.addEdge(5, 2);
    g.addEdge(5, 0);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);

    cout << "Following is a Topological Sort of the given graph \n";

    // Function Call
    g.topologicalSort();

    return 0;
}
/*Solution: In this article we will see another way to find the linear ordering of vertices
 in a directed acyclic graph (DAG). The approach is
based on the below fact:

A DAG G has at least one vertex with in-degree 0 and one vertex with out-degree 0.
Proof: There’s a simple proof to the above fact is that a DAG does not contain
a cycle which means that all paths will be of finite length.
Now let S be the longest path from u(source) to v(destination). Since S is the
longest path there can be no incoming edge to u and no outgoing edge from v,
if this situation had occurred then S would not have been the longest path
=> indegree(u) = 0 and outdegree(v) = 0

Algorithm: Steps involved in finding the topological ordering of a DAG:
Step-1: Compute in-degree (number of incoming edges) for each of the vertex present in the DAG
and initialize the count of visited nodes as 0.

Step-2: Pick all the vertices with in-degree as 0 and add them into a queue (Enqueue operation)

Step-3: Remove a vertex from the queue (Dequeue operation) and then.

    Increment count of visited nodes by 1.
    Decrease in-degree by 1 for all its neighboring nodes.
    If in-degree of a neighboring nodes is reduced to zero, then add it to the queue.

Step 5: Repeat Step 3 until the queue is empty.

Step 5: If count of visited nodes is not equal to the number of
nodes in the graph then the topological sort is not possible for the given graph.

How to find in-degree of each node?
There are 2 ways to calculate in-degree of every vertex:

    Take an in-degree array which will keep track of
    Traverse the array of edges and simply increase the counter of the destination node by 1.

    for each node in Nodes
        indegree[node] = 0;
    for each edge(src, dest) in Edges
        indegree[dest]++

    Time Complexity: O(V+E)
    Traverse the list for every node and then increment the in-degree of all the nodes connected to it by 1.

        for each node in Nodes
            If (list[node].size()!=0) then
            for each dest in list
                indegree[dest]++;

    Time Complexity: The outer for loop will be executed V number of times and the inner
	for loop will be executed E number of times, Thus overall time complexity is O(V+E).
    The overall time complexity of the algorithm is O(V+E)

Complexity Analysis:

    Time Complexity: O(V+E).
    The outer for loop will be executed V number of times and the inner for loop will be executed E number of times.
    Auxillary Space: O(V).
    The queue needs to store all the vertices of the graph. So the space required is O(V)
*/

class Solution { //BFS 判断环, 生成拓扑序列
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<int> G[numCourses], in(numCourses, 0), ans; //拓扑序列
        for (const vector<int> &v : prerequisites) {
            ++in[v[0]]; //入度+1
            G[v[1]].push_back(v[0]);
        }
        queue<int> q;
        int count = 0;
        for (int i = 0; i < numCourses; ++i) if (!in[i]) q.push(i);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            ans.push_back(u);
            ++count;
            for (const int &v : G[u]) if (!(--in[v])) q.push(v);
        }
        return count == numCourses ? ans : vector<int>{};
    }
};

// A C++ program to print topological
// sorting of a graph using indegrees.
#include <bits/stdc++.h>
using namespace std;

// Class to represent a graph
class Graph {
    // No. of vertices'
    int V;
    // Pointer to an array containing
    // adjacency listsList
    list<int>* adj;
public:
    // Constructor
    Graph(int V);
    // Function to add an edge to graph
    void addEdge(int u, int v);
    // prints a Topological Sort of
    // the complete graph
    void topologicalSort();
};
Graph::Graph(int V) {
    this->V = V;
    adj = new list<int>[V];
}
void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
}

// The function to do
// Topological Sort.
void Graph::topologicalSort() {
    // Create a vector to store
    // indegrees of all
    // vertices. Initialize all
    // indegrees as 0.
    vector<int> in_degree(V, 0);
    
    // Traverse adjacency lists
    // to fill indegrees of
    // vertices.  This step
    // takes O(V+E) time
    for (int u = 0; u < V; u++) {
        list<int>::iterator itr;
        for (itr = adj[u].begin(); itr != adj[u].end(); itr++)
            in_degree[*itr]++;
    }
    // Create an queue and enqueue
    // all vertices with indegree 0
    queue<int> q;
    for (int i = 0; i < V; i++)
        if (in_degree[i] == 0)
            q.push(i);
    // Initialize count of visited vertices
    int cnt = 0;

    // Create a vector to store
    // result (A topological
    // ordering of the vertices)
    vector<int> top_order;

    // One by one dequeue vertices
    // from queue and enqueue
    // adjacents if indegree of
    // adjacent becomes 0
    while (!q.empty()) {
        // Extract front of queue
        // (or perform dequeue)
        // and add it to topological order
        int u = q.front();
        q.pop();
        top_order.push_back(u);

        // Iterate through all its
        // neighbouring nodes
        // of dequeued node u and
        // decrease their in-degree
        // by 1
        list<int>::iterator itr;
        for (itr = adj[u].begin();
             itr != adj[u].end(); itr++)

            // If in-degree becomes zero,
            // add it to queue
            if (--in_degree[*itr] == 0)
                q.push(*itr);

        cnt++;
    }
    // Check if there was a cycle
    if (cnt != V) {
        cout << "There exists a cycle in the graph\n";
        return;
    }
    // Print topological order
    for (int i = 0; i < top_order.size(); i++)
        cout << top_order[i] << " ";
    cout << endl;
}

// Driver program to test above functions
int main() {
    // Create a graph given in the
    // above diagram
    Graph g(6);
    g.addEdge(5, 2);
    g.addEdge(5, 0);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);

    cout << "Following is a Topological Sort of\n";
    g.topologicalSort();

    return 0;
}
//--------------------------------------------
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 510;
vector<int> E[MAXN];
vector<int> ans;
int indegrees[MAXN] = {0}, N, M, u, v;

void minTopSort() {
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 1; i <= N; ++i) 
        if (indegrees[i] == 0)  
            pq.push(i);
    while (!pq.empty()) {
        int t = pq.top(); pq.pop();
        ans.push_back(t);
        for (int i = 0; i < E[t].size(); ++i) {
            int k = E[t][i];
            --indegrees[k];
            if (indegrees[k] == 0) pq.push(k);
        }    
    }
}

int main() {
    while (cin >> N >> M) {
        for (int i = 0; i < MAXN; ++i) E[i].clear();
        ans.clear();
        memset(indegrees, 0, sizeof(indegrees));
        
        for (int i = 0; i < M; ++i) {
            cin >> u >> v;
            ++indegrees[v];  //v的入度+1 
            E[u].push_back(v); //有向边 
        }
        minTopSort();        
        cout << ans[0];
        for (int i = 1; i < ans.size(); ++i) cout << " " << ans[i];
        cout << endl;
    }
    return 0;
}
//-----------------------------------------------------------
#include<iostream>
#include<list>
#include<vector>
#include<queue>

using namespace std;

class Graph
{
	int n; 		   //顶点数
	list<int> *adj;  //邻接表 
	void allTopologicalSortUtil(bool visited[], int indegree[], vector<int> &ans);  
public:
	Graph(int _n) { n = _n; adj = new list<int>[_n];}
	~Graph(){ delete [] adj; }
	void addEdge(int v, int w){ adj[v].push_back(w); }
	void allTopologicalSort(); 	 
};
void Graph::allTopologicalSortUtil(bool visited[], int indegree[], vector<int> &ans)
{
	if( ans.size() == n)		//找到一种
	{
		for(int i = 0; i < n ; i++)
		{
			cout<<ans[i]<<" ";
		}
		cout<<endl;
		return ;
	}

	for(int i = 0; i < n; i++)
	{
		if(indegree[i] == 0 && !visited[i])
		{//处理本分支上入度为一，还没访问过的节点
			list<int>::iterator j;
			for(j = adj[i].begin(); j != adj[i].end(); j++)
			{//令i的所有邻居的入度减一
				indegree[*j]--;
			}
			
			visited[i] = true;
			ans.push_back(i);
			
			allTopologicalSortUtil(visited, indegree, ans);
			 
			//访问一条分支后，把入度恢复回来
			for(j = adj[i].begin(); j != adj[i].end(); j++)
			{
				indegree[*j]++;
			}
			
			visited[i] = false;
			ans.pop_back();
		}
	} 
}
void Graph::allTopologicalSort()
{
	bool *visited = new bool[n]; 
	int *indegree = new int[n];
	vector<int> ans;
	
	for(int i = 0; i < n; i++)
	{
		visited[i] = false;
		indegree[i] = 0; 
	}
	
	for(int i = 0; i < n; i++)			//计算每个顶点的入度
	{
		list<int>::iterator it;
		for(it = adj[i].begin(); it != adj[i].end(); it++)
		{
			indegree[*it]++; 
		}
	}
	allTopologicalSortUtil(visited, indegree, ans);
}
int main() 
{ 
    Graph g(6); 
    g.addEdge(4, 2); 
    g.addEdge(4, 0); 
    g.addEdge(5, 0); 
    g.addEdge(5, 1); 
    g.addEdge(2, 3); 
    g.addEdge(3, 1); 
    cout << "这个图所有的拓扑排序是：\n"; 
    g.allTopologicalSort(); 
  
    return 0; 	
} 
