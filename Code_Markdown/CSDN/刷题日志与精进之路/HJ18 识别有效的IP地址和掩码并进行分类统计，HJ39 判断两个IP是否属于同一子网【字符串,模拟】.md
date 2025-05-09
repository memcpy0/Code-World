### 描述
请解析IP地址和对应的掩码，进行分类识别。要求按照A/B/C/D/E类地址归类，不合法的地址和掩码单独归类。
所有的IP地址划分为 A,B,C,D,E五类
A类地址从1.0.0.0到126.255.255.255;
B类地址从128.0.0.0到191.255.255.255;
C类地址从192.0.0.0到223.255.255.255;
D类地址从224.0.0.0到239.255.255.255；
E类地址从240.0.0.0到255.255.255.255
  
私网IP范围是：
从10.0.0.0到10.255.255.255
从172.16.0.0到172.31.255.255
从192.168.0.0到192.168.255.255

子网掩码为二进制下前面是连续的1，然后全是0。（例如：255.255.255.32就是一个非法的掩码）
（注意二进制下全是1或者全是0均为非法子网掩码）

注意：
1. 类似于`【0.*.*.*】`和`【127.*.*.*】`的IP地址不属于上述输入的任意一类，也**不属于不合法ip地址**，计数时**请忽略**
2. 私有IP地址和A,B,C,D,E类地址是不冲突的  

### 输入描述：
多行字符串。每行一个IP地址和掩码，用~隔开。请参考帖子https://www.nowcoder.com/discuss/276处理循环输入的问题。

### 输出描述：
统计A、B、C、D、E、错误IP地址或错误掩码、私有IP的个数，之间以空格隔开。

### 示例1
```java
输入：
10.70.44.68~255.254.255.0
1.0.0.1~255.0.0.0
192.168.0.2~255.255.255.0
19..0.~255.255.255.0
 
输出：
1 0 1 0 0 2 1

说明：
10.70.44.68~255.254.255.0的子网掩码非法，19..0.~255.255.255.0的IP地址非法，所以错误IP地址或错误掩码的计数为2；
1.0.0.1~255.0.0.0是无误的A类地址；
192.168.0.2~255.255.255.0是无误的C类地址且是私有IP；
所以最终的结果为1 0 1 0 0 2 1     
``` 
### 示例2
```java
输入：
0.201.56.50~255.255.111.255
127.201.56.50~255.255.111.255
 
输出：
0 0 0 0 0 0 0

说明：
类似于【0.*.*.*】和【127.*.*.*】的IP地址不属于上述输入的任意一类，也不属于不合法ip地址，计数时请忽略
```
---
### 解法 字符串+模拟
这道题需要详细整理思路。**输出目标**：统计A、B、C、D、E、**错误IP地址**、**错误掩码**、私有IP的个数。

需要注意的细节：
1.  类似于`【0.*.*.*】`和`【127.*.*.*】`的IP地址不属于上述输入的任意一类，也不属于不合法ip地址，计数时可以忽略
2.  **私有IP地址和A,B,C,D,E类地址是不冲突的，也就是说需要同时+1**
3.  ==如果子网掩码是非法的，则不再需要查看IP地址==
4.  全零【0.0.0.0】或者全一【255.255.255.255】的==子网掩码也是非法的==

因此思路如下：
1.  按行读取输入，根据字符 `~` 将IP地址与子网掩码分开
2.  查看子网掩码是否合法。
    - 合法，则继续检查IP地址
    - 非法，则相应统计项+1，继续下一行的读入
3.  查看IP地址是否合法
    - 合法，查看IP地址属于哪一类，是否是私有ip地址；相应统计项+1
    - 非法，相应统计项+1

具体来说：
1.  判断IP地址是否合法，如果满足下列条件之一即为非法地址
    - **数字段数不为4**
    - **存在空段**，即【192..1.0】这种
    - **某个段的数字**大于255
