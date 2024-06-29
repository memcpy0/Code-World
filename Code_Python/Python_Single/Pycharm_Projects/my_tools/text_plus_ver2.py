# -*- coding : utf-8 -*-
# CMD version

import os
import sys
from encode_converter import convert

def path_pocket(root):
    try:
        filenames = os.listdir(root)
        paths = [root + '/' + filename for filename in filenames]
        return paths
    except Exception as e:
        print("The error is %s" % e)

def read_text(paths):
    try:
        #for path in paths:
        #    with open(path, encoding='utf-8') as f:
        #        f_text = f.read()
        #       text_body.append(f_text)
        text_body = [open(path, encoding='utf-8').read() for path in paths]
        return text_body
    except:
        print("The text can't be read!")

def write_together(text_body):
    dir_directory = "C:/Users/dell/Desktop"
    dir_file = dir_directory + '/' + root.split('/')[-1] + "new" + '.txt'

    for text in text_body:
        with open(dir_file, 'a', encoding='utf-8') as f:
            f.write(text)
            # print(text[0:22])
            f.close()
    print("Good!")

if __name__ == "__main__":
    if sys.argv[1]:
        script, root = sys.argv
    else:
        root = input("Please input the root with / instead of \\")

    if os.path.exists(root) and os.path.isdir(root):
        paths = path_pocket(root)
    else:
        print("The root is not existed!")

    text_body = read_text(paths)
    write_together(text_body)







