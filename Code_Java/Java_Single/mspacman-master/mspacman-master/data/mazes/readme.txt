The files containing the graphs are structured as follows:

First line is a header that contains information regarding the maze. The information is as follows:

name of maze
start node for Ms Pac-Man
node that corresponds to the lair
start node for the ghosts
number of nodes in the maze
number of pills in the maze
number of power pills in the maze
number of junctions in the maze

All other lines corresponds to individual nodes on the graph. Each node has the following information:

node index
x-coordinate	
y-coordinate
neighbouring node in UP direction (-1 if none)
neighbouring node in RIGHT direction (-1 if none)
neighbouring node in DOWN direction (-1 if none)
neighbouring node in LEFT direction (-1 if none)
pill-index of the node (-1 if none)
power-pill index of the node (-1 if none)

包含图形的文件结构如下：


第一行是包含迷宫信息的标题。信息如下：

迷宫名称
MS Pac Man的启动节点
对应于巢穴的节点
重影的开始节点
迷宫中的节点数
迷宫里的药丸数量
迷宫中的能量丸数量
迷宫中的交叉点数量

所有其他行对应于图上的各个节点。每个节点都有以下信息：

节点索引
横坐标
Y坐标
向上的相邻节点（如果没有，则为-1）
右方向的相邻节点（如果没有，则为-1）
向下的相邻节点（如果没有，则为-1）
左方向的相邻节点（如果没有，则为-1）
节点的药丸索引（如果没有则为-1）
节点的电源丸索引（如果没有则为-1）