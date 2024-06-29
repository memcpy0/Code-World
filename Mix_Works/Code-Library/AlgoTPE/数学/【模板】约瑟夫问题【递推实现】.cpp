#include <iostream> 
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    int p = 0;
    for (int i = 2; i <= n; ++i) {
        p = (p + m) % i;
    }
    cout << p + 1;
    return 0;
} 
