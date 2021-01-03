import os

if not os.path.exists("deps"):
    os.mkdir("deps")
os.chdir("deps")

os.system("git clone --branch 2.6.x --single-branch https://github.com/SFML/SFML.git")
