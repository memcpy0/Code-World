    --    |c=='0'=[" 000   ","00  0  ","0 0 0  ","0  00  "," 000   "]
    --    |c=='1'=["  1    "," 11    ","  1    ","  1    ","11111  "]
    --    |c=='2'=[" 222   ","2   2  ","   2   ","  2    ","22222  "]
    --    |c=='3'=["3333   ","    3  "," 333   ","    3  ","3333   "]
    --    |c=='4'=["   4   ","  44   "," 4 4   ","44444  ","   4   "]
    --    |c=='5'=["55555  ","5      ","55555  ","    5  ","55555  "]
    --    |c=='6'=["66666  ","6      ","66666  ","6   6  ","66666  "]
    --    |c=='7'=["77777  ","   7   ","  7    ","  7    ","  7    "]
    --    |c=='8'=["88888  ","8   8  ","88888  ","8   8  ","88888  "]
    --    |c=='9'=["99999  ","9   9  ","99999  ","    9  ","99999  "]
    --    |c==' '=["   ","   ","   ","   ","   "]
    --    |otherwise = error "out of the scope of A-Z or 0-9"


module AnvancedAsciiPictures (
    Picture,
    printPicture,       -- :: Picture -> IO()
    flipV,              -- :: Picture -> Picture 
    flipH,              -- :: Picture -> Picture
    above,              -- :: Picture -> Picture -> Picture
    sideBySide,         -- :: Picture -> Picture -> Picture
    tree                -- :: Picture
) where
import Data.Char

type Line = [Char]
-- 一个字符画由多行字符串组成
type Picture = [Line]

-- 一个黑白字符图形
tree :: Picture
tree = ["    ##    ",
        "  #####   ",
        " #######  ",
        "######### ",
        "   ##     ",
        "   ##     "]

pics :: [Picture]
pics = [
        ["  A    "," A A   ","A   A  ","AAAAA  ","A   A  "],
        ["BBBB   ","B   B  ","BBBB   ","B   B  ","BBBB   "],
        [" CCCC  ","C      ","C      ","C      "," CCCC  "],
        ["DDD    ","D  D   ","D   D  ","D  D   ","DDD    "],
        ["EEEEE  ","E      ","EEEEE  ","E      ","EEEEE  "],
        ["FFFFF  ","F      ","FFFF   ","F      ","F      "],
        [" GGGG  ","G      ","G  GG  ","G   G  "," GGG   "],
        ["H   H  ","H   H  ","HHHHH  ","H   H  ","H   H  "],
        ["IIIII  ","  I    ","  I    ","  I    ","IIIII  "],
        [" JJJJ  ","   J   ","   J   ","J  J   "," JJ    "],
        ["K  KK  ","K K    ","KK     ","K K    ","K  KK  "],
        ["L      ","L      ","L      ","L      ","LLLLL  "],
        ["M   M  ","MM MM  ","M M M  ","M M M  ","M M M  "],
        ["N   N  ","NN  N  ","N N N  ","N  NN  ","N   N  "],
        [" OOO   ","O   O  ","O   O  ","O   O  "," OOO   "],
        ["PPPP   ","P   P  ","PPPP   ","P      ","P      "],
        [" QQQ   ","Q   Q  ","Q   Q  ","Q  Q   "," QQ Q  "],
        ["RRRR   ","R   R  ","RR R   ","R R    ","R  RR  "],
        [" SSSS  ","S      ","SSSSS  ","    S  ","SSSS   "],
        ["TTTTT  ","  T    ","  T    ","  T    ","  T    "],
        ["U   U  ","U   U  ","U   U  ","U   U  ","UUUUU  "],
        ["V   V  ","V   V  ","V   V  "," V V   ","  V    "],
        ["W   W  ","W W W  ","W W W  ","W W W  "," W W   "],
        ["X   X  "," X X   ","  X    "," X X   ","X   X  "],
        ["Y   Y  "," Y Y   ","  Y    ","  Y    ","  Y    "],
        ["ZZZZZ  ","   Z   ","  Z    "," Z     ","ZZZZZ  "]
    ]


-- 显示黑白字符图形
printPicture :: Picture -> IO()
-- printPicture pic = putStr (unlines pic)
printPicture pic = (putStr . unlines) pic -- 函数的复合

-- 上下翻转图形
flipV :: Picture -> Picture
flipV pic = reverse pic

-- 水平翻转图形
flipH :: Picture -> Picture
-- flipH pic = [reverse line | line <- pic]
flipH pic = map reverse pic

-- 将两个图形上下拼接
above :: Picture -> Picture -> Picture
above p q = p ++ q

-- 将两个图形左右拼接
sideBySide :: Picture -> Picture -> Picture
sideBySide p [] = p
sideBySide [] q = q
sideBySide p q = [line1 ++ line2 | (line1, line2) <- zip p q]

-- 在图形pic的右侧拼接pic左右翻转图形,在下面分别拼接它们的上下翻转图形
square :: Picture -> Picture
square pic = above (pic `sideBySide` picH) (picV `sideBySide` picHV)
    where 
        picH = flipH pic
        picV = flipV pic
        picHV = flipV picH

-- 查找字符对应的图片
letter2pic :: Char -> Picture
letter2pic c = pics !! ordc
    where ordc = if c >= 'a' 
                then (ord c - ord 'a')                 
                else (ord c - ord 'A')

-- 将一个字符串转换成字符图形列表
string2pics :: String -> [Picture]
-- string2pics s = [letter2pic c | c <- s]
string2pics s = map letter2pic s

-- 将字符图形列表左右拼接为一个图形
concatPics :: [Picture] -> Picture
-- concatPics [] = []
-- concatPics (x : xs) = sideBySide x (concatPics xs)
concatPics xs = foldr sideBySide [] xs

-- 将一个字符串转换为一个字符图形
say :: String -> Picture
-- say s = concatPics (string2pics s)
say s = (concatPics . string2pics) s -- 函数的复合

-- 打印一个字符串为字符图形
sayit :: String -> IO ()
-- sayit s = printPicture (say s)
sayit s = (printPicture . say) s -- 函数的复合