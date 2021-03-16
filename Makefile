all: main

main: main.exe

main.exe: main.obj captureScreen.obj imageSolver.obj serialDriver.obj saveBitmapImage.obj vars.obj utils.obj
	link main.obj captureScreen.obj imageSolver.obj serialDriver.obj saveBitmapImage.obj vars.obj utils.obj

main.obj: main.cpp
	cl /EHsc /c main.cpp

captureScreen.obj: captureScreen/captureScreen.cpp captureScreen/captureScreen.h
	cl /EHsc /c captureScreen/captureScreen.cpp

imageSolver.obj: imageSolver/imageSolver.cpp imageSolver/imageSolver.h
	cl /EHsc /c imageSolver/imageSolver.cpp

serialDriver.obj: serialDriver/serialDriver.cpp serialDriver/serialDriver.h
	cl /EHsc /c serialDriver/serialDriver.cpp

vars.obj: vars/vars.cpp vars/vars.h
	cl /EHsc /c vars/vars.cpp

utils.obj: utils/utils.cpp utils/utils.h
	cl /EHsc /c utils/utils.cpp

saveBitmapImage.obj: saveBitmapImage/saveBitmapImage.cpp saveBitmapImage/saveBitmapImage.h
	cl /EHsc /c saveBitmapImage/saveBitmapImage.cpp

clean: cleanObj cleanOutput
	del main.exe

cleanObj:
	del *.obj

cleanOutput:
	del *.bmp