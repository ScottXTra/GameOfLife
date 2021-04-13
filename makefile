LIBS = -F/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/ -framework OpenGL -framework GLUT -lm -Wno-deprecated-declarations

# Old MACOS
# framework information for older version of MACOS
#LIBS = -F/System/Library/Frameworks -framework OpenGL -framework GLUT -lm

# LINUX - Note that these will probably work but they can differ depending
# on your distribution.
#LIBS = -lGL -lGLU -lglut -lm -D__LINUX__

all: src/sim.c 
	gcc src/sim.c  -o bin/run $(LIBS) -O3 -pthread
	
clean:
	