import os,sys
import shutil
import fileinput

src = os.getcwd() + "\\template\\makefile.txt"
dest = os.getcwd() + "\\makefile"

shutil.copy(src, dest)

text_to_search = "template"
replacement_text = sys.argv[1]
with fileinput.FileInput(dest, inplace=True) as file:
    for line in file:
        print(line.replace(text_to_search, replacement_text), end='')

