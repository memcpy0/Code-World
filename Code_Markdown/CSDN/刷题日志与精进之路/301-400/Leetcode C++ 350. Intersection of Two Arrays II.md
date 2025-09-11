@[toc]

Given two arrays, write a function to **compute their intersection.**

Example 1:
```
Input: nums1 = [1,2,2,1], nums2 = [2,2]
Output: [2,2]
```
Example 2:
```
Input: nums1 = [4,9,5], nums2 = [9,4,9,8,4]
Output: [4,9]
```
Note:

-    Each element in the result should **appear as many times as it shows in both arra**ys.
-    The result can **be in any order**.

Follow up:

-    What if the given array is already sorted? How would you optimize your algorithm?
-    What if nums1's size is small compared to nums2's size? Which algorithm is better?
-    What if elements of nums2 are stored on disk, and the memory is limited such that you cannot load all elements into the memory at once?
 
题意：求两个数组的交集，和 $349$ 题 Intersection of Two Arrays不同的是，结果向量中的元素可以重复，次数为**两个数组中该元素同时出现的次数**。

思路1：用一个无序映射 $map$ 得到 $nums1$ 中的各个元素的出现次数。然后遍历 $nums2$ 中的元素，如果它在 $map$ 中有出现，就将其加入到结果向量中，然后减少 $map$ 中该元素的出现次数一次。
```cpp
class Solution {
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
        unordered_map<int, int> mp;
        vector<int> ans;
        for (auto i : nums1) ++mp[i];
        for (auto j : nums2) {
            if (mp[j] != 0) {
                ans.push_back(j);
                --mp[j];
            }
        }
        return ans;
    }
};
```
当然，对两个数组分别排序。然后使用双指针，或者二分的方法，也可以做这道题。复杂度差距不大，代码也不是太难，就不写了。

---
【Update 2020/07/13】思路2：看到评论区有个老哥用的是 `uthash` + 纯C代码。这里也来试一试。`https://blog.csdn.net/whatday/article/details/95926766`
```c
typedef struct HashTable {
	int k;
	int v;
	UT_hash_handle hh;
} *HashTable, *NodeHashTable;

#define MAX( a, b ) ((a) > (b) ? (a) : (b))

#define newHashTable() NULL

#define setHashTable( hashTable, key, value ) addHashTable( hashTable, key, value )

#define exitsHashTable( hashTable, key ) getNodeHashTable( hashTable, key )

#define cleanHashTable( hashTable )                             \
do {                                                            \
	NodeHashTable __current = NULL, __tmp = NULL;               \
                                                                \
	HASH_ITER( hh, hashTable, __current, __tmp ) {              \
		HASH_DEL( hashTable, __current );                       \
		free( __current );                                      \
	}                                                           \
} while( 0 )

static void addHashTable( HashTable *hashTable, int key, int value ) {
	NodeHashTable n = NULL;

	HASH_FIND_INT( *hashTable, &key, n );
	if( n == NULL ) {
		if( (n = (NodeHashTable) malloc( sizeof(*n) )) == NULL ) {
			exit( -1 );
		}
		n->k = key;
		HASH_ADD_INT( *hashTable, k, n );
	}
	n->v = value;
}

static NodeHashTable getNodeHashTable( HashTable hashTable, int key ) {
	NodeHashTable n = NULL;

	HASH_FIND_INT( hashTable, &key, n );

	return n;
}

static int getValueHashTable( HashTable hashTable, int key ) {
	NodeHashTable n = NULL;

	HASH_FIND_INT( hashTable, &key, n );

	return n != NULL ? n->v : INT_MIN;
}

static void *new(int sizeofE,int countE){void *p=malloc(sizeofE*countE);if(p==NULL)exit(-1);return p;}

int *intersect( int *nums1, int nums1Size, int *nums2, int nums2Size, int *returnSize ) {
	int *returnArray = NULL, i = 0;
	HashTable ht = NULL;

	*returnSize = 0;
	if( nums1 == NULL || nums2 == NULL ) {
		return NULL;
	}
	returnArray = (int *) new( sizeof(*returnArray), MAX( nums1Size, nums2Size ) );
	ht = newHashTable();
	for( i = 0; i < nums1Size; ++i ) {
		if( exitsHashTable( ht, nums1[i] ) ) {
			setHashTable( &ht, nums1[i], getValueHashTable( ht, nums1[i] ) + 1 );
			continue;
		}
		addHashTable( &ht, nums1[i], 1 );
	}
	for( i = 0; i < nums2Size; ++i ) {
		if( getValueHashTable( ht, nums2[i] ) > 0 ) {
			returnArray[(*returnSize)++] = nums2[i];
			setHashTable( &ht, nums2[i], getValueHashTable( ht, nums2[i] ) - 1 );
		}
	}
	cleanHashTable( ht );

	return returnArray;
}
```
