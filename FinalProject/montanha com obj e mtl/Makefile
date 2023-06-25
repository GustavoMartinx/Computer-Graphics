all:
	gcc -o app glad.o main.c glm.c glmdraw.c stb_loader.c util/readtex.c util/shaderutil.c util/trackball.c -lGLU -lGL -lglut -lGLEW -lm -lglfw
	./app
	rm -rf app