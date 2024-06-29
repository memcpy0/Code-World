/*
** Problem: https://leetcode.cn/problems/total-distance-traveled
** Article: https://memcpy0.blog.csdn.net/article/details/138390563
** Author: memcpy0
*/
```python
class Solution:
    def distanceTraveled(self, mainTank: int, additionalTank: int) -> int:
        return (mainTank + min((mainTank - 1) // 4, additionalTank)) * 10;
```

```cpp
class Solution {
public: 
    int distanceTraveled(int mainTank, int additionalTank) {
        return (mainTank + min((mainTank - 1) / 4, additionalTank)) * 10;
    }
};
```

```java
class Solution {
    public int distanceTraveled(int mainTank, int additionalTank) {
        return (mainTank + Math.min((mainTank - 1) / 4, additionalTank)) * 10;
    }
}
```