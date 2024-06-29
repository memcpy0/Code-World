f :: Num a => a -> a
f x = 4 * x + 1

area :: Double -> Double
area r = pi * r ^ 2

f2 :: Num a => (a, a) -> a
f2 (x, y) = 4 * x + 5 * y + 1

f2' :: Num a => a -> a -> a
f2' x y = 4 * x + 5 * y + 1

