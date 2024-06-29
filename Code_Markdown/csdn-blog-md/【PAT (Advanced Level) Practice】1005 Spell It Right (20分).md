Given a non-negative integer $N$, your task is to compute the sum of all the digits of $N$, and output every digit of the sum in English.
### Input Specification:

Each input file contains one test case. Each case occupies one line which contains an $N (≤10^{​100}$​​).
### Output Specification:

For each test case, output in one line the digits of the sum in English words. There must be one space between two consecutive words, but no extra space at the end of a line.
### Sample Input:

```cpp
12345
```

### Sample Output:

```cpp
one five
```

以前写的代码如下：
```cpp
#include <bits/stdc++.h>
char number[][10] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}; 

int main() {
    char r[110], s[110]; 
    scanf("%s", r);
    int sum = 0;
    for (int i = 0; r[i]; i++) 
        sum += (r[i] - '0');
    sprintf(s, "%d", sum);
    printf("%s", number[s[0] - '0']);
    for (int i = 1; s[i]; i++) printf(" %s", number[s[i] - '0']);
    return 0;
}
```
现在写的代码：
```cpp
#include <bits/stdc++.h>
using namespace std;
char dict[10][6] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
int main() {
    char str[110];
    scanf("%s", str);
    int sum = 0;
    for (int i = 0; str[i]; ++i) sum += (str[i] - '0');
    string t = to_string(sum);
    for (int i = 0; i < t.size(); ++i) 
        printf(" %s" + !i, dict[t[i] - '0']);
    return 0;
}
```
