#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long reverseAlignments = 0;
    void mergeSortAndCount(vector<int> &data, int l, int r, vector<int> &temp) {
        if (l >= r) return;
        int mid = l + (r - l) / 2, k = l, i = l, j = mid + 1;
        mergeSortAndCount(data, l, mid, temp);
        mergeSortAndCount(data, mid + 1, r, temp);
        while (i <= mid && j <= r) {
            if (data[i] <= data[j]) temp[k++] = data[i++];
            else if (data[i] > data[j]) {
                reverseAlignments += (mid - i + 1); //(j - k); 
                temp[k++] = data[j++];
            }
        }
        while (i <= mid) temp[k++] = data[i++];
        while (j <= r) temp[k++] = data[j++];
        for (int x = l; x <= r; ++x) data[x] = temp[x];
    }
    
    int InversePairs(vector<int> data) {
        //”√πÈ≤¢≈≈–Ú
        vector<int> temp;
        temp.resize(data.size() + 10);
        mergeSortAndCount(data, 0, data.size() - 1, temp);
        return reverseAlignments % 1000000007;
    }
};

int main() {
	vector<int> data = {
		1,2,3,4,5,6,7,0
	};
	Solution s;
	cout << s.InversePairs(data); //7
	return 0;
}
