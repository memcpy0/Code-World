# -*- coding = utf-8 -*-
import os

root = "C:/Users/dell/Desktop/[魔法科高中的劣等生01-23卷][txt版]"

filenames = os.listdir(root)
# print(filenames)

paths = [root + '/' + filename for filename in filenames]
# print(paths)

text_body = []
for path in paths:
    txt = open(path, encoding='utf-8').read()
    text_body.append(txt)

dir_directory = "C:/Users/dell/Desktop"
dir_file = dir_directory + '/' + root.split('/')[-1] + "new" + '.txt'

for text in text_body:
    with open(dir_file, 'a', encoding='utf-8') as f:
        f.write(text)
    # print(text[0:22])
        f.close()
