# LEVEL09

Le système de fichier comporte un exécutable level09 pour lesquels nous avons le SUID activé et un fichier token.
``` bash
level09@SnowCrash:~$ ls -la
total 24
dr-x------ 1 level09 level09  140 Mar  5  2016 .
d--x--x--x 1 root    users    340 Aug 30  2015 ..
-r-x------ 1 level09 level09  220 Apr  3  2012 .bash_logout
-r-x------ 1 level09 level09 3518 Aug 30  2015 .bashrc
-rwsr-sr-x 1 flag09  level09 7640 Mar  5  2016 level09
-r-x------ 1 level09 level09  675 Apr  3  2012 .profile
----r--r-- 1 flag09  level09   26 Mar  5  2016 token
```

L'examen du fichier avec ltrace nous indique qu'un appel à `ltrace()` est fait.


``` bash
level09@SnowCrash:~$ ltrace ./level09 4564
__libc_start_main(0x80487ce, 2, 0xbffff7d4, 0x8048aa0, 0x8048b10 <unfinished ...>
ptrace(0, 0, 1, 0, 0xb7e2fe38)                                                 = -1
puts("You should not reverse this"You should not reverse this
)                                            = 28
+++ exited (status 1) +++
```

`ltrace()` est utilisé ici comme une protection contre l'analyse dynamique.
En effet un programme ne peut etre tracé que par un seul processus parent. Ainsi le programme lancé avec `ltrace()`, arrive à `ptrace()` qui renvoit -1 et le programme s'arrete en affichant un message d'erreur. Cela nous oblige de passer par une analyse de type "Boite Noire" (Back-box-testing).

Le fichier token comprend une chaine de caractère qui ne permet pas de se connecter à flag09:

``` bash
level09@SnowCrash:~$ cat token
f4kmm6p|=�p�n��DB�Du{��
```

On comprend que le programme prend en argument une chaine de caractère et qu'il retourne une autre chaine calculée à partir de la première.

``` bash
level09@SnowCrash:~$ ./level09
You need to provied only one arg.
level09@SnowCrash:~$ ./level09 aaaaaa
abcdef
level09@SnowCrash:~$ ./level09 111111
123456
level09@SnowCrash:~$ ./level09 123456
13579;
```

La chaine de sortie correspond au caractère de la tables ASCII additionné à son index dans la chaine.
Donc pour "aaaaaa"
```
'a' = 'a' + 0 == 'a'
'a' = 'a' + 1 == 'b'
'a' = 'a' + 2 == 'c'
'a' = 'a' + 3 == 'd'
'a' = 'a' + 4 == 'e'
...
```

La conversion de la chaine présente dans token ne donne rien de probant:
``` bash
level09@SnowCrash:~$ ./level09 'f4kmm6p|=�p�n��DB�Du{��'
f5mpq;v�E���|���~����[��`������
```

La phrase vue avec `ltrace()` "You should not reverse this" nous indique une piste d'inversion.

Nous créons donc un script pour inverser la logique de l'executable level09. Donc soustraire pour chaque caractère dans la chaine la position de son index.


AJOUTER SCRIPT



``` bash
level09@SnowCrash:/tmp$ ./a.out /home/user/level09/token
f3iji1ju5yuevaus41q1afiuq

level09@SnowCrash:/tmp$ su flag09
Password: 
Don't forget to launch getflag !
flag09@SnowCrash:~$ getflag
Check flag.Here is your token : s5cAJpM8ev6XHw998pRWG728z
```


Petit point sur la différence entre le chiffrement et l'obfuscation:
- Le chiffrement transforme une information pour a rendre iisible à quiconque ne possède pas une clé secréte.
- L'obfuscation vis à rendre un message ou un code difficie à conprendre sans utiliser de secret externe.