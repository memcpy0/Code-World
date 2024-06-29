
@[toc]
 
 C++ `<Algorithm>` 算法总结

# 一、引言和参数说明
STL中包含了许多算法，这些算法主要定义在 `<algorithm>` 中。编程时，只需要在文件中加入 `#include <algorithm>`  就能够方便地使用这些算法。事实上，如果仅局限于"用"就太暴殄天物了，**STL中的算法实现完全值得我们去深入学习和剖析** 。

为了用好这些函数，必须先了解以下几个概念：

 - 容器是用来存放各种数据的一个东西，`<algorithm>` 中的算法是针对容器设计的。其中，`vector` 是最常用的容器。
   - 迭代器是 **与容器进行配套使用的** 。它的作用是对容器中的元素进行遍历，比如数组的指针就可以看作一种迭代器。迭代器一般支持解引用 `operator*`、自加 `operator++` 、相等 `operator==` 等操作。
   - 谓词是用来对 `<algorithm>` 中的函数进行定制操作的。它可以是lambda表达式或是函数/函数指针，并且<algorithm>中只有一元谓词和二元谓词，即函数只能接收一个或两个参数。对于需要额外参数的谓词可以使用lambda表达式的捕获功能。
   
- `pair`：它存在于 `<utility>` 中，已被 `include` 在了 `<algorithm>` 之中。部分算法会有两个返回值，而 `pair` 有两个成员 `first` 和 `second` ，可以·用来将两个值进行打包返回。
 

各个算法用到的主要参数的含义说明如下：

-    `beg` 和 `end` 是表示元素范围的迭代器。
 -   `beg2` 是第二个输入序列开始位置的迭代器，`end2` 表示第二个序列末尾位置（如果有的话）。如果没有 `end2` ，则假定 `beg2` 表示的序列与 `beg` 和 `end` 表示的序列**一样大**。`beg` 和 `beg2` 不必是相同的类型，但**两个序列中的元素都要能调用给定的可调用对象**。比如 `beg` 是一个 `std::vector<int>::iterator` ，而 `beg2` 可以是一个 `std::deque<int>::iterator` 。
  -  `dest` 是表示目的序列的迭代器，目的序列**必须保证能够存储算法生成的所有元素**。若无法确定大小，可以调用 `std::back_inserter()` 函数获取目的序列的插入迭代器。
-    `unaryPred` 和 `binayPred` 是一元和二元谓词，其实参都是序列中的元素。
 -   `comp` 是一个二元谓词，用于比较两个元素。
  -  `unaryOp` 和 `binaryOp` 是可调用对象。

此外，部分算法要求序列是有序的，默认是使用小于运算符 $<$ 定义的升序。若使用谓词版本，则有序是按comp升序。

# 一、查找对象的算法

这些算法都在**一个输入序列**中搜索一个指定值或是一个序列。

不接受谓词的版本使用底层相等运算符 $==$ 进行比较元素，接受谓词的版本**使用用户给定的谓词比较元素**。

## 1. 简单查找算法
```cpp
find(beg, end, val)
find_if(beg, end, unaryPred)
find_if_not(beg, end, unaryPred)
count(beg, end, val)
count_if(beg, end, unaryPred)
```
`find`、`find_if` 和 `find_if_not` 返回一个迭代器指向第一个满足条件的元素，没找到则返回 `end` 。`count` 和 `count_if` 返回一个计数器。
```cpp
std::vector<int> v{ -2,-1,0,1,2 };
std::vector<int>::iterator iter = find(v.begin(), v.end(), 5);//没找到iter==v.end()
if (iter == v.end()) std::cout << "oops";
long long n = std::count_if(v.begin(), v.end(), [](int val) {return val > 0; } ); //正数个数为2
```
```
all_of(beg, end, unaryPred)
any_of(beg, end, unaryPred)
none_of(beg, end, unaryPred)
```
这些算法都返回 `bool` ，说明序列中是否任意/存在/不存在元素满足条件，若序列为空，`any_of` 返回`false` ，`all_of` 和 `none_of` 返回 `true` 。
```
std::string s("123abc");
std::cout << std::all_of(s.begin(), s.end(), [](char c) {return isalpha(c); });//输出0，不全为字母
```
## 2. 查找重复值的算法
```cpp
    adjacent_find(beg, end)
    adjacent_find(beg, end, binaryPred)
```
返回一个迭代器指向 **第一对相邻重复元素** 的迭代器，若无相邻元素则返回end。
 
    std::vector<int> v{ 5,2,2,3,6,10 };
    std::vector<int>::iterator iter1 = std::adjacent_find(v.begin(), v.end());//iter1指向第一个2
    std::cout << *(iter1+1);
    auto iter2 = std::adjacent_find(v.begin(), v.end(), [](int a, int b) {return b == 2 * a; });//查找下一个元素是上一个元素二倍的位置，iter2指向3


    search_n(beg, end, cnt, val)
    search_n(beg, end, cnt, val, binaryPred)

在序列中查找 `cnt` 个相等的元素，相等由 `binaryPred` 所定义。

    struct A { int i; char c; };
    std::vector<A> vA{ {1,'v'},{2,'c'},{3,'d'},{3,'k'},{3,'o'},{5,'l'} };
    auto iter = std::search_n(vA.begin(), vA.end(), 3,
        A{ 3,'x' },        //查找与A{3,'x'}相等的连续3个元素出现的位置
        [](A a, A b)     //定义类型A的相等为成员变量i相等
        {return a.i == b.i; });  //iter指向{3,'d'} 

