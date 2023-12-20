<h1 align='center'> CG </h1>
<p align='center'> Este repositório contém os três projetos práticos da disciplina de Computação Gráfica desenvolvidos na linguagem de programação C através da API gráfica OpenGL. <br>
Além disso, cada pasta deste repositório corresponde a um projeto, sendo assim, em todos os diretórios encontram-se arquivos Readme.md com mais informações sobre cada um deles.</p>

<br>

## Trabalho 2 - Viewing 3D
Este trabalho consiste em dividir a janela do OpenGL quatro viewports, onde três projeções são ortogonais e uma (inferior direita) é perspectiva, cada uma com seu objeto 3D e sua câmera. O objetivo é manipular as câmeras de acordo com especificações fornecidas ajustando configurações como sua posição, para onde está apontando, planos near e far, vetor view up. Bem como os outros projetos deste repositório, este também foi desenvolvido por meio da API gráfica OpenGL, em C.

### Como Executar

1. Instale o OpenGL. Para mais informações, clique [aqui](https://edisciplinas.usp.br/pluginfile.php/4264396/mod_resource/content/1/guia-de-instalacao.pdf)

2. Clone este repositório
```
git clone https://github.com/GustavoMartinx/computer-graphics.git
```

3. Entre no diretório `trab2`
```
cd trab2/
```

4. Compile com as bibliotecas e execute
```
gcc main.c -lGL -lglut -lGLU -o main && ./main
```

## :camera: Preview do Projeto 2
![Preview pratica 2](FinalProject/img/v2/videos/trab2.gif)

<!-- ====================================================================-->



<br>

## Trabalho 1 - Transformações Geométricas 2D
Este trabalho tem como objetivo executar transformações geométricas em duas dimensões através da API gráfica OpenGL, em C.

### Como Executar

1. Instale o OpenGL. Para mais informações, clique [aqui](https://edisciplinas.usp.br/pluginfile.php/4264396/mod_resource/content/1/guia-de-instalacao.pdf)

2. Clone este repositório
```
git clone https://github.com/GustavoMartinx/computer-graphics.git
```

3. Entre no diretório `trab1`
```
cd trab1/
```

4. Compile com as bibliotecas e execute
```
gcc main.c -lGL -lglut -lGLU -o main && ./main
```

<br>

### :small_blue_diamond: Funcionalidades - Transformações Geométricas

#### Translação
- Pressione a tecla `t` para entrar no modo de Translação;
- `ArrowKeyUp` translada o objeto na direção do eixo Y positivo;
- `ArrowKeyDown` translada o objeto na direção do eixo Y negativo;
- `ArrowKeyRight` translada o objeto na direção do eixo X positivo;
- `ArrowKeyLeft` translada o objeto na direção do eixo X negativo;

#### Rotação
- Pressione a tecla `r` para entrar no modo de Rotação;
- `ArrowKeyRight` rotaciona o objeto na direção horária;
- `ArrowKeyLeft` rotaciona o objeto na direção anti-horária;

#### Escala
- Pressione a tecla `s` para entrar no modo de Escala;
- `ArrowKeyUp` aumenta a escala do objeto;
- `ArrowKeyDown` diminui a escala do objeto;


## :small_blue_diamond: Preview do Projeto 1
![Preview pratica 1](FinalProject/img/v2/videos/trab1.gif)


## :mortar_board: Autores

<table style="flex-wrap: wrap; display: flex; align-items: center;  flex-direction: column;" ><tr>


<td align="center"><a href="https://github.com/Fgarm">
 <img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/69016293?v=4" width="100px;" alt=""/>
<br />
 <b>Guilherme<br>Maturana</b></a>
 <a href="https://github.com/Fgarm" title="Repositorio Guilherme Maturana"></a>
</td>

<td align="center"><a href="https://github.com/GustavoMartinx">
 <img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/90780907?v=4" width="100px;" alt=""/>
<br />
 <b>Gustavo<br>Martins</b>
 </a> <a href="https://github.com/GustavoMartinx" title="Repositorio Gustavo Martins"></a>
</td>

<td align="center"><a href="https://github.com/RenanGAS">
 <img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/68087317?v=4" width="100px;" alt=""/>
<br />
 <b>Renan<br>Sakashita
</b>
 </a> <a href="https://github.com/RenanGAS" title="Repositorio Renan Sakashita"></a>

</td>

</tr></table>