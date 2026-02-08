Calculate $a+b$ and output the sum in standard format -- that is, the digits must be separated into groups of three by commas (unless there are less than four digits).
### Input Specification:

Each input file contains one test case. Each case contains a pair of integers a and b where $-10^6​ ≤ a,b≤ 10^6$ ​​. The numbers are separated by a space.
### Output Specification:
For each test case, you should output the sum of $a$ and $b$ in one line. The sum must be written in the standard format.

### Sample Input:

```clike
-1000000 9
```

### Sample Output:

```clike
-999,991
```

---
```cpp
#include <bits/stdc++.h>
using namespace std;
 
int main() {
    int a, b, c;
    scanf("%d%d", &a, &b);
    c = a + b;
    if (c == 0) { printf("0"); return 0; } //0
    if (c < 0) { printf("-"); c = -c; }	   //负数
    string t = to_string(c);
    if (t.size() <= 3) { cout << t; return 0; } //<=3的数字
    for (int i = 0; i < t.size(); ++i) {
        if (i != 0 && (t.size() - i) % 3 == 0) printf(",");
        printf("%c", t[i]);
    }
    return 0;
}
```