## 3. 查找子序列的算法
 
    search(beg1, end1, beg2, end2)
    search(beg1, end1, beg2, end2, binaryPred)

返回一个迭代器指向 **第二个序列** 在第一个序列中 **第一次出现的位置** ，若未找到则返回 `end1` 。

    std::vector<int> v{ 1,2,3,4,6,12,5,11 };
    int array[3] = { 2,2,3 };
    auto iter1 = std::search(v.begin(), v.end(), std::begin(array), std::end(array));//在v中查找子序列array，没找到，iter1指向v的尾后
    auto iter2 = std::search(v.begin(), v.end(), std::begin(array), std::end(array),
        [](int a, int b) //在v中查找连续的3个数，分别能被2，2，3整除
        {return a % b == 0; });//iter2指向4

    find_first_of(beg1, end1, beg2, end2)
    find_first_of(beg1, end1, beg2, end2, binaryPred)


查找子序列的算法

    search(beg1, end1, beg2, end2)
    search(beg1, end1, beg2, end2, binaryPred)

返回一个迭代器指向第二个序列在第一个序列中第一次出现的位置，若未找到则返回end1。

    std::vector<int> v{ 1,2,3,4,6,12,5,11 };
    int array[3] = { 2,2,3 };
    auto iter1 = std::search(v.begin(), v.end(), std::begin(array), std::end(array));//在v中查找子序列array，没找到，iter1指向v的尾后
    auto iter2 = std::search(v.begin(), v.end(), std::begin(array), std::end(array),
        [](int a, int b) //在v中查找连续的3个数，分别能被2，2，3整除
        {return a % b == 0; });//iter2指向4

    find_first_of(beg1, end1, beg2, end2)
    find_first_of(beg1, end1, beg2, end2, binaryPred)

返回一个迭代器指向第二个序列中任意一个元素在第一个序列中首次出现的位置，若未找到则返回end1。

    std::vector<int> v{ 1,2,3,4,6,12,5,11 };
    int array[3] = { 5,4,3 };
    auto iter1 = std::find_first_of(v.begin(), v.end(), array, array + 3);//array中的元素第一个在v中出现的是3，iter1指向v中的3
    auto iter2 = std::find_first_of(v.begin(), v.end(), array, array + 3,
        [](int a, int b)//在v中查找第一个与array中元素相加等于10的元素（即寻找5，6，7）
        {return a + b == 10; });//iter2指向6，6+4==10

    find_end(beg1, end1, beg2, end2)
    find_end(beg1, end1, beg2, end2, binaryPred)

类似search，但是返回的第二个序列在第一个序列中最后一次出现的位置，若未找到同样返回end1。

    std::vector<int> v{ 1,2,3,4,5,6,1,2,3,7,8,9 };
    int array[3] = { 1,2,3 };
    std::forward_list<int> lst{ 9,8,7 };
    auto iter1 = std::find_end(v.begin(), v.end(), array, array + 3);//iter1指向v中第二个（最后一个）{1，2，3}的首位置元素1
    auto iter2 = std::find_end(v.begin(), v.end(), lst.begin(), lst.end());//没找到，iter2指向v的尾后

其他只读算法

    for_each(beg, end, unaryOp)

对序列中的每个元素应用可调用对象unaryOp，若迭代器允许通过解引用向序列中的元素写入值，则unaryOp可能修改元素。然而，范围for循环可以达到相同的目的且更书写简短。

    std::vector<int> v{ 1,2,3,4,5 };
    std::for_each(v.begin(), v.end(), [](int& a) {a *= 2; });//传入引用，将v中的每个元素修改为自身的2倍
    for (int& i : v)//范围for循环，使用引用同样可以修改元素
    {//若只有一条语句则可以省略花括号
        std::cout << i << ' ';
    }

    mismatch(beg1, end1, beg2)
    mismatch(beg1, end1, beg2, binaryPred)

返回一个迭代器的pair表示两个序列中第一个不匹配的元素，如果所有元素都匹配，则返回的pair中的第一个迭代器指向end1，第二个迭代器指向beg2中偏移量等于第一个序列长度的位置。

    std::vector<int> v1{ 1,2,3 };
    std::vector<int> v2{ 1,2,4,5 };//v2的长度需不小于v1的长度
    std::pair<std::vector<int>::iterator, std::vector<int>::iterator>//一般用auto
        iters1 = std::mismatch(v1.begin(), v1.end(), v2.begin());//iters1.first指向v1中的3，iters1.second指向v2中的4
    auto iters2 = std::mismatch(v1.begin(), v1.end(), v2.begin(),//没找到不匹配的元素对
        [](int a, int b)//iters2.first指向v1的尾后，iters2.second指向v2中的5
        {return abs(a - b) < 2; });//定义匹配为两元素之差的绝对值小于2

    equal(beg1, end1, beg2)
    equal(beg1, end1, beg2, binaryPred)

