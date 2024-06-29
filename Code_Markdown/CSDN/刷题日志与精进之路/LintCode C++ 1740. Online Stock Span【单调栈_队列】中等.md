Write a class `StockSpanner` which collects daily price quotes for some stock, and returns the **span** of that stock's price for the current day.

The span of the stock's price today is defined as the maximum number of **consecutive days** (starting from today and going backwards) for which the price of the stock was less than or equal to `today's price` .

For example, if the price of a stock over the next `7` days were `[100, 80, 60, 70, 60, 75, 85]`, then the stock spans would be `[1, 1, 1, 2, 1, 4, 6]` .

-    Calls to `StockSpanner.next(int price)` will have `1 <= price <= 10^5` .
-    There will be at most `10000` calls to `StockSpanner.next` per test case.
-    There will be at most `150000` calls to `StockSpanner.next` across all test cases.
-    The total time limit for this problem has been reduced by `75%` for C++, and `50%` for all other languages.

Have you met this question in a real interview?  

<b><font size="5px" color="#626262">Example</font></b> 
Example 1:
```c
Input: prices = [100,80,60,70,60,75,85]
Output: [1,1,1,2,1,4,6]
Explanation: 
First, S = StockSpanner() is initialized.  Then:
S.next(100) is called and returns 1,
S.next(80) is called and returns 1,
S.next(60) is called and returns 1,
S.next(70) is called and returns 2,
S.next(60) is called and returns 1,
S.next(75) is called and returns 4,
S.next(85) is called and returns 6.

Note that (for example) S.next(75) returned 4, because the last 4 prices
(including today's price of 75) were less than or equal to today's price.
```

Example 2:
```c
Input: prices = [50,80,80,70,90,75,85]
Output: [1,2,3,1,5,1,2]
Explanation: ：
First, S = StockSpanner() is initialized.  Then:
S.next(50) is called and returns 1,
S.next(80) is called and returns 3
S.next(70) is called and returns 1
S.next(90) is called and returns 5
S.next(75) is called and returns 1
S.next(85) is called and returns 2
```

题意：编写一个 `StockSpanner` 类，它的 `next(int price)` 方法返回给出的股票当日价格的 `span` 跨度。此跨度定义为：股票价格小于等于今日价格的最大连续天数，且从今天开始往回数，包括今天。

---
思路1：如果不是这样写的类，可以暴力双重循环，对一个价格数组中的每个价格 `price_i` ，从后往前数，如果前面的价格 `price_j` 小于等于今日价格，就加一。

思路2：由于这是一个在线问题，我们需要将输入的 `price` 存储起来。同时，对于这种找左边或者右边有多少个元素比当前元素大/小的问题，一般都是用单调栈/队列——要找左边有多少元素 `<=` 当前元素，所以用单调递减栈：
- 注意边界，第一个 `price` 时 `stack` 为空，所以特判一下；
- 后面进栈的 `price_i` ，持续和栈顶元素比较：
	- 如果栈顶元素的价格 `price_j` 更高，`price_i` 直接进栈，返回 `1` ；
	- 如果栈顶的元素价格 `price_j <= price_i` ，就加上这个栈顶中保存的 `<= price_j` 的元素数量，然后弹出栈顶元素，继续比较；
- 由于这是单调递减栈，小于等于栈顶元素 `price_j` 的天数数量要么是 `1` （即其自身），要么代表了一段连续的价格区间——它们都 `<= price_j` ，因此也 `<= price_i` 。弹出该栈顶元素后，新的栈顶元素 `> price_j` ，不会出现区间的重叠计算。

代码：
```cpp
class StockSpanner {
public:
    StockSpanner() { }
    /**
     * @param price: 
     * @return: int
     */
    stack<pair<int, int>> monotonicStack;
    int next(int price) {
        int ans = 1;
        while (!monotonicStack.empty() && monotonicStack.top().first <= price) {
            ans += monotonicStack.top().second;
            monotonicStack.pop();
        }
        monotonicStack.push(make_pair(price, ans));
        return ans;
    }
};
```

