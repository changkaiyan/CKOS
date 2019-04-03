import os
import sys
try:
    print("Input File:"+sys.argv[1])
    print("Output File:"+sys.argv[2])
except:
    print("Argument is not valid")
try:
    file = open(sys.argv[1], "rb")
    file2 = open(sys.argv[2], "wb")
except:
    print("文件异常!")
    sys.exit(0)
all = file.read()
if len(all)>510:
    print("文件大小超出512byte!")
    sys.exit(0);
file2.write(all)
for i in range(510-len(all)):
    file2.write(b'\x00')
file2.write(b'\x55\xaa')
try:
    file.close()
    file2.close()
except:
    print("文件关闭异常，可能不可以保存数据")
print("完成任务!")