确定两个序列是否相等，返回一个bool值。

    std::vector<int> v1{ 1,2,3 };
    std::vector<int> v2{ 1,2,4,5 };
    bool b = std::equal(v1.begin(), v1.end(), v2.begin(),//相等返回true，只检查前v1.size()个元素
        [](int a, int b)
        {return abs(a - b) < 2; });//定义相等为两元素之差的绝对值小于2

二分搜索算法

这些算法要求序列是有序的，算法默认使用小于运算符进行比较，若使用谓词，则对小于进行重新定义。lower_bound，upper_bound和equal_range返回的迭代器指向给定元素在序列中的正确插入位置——插入后序列还是有序的，注意容器中元素的插入（seq.insert(iter, val)）是在迭代器之前进行插入，单向链表除外（lst.insert_after(iter, val)）。

    binary_search(beg, end, val)
    binary_search(beg, end, val, comp)

返回一个bool值，表示序列中是否存在等于val的元素。

    std::vector<int> v1{ 10,9,8,7,6,4,3,2,1 };
    std::vector<int> v2{ 1,2,3,4,6,7,8,9,10 };
    bool b1 = std::binary_search(v1.begin(), v1.end(), 6);//b1==false，v1不是升序使得搜索方向错误
    bool b2 = std::binary_search(v2.begin(), v2.end(), 6);//b2==true

    struct A { int i; char c; };
    std::vector<A> vA{ {1,'v'},{2,'c'},{3,'d'},{4,'k'},{5,'o'},{6,'l'} };//按i升序
    bool b3 = std::binary_search(vA.begin(), vA.end(), A{ 3,'h' },//返回true
        [](A a, A b) {return a.i < b.i; });//重定义A的小于为成员变量i的小于

    lower_bound(beg, end, val)
    lower_bound(beg, end, val, comp)
    upper_bound(beg, end, val)
    upper_bound(beg, end, val, comp)
    equal_range(beg, end, val)
    equal_range(beg, end, val, comp)

lower_bound返回一个迭代器指向第一个大于等于val的值，upper_bound返回一个迭代器指向第一个大于val的值，equal_range返回一个迭代器pair，其first成员是lower_bound的返回值，second成员是upper_bound的返回值。

    std::vector<double>v{ 1,2,3,4,4,4,5,6 };//升序
    auto iter1 = std::lower_bound(v.begin(), v.end(), 4.0);//iter1指向第一个不小于4.0的值，v中第一个4
    auto iter2 = std::upper_bound(v.begin(), v.end(), 4.5);//iter2指向第一个大于4.5的值，v中最后一个4后的5
    auto iters = std::equal_range(v.begin(), v.end(), 4.0);//first指向第一个4，second指向5

写容器元素的算法
只写不读元素的算法

    fill(beg, end, val)
    fill_n(dest, cnt, val)
    generate(beg, end, Gen)
    generate_n(dest, cnt, Gen)

给序列中每个元素赋一个新值。fill将值val赋给元素，generate执行生成器对象Gen()生成新值，生成器是一个可调用对象，每次调用会生成一个新值，fill和generate都返回void，_n版本返回一个迭代器指向写入到输出序列的元素的下一个位置。

    std::vector<int>v(6, -2);//v中存了6个-2
    std::fill_n(v.begin(), 3, 5);//v的前3个元素被改写为5
    std::fill(v.begin() + 3, v.end(), 10);//v[3]及之后的元素被改写为10
    std::fill_n(std::back_inserter(v), 3, 15);//在v的末尾再添加3个15
    
    std::default_random_engine e(static_cast<unsigned>(time(0)));//随机数生成器
    std::uniform_int_distribution<int> u1(0, 10);//生成[0,10]的随机数
    std::uniform_int_distribution<int> u2(11, 20);//生成[11,20]的随机数

    std::generate_n(v.begin(), 5, [&]() {return u1(e); });//将v的前5个数改写为[0,10]的随机数
    std::generate(v.begin() + 5, v.end(), [&]() {return u2(e); });//将v[5]及之后的元素改写为[11,20]的随机数

使用输入迭代器的写算法

    copy(beg, end dest)
    copy_if(beg, end, dest, unaryPred)
    copy_n(beg, n, dest)

将输入范围内的元素复制到目的序列，copy复制所有元素，copy_if复制满足条件的元素，copy_n复制前n个元素，输入序列必须有至少n个元素。

    int array1[5] = { 1,2,3,4,5 };
    int array2[5];
    std::copy(std::begin(array1), std::end(array1), std::begin(array2));//数组不允许直接复制
    std::vector<int>v;
    v.reserve(5);
    std::copy_if(std::begin(array1), std::end(array1), std::back_inserter(v), [](int a) {return a & 1; });//只复制奇数，v={1,3,5}
    std::copy_n(array1 + 1, 2, v.begin());//v之前有3个元素，不会越界，现在v={2,3,5}

    move(beg, end, dest)

对输入序列中的每个元素调用std::move，将其移动到目的序列。

    std::vector<int>v(5);//v中有5个元素
    {//一个新的生存区
        std::vector<int>temp{ 1,2,3,4,5 };
        std::move(temp.begin(), temp.end(), v.begin());
    }//生存区结束

    transform(beg, end, dest, unaryOp)
    transform(beg1, end1, beg2, dest, binaryOp)

