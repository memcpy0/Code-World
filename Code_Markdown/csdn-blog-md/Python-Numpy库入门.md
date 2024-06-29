# Numpy库入门
[TOC]

## 0、引入
`import numpy as np`
## 1、 Numpy中的数组对象ndarray
**ndarray**在程序中的别名是array，用**np.array() 生成一个ndarray数组**。
输出成[ ]形式，元素由空格分割。
如：
```python
>>> a = np.array([[0,1,2,3,4],
             [9,8,7,6,5]])  
>>> print(a)  
[[0 1 2 3 4]
 [9 8 7 6 5]]             
```
注意：np.ndarray和np.array是不一样的，前者是`class ndarray in module numpy`，是类；后者是`built-in function array in module numpy.core.multiarray`，是函数。一般用后者将array-like对象(包括列表和元组)转换为数组对象，创建的数组是`numpy.ndarray`数组。

看np.array()的文档：
```python
array(...)
    array(object, dtype=None, copy=True, order='K', subok=False, ndmin=0)
    
    Create an array.
    
    Parameters
    ----------
    object : array_like
        An array, any object exposing the array interface, an object whose
        __array__ method returns an array, or any (nested) sequence.
    dtype : data-type, optional
        The desired data-type for the array.  If not given, then the type will
        be determined as the minimum type required to hold the objects in the
        sequence.  This argument can only be used to 'upcast' the array.  For
        downcasting, use the .astype(t) method.
    copy : bool, optional
        If true (default), then the object is copied.  Otherwise, a copy will
        only be made if __array__ returns a copy, if obj is a nested sequence,
        or if a copy is needed to satisfy any of the other requirements
        (`dtype`, `order`, etc.).
    order : {'K', 'A', 'C', 'F'}, optional
        Specify the memory layout of the array. If object is not an array, the
        newly created array will be in C order (row major) unless 'F' is
        specified, in which case it will be in Fortran order (column major).
        If object is an array the following holds.
    
        ===== ========= ===================================================
        order  no copy                     copy=True
        ===== ========= ===================================================
        'K'   unchanged F & C order preserved, otherwise most similar order
        'A'   unchanged F order if input is F and not C, otherwise C order
        'C'   C order   C order
        'F'   F order   F order
        ===== ========= ===================================================
    
        When ``copy=False`` and a copy is made for other reasons, the result is
        the same as if ``copy=True``, with some exceptions for `A`, see the
        Notes section. The default order is 'K'.
    subok : bool, optional
        If True, then sub-classes will be passed-through, otherwise
        the returned array will be forced to be a base-class array (default).
    ndmin : int, optional
        Specifies the minimum number of dimensions that the resulting
        array should have.  Ones will be pre-pended to the shape as
        needed to meet this requirement.
    
    Returns
    -------
    out : ndarray
        An array object satisfying the specified requirements.
```
这里透漏出了np.array的具体参数，不多做实验。

## 2、 ndarray对象的属性
> 注：轴(axis) : 保存数据的维度