2.  判断子网掩码是否合法，如果满足下列条件之一即为非法掩码
    - **不是一个合格的IP地址**
    - 在二进制下，不满足前面连续是1，然后全是0
    - 在二进制下，全为0或全为1
3.  如何判断一个掩码地址是不是满足前面连续是1，然后全是0？
    - **将掩码地址转换为32位无符号整型**，假设这个数为b。**如果此时b为0，则为非法掩码**
    - **将b按位取反后+1**。如果此时b为1，则b原来是二进制全1，非法掩码
    - ==如果b和b-1做按位与运算后为0，则说明是合法掩码，否则为非法掩码==
    - 特别地，对于 `127.*.*.*` 这种不属于任一类的需要特判。

代码如下所示：
```cpp
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
using namespace std;
bool judgeIP(string ip) {
    int j = 0;
    istringstream iss(ip);
    string seg;
    while (getline(iss, seg, '.'))
        if (++j > 4 || seg.empty() || stoi(seg) > 255)  
            return false;
    return j == 4; // 四段，都非空，都<=255
}

bool isPrivate(string ip) {
    istringstream iss(ip);
    string seg;
    vector<int> v;
    while (getline(iss, seg, '.')) v.push_back(stoi(seg));
    if (v[0] == 10) return true;
    if (v[0] == 172 && (v[1] >= 16 && v[1] <= 31)) return true;
    if (v[0] == 192 && v[1] == 168) return true;
    return false;
}

bool isMask(string ip) {
    istringstream iss(ip);
    string seg;
    unsigned b = 0;
    while (getline(iss, seg, '.')) b = (b << 8) + stoi(seg);
    if (!b) return false; // 全0
    b = ~b + 1;
    if (b == 1) return false; // 全1
    if ((b & (b - 1)) == 0) return true; // 只有1个1
    return false;
}

int main() {
    string input;
    int a = 0, b = 0, c = 0, d = 0, e = 0, err = 0, p = 0;
    while (cin >> input) {
        istringstream is(input);
        string add;
        vector<string> v;
        while (getline(is, add, '~')) v.push_back(add);
        int first = stoi(v[0].substr(0, v[0].find_first_of('.')));
        if (first != 127 && (!judgeIP(v[1]) || !isMask(v[1]))) err++; // 掩码不是合法IP或掩码错误
        else {
            if (!judgeIP(v[0])) err++; // 不是合法IP
            else {
                if (isPrivate(v[0])) ++p; // 是私有地址
                if (first > 0 && first < 127) ++a;
                else if (first > 127 && first < 192) ++b;
                else if (first > 191 && first < 224) ++c;
                else if (first > 223 && first < 240) ++d;
                else if (first > 239 && first < 256) ++e;
            }
        }
    }
    cout << a << " " << b << " " << c << " " << d << " " << e << " " << err << " " << p << endl;
    return 0;
}
```

---
### 描述
IP地址是由4个0-255之间的整数构成的，用"."符号相连。二进制的IP地址格式有32位，例如：10000011，01101011，00000011，00011000;每八位用十进制表示就是131.107.3.24

子网掩码是用来判断任意两台计算机的IP地址是否属于同一子网络的根据。子网掩码与IP地址结构相同，是32位二进制数，由1和0组成，且1和0分别连续，其中网络号部分全为“1”和主机号部分全为“0”。

你可以简单的认为子网掩码是一串连续的1和一串连续的0拼接而成的32位二进制数，左边部分都是1，右边部分都是0。

利用子网掩码可以判断两台主机是否在同一子网中。若两台主机的IP地址分别与它们的子网掩码进行逻辑“与”运算（按位与/AND）后的结果相同，则说明这两台主机在同一子网中。

示例：  
IP 地址　 192.168.0.1  
子网掩码　 255.255.255.0
转化为二进制进行运算：
IP 地址　  11000000.10101000.00000000.00000001  
子网掩码　11111111.11111111.11111111.00000000
AND运算   11000000.10101000.00000000.00000000
转化为十进制后为：  
192.168.0.0

