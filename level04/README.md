# LEVEL04

Le systeme de fichier se présente de cette manière:

``` bash
level04@SnowCrash:~$ ls -la
total 16
dr-xr-x---+ 1 level04 level04  120 Mar  5  2016 .
d--x--x--x  1 root    users    340 Aug 30  2015 ..
-r-x------  1 level04 level04  220 Apr  3  2012 .bash_logout
-r-x------  1 level04 level04 3518 Aug 30  2015 .bashrc
-rwsr-sr-x  1 flag04  level04  152 Mar  5  2016 level04.pl
-r-x------  1 level04 level04  675 Apr  3  2012 .profile
```

Le ```s``` dans ```rws``` remplace le ```x``` pour signifier que le programme est ```setuid```, le bit SUID permet au processus d'hériter de l'EUID du propriétaire du fichier, ici ```flag04```. On comprend que l'on va pouvoir utiliser le script pour lancer la commande ```getflag```.


La commande ```cat``` permet d'afficher un script perl:

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

Ce script permet d'afficher l'argument passé dans les parametres http.
L'arguemnt de x est récupéré via param qui récupère le paramètre http 'x'.
Dans la fonction x, la variable ```$y``` est initialisée avec la valeur de ```$_[0]``` qui désigne le premier argument passé en paramètre, donc x, le paramètre entré par l'utilisateur. 
Tout ce qui se situe entre les backticks est évalué et exécuté par un sous-shell avant la principale commande ```print``` et la sortie de cette commande devient l'argument de ```print```. On va donc pouvoir utiliser une technique d'injection shell pour trouver le resutat de ```getflag```. Avec ```2>&1``` on affiche aussi les erreurs dans stdout, sans cette redirection on ne verrait pas si la commande était mal formatée.

Le script souffre d'un manque de sanitization des entrées utilisateur : le contenu de $y est interprété durectement par le sous-shell.

On va pouvoir utiliser ```curl``` pour exécuter le script via localhost sur le port 4747.
On envoit la substitution de commande ```'$(getflag)'``` au script via l'argument x.
L'utilisation des simple quotes empéchent l'expansion locale de la commande par notre propre shell, sans quoi ```$(getflag)``` serait exécuté en tant que level04.
Le shell du script en recevant ```$(getflag)``` ne va pas l'interpréter comme une simple chaine de caractère mais va exécuter ce qui se trouve entre parenthèse et remplacer le texte par son résultat.

``` diff
level04@SnowCrash:~$ curl http://localhost:4747?x='$(getflag)'
+Check flag.Here is your token : ne2searoevaevoem4ov4ar8ap
```