对输入序列中的元素进行变换，结果写入目的序列，输入序列不作改变。

    std::vector<int>v1{ 1,2,3,4,5 };
    std::vector<int>v2{ 5,4,3,2,1 };
    std::vector<int>v3;
    std::vector<int>v4;
    std::transform(v1.begin(), v1.end(), std::back_inserter(v3), [](int a) {return a * 2; });//将v1中元素值的2倍写入v3
    std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(v4), [](int a, int b) {return a * b; });//v4中元素为v1与v2中对应元素的乘积

    replace_copy(beg, end, dest, old_val, new_val)
    replace_copy_if(beg, end, dest, unaryPred, new_val)

将序列中满足条件的元素替换为新值写入到目的序列中，输入序列不作改变。

    std::vector<int>v1{ 1,2,3,4,5 };
    std::vector<int>v2(v1.size());
    std::vector<int>v3(v1.size());
    std::replace_copy(v1.begin(), v1.end(), v2.begin(), 3, 333);//将v1中的3替换为333后写入v2，v2={1,2,333,4,5}
    std::replace_copy_if(v1.begin(), v1.end(), v3.begin(),//v3={-1,-1,3,4,5}
        [](int a) {return a < 3; }, -1);//将v1中小于3的元素替换为-1写入v3

    merge(beg1, end1, beg2, end2, dest)
    merge(beg1, end1, beg2, end2, dest, comp)

将两个有序序列合并为一个有序序列写入dest，默认使用小于运算符进行比较，使用comp可以重定义小于运算符。

    std::vector<int> v1{ 1,3,5,7,9 };//升序
    std::vector<int> v2{ 2,4,6,8,10 };//升序
    std::vector<int>v3;
    v3.reserve(v1.size() + v2.size());
    std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));//v3=[1-10]

使用前向迭代器的写算法

这些算法会向输入序列写入元素

    iter_swap(iter1, iter2)
    swap_ranges(beg1, end1, beg2)

iter_swap交换两个迭代器指向的元素，不交换迭代器。swap_ranges交换两个范围中的元素，两个范围不能重叠，返回递增后的beg2，指向最后一个交换元素之后的位置。

    std::vector<int> v1{ 1,3,5,7,9 };
    std::vector<int> v2{ 2,4,6,8,10 };
    std::iter_swap(v1.begin(), v1.begin() + 1);//交换v1的前两个元素，v1={3,1,5,7,9}
    std::swap_ranges(v2.begin(), v2.begin() + 2, v2.begin() + 3);//将v2的前2个元素与从v2[3]开始的2个元素进行位置交换，v2={8,10,6,2,4}

    replace(beg, end, old_val, new_val)
    replace_if(beg, end, unaryPred, new_val)

使用新值替换每个匹配元素。

    //将v中的非0元素置0，0元素置1
    std::vector<int> v{ -2,-1,0,1,2 };
    std::replace(v.begin(), v.end(), 1, 2);//将v中的1改写为2
    std::replace_if(v.begin(), v.end(), [](int a) {return !a; }, 1);//将v中的0改写为1
    std::replace_if(v.begin(), v.end(), [](int a) {return a != 1; }, 0);//将v中的非0值改写为0

使用双向迭代器的写算法

    copy_backward(beg, end, dest)
    move_backward(beg, end, dest)

将序列中的元素复制/移动到目的位置，dest是输出序列的尾后迭代器。返回一个迭代器指向dest中beg所对应的元素。注意，这两个函数不会更改源序列元素在目的序列中的位置！

```cpp
std::vector<int> v1{ -3,-2,-1 };
std::vector<int> v2{ 1,2,3,4,5,6,7,8,9 };//将v2最后三个元素改写为-3，-2，-1
auto iter=std::copy_backward(v1.begin(), v1.end(), v2.end());//v2={1,2,3,4,5,6,-3,-2,-1}

inplace_merge(beg, mid, end)
inplace_merge(beg, mid, end, comp)
```

将同一个序列中的两个有序子序列合并为一个单一的有序序列，并写入原序列。默认使用小于运算符进行比较，也可对小于运算符进行重定义。

```cpp
std::vector<int> v{ 5,3,1,6,4,2 };//v的前3个元素与后3个元素都是按谓词升序排列的
std::inplace_merge(v.begin(), v.begin() + 3, v.end(),
[](int a, int b) {return a > b; });//将大于定义为比较运算符
```

划分与排序算法

划分与排序算法都提供了稳定与不稳定版本，稳定版本确保相等元素的相对位置保持不变，但是可能会消耗更多时间与内存。
划分算法

```cpp
partition(beg, end, unaryPred)
stable_partition(beg, end, unaryPred)
```

使用unaryPred划分输入序列，满足条件的元素放在序列前面，不满足的放不序列后面。返回一个指向最后一个满足条件元素之后的位置。若所有元素都不满足条件，则返回beg。

```cpp
struct A { int i; char c; };
std::vector<A>vA1{ {6,'g'},{2,'h'},{-6,'t'},{3,'s'},{4,'p'} };
auto vA2 = vA1;
//按A的i是否是正数进行划分
std::partition(vA1.begin(), vA1.end(), [](A a) {return a.i > 0; });//Visual Stdio 2019的运算结果是vA1={{6,'g'},{2,'h'},{4,'p'},{3,'s'},{-6,'t'}}，满足条件的{3,'s'}原来排在满足条件的{4,'p'}前面，划分后{3,'s'}排在了{4,'p'}的后面
std::stable_partition(vA2.begin(), vA2.end(), [](A a) {return a.i > 0; });//vA2={{6,'g'},{2,'h'},{3,'s'},{4,'p'},{-6,'t'}}，两部分内元素之间的相对位置没有改变
```

