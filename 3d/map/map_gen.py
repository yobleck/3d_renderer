#hex to rgb
#https://stackoverflow.com/questions/3723846/convert-from-hex-color-to-rgb-struct-in-
import sys;
import numpy;
numpy.set_printoptions(threshold=sys.maxsize); #default is    np.set_printoptions(threshold=1000);
    
array = numpy.empty([100,100,100], dtype=int);
#print(array);

for x in range(100):
    for y in range(100):
        for z in range(100):
            if(x==0):
                array[x][y][z]=1; #bottom
            elif(x==99):
                array[x][y][z]=2; #top
            elif(y==0):
                array[x][y][z]=3; #front
            elif(y==99):
                array[x][y][z]=4; #back
            elif(z==0):
                array[x][y][z]=5; #left
            elif(z==99):
                array[x][y][z]=6; #right
            else:
                array[x][y][z]=0; #default case

print(array);

with open("map.wtf","w") as outfile:
    for i in array:
        numpy.savetxt(outfile, i, fmt="%.4g"); #cant handle 3d array     https://stackoverflow.com/questions/3685265/how-to-write-a-multidimensional-array-to-a-text-file
        outfile.write("#new slice\n");
