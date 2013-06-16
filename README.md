Dutch Nao Team
==========================================
Based on the source code of team Humboldt.


1. Some things before compile
==========================================

* toolchain.sh once you clone the repository, you have to run the toolchain script.
this script is going to download and install the toolchain needed for 
compiling the code. 
If you have any problems with opencv installation, replace opencv.sh in folder:
"dnt/NaoTH2011-light/NaoTHToolChain_Ubuntu64/toolchain_native/extern/install_scripts"
with the script in folder "dnt/additional", this script installs the version 2.4.3.


* naoqisdk.sh to download the proper sdk for 
compiling the code for the robot.

* webots.sh to download and extract the webots 6.3.3 version
on your repository. 


* XabslEditor.sh for the download of the XabslEditor source code.
After running the script you have to compile the project through 
Netbeans and run the .jar file inside dist/ directory.


2. Ready to compile
=========================================
visit https://github.com/IntelligentRoboticsLab/dnt/wiki
see how you can compile the code for your pc and for the
robot. 

3. Issues & Documentation
=========================================
Documentation is important, feel free to add whatever you think that is
interesting-helpful for other people to know about.
Issues are also used by our team members to keep track of problems, improvements, etc.

4. THE MOST IMPORTANT
=========================================
DO NOT push your code when it doesn't compile.
Especially to the master branch. The master branch contains the
complete bug-free code, which we are going to put to the robots.
