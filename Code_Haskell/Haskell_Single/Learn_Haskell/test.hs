double x x = x + x
doublexy x y = doublex x + doublex y
numbers = [1, 3, 4, 5]

boomborg ls = [if x < 10 then "boom" else "borg" | x <- ls, odd x]

myLength ls = sum [1 | _ <- ls]

lst = [[1, 3, 5, 7, 9], [2, 4, 6, 8], [3, 6, 12]]
oddNumsList = [[x | x <- ls, odd x] | ls <- lst] 

type Fraction = (Integer, Integer)
rateq :: Fraction -> Float
rateq (a, b) = (fromIntegral a) / fromIntegral  b
