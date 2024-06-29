At the beginning of every day, the first person who signs in the computer room will unlock the door, and the last one who signs out will lock the door. Given the records of signing in's and out's, you are supposed to find the ones who have unlocked and locked the door on that day.
### Input Specification:

Each input file contains one test case. Each case contains the records for one day. The case starts with a positive integer $M$, which is the total number of records, followed by $M$ lines, each in the format:

```swift
ID_number Sign_in_time Sign_out_time
```

where times are given in the format `HH:MM:SS`, and `ID_number` is a string with no more than 15 characters.
### Output Specification:

For each test case, output in one line the ID numbers of the persons who have unlocked and locked the door on that day. The two ID numbers must be separated by one space.

Note: It is guaranteed that the records are consistent. That is, the sign in time must be earlier than the sign out time for each person, and there are no two persons sign in or out at the same moment.
### Sample Input:

```swift
3
CS301111 15:30:28 17:00:10
SC3021234 08:00:00 11:25:25
CS301133 21:45:00 21:58:40
```

### Sample Output:
```swift
SC3021234 CS301133
```

现在写的代码：
```cpp
#include <bits/stdc++.h>
using namespace std;
struct record { string name, signin, signout; };
int main() {
    int m;
    scanf("%d", &m);
    char id[17], et[10], lt[10];
    vector<record> vi;
    for (int i = 0; i < m; ++i) {
        scanf("%s%s%s", id, et, lt);
        vi.push_back(record{id, et, lt});
    }
    sort(vi.begin(), vi.end(), [&](const record &a, const record &b) { return a.signin < b.signin; });
    cout << vi[0].name;
    cout << " ";
    sort(vi.begin(), vi.end(), [&](const record &a, const record &b) { return a.signout > b.signout; });
    cout << vi[0].name;
    return 0;
}
```
以前写的代码反而更加简单一些：
```cpp
#include <bits/stdc++.h>
using namespace std;
struct person {
    string name, inTime, outTime;
};

int main() {
    int m, e = 0, l = 0;
    scanf("%d", &m);
    person p[m];
    for (int i = 0; i < m; i++) {
        cin >> p[i].name >> p[i].inTime >> p[i].outTime;
        if (p[i].inTime < p[e].inTime) e = i;
        if (p[i].outTime > p[l].outTime) l = i;
    }
    cout << p[e].name << " " << p[l].name << endl;
    return 0;
}
```

