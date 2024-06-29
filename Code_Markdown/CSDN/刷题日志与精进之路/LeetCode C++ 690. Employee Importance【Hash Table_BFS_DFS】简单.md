<p>You are given a data structure of employee information, which includes the employee's <b>unique id</b>, their&nbsp;<b>importance value</b> and their&nbsp;<b>direct</b> subordinates' id.</p>

<p>For example, employee 1 is the leader of employee 2, and employee 2 is the leader of employee 3. They have importance value 15, 10 and 5, respectively. Then employee 1 has a data structure like [1, 15, [2]], and employee 2 has [2, 10, [3]], and employee 3 has [3, 5, []]. Note that although employee 3 is also a subordinate of employee 1, the relationship is <b>not direct</b>.</p>

<p>Now given the employee information of a company, and an employee id, you need to return the total importance value of this employee and all their&nbsp;subordinates.</p>

<p><b>Example 1:</b></p>

```clike
Input: [[1, 5, [2, 3]], [2, 3, []], [3, 3, []]], 1
Output: 11
Explanation:
Employee 1 has importance value 5, and he has two direct subordinates: employee 2 and employee 3. They both have importance value 3. So the total importance value of employee 1 is 5 + 3 + 3 = 11. 
```

 

<p><b>Note:</b></p> 
<ol>
	<li>One employee has at most one <b>direct</b> leader and may have several subordinates.</li>
	<li>The maximum number of employees won't exceed 2000.</li>
</ol>

题意：给定一个保存员工信息的数据结构，它包含了员工 **唯一的 `id`** ，**重要度** 和 **直系下属的 `id`** 。现在输入一个公司的所有员工信息，以及单个员工 `id` ，返回这个员工和他所有下属的重要度之和。 

---
### 解法 邻接哈希表+DFS
由于不知道 `id` 的数据范围，也不知道雇佣树的根节点 `id` （虽然这题也不需要知道），所以我们需要用哈希表来存储一个节点 `id` 和它对应的下属 `id` 列表（这里实际上是一个 `Employee` 指针）。然后用DFS求出结果即可：
```cpp
/*
// Definition for Employee.
class Employee {
public:
    int id;
    int importance;
    vector<int> subordinates;
};
*/ 
class Solution {
private: 
    unordered_map<int, Employee*> g; 
    int dfs(int cur) {
        int own = g[cur]->importance;
        for (const int &sub : g[cur]->subordinates) own += dfs(sub);
        return own;
    }
public:
    int getImportance(vector<Employee*> employees, int id) { 
        for (Employee* e : employees) g[e->id] = e; 
        return dfs(id);
    }
};
```
运行效率如下：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了98.07% 的用户
内存消耗：13.8 MB, 在所有 C++ 提交中击败了65.12% 的用户
```
BFS版写法如下：
```cpp
class Solution { 
public:
    int getImportance(vector<Employee*> employees, int id) {
        unordered_map<int, Employee*> g;   
        for (Employee* e : employees) g[e->id] = e; 
        int ans = 0;
        queue<int> q;
        q.push(id);
        while (!q.empty()) {
            int t = q.front(); q.pop();
            ans += g[t]->importance;
            for (const int &sub : g[t]->subordinates) q.push(sub);
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了99.53% 的用户
内存消耗：13.8 MB, 在所有 C++ 提交中击败了66.09% 的用户
```
