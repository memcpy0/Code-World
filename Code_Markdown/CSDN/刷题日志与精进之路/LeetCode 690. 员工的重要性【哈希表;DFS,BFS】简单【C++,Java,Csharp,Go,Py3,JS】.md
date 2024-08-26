> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

你有一个保存员工信息的数据结构，它包含了员工唯一的 id ，重要度和直系下属的 id 。

给定一个员工数组 `employees`，其中：
- `employees[i].id` 是第 `i` 个员工的 ID。
- `employees[i].importance` 是第 `i` 个员工的重要度。
- `employees[i].subordinates` 是第 `i` 名员工的直接下属的 ID 列表。

给定一个整数 `id` 表示一个员工的 ID，返回这个员工和他所有下属的重要度的 **总和**。

**示例 1：**
**![](https://pic.leetcode.cn/1716170448-dKZffb-image.png)**
```c
输入：employees = [[1,5,[2,3]],[2,3,[]],[3,3,[]]], id = 1
输出：11
解释：
员工 1 自身的重要度是 5 ，他有两个直系下属 2 和 3 ，而且 2 和 3 的重要度均为 3 。因此员工 1 的总重要度是 5 + 3 + 3 = 11 。
```
**示例 2：**
**![](https://pic.leetcode.cn/1716170929-dkWpra-image.png)**
```c
输入：employees = [[1,2,[5]],[5,-3,[]]], id = 5
输出：-3
解释：员工 5 的重要度为 -3 并且没有直接下属。
因此，员工 5 的总重要度为 -3。
```
**提示：**
- `1 <= employees.length <= 2000`
- `1 <= employees[i].id <= 2000`
- 所有的 `employees[i].id` **互不相同**。
- `-100 <= employees[i].importance <= 100`
- 一名员工最多有一名直接领导，并可能有多名下属。
- `employees[i].subordinates` 中的 ID 都有效。
---
由于一个员工最多有一个直系领导，可以有零个或若干个直系下属，因此员工之间的领导和下属关系构成树的结构。给定一个员工编号，要求计算这个员工及其所有下属的重要性之和，即为找到以该员工为根节点的子树的结构中，每个员工的重要性之和。

对于树结构的问题，可以使用深度优先搜索或广度优先搜索的方法解决。
### 方法一：深度优先搜索
深度优先搜索的做法非常直观。根据**给定的员工编号**找到**员工**，从该员工开始遍历，对于每个员工，将其重要性加到总和中，然后对**该员工的每个直系下属**继续遍历，直到所有下属遍历完毕，此时的总和即为给定的员工及其所有下属的重要性之和。

实现方面，由于给定的是员工编号，且每个员工的编号都不相同，因此可以使用**哈希表**存储每个员工编号和对应的员工，即可通过员工编号得到对应的员工。
```python
class Solution:
    def getImportance(self, employees: List['Employee'], idx: int) -> int:
        mp = {employee.id: employee for employee in employees}

        def dfs(idx: int) -> int:
            employee = mp[idx]
            total = employee.importance + sum(dfs(subIdx) for subIdx in employee.subordinates)
            return total
        
        return dfs(idx)
```
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
```java
class Solution {
    Map<Integer, Employee> map = new HashMap<Integer, Employee>();

    public int getImportance(List<Employee> employees, int id) {
        for (Employee employee : employees) {
            map.put(employee.id, employee);
        }
        return dfs(id);
    }

    public int dfs(int id) {
        Employee employee = map.get(id);
        int total = employee.importance;
        List<Integer> subordinates = employee.subordinates;
        for (int subId : subordinates) {
            total += dfs(subId);
        }
        return total;
    }
}
```
```javascript
/**
 * Definition for Employee.
 * function Employee(id, importance, subordinates) {
 *     this.id = id;
 *     this.importance = importance;
 *     this.subordinates = subordinates;
 * }
 */

/**
 * @param {Employee[]} employees
 * @param {number} id
 * @return {number}
 */
var GetImportance = function(employees, id) {
    const map = new Map();
    for (const employee of employees) {
        map.set(employee.id, employee);
    }
    const dfs = (id) => {
        const employee = map.get(id);
        let total = employee.importance;
        const subordinates = employee.subordinates;
        for (const subId of subordinates) {
            total += dfs(subId);
        }
        return total;
    }
    return dfs(id);
};
```
```csharp
class Solution {
    Dictionary<int, Employee> dictionary = new Dictionary<int, Employee>();

    public int GetImportance(IList<Employee> employees, int id) {
        foreach (Employee employee in employees) {
            dictionary.Add(employee.id, employee);
        }
        return DFS(id);
    }

    public int DFS(int id) {
        Employee employee = dictionary[id];
        int total = employee.importance;
        IList<int> subordinates = employee.subordinates;
        foreach (int subId in subordinates) {
            total += DFS(subId);
        }
        return total;
    }
}
```
```go
func getImportance(employees []*Employee, id int) (total int) {
    mp := map[int]*Employee{}
    for _, employee := range employees {
        mp[employee.Id] = employee
    }

    var dfs func(int)
    dfs = func(id int) {
        employee := mp[id]
        total += employee.Importance
        for _, subId := range employee.Subordinates {
            dfs(subId)
        }
    }
    dfs(id)
    return
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是员工数量。需要遍历所有员工，在哈希表中存储员工编号和员工的对应关系，深度优先搜索对每个员工遍历一次。
- 空间复杂度：$O(n)$ ，其中 $n$ 是员工数量。空间复杂度主要取决于哈希表的空间和递归调用栈的空间，哈希表的大小为 $n$ ，栈的深度不超过 $n$ 。

### 方法二：广度优先搜索
也可以使用广度优先搜索的做法。和深度优先搜索一样，使用哈希表存储每个员工编号和对应的员工，即可通过员工编号得到对应的员工。根据给定的员工编号找到员工，从该员工开始广度优先搜索，对于每个遍历到的员工，将其重要性加到总和中，最终得到的总和即为给定的员工及其所有下属的重要性之和。
```python
class Solution:
    def getImportance(self, employees: List['Employee'], idx: int) -> int:
        mp = {employee.id: employee for employee in employees}

        total = 0
        que = collections.deque([idx])
        while que:
            curIdx = que.popleft()
            employee = mp[curIdx]
            total += employee.importance
            for subIdx in employee.subordinates:
                que.append(subIdx)
        
        return total
```
```java
class Solution {
    public int getImportance(List<Employee> employees, int id) {
        Map<Integer, Employee> map = new HashMap<Integer, Employee>();
        for (Employee employee : employees) {
            map.put(employee.id, employee);
        }
        int total = 0;
        Queue<Integer> queue = new LinkedList<Integer>();
        queue.offer(id);
        while (!queue.isEmpty()) {
            int curId = queue.poll();
            Employee employee = map.get(curId);
            total += employee.importance;
            List<Integer> subordinates = employee.subordinates;
            for (int subId : subordinates) {
                queue.offer(subId);
            }
        }
        return total;
    }
}
```
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
```js
var GetImportance = function(employees, id) {
    const map = new Map();
    for (const employee of employees) {
        map.set(employee.id, employee);
    }
    let total = 0;
    const queue = [];
    queue.push(id);
    while (queue.length) {
        const curId = queue.shift();
        const employee = map.get(curId);
        total += employee.importance;
        const subordinates = employee.subordinates;
        for (const subId of subordinates) {
            queue.push(subId);
        }
    }
    return total;
};
```
```go
func getImportance(employees []*Employee, id int) (total int) {
    mp := map[int]*Employee{}
    for _, employee := range employees {
        mp[employee.Id] = employee
    }

    queue := []int{id}
    for len(queue) > 0 {
        employee := mp[queue[0]]
        queue = queue[1:]
        total += employee.Importance
        for _, subId := range employee.Subordinates {
            queue = append(queue, subId)
        }
    }
    return
}
```
```csharp
class Solution {
    public int GetImportance(IList<Employee> employees, int id) {
        Dictionary<int, Employee> dictionary = new Dictionary<int, Employee>();
        foreach (Employee employee in employees) {
            dictionary.Add(employee.id, employee);
        }
        int total = 0;
        Queue<int> queue = new Queue<int>();
        queue.Enqueue(id);
        while (queue.Count > 0) {
            int curId = queue.Dequeue();
            Employee employee = dictionary[curId];
            total += employee.importance;
            IList<int> subordinates = employee.subordinates;
            foreach (int subId in subordinates) {
                queue.Enqueue(subId);
            }
        }
        return total;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是员工数量。需要遍历所有员工，在哈希表中存储员工编号和员工的对应关系，广度优先搜索对每个员工遍历一次。
- 空间复杂度：$O(n)$ ，其中 $n$ 是员工数量。空间复杂度主要取决于哈希表的空间和队列的空间，哈希表的大小为 $n$ ，队列的大小不超过 $n$ 。
