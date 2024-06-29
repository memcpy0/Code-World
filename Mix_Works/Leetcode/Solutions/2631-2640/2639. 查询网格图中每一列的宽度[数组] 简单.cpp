/*
** Problem: https://leetcode.cn/problems/find-the-width-of-columns-of-a-grid
** Article: https://memcpy0.blog.csdn.net/article/details/138252831
** Author: memcpy0
*/
```python
class Solution:
    def findColumnWidth(self, grid: List[List[int]]) -> List[int]:
        return [max(len(str(x)) for x in col) 
            for col in zip(*grid)]
```
```java
class Solution {
    public int[] findColumnWidth(int[][] grid) {
        int n = grid[0].length;
        int[] ans = new int[n];
        for (int j = 0; j < n; ++j) 
            for (int[] row : grid) 
                ans[j] = Math.max(ans[j], Integer.toString(row[j]).length());
        return ans;
    }
}
```
```cpp
class Solution {
public:
    vector<int> findColumnWidth(vector<vector<int>>& grid) {
        int n = grid[0].size();
        vector<int> ans(n);
        for (int j = 0; j < n; ++j)
            for (auto& row : grid)
                ans[j] = max(ans[j], (int) to_string(row[j]).length());
        return ans;
    }
};
```
也可以手动计算长度。
```python
class Solution:
    def findColumnWidth(self, grid: List[List[int]]) -> List[int]:
        ans = [0] * len(grid[0])
        for j, col in enumerate(zip(*grid)):
            for x in col:
                x_len = int(x <= 0)
                x = abs(x)
                while x:
                    x_len += 1
                    x //= 10
                ans[j] = max(ans[j], x_len)
        return ans
```
```java
class Solution {
    public int[] findColumnWidth(int[][] grid) {
        int n = grid[0].length;
        int[] ans = new int[n];
        for (int j = 0; j < n; ++j) {
            for (int[] row : grid) {
                int len = row[j] <= 0 ? 1 : 0;
                for (int x = row[j]; x != 0; x /= 10) ++len;
                ans[j] = Math.max(ans[j], len);
            }
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    vector<int> findColumnWidth(vector<vector<int>>& grid) {
        int n = grid[0].size();
        vector<int> ans(n);
        for (int j = 0; j < n; ++j) {
            for (auto& row : grid) {
                int len = row[j] <= 0;
                for (int x = row[j]; x; x /= 10) ++len;
                ans[j] = max(ans[j], len);
            }
        }
        return ans;
    }
};
```
### 优化
```python
class Solution:
    def findColumnWidth(self, grid: List[List[int]]) -> List[int]:
        return [len(str(max(max(col), -10 * min(col))))
            for col in zip(*grid)]
```
```java
class Solution {
    public int[] findColumnWidth(int[][] grid) {
        int n = grid[0].length;
        int[] ans = new int[n];
        for (int j = 0; j < n; j++) {
            int mn = 0;
            int mx = 0;
            for (int[] row : grid) {
                mn = Math.min(mn, row[j]);
                mx = Math.max(mx, row[j]);
            }
            int len = 1;
            for (int x = Math.max(mx / 10, -mn); x > 0; x /= 10) {
                len++;
            }
            ans[j] = len;
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    vector<int> findColumnWidth(vector<vector<int>>& grid) {
        int n = grid[0].size();
        vector<int> ans(n);
        for (int j = 0; j < n; j++) {
            int mn = 0, mx = 0;
            for (auto& row : grid) {
                mn = min(mn, row[j]);
                mx = max(mx, row[j]);
            }
            int len = 1;
            for (int x = max(mx / 10, -mn); x; x /= 10) {
                len++;
            }
            ans[j] = len;
        }
        return ans;
    }
}
```