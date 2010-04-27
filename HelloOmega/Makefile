
HelloOmega: HelloOmega.o
	g++-4.0 HelloOmega.o -framework GLUT -framework OpenGL -L/opt/local/lib -L/usr/local/lib -lobjc -lGLEW -lm -lcv -lhighgui -lcvaux -lcxcore -o HelloOmega

HelloOmega.o: HelloOmega.c
	g++-4.0 -c -o HelloOmega.o HelloOmega.c -I/System/Library/Frameworks/GLUT.framework/Headers -I/System/Library/Frameworks/OpenGL.framework/Headers -I/opt/local/include  -I/usr/local/include/opencv     
	
clean:
	rm -rf HelloOmega.o HelloOmega

.PHONY: clean