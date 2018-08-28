#!/usr/bin/env python
import os,shutil,string
import sys
pathCur= os.getcwd()
print pathCur
#库文件路径
spath = pathCur + os.path.sep+"..\\" + "frameworks\\runtime-src\\proj.win32\\Debug.win32" 
print "res ====   " + spath
#资源目录
resPath = pathCur + os.path.sep+"..\\"
print "resPath ===== " + resPath
#目标位置
dPath = pathCur + os.path.sep+ "..\\..\\" + "win32"
print "desc====   " + dPath

#删除旧资源
if os.path.isdir(dPath):
    shutil.rmtree(dPath)

os.mkdir(dPath)

#拷库文件
items = os.listdir(spath)
newlist = []
for names in items:
    if names.endswith(".lib") or names.endswith(".dll") or names.endswith("libcocos2d.pdb"):
        tempFile = os.path.join(spath,names)
        print( tempFile)
        if os.path.isfile(tempFile):
            newlist.append(names)
            shutil.copy2(tempFile, dPath)
        else:
            print("%s 不是文件" % (names))
print newlist
print len(newlist)

#拷脚本与资源
newlist = []
items = os.listdir(resPath)
for names in items:
    if names.endswith("Resources"):
        tempFile = os.path.join(resPath,names)    
        if os.path.isdir(tempFile):
            newlist.append(names)
            print("%s    os dir" % (tempFile))
            DestPath = dPath + os.path.sep+ names
            print("%s " % (DestPath))
            shutil.copytree(tempFile,DestPath )


#拷执行文件
temp = pathCur + os.path.sep+"..\\" + "simulator\win32"
items = os.listdir(temp)
print items
for names in items:
    if names.endswith("MahjongCD.exe"):
        newlist.append(names)
        temp = temp + os.path.sep + names
        print temp
        shutil.copy2(temp, dPath)

print newlist
print len(newlist)
