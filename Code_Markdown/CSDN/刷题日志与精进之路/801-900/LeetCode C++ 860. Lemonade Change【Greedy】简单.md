At a lemonade stand, each lemonade costs `$5`. 

Customers are standing in a queue to buy from you, and order one at a time (in the order specified by `bills`).

Each customer will only buy one lemonade and pay with either a `$5`, `$10`, or `$20` bill.  You must provide the correct change to each customer, so that the net transaction is that the customer pays `$5`.

Note that you don't have any change in hand at first.

Return `true` if and only if you can provide every customer with correct change.

 

**Example 1:**

```swift
Input: [5,5,5,10,20]
Output: true
Explanation: 
From the first 3 customers, we collect three $5 bills in order.
From the fourth customer, we collect a $10 bill and give back a $5.
From the fifth customer, we give a $10 bill and a $5 bill.
Since all customers got correct change, we output true.
```

**Example 2:**

```swift
Input: [5,5,10]
Output: true
```

**Example 3:**

```swift
Input: [10,10]
Output: false
```

**Example 4:**

```swift
Input: [5,5,10,10,20]
Output: false
Explanation: 
From the first two customers in order, we collect two $5 bills.
For the next two customers in order, we collect a $10 bill and give back a $5 bill.
For the last customer, we can't give change of $15 back because we only have two $10 bills.
Since not every customer received correct change, the answer is false.
```

 

**Note:**
- `0 <= bills.length <= 10000`
- `bills[i]` will be either `5`, `10`, or `20`.





题意：在柠檬水摊上，每一杯柠檬水的售价为 `5` 美元。顾客排队购买柠檬水，按账单支付的顺序一次购买一杯，支付 `5` 美元、`10` 美元或 `20` 美元。一开始手头没有任何零钱，如果能给每位顾客正确找零，返回 `true` ，否则返回 `false` 。

---
### 解法 贪心算法
按照顺序找零，5美元不需要找零，10美元只需要找一张5美元的零钱，20美元找零有两种情况：找一张5美元和一张10美元，或者找三张5美元的零钱。
```cpp
class Solution {
public:
    bool lemonadeChange(vector<int>& bills) {
        int money[3] = {0, 0, 0}; //5, 10, 20$的数量
        for (const int &v : bills) {
            switch (v) {
                case 5: 
                	++money[0]; //5$数量+1
                	break; 
                case 10: 
                    ++money[1]; //10$数量+1
                    if (money[0] > 0) --money[0]; //5$数量-1
                    else return false; //无法找零
                    break;
                case 20: 
                    ++money[2]; //20$数量+1
                    if (money[1] >= 1 && money[0] >= 1) --money[1], --money[0];
                    else if (money[0] >= 3) money[0] -= 3;
                    else return false;
                    break;
            }
        }
        return true;
    }
};
```
效率如下：
```cpp
执行用时：48 ms, 在所有 C++ 提交中击败了15.79% 的用户
内存消耗：16.3 MB, 在所有 C++ 提交中击败了5.11% 的用户
```
我们发现，找零时20美元不会被使用，因此可以不记录；无论顾客花费多少钱买柠檬水，我们要想顺利找零，手中**必须**拥有5美元的零钱，因此返回 `false` 的条件是 `five < 0` ：
```cpp
class Solution {
public:
    bool lemonadeChange(vector<int>& bills) {
        int five = 0, ten = 0;
        for (const int &v : bills) {
            if (v == 5) ++five;
            else if (v == 10) { ++ten; --five; }
            else if (v == 20) {
                if (ten > 0) { --ten; --five; } //如果存在10$时,先用10$
                else five -= 3; //否则使用3张5$
            }    
            if (five < 0) return false; 
        }
        return true;
    }
};
```
效率还是这么低：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了11.48% 的用户
内存消耗：16.3 MB, 在所有 C++ 提交中击败了5.27% 的用户
```
---
**20201210 Update** 今日打卡更新Java代码：
```java
class Solution {
    public boolean lemonadeChange(int[] bills) {
        int five = 0, ten = 0;
        for (int v : bills) {
            if (v == 5) ++five;
            else if (v == 10) { --five; ++ten; } //找5$
            else {
                if (ten > 0) { --five; --ten; } //找15$
                else five -= 3;
            }
            if (five < 0) return false;
        }
        return true;
    }
}
```
Java的运行速度就不一样了：
```java
执行用时：2 ms, 在所有 Java 提交中击败了99.72% 的用户
内存消耗：39.5 MB, 在所有 Java 提交中击败了64.12% 的用户
```
