# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-11 17:05
program     : 学习线程模块中的队列
"""#

import queue

q = queue.Queue()

lfq = queue.LifoQueue() # 后进先出 栈
lfq.put(4)
lfq.put(3)
lfq.put(2)
print(lfq.get())   # 2

pq = queue.PriorityQueue()  # 优先队列 数值越小越优先 值相同就先进先出
pq.put((10, 'a'))  # （优先级, 数据）
pq.put((1, 'b'))
pq.put((2, 'c'))
print(pq.get())  # (1, 'b')
