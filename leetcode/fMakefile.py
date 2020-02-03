
# coding: utf-8

# In[16]:


import os,sys
import shutil
import fileinput



src = os.getcwd() + "\\template\\makefile"
dest = os.getcwd() + "\\makefile"


# In[17]:


shutil.copy(src, dest)


# In[18]:


text_to_search = "template"
replacement_text = sys.argv[1]
with fileinput.FileInput(dest, inplace=True) as file:
    for line in file:
        print(line.replace(text_to_search, replacement_text), end='')