IP 地址　 192.168.0.254  
子网掩码　 255.255.255.0
转化为二进制进行运算：
IP 地址　11000000.10101000.00000000.11111110  
子网掩码  11111111.11111111.11111111.00000000
AND运算  11000000.10101000.00000000.00000000
转化为十进制后为：  
192.168.0.0

通过以上对两台计算机IP地址与子网掩码的AND运算后，我们可以看到它运算结果是一样的。均为192.168.0.0，所以这二台计算机可视为是同一子网络。

输入一个子网掩码以及两个ip地址，判断这两个ip地址是否是一个子网络。若IP地址或子网掩码格式非法则输出1，若IP1与IP2属于同一子网络输出0，若IP1与IP2不属于同一子网络输出2。  

注:有效掩码与IP的性质为：  
1. 掩码与IP每一段在 0 - 255 之间  
2. 掩码的二进制字符串前缀为网络号，都由‘1’组成；后缀为主机号，都由'0'组成

### 输入描述：
3行输入，第1行是输入子网掩码、第2，3行是输入两个ip地址  
题目的示例中给出了三组数据，但是在实际提交时，你的程序可以只处理一组数据（3行）。

### 输出描述：
若IP地址或子网掩码格式非法则输出1，若IP1与IP2属于同一子网络输出0，若IP1与IP2不属于同一子网络输出2

### 示例1
输入：

```
255.255.255.0
192.168.224.256
192.168.10.4
255.0.0.0
193.194.202.15
232.43.7.59
255.255.255.0
192.168.0.254
192.168.0.1
```
 输出：
```java
1
2
0
```
说明：
对于第一个例子:
255.255.255.0
192.168.224.256
192.168.10.4
其中IP:192.168.224.256不合法，输出1

对于第二个例子:
255.0.0.0
193.194.202.15
232.43.7.59
2个与运算之后，不在同一个子网，输出2

对于第三个例子，2个与运算之后，如题目描述所示，在同一个子网，输出0。

### 解法 字符串+模拟
这里只需要先判断掩码和IP是否合法，合法的话，将掩码和IP分别转为四个整数，分别按位与、并将位运算结果进行比较即可。
```java
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;
string mask, ip1, ip2;
bool judgeIP(string ip) {
    istringstream iss(ip);
    vector<int> add;
    string seg;
    int j = 0;
    while (getline(iss, seg, '.')) {
        int t = stoi(seg);
        if (++j > 4 || seg.empty() || t > 255 || t < 0)
            return false;
    }
    return true;
}
bool isMask(string mask) {
    istringstream iss(mask);
    string seg;
    unsigned b;
    while (getline(iss, seg, '.')) b = (b << 8) + (stoi(seg));
    if (b == 0 || ~b == 0) return false;
    b = ~b + 1;
    if ((b & (b - 1)) == 0) return true;
    return false;
}
vector<int> toInt(string ip) {
    istringstream iss(ip);
    string seg;
    vector<int> i;
    while (getline(iss, seg, '.')) i.push_back(stoi(seg));
    return i;
}
void maskProc(vector<int>& i, vector<int>& m) {
    i[0] &= m[0], i[1] &= m[1], i[2] &= m[2], i[3] &= m[3];
}
int main() {
    while (cin >> mask >> ip1 >> ip2) {
        if (!judgeIP(mask) || !isMask(mask) || !judgeIP(ip1) || !judgeIP(ip2)) { // 不合法
            cout << 1 << endl;
            continue;
        }
        vector<int> &&i1 = toInt(ip1);
        vector<int> &&i2 = toInt(ip2);
        vector<int> &&m = toInt(mask);
        maskProc(i1, m); maskProc(i2, m);
        if (i1 == i2) printf("0\n");
        else printf("2\n");
    }
    return 0; 
}
```