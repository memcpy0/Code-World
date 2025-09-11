A binary watch has 4 LEDs on the top which represent the **hours (0-11)**, and the 6 LEDs on the bottom represent the **minutes (0-59)**.

Each LED represents a zero or one, with the least significant bit on the right.
<img src="https://img-blog.csdnimg.cn/20201103234454584.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="50%">

For example, the above binary watch reads "3:25".

Given a non-negative integer n which represents the number of LEDs that are currently on, return all possible times the watch could represent.

**Example:**

```swift
Input: n = 1
Return: ["1:00", "2:00", "4:00", "8:00", "0:01", "0:02", "0:04", "0:08", "0:16", "0:32"]
```

**Note:**
- The order of output does not matter.
 - The hour must not contain a leading zero, for example "01:00" is not valid, it should be "1:00".
 - The minute must be consist of two digits and may contain a leading zero, for example "10:2" is not valid, it should be "10:02".

题意：有一块二进制手表，其中顶部的4个LED灯代表小时 `0 - 11` ，底部的6个LED灯代表分钟 `0 - 59` 。现在给出亮灯的数目 `n` ，返回手表可能表示的所有时间。

---
### 解法 二进制枚举+位操作
初一看还是个比较麻烦的题目，不过看到 `Each LED represents a zero or one` 时，很容易想到使用二进制枚举的方法——10位二进制数能表达的最大数据范围也就是 `1024` ，更何况一些状态还用不到——因为最大的时间是 `11:59` 或者说 `1011 111011` 。

于是具体代码如下，对每个二进制数中的置位 `set bit` 进行计数，如果置位位数等于 `n` 且**低六位不超过 $60$** (不可能出现几时60分的时间)，就使用 `getTime` 得到时间的字符串表示：
```cpp 
class Solution {
private:
    string getTime(int time) { //从整数中得到字符串时间
        int hours = (time >> 6) & 0xf, minutes = time & 0x3f; //取出高四位和低六位 
        return to_string(hours) + ":" + (minutes < 10 ? "0" : "") + to_string(minutes);
    }  //注意小时的表示没有leading zero
public:
    vector<string> readBinaryWatch(int num) {
        if (num == 0) return {"0:00"};
        vector<string> timeList;
        int total = 0x2fb;  //1011(0-11) 111011(0-59) //最大时间为11:59
        for (int i = 0; i <= total; ++i) {
            int setbits = 0, t = i;
            while (t) {
                t = t & (t - 1);
                ++setbits;
            }
            if (setbits == num && (i & 0x3f) < 60) timeList.push_back(getTime(i));  //分钟数最大为111011,即59分
        }
        return timeList;
    }
};
```
效率很高：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.7 MB, 在所有 C++ 提交中击败了22.93% 的用户
```