```cpp
is_partitioned(beg, end, unaryPred)
```

若所有满足条件的元素都排在不满足条件的元素之前，则返回true，若序列为空，也返回true。

```cpp
int array[5] = { 1,3,5,6,8 };
bool b = std::is_partitioned(array, array + 5, [](int i) {return i & 1; });//是否奇数排在偶数前面

partition_point(beg, end, unaryPred)
```

输入序列必须是已用unaryPred划分过的，返回满足条件的最后一个元素的尾后迭代器。

```cpp
int array[5] = { 1,3,5,6,8 };
auto iter = std::partition_point(array, array + 5, [](int i) {return i & 1; });//iter指向最后一个奇数5的下一个位置，iter指向6

partition_copy(beg, end, dest1, dest2, unaryPred)
```

将序列中满足条件的元素复制到目的序列1，不满足条件的元素复制到目的序列2，返回一个迭代器pair，其first成员指向复制到目的序列1元素的下一位置，second成员指向复制到目的序列2元素的下一位置。

```cpp
//将array中的奇数写入a1，偶数写入a2
int array[5] = { 1,3,5,6,8 };
int a1[5] = { 15,25,35,45,55 };//划分后a1={1,3,5,45,55}
int a2[5] = { 10,20,30,40,50 };//划分后a2={6,8,30,40,50}
auto iters = std::partition_copy(array, array + 5, a1, a2, [](int a) {return a & 1; });//iters.first指向45，iters.second指向30
```

# 排序算法

这些算法要求随机访问迭代器，默认使用小于运算符比较元素。除partial_sort_copy外，其他排序算法都返回void。

```cpp
sort(beg, end)
stable_sort(beg, end)
sort(beg, end, comp)
stable_sort(beg, end, comp)
```

对序列进行排序。

```cpp

```cpp
bool comp(int a, int b)//降序
{ return a > b; }

int main()
{
    std::vector<int> v{ 7,5,1,4,6,9,3,2,8 };
    std::sort(v.begin(), v.end(), comp);//传入函数指针
}

    is_sorted(beg, end)
    is_sorted(beg, end, comp)
    is_sorted_until(beg, end)
    is_sorted_until(beg, end, comp)
```

```

is_sorted返回一个bool值指出整个序列是否有序。is_sorted_until在输入序列中查找最长初始有序子序列，并返回子序列的尾后迭代器。

```cpp
std::vector<int> v{ 1,2,3,4,5,4,3,2,1 };
bool b = std::is_sorted(v.begin(), v.end());//是否升序，否
auto iter1 = std::is_sorted_until(v.begin(), v.end(), [](int a, int b) {return a < b; });//从开始到5都是升序的，iter1指向5后的4
auto iter2 = std::is_sorted_until(v.begin(), v.end(), [](int a, int b) {return a > b; });//只有最开始的元素是降序的，iter2指向第1个（从0开始）元素2

partial_sort(beg, mid, end)
partial_sort(beg, mid, end, comp)
```

将beg至end中的元素按comp进行排序，但是只排前mid-beg个，算法结束后，beg至end中的元素都是已排好序的且不会比mid至end中的任意元素更大。

```cpp
std::vector<int> v{ 7,5,1,4,6,9,3,2,8 };
std::partial_sort(v.begin(), v.begin() + 5, v.end());//将v中最小的5个元素排序，Visual Studio 2019排序结果为v={1,2,3,4,5,9,7,6,8}

partial_sort_copy(beg, end, destBeg, destEnd)
partial_sort_copy(beg, end, destBeg, destEnd, comp)
```

排序输入范围中的元素，并将足够多的已排序元素放入destBeg至destEnd所指示的序列中。若目的范围长度不小于输入序列，则排序整个输入序列并将结果写入目的序列的前面。若目的序列长度小于输入序列，则只复制输入序列中与目的范围一样多的元素。算法返回一个指向目的序列已排序元素之后位置的迭代器。

```cpp
std::vector<int> v1{ 2,5,3,4,1 };
std::vector<int> v2(7, -1);//v2={1,2,3,4,5,-1,-1}
std::vector<int> v3(3, -1);//v3={1,2,3}
std::partial_sort_copy(v1.begin(), v1.end(), v2.begin(), v2.end());
std::partial_sort_copy(v1.begin(), v1.end(), v3.begin(), v3.end());

nth_element(beg, nth, end)
nth_element(beg, nth, end, comp)
```

参数nth是一个迭代器，算法结束后，此迭代器指向的元素正好是序列排序后在此位置上的值。序列中的元素会围绕nth进行划分：nth之前的元素都小于等于它，之后的元素都大于等于它。

```cpp
//取中位数
std::vector<int> v{ 2,5,3,4,1 };
auto iter = v.begin() + 2;
std::nth_element(v.begin(), iter, v.end());//iter指向3
```

通用重排操作

<algorithm>中的算法不会对容器的大小进行更改！

这些算法会对序列中的元素进行重排，前两个算法remove和unique会将序列的前一部分元素满足某种标准，并返回一个迭代器标记子序列的末尾，其它算法都重排整个序列。

