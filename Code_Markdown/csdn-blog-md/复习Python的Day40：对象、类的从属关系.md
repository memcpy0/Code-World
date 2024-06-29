# 复习Python的第四十天

## 习题 45:  对象、类、以及从属关系
从整个计算机科学的角度来说，**对象是对客观事物的抽象，类是对对象的抽象，大类是对小类的抽象**。类和对象都是一种抽象的数据类型。

对象(Object)：是指在应用问题中出现的各种实体、事件和规格说明等，它是由**一组属性和在这组值上的一组服务**（这里的服务指的是操作，就是我们写在类里的函数所提供的功能）构成的，其中属性值确定了对象的状态。

类(Class)：把**具有相同属性和服务的对象归到同一类**，而把一个类中的的每一个对象称为该类的一个实例(Instance)，它们具有相同的服务。

继承：面向对象方法最有特色的方面。

继承性(Inheritance)是指，在某种情况下，一个类会有“子类”。子类比原本的类（称为父类）要更加具体化、定制化。

在书中，作者举了鱼和泥鳅以及小方的例子，鱼是一个类，泥鳅也是一个类，小方是泥鳅的实例(对象)，泥鳅是鱼的实例(或者更准确的说是子类)。

>贴代码：标明它是“is-a”还是“has-a”的**关系**，并讲明白这个关系是什么，记住，“是啥”指的是鱼和泥鳅的关系，而“有啥”指的是泥鳅和鳃的关系。
```python
## Animal is-a object class(yes, sort of confusing) look at the extra credit

class Animal(object):
	pass

## Dog is-a Animal class 
class Dog(Animal):
	
	def __init__(self, name):
		## Dog has-a name 
		self.name = name

## Cat is-a Animal class
class Cat(Animal):
	
	def __init__(self, name):
		## Cat has-a name 
		self.name = name

## Person is-a object class
class Person(object):
	
	def __init__(self, name):
		## Person has-a name 
		self.name = name
		
		## Person has-a pet of some kind
		self.pet = None # 默认属性

## Employee is-a Person class
class Employee(Person):
	def __init__(self, name, salary):
		## Employee inherit Person hmm what is this strange magic?
		super(Employee, self).__init__(name)
		
		## Employee has-a salary
		self.salary = salary

## Fish is-a object class
class Fish(object):
	pass

## Salmon is-a Fish class
class Salmon(Fish):
	pass

## Halibut is-a Fish class
class Halibut(Fish):
	pass

## rover is-a Dog
rover = Dog("Rover")

## satan is-a Cat
satan = Cat("Satan")

## mary is-a Person
mary = Person("Mary")

## mary has a pet Cat named satan
mary.pet = satan 

## frank is-a Employee
frank = Employee("Frank", 120000)

## frank has-a pet Dog named rover
frank.pet = rover

## flipper is-a Fish
flipper = Fish()

## crouse is-a Salmon Fish
crouse = Salmon()

## harry is-a Halibut Fish
harry = Halibut()
```
通过这段代码，记住，类既可以在对象上定义，也可以在其他类上定义（体现继承性）。

### 本节课涉及的知识
#### 关于super()
```python
super(Employee, self).__init__(name)
```
super函数是在Python2.2后被添加的函数，用来实现类的继承。

下面是Python3的文档：
```python
class super(object)
 |  super() -> same as super(__class__, <first argument>)
 |  super(type) -> unbound super object
 |  super(type, obj) -> bound super object; requires isinstance(obj, type)
 |  super(type, type2) -> bound super object; requires issubclass(type2, type)
 |  Typical use to call a cooperative superclass method:
 |  class C(B):
 |      def meth(self, arg):
 |          super().meth(arg)
 |  This works for class methods too:
 |  class C(B):
 |      @classmethod
 |      def cmeth(cls, arg):
 |          super().cmeth(arg)
```
super(B, self).init()是这样实现继承的：super(B, self)首先找B的父类（就是类A），然后把类B的对象self转换为类A的对象，然后被转换后的类A对象调用自己的init函数。

super这种用法只有指明子类的机制，虽然是一种解决缺陷的办法但是可能也存在问题。而多继承类定义中有其他的方式。再加上Python2和Python3又不一样，所以我不打算在本书的这里多做功夫，以后会讲更多。

#### 关于class Name(object)
> 真正的原因是在 Python 早期，它对于class的定义在很多方面都是严重有问题的。当他们承认这一点的时候已经太迟了，所以逼不得已，他们需要支持这种有问题的class。为了解决已有的问题，他们需要引入一种“新类”，这样的话“旧类”还能继续使用，而你也有一个新的正确的类可以使用了。

>这就用到了“类即是对象”的概念。**他们决定用小写的“object”这个词作为一个类，让你在创建新类时从它继承下来**。有点晕了吧？一个类从另一个类继承，而后者虽然是个类，但名字却叫“object”……不过在定义类的时候，别忘记要从 object继承就好了。

一个词的不同就让这个概念变得更难理解，真是！还能说什么呢？！现在我们可以试着去理解“一个是对象的类”这个概念了。所以假设Python的class永远都要求你加上 (object) 好了。

