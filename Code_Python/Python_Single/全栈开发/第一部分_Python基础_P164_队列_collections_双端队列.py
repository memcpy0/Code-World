# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-27 00:12
program     : 学习双端队列
"""

import queue
from collections import deque

# 队列 先进先出
q = queue.Queue()
print(q.qsize())     # 0

q.put(1)
q.put('a')
q.put([1, 2, 3])
q.put(('1', 'x', 'y'))

# print(q.qsize())     # 4
# print(q.get())     # 1
#
# print(q.qsize())     # 3
# print(q.get())     # a
#
# print(q.qsize())     # 2
# print(q.get())     # [1, 2, 3]
#
# print(q.qsize())     # 1
# print(q.get())     # ('1', 'x', 'y')

# deque 双端队列 实现得没有queue.Queue严格
dq = deque()
dq.append('a')
dq.append('b')
dq.append('c')
print(dq)    # deque(['a', 'b', 'c'])

print(dq.popleft())
print(dq.popleft())
print(dq.popleft())
print(dq)    # deque([])

# pop 和 appendleft
# remove() # 能够删除指定元素
# insert()
# 实现双端队列底层的数据结构是链表, 插入删除很快 远远高于列表
# append和pop对于列表的效率没有影响

# namedtuple
# defaultdict