这些算法的基本版本都进行原址操作，_copy版本不改变原序列而将重排结果写入指定目的序列。
使用前向迭代器的重排算法

  
 
```cpp
remove(beg, end, val)
remove(beg, end, unaryPred)
remove_copy(beg, end, dest, val)
remove_copy(beg, end, dest, unaryPred)
```

从序列中通过覆盖“删除”元素，返回一个指向最后有效元素之后的位置的迭代器。迭代器之后的元素不可再使用！

```cpp
std::vector<int> v{ 2,5,-3,4,-1 };
auto iter = std::remove_if(v.begin(), v.end(), [](int a) {return a < 0; });//v.size()==5
v.erase(iter, v.end());//v={2,5,4}

unique(beg, end)
unique(beg, end, binaryPred)
unique_copy(beg, end, dest)
unique(beg, end, dest, binaryPred)
```

对相邻的重复元素，通过覆盖“删除”元素，返回一个指向最后有效元素之后的位置的迭代器。迭代器之后的元素不可再使用！

```cpp
std::vector<int> v{ 2,4,3,5,2,3,4,1,3 };//v.size()==9
std::sort(v.begin(), v.end());//需要先排序
auto iter = std::unique(v.begin(), v.end());//v.size()==9
v.erase(iter, v.end());//v={1,2,3,4,5}

rotate(beg, mid, end)
rotate_copy(beg, mid, end, dest)
```

让mid成为首元素，之后是mid+1至end前的元素，再接着是beg至mid之前的元素，返回一个指向原来beg位置的元素。

```cpp
std::string s("0123456789");
std::rotate(s.begin(), s.begin() + 2, s.end());//s="2345678901"
```

使用双向迭代器的重排算法

```cpp
reverse(beg, end)
reverse_copy(beg, end)
```

翻转序列元素，reverse返回void，reverse_copy返回一个指向复制到目的序列的元素的下一个位置的迭代器。

```cpp
std::string s("abc");
std::reverse(s.begin(), s.end());//s="cba"
```

使用随机访问迭代器的重排算法

```cpp
shuffle(beg, end, uniform_rand)
```

混洗序列中的元素，序列元素的一种随机排列。

```cpp
std::default_random_engine e(static_cast<unsigned>(time(0)));
std::vector<int> v{ 1,2,3,4,5,6,7,8,9 };
std::shuffle(v.begin(), v.end(), e);//一种可能，v={5,3,4,2,6,7,1,8,9}
```

排列算法

n个元素占据n个位置，改变不同元素的相对位置，就可以获取一个新的排列。比如由abc构成的序列有3!=6种排列：abc，acb，bac，bca，cab，cba。注意，这些排列是按升序排的。此外，生成排列的算法要求双向迭代器。

```cpp
is_permutation(beg1, end1, beg2)
is_permutation(beg1, end1, beg2, binaryPred)
```

若两个序列有相同的元素，不论位置，则返回true，否则返回false。

```cpp
std::string s1("abcd");
std::string s2("bcad");
bool b = std::is_permutation(s1.begin(), s1.end(), s2.begin(), s2.end());//true

next_permutation(beg, end)
next_permutation(beg, end, comp)
prev_permutation(beg, end)
prev_permutation(beg, end, comp)
```

next_permutation将序列转换为下一个排列，若当前序列已是最后一个排列，则将其转换为第一个排列，并且返回false，否则返回true。prev_permutation与next_permutation类似。

```cpp
std::string s("abcd");
do
{
    std::cout << s << std::endl;
} while (std::next_permutation(s.begin(), s.end()));
```

有序序列的集合算法

这些算法都要求序列是有序的。除includes外，其他的算法都返回一个写入dest的元素之后位置的迭代器。

```cpp
include(beg1, end1, beg2, end2)
include(beg1, end1, beg2, end2, comp)
```

如果第二个序列中的每个元素都包含在第一个序中，则返回true，否则返回false。

```cpp
std::string s1("abcd");//有序
std::string s2("acd");//有序
bool b = std::includes(s1.begin(), s1.end(), s2.begin(), s2.end());//true

set_union(beg1, end1, beg2, end2, dest)
set_union(beg1, end1, beg2, end2, dest, comp)
```

取并集，目的序列中包含两个输入序列中所有出现过的元素，输入序列中不要有重复元素。

```cpp
std::string s1("abcd");//有序
std::string s2("acde");//有序
std::string s3(8, ' ');
auto iter = std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), s3.begin());
s3.erase(iter, s3.end());//s3="abcde"

set_intersection(beg1, end1, beg2, end2, dest)
set_intersection(beg1, end1, beg2, end2, dest, comp)
```

取交集，目的序列中包含两个输入序列中共同出现过的元素。

```cpp
std::string s1("abcd");//有序
std::string s2("acde");//有序
std::string s3(4, ' ');
auto iter = std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), s3.begin());
s3.erase(iter, s3.end());//s3="acd"

set_difference(beg1, end1, beg2, end2, dest)
set_difference(beg1, end1, beg2, end2, dest, comp)
```

集合1减去集合2，目的序列中包含出现在输入序列1但没出现在序列2中的元素。

