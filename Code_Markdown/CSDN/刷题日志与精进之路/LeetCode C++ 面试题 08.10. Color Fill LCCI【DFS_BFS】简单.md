

Implement the "paint fill" function that one might see on many image editing programs. That is, given a screen (represented by a two-dimensional array of colors), a point, and a new color, fill in the surrounding area until the color changes from the original color.

**Example1:**

```swift
Input: 
image = [[1,1,1],[1,1,0],[1,0,1]] 
sr = 1, sc = 1, newColor = 2
Output: [[2,2,2],[2,2,0],[2,0,1]]
Explanation: 
From the center of the image (with position (sr, sc) = (1, 1)), all pixels connected 
by a path of the same color as the starting pixel are colored with the new color.
Note the bottom corner is not colored 2, because it is not 4-directionally connected
to the starting pixel.
```

**Note:**
-  The length of `image` and `image[0]` will be in the range `[1, 50]`.
 - The given starting pixel will satisfy `0 <= sr < image.length` and `0 <= sc < image[0].length`.
- The value of each color in `image[i][j]` and `newColor` will be an integer in `[0, 65535]`.

题意：编写函数实现许多图片编辑软件都支持的**颜色填充**功能。待填充的图像用二维数组 `image` 表示，元素为初始颜色值。初始坐标的横坐标为 `sr` 、纵坐标为 `sc`，需要填充的新颜色为 `newColor` 。用新颜色填充初始坐标点的周围区域(颜色相同且在上、下、左、右四个方向上相邻的若干元素），并返回填充后的图像。

---
### 解法1 DFS 
```cpp
class Solution {
private:
    int n, m;
    int moves[4][2] = {0, -1, 0, 1, -1, 0, 1, 0};
    void dfs(vector<vector<int>>& image, int r, int c, int oldColor, int newColor) {
        image[r][c] = newColor; //填充新颜色 
        for (int i = 0; i < 4; ++i) {
            int tr = r + moves[i][0], tc = c + moves[i][1]; //避免越界和重复访问
            if (tr >= 0 && tr < n && tc >= 0 && tc < m && image[tr][tc] == oldColor) { 
                dfs(image, tr, tc, oldColor, newColor);
            }
        }
    }
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
        int oldColor = image[sr][sc];
        if (oldColor == newColor) return image;
        n = image.size(), m = image[0].size();
        dfs(image, sr, sc, oldColor, newColor);
        return image;
    }
};
```
提交后效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了98.96% 的用户
内存消耗：13.6 MB, 在所有 C++ 提交中击败了37.33% 的用户
```
用Java写的DFS代码：
```java
class Solution {
    public int[][] floodFill(int[][] image, int sr, int sc, int newColor) {
        fill(image, sr, sc, image[sr][sc], newColor);
        return image;
    }
    private void fill(int[][] image, int r, int c, int oldColor, int newColor) {
        if (r < 0 || r >= image.length || c < 0 || c >= image[0].length || image[r][c] != oldColor || image[r][c] == newColor) return;
        image[r][c] = newColor;
        fill(image, r + 1, c, oldColor, newColor);
        fill(image, r - 1, c, oldColor, newColor);
        fill(image, r, c + 1, oldColor, newColor);
        fill(image, r, c - 1, oldColor, newColor);
    }
}  
```
运行效率不用说：
```cpp
执行用时：1 ms, 在所有 Java 提交中击败了94.68% 的用户
内存消耗：39.4 MB, 在所有 Java 提交中击败了84.78% 的用户
```


---
### 解法2 BFS 
用BFS也可以做出这道题：
```cpp
class Solution { 
private:
    using cord = pair<int, int>;
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
        int oldColor = image[sr][sc];
        if (oldColor == newColor) return image;
        int n = image.size(), m = image[0].size();
        int moves[4][2] = {0, -1, 0, 1, -1, 0, 1, 0}; 
        queue<cord> q;
        image[sr][sc] = newColor;
        q.push(cord{sr, sc});
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                cord t = q.front(); q.pop();
                for (int j = 0; j < 4; ++j) {
                    int tr = t.first + moves[j][0], tc = t.second + moves[j][1];
                    if (tr >= 0 && tr < n && tc >= 0 && tc < m && image[tr][tc] == oldColor) { 
                        image[tr][tc] = newColor;
                        q.push(cord{tr, tc});
                    }
                }
            }
        }
        return image;
    }
};
```
提交后运行结果如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了91.04% 的用户
内存消耗：14 MB, 在所有 C++ 提交中击败了6.33% 的用户
```
