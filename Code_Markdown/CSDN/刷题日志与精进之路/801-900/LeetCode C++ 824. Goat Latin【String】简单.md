A sentence `S` is given, composed of words separated by spaces. Each word consists of lowercase and uppercase letters only. We would like to convert the sentence to `"Goat Latin"` (a made-up language similar to `"Pig Latin"` .)

The rules of Goat Latin are as follows:
-    If a word begins with a vowel (`a, e, i, o, u`), append `"ma"` to the end of the word. For example, the word `'apple'` becomes `'applema'` .
     
- If a word begins with a consonant (i.e. not a vowel), remove the first letter and append it to the end, then add `"ma"` . For example, the word `"goat"` becomes `"oatgma"` .
     
 - Add one letter `'a'` to the end of each word per its word index in the sentence, starting with `1` . For example, the first word gets `"a"` added to the end, the second word gets `"aa"` added to the end and so on.

Return the final sentence representing the conversion from `S` to Goat Latin. 

 

**Example 1:**
```swift
Input: "I speak Goat Latin"
Output: "Imaa peaksmaaa oatGmaaaa atinLmaaaaa"
```
**Example 2:**
```swift
Input: "The quick brown fox jumped over the lazy dog"
Output: "heTmaa uickqmaaa rownbmaaaa oxfmaaaaa umpedjmaaaaaa overmaaaaaaa hetmaaaaaaaa azylmaaaaaaaaa ogdmaaaaaaaaaa"
```
**Notes:**
- `S` contains only uppercase, lowercase and spaces. Exactly one space between each word.
- `1 <= S.length <= 150` .

题意：给定一个由空格分割单词的句子，将其转换为山羊拉丁文。

---
### 思路
分割这个句子为单词序列，然后对每个单词依次处理即可：
```cpp
class Solution {
private:
    void split(const string &s, vector<string> &token_list, char delim = ' ') {
        if (s.empty()) return;
        size_t beginPos = s.find_first_not_of(delim);
        size_t endPos = s.find_first_of(delim, beginPos);
        while (beginPos != string::npos) {
            token_list.push_back(s.substr(beginPos, endPos - beginPos));
            beginPos = s.find_first_not_of(delim, endPos);
            endPos = s.find_first_of(delim, beginPos);
        }
    }
public:
    string toGoatLatin(string s) {
        if (s.empty()) return string();
        vector<string> token_list;
        split(s, token_list, ' ');
        string ans;
        for (int i = 0; i < token_list.size(); ++i) {
            string &last = token_list[i];
            char c = tolower(last[0]);
            switch (c) {
                case 'a':
                case 'e':
                case 'i':
                case 'o':
                case 'u': //首字母开头是元音vowel
                    last.append("ma");
                    break;
                default: //首字母开头是辅音consonant
                    last.push_back(last[0]);
                    last.erase(0, 1);
                    last.append("ma");
                    break;
            }
            last.append(i + 1, 'a');
            if (i) ans.push_back(' '); //添加空格
            ans += last;
        }
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了59.68% 的用户
内存消耗：6.6 MB, 在所有 C++ 提交中击败了89.58% 的用户
```
