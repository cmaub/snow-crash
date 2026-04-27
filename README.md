# snow-crash

Le projet `SnowCrash` de l'école 42 est une introduction à la cyber sécurité. Il permet de se familiariser avec les bases de la sécurité informatique, notamment sous `Linux`. On y trouve différents langages (ASM, Perl, PHP, etc.) et il permet de prendre conscience de problème liés à de simples erreurs de programmation ou de configuration.

Ce projet est composé de 14 niveaux, chacun contenant une faille de sécurité (reverse engineering, injection, TOCTOU...).

# Instructions

Pour ce projet, il faut utiliser une VMV (64 bits) pour laquelle l’ISO est fournie avec le sujet. au lancement de la VM on trouve un terminal avec une adresse IP :

```
	   _____                      _____               _     
	  / ____|                    / ____|             | |    
	 | (___  _ __   _____      _| |     _ __ __ _ ___| |__  
	  \___ \| '_ \ / _ \ \ /\ / / |    | '__/ _` / __| '_ \ 
	  ____) | | | | (_) \ V  V /| |____| | | (_| \__ \ | | |
	 |_____/|_| |_|\___/ \_/\_/  \_____|_|  \__,_|___/_| |_|
                                                        
  Good luck & Have fun

          192.168.56.105 
level00@192.168.56.105's password: 

```

Le couple `user:password` du premier niveau est donné avec le sujet, à savoir:

- `user`        -> `level00`
- `password`    -> `level00`

il est conseillé dans le sujet de se connecter en ssh à la VM:

```
ssh level00@192.168.56.105 -p 4242
```

Pour passer d'un niveau à l'autre, il faudra soit trouver le mot de passe du niveau suivant (level00 -> level01), soit le mot de passse du user `flag` (level00 -> flag00) correspondant pour pourvoir utiliser la commande `getflag` et obtenir le précieux sésame.

par exemple:

```Diff
level00@SnowCrash:~$ su flag00
Password: 
Don't forget to launch getflag !
flag00@SnowCrash:~$ getflag
+Check flag.Here is your token : ????????????????????????
flag00@SnowCrash:~$ 

```

Sources:
    - https://www.vaadata.com/blog/what-is-command-injection-exploitations-and-security-best-practices/
    - https://www.sysdig.com/blog/exploring-syscall-evasion