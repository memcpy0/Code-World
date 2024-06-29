#include <bits/stdc++.h>
using namespace std;
/*
Splay是一种自组织的数据结构,当我们在使用这个数据结构的时候,我们可以通过一些情况来调整这个数据结构
,比如使用频率等
输入法的候选框就可以用SPLAY实现 
向CACHE一样,对于经常用的数据越用访问得越快

伸展树的核心操作是伸展——把一个结点通过旋转调整到某个结点处（一般是伸展到根结点）
旋转就是左旋(ZAG)和右旋(ZIG)
伸展操作非常灵活，可以用它完成许多树的操作，比如分裂、合并、区间操作、LCT等 

一直用单旋转,伸展完后又是一条链.
双旋转：一字型(同构调整) 
①对G Zig，对P Zig(ZIG-ZIG) 
     G
    /
   P
  /
 X 
②对G Zag，对P Zag 
 G
  \
   P
   \
    X 
之字形：异构调整
①对P ZAG, 对G ZIG(ZAG-ZIG) 
     G
    /
   P
    \ 
     X 
②对P ZIG, 对G ZAG(ZIG-ZAG)
   G
   \
    P
   / 
  X 
*/ 
int main() {
    freopen(".in","r",stdin);
    freopen(".out","w",stdout);

    return 0;
}

