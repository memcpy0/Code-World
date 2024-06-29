An `image` is represented by a `2-D` array of integers, each integer representing the pixel value of the image (from `0` to `65535`).

Given a coordinate `(sr, sc)` representing the starting pixel (row and column) of the flood fill, and a pixel value `newColor` , "flood fill" the image.

To perform a "flood fill", consider the starting pixel, plus any pixels connected 4-directionally to the starting pixel of the same color as the starting pixel, plus any pixels connected 4-directionally to those pixels (also with the same color as the starting pixel), and so on. Replace the color of all of the aforementioned pixels with the newColor.

At the end, return the modified image.

**Example 1:**
```cpp
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
- The length of `image` and `image[0]` will be in the range `[1, 50]` .
- The given starting pixel will satisfy `0 <= sr < image.length` and `0 <= sc < image[0].length` .
- The value of each color in `image[i][j]` and `newColor` will be an integer in `[0, 65535]` .

题意：给出一幅以二维整数数组表示的图画，每一个整数表示该图画的像素值大小，数值在 `0` 到 `65535` 之间。然后给出一个坐标 `(sr, sc)` 表示图像渲染开始的像素值 `(行, 列)` 和一个新的颜色值 `newColor` 。接着重新上色这幅图像，从初始坐标开始，记录其上下左右四个方向上**像素值与初始坐标相同**的**相连像素点**，再记录**这四个方向上符合条件的像素点**与它们对应四个方向上**像素值与初始坐标相同**的**相连像素点**，……，重复该过程。将所有被记录的像素点的颜色值改为新的颜色值。最后返回经过上色渲染后的图像。

---
思路：DFS，求出坐标所在的连通分量，对这个连通分量上的所有像素点涂上相同的颜色。

代码：
```cpp
class Solution {
public:
    vector<vector<bool>> visited;
    vector<vector<int>> Move;
    int oldColor;
    void dfs(vector<vector<int>>& im, int sr, int sc, int nc) {
        visited[sr][sc] = true;
        im[sr][sc] = nc;
        for (int i = 0; i < 4; ++i) {
            int tr = sr + Move[i][0], tc = sc + Move[i][1];
            if (tr >= 0 && tr < im.size() && tc >= 0 && tc < im[0].size() && !visited[tr][tc] && im[tr][tc] == oldColor)
                dfs(im, tr, tc, nc);
        }
    }

    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
        if (image.empty() || image[0].empty()) return image;
        oldColor = image[sr][sc];
        if (newColor == oldColor) return image;
        
        int m = image.size(), n = image[0].size();
        Move = {{-1,  0}, {1, 0}, {0, -1}, {0, 1}};
        visited.resize(m);
        for (int i = 0; i < m; ++i) visited[i].resize(n);
        dfs(image, sr, sc, newColor);
        return image;
    }
};
```
还可以写成下面这样，时间效率高一些，空间效率更高，不使用额外的空间：
```cpp
class Solution {
public:
    void dfs(vector<vector<int>>& im, int r, int c, int newColor, int oldColor) {
        if (r < 0 || r >= im.size() || c < 0 || c >= im[0].size() || im[r][c] == newColor || im[r][c] != oldColor)
            return;
        im[r][c] = newColor;
        dfs(im, r + 1, c, newColor, oldColor);
        dfs(im, r, c + 1, newColor, oldColor);
        dfs(im, r - 1, c, newColor, oldColor);
        dfs(im, r, c - 1, newColor, oldColor);
    }

    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
        if (image.empty() || image[0].empty()) return image;
        dfs(image, sr, sc, newColor, image[sr][sc]);
        return image;
    }
};
```
效率：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了91.99% 的用户
内存消耗：13.6 MB, 在所有 C++ 提交中击败了90.72% 的用户
```