```cpp
std::string s1("abcd");//有序
std::string s2("acde");//有序
std::string s3(4, ' ');
auto iter = std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end`在这里插入代码片`(), s3.begin());
s3.erase(iter, s3.end());//s3="b"

set_symmetric_difference(beg1, end1, beg2, end2, dest)
set_symmetric_difference(beg1, end1, beg2, end2, dest, comp)
```

集合1加上集合2再减去二者的交集，目的序列中包含出现在一个序列但没出现在另一个序列中的元素。

```cpp
std::string s1("abcd");//有序
std::string s2("acde");//有序
std::string s3(8, ' ');
auto iter = std::set_symmetric_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), s3.begin());
s3.erase(iter, s3.end());//s3="be"
```

最大值和最小值

```cpp
min(val1, val2)
min(val1, val2, comp)
min(init_list)
min(init_list, comp)
max(val1, val2)
max(val1, val2, comp)
max(init_list)
max(init_list, comp)
```

返回val1和val2中的最小值/最大值，或是初始化列表中的最小值/最大值。两个实参类型必须一致。参数和返回类型都是const的引用。

```cpp
double m1 = std::min(2.0, 6.2);//2.0
int m2 = std::max({ 2,5,6,9,-10,-8 }, [](int a, int b) {return std::abs(a) < std::abs(b); });//绝对值最大者，-10

minmax(val1, val2)
minmax(val1, val2, comp)
minmax(init_list)
minmax(init_list, comp)
```

返回一个pair，其first成员为最小值，second成员为最大值。

```cpp
int a = 3, b = 2;
auto ms = std::minmax(a, b);
a = ms.first;//first是b的引用，现在a=2，b=2
b = ms.second;//second是a的引用，但是a已经更改为2，b=2
std::cout << a << ' ' << b << 'n';//2,2

min_element(beg, end)
min_element(beg, end, comp)
max_element(beg, end)
max_element(beg, end, comp)
minmax_element(beg, end)
minmax_element(beg, end, comp)
```

前四个算法返回一个迭代器指向序列中的最小值/最大值，后两个返回一个迭代器pair，其first成员指向最小值，second成员指向最大值。

```cpp
int array[10] = { 1,5,9,6,3,5,7,8,2,4 };
auto iters = std::minmax_element(array, array + 10);
long long posDiff = iters.second - iters.first;//位置距离
int valDiff = *iters.second - *iters.first;//值的距离
```

字典序比较

```cpp
lexicographical_compare(beg1, end1, beg2, end2)
lexicographical_compare(beg1, end1, beg2, end2, comp)
```

判断序列1的字典序是否小于序列2，若小于，则返回true，否则返回false。

```cpp
int array[6] = { 1,5,9,6,5,5 };
std::vector<int> v{ 1,5,9,6,4 };
std::deque<int> d{ 1,5,9,6,4 };
std::forward_list<int>l{ 1,5,9 };
bool b1 = std::lexicographical_compare(array, array + 6, v.begin(), v.end());//5不小于4，false
bool b2 = std::lexicographical_compare(v.begin(), v.end(), d.begin(), d.end());//相等需不小于，false
bool b3 = std::lexicographical_compare(d.begin(), d.end(), l.begin(), l.end());//链表长度更短，false
```

 
<algorithm>是C++标准程序库中的一个头文件，定义了C++ STL标准中的基础性的算法(均为函数模板)。<algorithm>定义了设计用于元素范围的函数集合。任何对象序列的范围可以通过迭代器或指针访问。

`std::adjacent_find`  在序列中查找第一对相邻且值相等的元素；

`std::find`  对一个输入序列，查找第一个等于给定值的元素；

`std::find_end`  查找有B定义的序列在A序列中最后一次出现的位置(B可能是A的子序列)；

`std::find_first_of`  查找A序列中第一个与序列B中任一元素值相等的元素位置；

`std::find_if`  在序列中返回满足谓词(给定的条件)的第一个元素；

`std::find_if_not` 在序列中返回不满足谓词的第一个元素；

`std::all_of` 如果序列中所有元素均满足给定的条件，则返回true；

`std::any_of` 如果序列中存在元素满足给定的条件，则返回true；

`std::none_of` 如果序列中所有元素均不满足给定的条件，则返回true；

`std::binary_search` 对一个升序序列做二分搜索，判断序列中是否有与给定值相等的元素；

`std::search` 在序列A中，搜索B首次出现的位置(B可能是A的子序列)；

`std::search_n` 在给定序列中，搜索给定值连续出现n次的位置；

`std::copy` 将一个序列中的元素拷贝到新的位置；

`std::copy_backward` 把一个序列复制到另一个序列，按照由尾到头顺序依次复制元素；

`std::copy_if` 将一个序列中满足给定条件的元素拷贝到新的位置；

`std::copy_n`  将一个序列中的前n个元素拷贝到新的位置；

`std::count` 返回序列中等于给定值元素的个数；

`std::count_if` 返回序列中满足给定条件的元素的个数；

`std::equal` 比较两个序列的对应元素是否相等；

`std::equal_range` 在已排序的序列中，查找元素等于给定值组成的子范围；

`std::lower_bound` 在升序的序列中，查找第一个不小于给定值的元素；

`std::upper_bound` 在已排序的序列中，查找第一个大于给定值的元素；

`std::fill` 用给定值填充序列中的每个元素；

