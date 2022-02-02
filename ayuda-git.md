1) Para ver en qué rama estás editando:                                                   `git status` 
2) Ver que cambios se hicieron en el repositorio remoto (GitHub) en la rama <rama>, sin descargarlos al proyecto:   
     (1) `git fetch` y (2) `git diff <rama> origin/<rama>`                        Ejemplo: `git diff main origin/main` 
3) Ver todos los cambios del repositorio remoto en todas las ramas:   
     (1) `git fetch` y (2) `git diff ...origin` 
4) Crear una nueva rama llamada <nueva>: `git checkout -b <nueva>` 
5) Eliminar una rama: `git branch -d <rama>` 
  
6) Fusionar la rama <test> con la rama <objetivo>:  
    (1) `git checkout <objetivo>`               cambiar a la rama objetivo   
    (2) `git fetch`                             obtener los últimos datos remotos de la rama objetivo  
    (3) `git pull`                              descargarlos  
    (4) `git merge <test>                       unir la rama   
  
7) Hacer un commit rápido: `git add` y `git commit -m "comentario"`
