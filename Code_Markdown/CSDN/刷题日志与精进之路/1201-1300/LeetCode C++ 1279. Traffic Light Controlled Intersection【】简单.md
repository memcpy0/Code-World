<p>There is an intersection of two roads. First road is road A where cars travel&nbsp;from North to South in direction 1 and from South to North in direction 2. Second road is road B where cars travel from West to East in direction 3 and from East to West in direction 4.</p>

<p><img style="width: 600px; height: 417px;" src="https://assets.leetcode.com/uploads/2019/11/11/exp.png" alt=""></p>

<p>There is a traffic light&nbsp;located on each road before the intersection. A traffic light&nbsp;can&nbsp;either be green or red.</p>

<ol>
	<li><strong>Green</strong> means&nbsp;cars can cross the intersection&nbsp;in both directions of the road.</li>
	<li><strong>Red</strong> means cars in both directions cannot cross the intersection and must wait until the light turns green.</li>
</ol>

<p>The traffic lights cannot be green on both roads at the same time. That means when the light is green on road A, it is red on&nbsp;road B and when the light is green on road B, it is red on&nbsp;road A.</p>

<p>Initially, the traffic light is <strong>green</strong> on road A and <strong>red</strong>&nbsp;on road B. When the light is green on one road, all cars can cross the intersection in both directions until the light becomes green on the other road.&nbsp;No two cars traveling on different roads should cross at the same time.</p>

<p>Design a deadlock-free&nbsp;traffic light controlled system at this intersection.</p>

<p>Implement the function&nbsp;<code>void carArrived(carId, roadId, direction, turnGreen, crossCar)</code> where:</p>

<ul>
	<li><code>carId</code>&nbsp;is the id of the car that arrived.</li>
	<li><code>roadId</code>&nbsp;is the id of the road that the car travels&nbsp;on.</li>
	<li><code>direction</code>&nbsp;is the direction of the car.</li>
	<li><code>turnGreen</code>&nbsp;is a function you can call to turn the traffic light to green on the current road.</li>
	<li><code>crossCar</code>&nbsp;is a function you can call to let the current car cross the intersection.</li>
</ul>

<p>Your answer is considered correct if it avoids cars deadlock in the intersection.&nbsp;Turning the light green on a road when it was already green is considered a&nbsp;wrong answer.</p>

 
<p><strong>Example 1:</strong></p>

```haskell
Input: cars = [1,3,5,2,4], directions = [2,1,2,4,3], arrivalTimes = [10,20,30,40,50]
Output: [
"Car 1 Has Passed Road A In Direction 2",    // Traffic light on road A is green, car 1 can cross the intersection.
"Car 3 Has Passed Road A In Direction 1",    // Car 3 crosses the intersection as the light is still green.
"Car 5 Has Passed Road A In Direction 2",    // Car 5 crosses the intersection as the light is still green.
"Traffic Light On Road B Is Green",          // Car 2 requests green light for road B.
"Car 2 Has Passed Road B In Direction 4",    // Car 2 crosses as the light is green on road B now.
"Car 4 Has Passed Road B In Direction 3"     // Car 4 crosses the intersection as the light is still green.
] 
```

<p><strong>Example 2:</strong></p>

```haskell
Input: cars = [1,2,3,4,5], directions = [2,4,3,3,1], arrivalTimes = [10,20,30,40,40]
Output: [
"Car 1 Has Passed Road A In Direction 2",    // Traffic light on road A is green, car 1 can cross the intersection.
"Traffic Light On Road B Is Green",          // Car 2 requests green light for road B.
"Car 2 Has Passed Road B In Direction 4",    // Car 2 crosses as the light is green on road B now.
"Car 3 Has Passed Road B In Direction 3",    // Car 3 crosses as the light is green on road B now.
"Traffic Light On Road A Is Green",          // Car 5 requests green light for road A.
"Car 5 Has Passed Road A In Direction 1",    // Car 5 crosses as the light is green on road A now.
"Traffic Light On Road B Is Green",          // Car 4 requests green light for road B. Car 4 blocked until car 5 crosses and then traffic light is green on road B.
"Car 4 Has Passed Road B In Direction 3"     // Car 4 crosses as the light is green on road B now.
]
Explanation: This is a dead-lock free scenario. Note that the scenario when car 4 crosses before turning light into green on road A and allowing car 5 to pass is also correct and Accepted scenario. 
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= cars.length &lt;= 20</code></li>
	<li><code>cars.length = directions.length</code></li>
	<li><code>cars.length = arrivalTimes.length</code></li>
	<li>All values of <code>cars</code> are unique</li>
	<li><code>1 &lt;= directions[i] &lt;= 4</code></li>
	<li><code>arrivalTimes</code> is non-decreasing</li>
</ul>




题意：这是两条路的交叉路口。第一条路是 A 路，车辆可沿&nbsp;1 号方向由北向南行驶，也可沿&nbsp;2 号方向由南向北行驶。第二条路是 B 路，车辆可沿&nbsp;3 号方向由西向东行驶，也可沿 4 号方向由东向西行驶。 

<p>每条路在路口前都有一个红绿灯。红绿灯可以亮起红灯或绿灯。</p>

<ol>
	<li><strong>绿灯</strong>表示两个方向的车辆都可通过路口。</li>
	<li><strong>红灯</strong>表示两个方向的车辆都不可以通过路口，必须等待绿灯亮起。</li>
</ol>

<p>两条路上的红绿灯不可以同时为绿灯。这意味着，当 A 路上的绿灯亮起时，B 路上的红灯会亮起；当 B&nbsp;路上的绿灯亮起时，A&nbsp;路上的红灯会亮起.</p>

<p>开始时，A 路上的<strong>绿灯</strong>亮起，B 路上的<strong>红灯</strong>亮起。当一条路上的绿灯亮起时，所有车辆都可以从任意两个方向通过路口，直到另一条路上的绿灯亮起。不同路上的车辆不可以同时通过路口。</p>

<p>给这个路口设计一个没有死锁的红绿灯控制系统。即实现函数&nbsp;<code>void carArrived(carId, roadId, direction, turnGreen, crossCar)</code>&nbsp;:</p>

<ul>
	<li><code>carId</code>&nbsp;为到达车辆的编号。</li>
	<li><code>roadId</code>&nbsp;为车辆所在道路的编号。</li>
	<li><code>direction</code>&nbsp;为车辆的行进方向。</li>
	<li><code>turnGreen</code>&nbsp;是一个函数，调用此函数会使当前道路上的绿灯亮起。</li>
	<li><code>crossCar</code>&nbsp;是一个函数，调用此函数会允许车辆通过路口。</li>
</ul>

<p>当你的答案避免了车辆在路口出现死锁，此答案会被认定为正确的。当路口已经亮起绿灯时仍打开绿灯，此答案会被认定为错误的。</p>

 
