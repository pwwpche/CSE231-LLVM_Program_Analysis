#!/usr/bin/python
import shutil
import os
import time
import re

if not os.path.isfile("./231DFA.h"):
    print "Unable to find 231DFA.h!"
    exit(1)

if not os.path.isfile("./ReachingDefinitionAnalysis.cpp"):
    print "Unable to find ReachingDefinitionAnalysis.cpp!"
    exit(1)	

pid = raw_input("Input your PID (e.g. A12345678):")
pattern = r"^[AU][0-9]{8}$"
pattern_obj = re.compile(pattern)

result = pattern_obj.match(pid)
if result == None:
    print "Invalid PID " + pid + " entered."
    exit(0)

print "Submitting these files:"
print "231DFA.h"
print "ReachingDefinitionAnalysis.cpp"
print "With this PID:\""+pid+"\""

inp = raw_input("Are these the files/PID you want to turn in? (Yes/No)")

if inp == "Yes":
    #package
    print "Packaging up your turnin..."

    dirname = "/tmp/part2-" + pid
    if os.path.exists(dirname):
        print "Removing old turnin directory"
        shutil.rmtree(dirname)
    if os.path.exists(dirname+".tar.gz"):
        print "Removing old turnin .tar.gz"
        os.remove(dirname+".tar.gz")

    os.mkdir(dirname)
    os.system("cp ./231DFA.h " + dirname)
    os.system("cp ./ReachingDefinitionAnalysis.cpp " + dirname)
    time.sleep(1)
    puretarname = "part2-" + pid + ".tar.gz"
    fulltarname = "/tmp/" + puretarname 
    os.system("tar -czvf " + fulltarname + " " + dirname)
    print "Sending files..."
    r = os.system("printf \"\\nput " + fulltarname + " " + puretarname + "\\n\" | ftp -v -p vm134.sysnet.ucsd.edu | grep successfully")
    if r != 0:
        print "\n****ERROR Occurred during FTP upload. Please check network connectivity before contacting a TA."
    else:
        print "\nSuccess! Your turnin has been received!"
else:
    print "Turnin CANCELED. No files were sent."

