lenOfBinary :: Integer -> Integer 
lenOfBinary 0 = 1
lenOfBinary 1 = 2
lenOfBinary n = lenOfBinary (n - 1) + lenOfBinary(n - 2) + 2 ^ (n - 2)