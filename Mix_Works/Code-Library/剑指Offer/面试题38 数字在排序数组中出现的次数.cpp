#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    int lower(const vector<int> &nums, int target) {
        int lo = 0, hi = nums.size();
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (nums[mid] >= target)
                hi = mid;
            else 
                lo = mid + 1;
        }
        return lo;
    }
    int upper(const vector<int> &nums, int target) {
        int lo = 0, hi = nums.size();
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (nums[mid] > target)
                hi = mid;
            else 
                lo = mid + 1;
        }
        return lo;
    }
    int search(vector<int>& nums, int target) {
        int left = lower(nums, target), right = upper(nums, target);
        return right - left;
    }
};
/*
����ö��ֲ����㷨���������ҵ���һ��k�����ֲ����㷨�����������м�����ֺ�k�Ƚ�
����м�����ֱ�k����ôkֻ���ܳ����������ǰ��Σ���һ���������ǰ��β��ҾͿ����ˡ�

����м�����ֱ�kС����ôkֻ���ܳ���������ĺ��Σ���һ������ֻ������ĺ��β��ҾͿ����ˡ�

����м�����ֺ�k����أ�
- ���ж����Ƿ��ǵ�һ��k:
	- ���ǰ��һ�����ֲ���k�����м�����ָպ��ǵ�һ��k;
	- ���ǰ��һ��������k�����һ��k�϶����������ǰ��Σ���һ����Ȼ��ǰ��β���
	
�ݹ�����ҵ����������еĵ�һ��k��λ�� 
�ں���GetFirstK�У�������鲻��������k���򷵻�-1����������һ��k���򷵻ص�һ��k�������е��±� 
*/ 

int GetFirstK(int *data, int length, int k, int start, int end) {
	if (start > end) return -1;
	int middleIndex = (start + end) / 2;
	int middleData = data[middleIndex];
	if (middleData == k) {
		if ((middleIndex > 0 && data[middleIndex - 1] != k) || middleIndex == 0)
			return middleIndex;
		else 
			end = middleIndex - 1;
	} 
	else if (middleData > k) 
		end = middleIndex - 1;
	else 
		start = middleIndex + 1;
	return GetFirstK(data, length, k, start, end);
}
/*
����ö��ֲ����㷨���������ҵ����һ��k�����ֲ����㷨�����������м�����ֺ�k�Ƚ�
����м�����ֱ�k����ôkֻ���ܳ����������ǰ��Σ���һ���������ǰ��β��ҾͿ����ˡ�

����м�����ֱ�kС����ôkֻ���ܳ���������ĺ��Σ���һ������ֻ������ĺ��β��ҾͿ����ˡ�

����м�����ֺ�k����أ�
- ���ж����Ƿ������һ��k:
	- �����һ�����ֲ���k�����м�����ָպ������һ��k;
	- �����һ��������k�������һ��k�϶���������ĺ��Σ���һ����Ȼ�ں��β��� 

�ݹ�����ҵ����������е����һ��k��λ�� 
�ں���GetLastK�У�������鲻��������k���򷵻�-1����������һ��k���򷵻����һ��k�������е��±� 
*/ 
int GetLastK(int *data, int length, int k, int start, int end) {
	if (start > end) return -1;	
	int middleIndex = (start + end) / 2;
	int middleData = data[middleIndex];
	
	if (middleData == k) {
		if ((middleIndex < length - 1 && data[middleIndex + 1] != k) || middleIndex == length - 1) 
			return middleIndex;
		else
			start = middleIndex - 1;	
	}
	else if (middleData < k) 
		start = middleIndex + 1;
	else
		end = middleIndex - 1;
	return GetLastK(data, length, k, start, end);
}
int GetNumberOfK(int *data, int length, int k) {
	int number = 0;
	if (data != nullptr && length > 0) {
		int first = GetFirstK(data, length, k, 0, length - 1);
		int last = GetLastK(data, length, k, 0, length - 1);
		if (first > -1 && last > -1)
			number = last - first + 1;
	}
	return number;
}
/*
���ܲ��ԣ������а������ҵ����֡�û�в��ҵ����֡����ҵ����ֳ���һ�λ��߶��
�߽�ֵ���ԣ�������������ֵ����Сֵ��������ֻ��һ������
����������ԣ���ʾ�����ָ��ΪNULLָ�� 
*/
int main() { 
    return 0;
}
/*
LeetCode C++ ��ָ Offer 53 - I. �����������в������� I�����֡���
ͳ��һ��������**��������**�г��ֵĴ�����

ʾ�� 1:
```cpp
����: nums = [5,7,7,8,8,10], target = 8
���: 2
```
ʾ�� 2:
```cpp
����: nums = [5,7,7,8,8,10], target = 6
���: 0
```
**���ƣ�** `0 <= ���鳤�� <= 50000`

 
---
˼·1��ʹ�ù�ϣ���������ȫû�����õ���Ŀ���������������£�
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        unordered_map<int, int> mp;
        for (const auto i : nums) ++mp[i];
        return mp[target];
    }
};
```
Ч�ʣ�
```cpp
ִ����ʱ��16 ms, ������ C++ �ύ�л�����89.16% ���û�
�ڴ����ģ�13.4 MB, ������ C++ �ύ�л�����13.27% ���û�
```

---
˼·2��˳��ɨ�裬Ȼ��������������£�
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int cnt = 0;
        for (const int i : nums) if (i == target) ++cnt;
        return cnt;
    }
};
```
Ч�����£���Ϊ�������жϣ����ܳ��ַ�֧Ԥ��ʧ�ܣ�
```cpp
ִ����ʱ��20 ms, ������ C++ �ύ�л�����64.38% ���û�
�ڴ����ģ�13.2 MB, ������ C++ �ύ�л�����60.30% ���û�
```
---
˼·3�������ҵ�����ߵ�ֵ��Ȼ��˳��ɨ�赽���ұߵ�ֵ���� `lower_bound` �������������£�
```cpp

```
*/