`std::fill_n` 用给定值填充序列中的n个元素；

`std::for_each` 将指定函数应用于范围内的每一个元素；

`std::generate` 对序列中的每个元素，用依次调用函数gen的返回值赋值；

`std::generate_n` 对序列中的n个元素，用依次调用指定函数gen的返回值赋值；

`std::includes` 判断第二个已排序的序列是否全部都出现在第一个已排序的序列中；

`std::inplace_merge` 对两个升序的序列执行原地合并，合并后的序列仍保持升序；

`std::merge` 对两个升序的序列合并，结果序列保持升序；

`std::is_heap` 判断序列是否为二叉堆；

`std::is_heap_until` 查找第一个不是堆顺序的元素；

`std::make_heap` 对于一个序列，构造一个二叉堆；

`std::pop_heap` 堆的根节点被移除，堆的元素数目减1并保持堆性质；

`std::push_heap` 向堆中增加一个新元素，新元素最初保存在last-1位置；

`std::sort_heap` 对一个堆，执行原地堆排序，得到一个升序结果；

`std::is_partitioned` 判断序列是否按指定谓词划分过；

`std::partition` 对序列重排，使得满足谓词的元素位于最前；

`std::partition_copy` 输入序列中，满足谓词的元素复制到result_true，其它元素复制到result_false；

`std::partition_point` 输入序列已经是partition，折半查找到分界点；

`std::stable_partiton` 对序列重排，使得满足谓词的元素在前，不满足谓词的元素在后，且两组元素内部的相对顺序不变；

`std::is_permutation` 判断两个序列是否为同一元素的两个排列；

`std::next_permutation` n个元素有n!中排列。这些排列中，规定升序序列为最小排列，降序序列为最大的排列，任意两个排列按照字典序分出大小。该函数返回当前序列作为一个排列按字典序的下一个排列；

`std::prev_permutation` 返回当前序列作为一个排列按字典序的上一个排列；

`std::is_sorted` 判断序列是否为升序；

`std::is_sorted_until` 查找序列中第一个未排序的元素；

`std::nth_element` 对序列重排，使得指定的位置出现的元素就是有序情况下应该在该位置出现的那个元素，且在指定位置之前的元素都小于指定位置元素，在指定位置之后的元素都大于指定位置元素；

`std::partial_sort` 对序列进行部分排序；

`std::partial_sort_copy` 拷贝部分排序的序列；

`std::sort` 对序列进行排序；

`std::stable_sort` 对序列进行稳定排序；

`std::iter_swap` 交换两个迭代器指向的元素；

`std::swap` 交换两个对象，优先使用移动语义；

`std::swap_ranges` 交换两个序列中对应元素；

`std::lexicographical_compare` 对两个序列做字典比较，如果第一个序列在字典序下小于第二个序列，则返回true；

`std::min`  返回两个值中的最小值；

`std::min_element` 返回序列中的最小值；

`std::max` 返回两个值中的最大值；

`std::max_element` 返回序列中的最大值；

`std::minmax` 返回由最小值与最大值构成的 `std::pair`；

`std::minmax_element` 返回由序列中最小元素与最大元素构成的 `std::pair` ；

`std::mismatch`  比较两个序列的对应元素，返回用 `std::pair` 表示的第一处不匹配在两个序列的位置；

`std::move` 把输入序列中的逐个元素移动到结果序列；注意与   http://blog.csdn.net/fengbingchun/article/details/52558914 中的不同；

`std::move_backward` 把输入序列中的逐个元素自尾到头移动到结果序列；

`std::shuffle` 使用均匀随机数生成器，随机打乱指定范围中的元素的位置；

`std::random_shuffle` n个元素有!n个排列，该函数给出随机选择的一个排列；

`std::remove` 删除序列中等于给定值的所有元素；

`std::remove_if` 删除序列中满足给定谓词的元素；

`std::remove_copy` 把一个序列中不等于给定值的元素复制到另一个序列中；

`std::remove_copy_if` 把一个序列中不满足给定谓词的元素复制到另一个序列中；

`std::replace` 把序列中等于给定值的元素替换为新值；

`std::replace_if` 把序列中满足给定谓词的元素替换为新值；

`std::replace_copy` 拷贝序列，对于等于老值的元素复制时使用新值；

`std::replace_copy_if` 拷贝序列，对于满足给定谓词的元素复制时使用新值；

`std::reverse` 把序列中的元素逆序；

`std::reverse_copy` 拷贝序列的逆序到另一个序列中；

`std::rotate` 等效于循环左移序列，使得迭代器middle所指的元素成为首元素；

`std::rotate_copy` 等效于循环左移序列并拷贝到新的序列中，使得迭代器middle所指的元素成为首元素；

`std::set_difference` 两个升序序列之差；

`std::set_intersection` 两个升序序列的交；

`std::set_symmetric_difference` 两个升序序列的对称差；

`std::set_union` 两个升序序列的并；

`std::transform` 对序列中的每一个元素，执行一元操作，结果写入另一序列中；或对两个序列中对应的每一对元素，执行二元操作，结果写入另一序列中；

`std::unique` 对序列中一群连续的相等的元素，仅保留第一个元素；

`std::unique_copy` 把一个序列中的元素拷贝到另一个序列，对于一群连续的相等的元素，仅拷贝第一个元素。
 


  














