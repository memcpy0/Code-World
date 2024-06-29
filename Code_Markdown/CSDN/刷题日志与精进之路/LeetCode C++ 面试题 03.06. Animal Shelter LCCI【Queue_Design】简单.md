 `enqueue` method has a `animal` parameter, `animal[0]` represents the number of the animal, `animal[1]` represents the type of the animal, 0 for cat and 1 for dog.

`dequeue*` method returns `[animal number, animal type]`, if there's no animal that can be adopted, return `[-1, -1]`.

**Example1:**

```swift
Input: 
["AnimalShelf", "enqueue", "enqueue", "dequeueCat", "dequeueDog", "dequeueAny"]
[[], [[0, 0]], [[1, 0]], [], [], []]
 Output: 
[null,null,null,[0,0],[-1,-1],[1,0]]
```

**Example2:**

```swift
Input: 
["AnimalShelf", "enqueue", "enqueue", "enqueue", "dequeueDog", "dequeueCat", "dequeueAny"]
[[], [[0, 0]], [[1, 0]], [[2, 1]], [], [], []]
 Output: 
[null,null,null,null,[2,1],[0,0],[1,0]]
```

**Note:** The number of animals in the shelter will not exceed `20000`.

题意：有家动物收容所只收容狗与猫，且严格遵守“先进先出”的原则。在收养该收容所的动物时，收养人只能收养所有动物中“最老”（由其进入收容所的时间长短而定）的动物，或者可以挑选猫或狗（同时必须收养此类动物中“最老”的）。创建适用于这个系统的数据结构，实现 `enqueue`、`dequeueAny`、`dequeueDog` 和 `dequeueCat` 。 

---
### 解法 队列
题目没有说清的是，它会使用 `animal number` 表示动物进入收容所的时间长短，序号越小的动物越早进入收容所，且序号会自增。因此，我们使用分别使用两个队列表示两种动物，`dequeueAny` 会出队序号更小的动物：
```cpp
class AnimalShelf {
private:
    queue<vector<int>> cat, dog;
public:
    AnimalShelf() {}
    
    void enqueue(vector<int> animal) {
        if (animal[1] == 0) cat.push(animal);
        else dog.push(animal);
    }
    
    vector<int> dequeueAny() {
        if (cat.empty() && dog.empty()) return {-1, -1}; 
        if (cat.empty()) return dequeueDog();
        if (dog.empty()) return dequeueCat();
        if (cat.front()[0] < dog.front()[0]) return dequeueCat();
        else return dequeueDog();  
    }
    
    vector<int> dequeueDog() {
        if (dog.empty()) return {-1, -1};
        vector<int> t = dog.front(); dog.pop();
        return t;
    }
    
    vector<int> dequeueCat() {
        if (cat.empty()) return {-1, -1};
        vector<int> t = cat.front(); cat.pop();
        return t;
    }
};
```
提交后运行结果如下：
```cpp
执行用时：308 ms, 在所有 C++ 提交中击败了72.16% 的用户
内存消耗：110.8 MB, 在所有 C++ 提交中击败了22.25% 的用户
```
不存储 `vector<int>` 而只存储动物的编号，修改代码如下：
```cpp
class AnimalShelf {
private:
    queue<int> cat, dog;
public:
    AnimalShelf() {}
    
    void enqueue(vector<int> animal) {
        if (animal[1] == 0) cat.push(animal[0]);
        else dog.push(animal[0]);
    }
    
    vector<int> dequeueAny() {
        if (cat.empty() && dog.empty()) return {-1, -1}; 
        if (cat.empty()) return dequeueDog();
        if (dog.empty()) return dequeueCat();
        if (cat.front() < dog.front()) return dequeueCat();
        else return dequeueDog();  
    }
    
    vector<int> dequeueDog() {
        if (dog.empty()) return {-1, -1};
        int v = dog.front(); dog.pop();
        return {v, 1};
    }
    
    vector<int> dequeueCat() {
        if (cat.empty()) return {-1, -1};
        int v = cat.front(); cat.pop();
        return {v, 0};
    }
};
```
运行效率大大提升：
```cpp
执行用时：264 ms, 在所有 C++ 提交中击败了99.79% 的用户
内存消耗：110.4 MB, 在所有 C++ 提交中击败了58.59% 的用户
```
