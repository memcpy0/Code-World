A web developer needs to know how to design a web page's size. So, given a specific rectangular web page’s area, your job by now is to design a rectangular web page, whose length L and width W satisfy the following requirements:
1. The area of the rectangular web page you designed must equal to the given target area.
 2. The width `W` should not be larger than the length `L` , which means `L >= W` .
  3. The difference between length `L` and width `W` should be as small as possible.

*Return an array `[L, W]` where `L` and `W` are the length and width of the web page you designed in sequence.*

 

**Example 1:**

```swift
Input: area = 4
Output: [2,2]
Explanation: The target area is 4, and all the possible ways to construct it are [1,4], [2,2], [4,1]. 
But according to requirement 2, [1,4] is illegal; according to requirement 3,  [4,1] is not optimal compared to [2,2]. So the length L is 2, and the width W is 2.
```

**Example 2:**

```swift
Input: area = 37
Output: [37,1]
```

**Example 3:**

```swift
Input: area = 122122
Output: [427,286]
```

 

**Constraints:** <code>1 <= area <= 10<sup>7</sup></code>

题意：作为一位web开发者， 懂得怎样去规划一个页面的尺寸是很重要的。 现给定一个具体的矩形页面面积，设计一个长度为 `L` 、宽度为 `W` 且满足以下要求的矩形页面。要求：
1. 设计的矩形页面必须等于给定的目标面积。 
2. 宽度 `W` 不应大于长度 `L` 。 
3. 长度 `L` 和宽度 `W` 之间的差距应当尽可能小。

按照顺序输出页面的长度和宽度。

----
### 解法1 数学
由于 `W <= L, W * L = area` ，所以 `W <= sqrt(area)` ；且要求长度和宽度的差距尽可能小，因此从 `sqrt(area)` 往小搜索 `W` ：
```cpp
class Solution {
public:
    vector<int> constructRectangle(int area) {
        if (area == 1) return {1, 1};
        int sqr = sqrt(area);
        for (int w = sqr; w >= 1; --w) {
            int l = area / w;
            if (l * w == area) return {l, w};
        }
        return {area, 1};
    }
};
```
因为既做除法，又做乘法，所以效率不高：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了50.54% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了13.05% 的用户
```
因此写成下述形式：
```cpp
class Solution {
public:
    vector<int> constructRectangle(int area) { 
        int sqr = sqrt(area), w;
        for (w = sqr; w >= 1; --w) 
        	if (area % w == 0) break;
        return {area / w, w};
    }
};
```
 效率如下，相当优秀：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.5 MB, 在所有 C++ 提交中击败了5.03% 的用户
```
---
### 解法2 双指针
同样的道理，只是这里同时使用了 `w = l = sqrt(area)` ，分别往两边搜索：
```cpp
class Solution {
public:
    vector<int> constructRectangle(int area) { 
        int l, w;
        l = w = sqrt(area);
        while (true) {
            int t = w * l;
            if (t == area) break;
            else if (t > area) --w;
            else ++l;
        }
        return {l, w};
    }
};
```
执行效率低一些：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了30.91% 的用户
内存消耗：6.4 MB, 在所有 C++ 提交中击败了7.55% 的用户
```
