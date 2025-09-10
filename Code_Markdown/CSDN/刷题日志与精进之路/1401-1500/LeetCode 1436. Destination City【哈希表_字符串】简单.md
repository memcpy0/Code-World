> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


You are given the array `paths`, where `paths[i] = [cityAi, cityBi]` means there exists a direct path going from `cityAi` to `cityBi`. *Return the destination city, that is, the city without any path outgoing to another city.*

It is guaranteed that the graph of paths forms a line without any loop, therefore, there will be exactly one destination city.


**Example 1:**

```swift
Input: paths = [["London","New York"],["New York","Lima"],["Lima","Sao Paulo"]]
Output: "Sao Paulo" 
Explanation: Starting at "London" city you will reach "Sao Paulo" city which is the destination city. Your trip consist of: "London" -> "New York" -> "Lima" -> "Sao Paulo".
```

**Example 2:**

```swift
Input: paths = [["B","C"],["D","B"],["C","A"]]
Output: "A"
Explanation: All possible trips are: 
"D" -> "B" -> "C" -> "A". 
"B" -> "C" -> "A". 
"C" -> "A". 
"A". 
Clearly the destination city is "A".
```

**Example 3:**

```swift
Input: paths = [["A","Z"]]
Output: "Z"
```

 

**Constraints:**
- `1 <= paths.length <= 100`
-  `paths[i].length == 2`
 - `1 <= cityAi.length, cityBi.length <= 10`
-  `cityAi != cityBi`
-  All strings consist of lowercase and uppercase English letters and the space character.


题意：给出一个路径数组 `paths` ，其中 `paths[i] = [cityAi, cityBi]` ，表示存在一条从 `cityAi` 出发到达 `cityBi` 的有向路径。找出这次旅行的终点站，即没有任何可以通往其他城市的线路的城市。

---
### 解法1 哈希表（计算出度、无出边）
一开始想到的就是，终点站城市是一个出度为零的目标城市。因此计算每个 `paths[i]` 中起点城市 `cityAi` 的出度，然后找到零出度的目标城市 `cityBi` 。整个算法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
class Solution {
public:
    string destCity(vector<vector<string>>& paths) {
        unordered_map<string, int> rec; //city : outdegree 
        for (const vector<string> &path : paths) ++rec[path[0]]; //起点城市的出度+1
        for (const vector<string> &path : paths)
            if (rec[path[1]] == 0) return path[1];
        return "";
    }
};
//执行用时：20 ms, 在所有 C++ 提交中击败了98.45% 的用户
//内存消耗：11.2 MB, 在所有 C++ 提交中击败了49.84% 的用户
```
实际上计算出度是完全不必要的，我们只需要知道哪个城市没有出边即可：
```cpp
//C++ version
class Solution {
public:
    string destCity(vector<vector<string>>& paths) {
        unordered_map<string, bool> rec; //记录每个点是否有出度
        for (const vector<string>& path : paths) rec[path[0]] = true;
        for (const vector<string>& path : paths) 
            if (rec[path[1]] == false) return path[1];
        return "";
    }
};
//执行用时：8 ms, 在所有 C++ 提交中击败了95.67% 的用户
//内存消耗：10.3 MB, 在所有 C++ 提交中击败了82.95% 的用户
```
---
### 解法2 模拟+哈希表
根据题意，取任何一个城市作为起点，然后使用 `paths` 中的路线信息开始搜索，直到当前城市无法到达下一个城市时，即是答案。为了快速找到某个城市所能到达的城市，可以使用哈希表对 `paths` 中的路线信息进行预处理。
```java
//Java version
class Solution {
    public String destCity(List<List<String>> paths) {
        Map<String, String> rec = new HashMap<>();
        for (List<String> path : paths) rec.put(path.get(0), path.get(1));
        String ans = paths.get(0).get(0);
        while (rec.containsKey(ans)) ans = rec.get(ans);
        return ans;
    }
}
//执行用时：2 ms, 在所有 Java 提交中击败了94.69% 的用户
//内存消耗：38 MB, 在所有 Java 提交中击败了81.70% 的用户
```
