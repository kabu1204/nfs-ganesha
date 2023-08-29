import os

dir = "/home/ycy/repo/shared/"
N = 1000

if __name__=="__main__":
	for i in range(N):
		filename = dir + "f{}.txt".format(i+1)
		open(filename, "a").close()
