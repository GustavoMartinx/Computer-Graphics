<h1 align="center">:mount_fuji: Snowland  </h1>
<p align="center">
Intitulamos 'Snowland' o nosso Projeto Final da disciplina de Computação Gráfica cujo objetivo foi criar uma cena com objetos 3D por meio da API gráfica OpenGL na qual deveria conter interações com o teclado e/ou mouse, animações através de transformações geométricas, configurações de iluminação, remoção de superfícies ocultas, entre outros requisitos.<br><br>
Para tanto, foi utilizada a Linguagem de Programação C em conjunto à API gráfica OpenGL e para criar os objetos 3D foi utilizado o software Blender na versão 3.5 LTS.
</p>

<!-- <p align="center">
<img src="https://img.shields.io/tokei/lines/github/jhonatancunha/fat32_file_system?style=for-the-badge" alt="">
<img src="https://img.shields.io/github/license/jhonatancunha/fat32_file_system?color=blue&style=for-the-badge" alt="">
<img src="https://img.shields.io/github/last-commit/jhonatancunha/fat32_file_system?color=blue&style=for-the-badge" alt="">
</p>
:cloud:
<br> -->



<br>

## :evergreen_tree: Como executar

1. Clone este repositório
    ```bash
    git clone https://github.com/GustavoMartinx/Computer-Graphics.git
    ```

2. Acesse o diretório ``montanha com obj e mtl`` e execute este comando para compilar o código-fonte em ``C``
    ```ps
    gcc -o app main.c glm.c glmdraw.c util/readtex.c util/shaderutil.c util/trackball.c -lGLU -lGL -lglut -lGLEW -lm
    ```

3. Por fim inicie o executável
    ```bash
    ./app
    ```

## :snowflake: Funcionalidades

#### Controle da câmera
- `w` `a` `s` `d` movimentam a câmera nos eixos X e Z;
- `[espaço]` e `q` movimentam a câmera no eixo Y;
- com `CapsLock` ativo, as telas acima exercem as mesmas funções, porém com maior velocidade;
- com o `mouse` é possível controlar a direção na qual a câmera aponta;
- `c` move a câmera para uma posição pré configurada próxima à cabana;
- `u` move a câmera para a posição padrão de visão.

#### Animações Geométricas
- `p` abre e fecha a porta da cabana;
- `b` cria uma bola de neve e a faz cair montanha a baixo;
- quando tens `fe`, moves montanhas com `i` `j` `k` `l`;
- `lu` ativa as configurações de iluminação.

<!-- <br>

## :mount_fuji: Objetos 3D Criados -->

<br>

### :snowman: Autores

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