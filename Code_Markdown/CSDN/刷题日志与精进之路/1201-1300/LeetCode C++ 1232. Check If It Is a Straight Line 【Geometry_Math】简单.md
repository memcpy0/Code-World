You are given an array coordinates, `coordinates[i] = [x, y]`, where `[x, y] ` represents the coordinate of a point. Check if these points **make a straight line** in the `XY` plane.

 Example 1:
 <img src="https://img-blog.csdnimg.cn/20200628144558469.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">
```cpp
Input: coordinates = [[1,2],[2,3],[3,4],[4,5],[5,6],[6,7]]
Output: true
```

**Example 2:**
<img src="https://img-blog.csdnimg.cn/20200628144605647.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">
```cpp
Input: coordinates = [[1,1],[2,2],[3,4],[4,5],[5,6],[7,7]]
Output: false
```
**Constraints**:

-    `2 <= coordinates.length <= 1000
    coordinates[i].length == 2`
-    `-10^4 <= coordinates[i][0], coordinates[i][1] <= 10^4`
-    coordinates contains **no duplicate point**.

题意：判断给出的这些坐标点是否形成一条直线。

---
### 解法 求斜率
题目中说明没有重复点；计算斜率可能出现除以零的情况，将斜率公式变形即可。代码如下：
```cpp
class Solution {
public:
    bool checkStraightLine(vector<vector<int>>& coordinates) {
        //   (y1-y0)/(x1-x0) = (yi-y0)/(xi-x0)
        //-> (y1-y0)*(xi-x0) = (yi-y0)*(x1-x0)
        //循环检查其他点是否处于第一个和第二个点形成的直线上
        auto &c0 = coordinates[0], &c1 = coordinates[1];
        for (int i = 2; i < coordinates.size(); ++i) {
            auto &c2 = coordinates[i];
            if ((c1[1] - c0[1])*(c2[0] - c0[0]) != (c2[1] - c0[1])*(c1[0] - c0[0]))
                return false;
        }
        return true;
    }
};
```
效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了76.42% 的用户
内存消耗：10.3 MB, 在所有 C++ 提交中击败了100.00% 的用户
```

