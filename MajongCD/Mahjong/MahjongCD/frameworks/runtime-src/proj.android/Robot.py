#!/usr/bin/env python
"author:hcb"
"date:2017-8-4"
"仅资源与脚本的拷贝：从Resource到Assets"

import shutil
import string
import sys, os.path
import os
import re
import getpass
import datetime
curPath = os.getcwd()

def viewInfo():
    print(u' 请选择相应的功能: ')
    print(u"1、拷贝资源到android/assets(android)")
    print(u"2、清除小版本更新信息")
    print(u"3、生成win32运行环境")
    print(u"99、帮助")
    print(u"0：退出")
    print(u"\n")

#游戏名字
game = "chexuan"
#文件拷贝
def fileCopy(src,des):
    print src
    print des
    shutil.copyfile(src,des)
#目录拷贝
def dirCopy(src,des):
    if os.path.lexists(des):
	    shutil.rmtree(des)
    shutil.copytree(src,des)
    print("copy: %s \nto: %s\n is done!" %  (src,des))

#删除文件
def delFile(desFilName):
    if os.path.exists(desFilName):
        print(u"\n")
        os.remove(desFilName)
        

#删除目录
def delPath(des):
    if os.path.lexists(des):
        shutil.rmtree(des)		


if len(sys.argv) == 2:
	args = sys.argv[1]
else:
    args = ""

def readme():
    print(u"1、拷贝资源到android/assets(android):\n\t输入命令2或者android或者直接回车,把resource目录下的文件拷贝到android/assets")
	
def ResCopy():
    item = raw_input(u"select item:")
    condition = ""
    if item == "0":
        condition =0
    elif item.lower() == "" or item.lower()== "android" or item =="1":
        condition =1
    elif item == "2":
        condition = 2
    elif item == "3":
        condition = 3	
    elif item == "99" or item.lower() == "help":
        condition = 99
    	  
   #打印当前时间
    print u"当前时间戳:%s" % (datetime.datetime.now())
    if condition ==0:
        quit()
    elif condition ==1:
        print(u"拷贝资源到android/assets")
        #os.system("cls")
        _src = curPath + "\\..\\..\\..\\Resources"
        _des = curPath + "\\assets"
        dirCopy(_src, _des)
    elif condition ==2:
        #删除更新文件
        _src = curPath + "\\..\\..\\..\\"
        _des = _src + "assets"
        delPath(_des)
        _des = _src + "CheckList_Star.ini"
        delFile(_des)
    elif condition ==99:
        readme()
        print("\n")
if __name__ == "__main__":
    viewInfo()
    while True:
        ResCopy()