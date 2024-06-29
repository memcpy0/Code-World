-- uncurried function
add :: (Int, Int) -> Int
add (x, y) = x + y

-- curried function
add' :: Int -> Int -> Int
add' x y = x + y

a :: Int
a = 5 

main = putStrLn "Hello, World!"