- `ndim`：秩(rank)，轴的数量，即为轴(维度)的数量
- `shape`：ndarray对象的尺度，即为矩阵的n行m列
- `size`：ndarray对象的元素个数，相当于shape中n*m的值
- `dtype`：ndarray对象的元素类型
- `itemsize` : ndaray对象中每个元素的大小，以字节为单位
> The Codes Example
```python
>>> a = np.array([[0,1,2,3,4],
             [9,8,7,6,5]])  
>>> a
array([[0, 1, 2, 3, 4],
       [9, 8, 7, 6, 5]])
>>> a.ndim  # 数组维度:a为两维数组
2 
>>> a.shape 
(2, 5)
>>> a.size  # 元素个数
10
>>> a.dtype # 元素类型
dtype('int32')
>>> a.itemsize 
4
>>> a.T  # 矩阵的转置
array([[0, 9],
       [1, 8],
       [2, 7],
       [3, 6],
       [4, 5]])
>>> a.imag # 虚部数组
array([[0, 0, 0, 0, 0],
       [0, 0, 0, 0, 0]])
>>> a.real # 实部数组
array([[0, 1, 2, 3, 4],
       [9, 8, 7, 6, 5]])
>>> a.flat # 返回其迭代器
<numpy.flatiter at 0x128d235e650>
```
我们详细看一下ndarray类的属性：
```python
Attributes
 |  ----------
 |  T : ndarray
 |      Transpose of the array.
 |  data : buffer
 |      The array's elements, in memory.
 |  dtype : dtype object
 |      Describes the format of the elements in the array.
 |  flags : dict
 |      Dictionary containing information related to memory use, e.g.,
 |      'C_CONTIGUOUS', 'OWNDATA', 'WRITEABLE', etc.
 |  flat : numpy.flatiter object
 |      Flattened version of the array as an iterator.  The iterator
 |      allows assignments, e.g., ``x.flat = 3`` (See `ndarray.flat` for
 |      assignment examples; TODO).
 |  imag : ndarray
 |      Imaginary part of the array.
 |  real : ndarray
 |      Real part of the array.
 |  size : int
 |      Number of elements in the array.
 |  itemsize : int
 |      The memory use of each array element in bytes.
 |  nbytes : int
 |      The total number of bytes required to store the array data,
 |      i.e., ``itemsize * size``.
 |  ndim : int
 |      The array's number of dimensions.
 |  shape : tuple of ints
 |      Shape of the array.
 |  strides : tuple of ints
 |      The step-size required to move from one element to the next in
 |      memory. For example, a contiguous ``(3, 4)`` array of type
 |      ``int16`` in C-order has strides ``(8, 2)``.  This implies that
 |      to move from element to element in memory requires jumps of 2 bytes.
 |      To move from row-to-row, one needs to jump 8 bytes at a time
 |      (``2 * 4``).
 |  ctypes : ctypes object
 |      Class containing properties of the array needed for interaction
 |      with ctypes.
 |  base : ndarray
 |      If the array is a view into another array, that array is its `base`
 |      (unless that array is also a view).  The `base` array is where the
 |      array data is actually stored.
```

## 3、 ndarray数组元素类型之非同质
有时ndarray中的元素不是同一种类型，即由非同质对象组成，这时ndarray的dtype又是什么类型呢？
```
>>> x = np.array([[9,8,7,6,5],
              [1,2,3,4]])  
>>> x # 非同质ndarray元素类型为object,无法有效发挥NumPy优势，尽量避免使用
array([[9, 8, 7, 6, 5], [1, 2, 3, 4]], dtype=object)
>>> x.shape
(2,)
>>> x.size  # This is a question！
2
>>> x.dtype
dtype('O')
```

## 4、 ndarray数组创建的多种方法
### (1) 方法一
从Python中的元组和列表等类型创建ndarray数组，当np.array()不指定dtype时，NumPy将根据数据情况关联一个dtype类型，即会给出满足序列对象的最小类型。
`If not given, then the type will be determined as the minimum type required to hold the objects in the sequence.`

用法：
`np.array(object, dtype=None, copy=True, order='K', subok=False, ndmin=0)`
```python
>>> x = np.array([0,1,2,3]) #从列表类型创建
[0 1 2 3]

>>> x = np.array((4,5,6,7)) #从元组类型创建
[4 5 6 7]

>>> x = np.array([[1,2],[9,8],(0.1,0.2)]) #从列表和元组混合类型创建
[[ 1.   2. ]
 [ 9.   8. ]
 [ 0.1  0.2]]
```

### (2) 方法二
使用NumPy中函数创建ndarray数组，如：

- arange
- ones
- zeros
- eye
- full
- ones_like
- zeros_like
- full_like (a)
- linspace()
- concatenate() !
