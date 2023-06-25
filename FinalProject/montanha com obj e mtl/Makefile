all:
	gcc -o app main.c glad.o glm.c glmdraw.c stb_loader.c util/readtex.c util/shaderutil.c util/trackball.c shader.o -lGLU -lGL -lglut -lGLEW -lm -lglfw -lstdc++

comp:
	gcc -c glad.c -o glad.o
	gcc -c glm.c -o glm.o
	gcc -c glmdraw.c -o glmdraw.o
	gcc -c stb_loader.c -o stb_loader.o
	gcc -c util/readtex.c -o readtex.o
	gcc -c util/shaderutil.c -o shaderutil.o
	gcc -c util/trackball.c -o trackball.o
	gcc -c shader.c -o shader.o

	gcc -o app main.c glad.o glm.o glmdraw.o stb_loader.o readtex.o shader.o shaderutil.o trackball.o -lGLU -lGL -lglut -lGLEW -lm -lglfw -lstdc++

del:
	rm -rf glad.o
	rm -rf glm.o
	rm -rf glmdraw.o
	rm -rf stb_loader.o
	rm -rf readtex.o
	rm -rf shaderutil.o
	rm -rf trackball.o
	rm -rf shader.o
	rm -rf app