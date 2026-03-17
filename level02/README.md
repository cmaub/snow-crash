# LEVEL02

En observant le systeme de fichier on trouve un fichier suspect "level02.pcap"
``` bash
level02@SnowCrash:~$ ls -la
total 24
dr-x------ 1 level02 level02  120 Mar  5  2016 .
d--x--x--x 1 root    users    340 Aug 30  2015 ..
-r-x------ 1 level02 level02  220 Apr  3  2012 .bash_logout
-r-x------ 1 level02 level02 3518 Aug 30  2015 .bashrc
----r--r-- 1 flag02  level02 8302 Aug 30  2015 level02.pcap
-r-x------ 1 level02 level02  675 Apr  3  2012 .profile
```

L'interieur du fichier est illisible.
Un fichier PCAP ("packet capture") est une interface de programmation permettant de capturer un trafic reseau. Il permet d'observer la communication reseau. Pour l'analyser il faut utiliser un outil de supervision reseau, ici nous avons utilise Wireshark.


Utiliser l'outil "Follow -> TCP stream"
Chaines de caracteres suspecte:
```
..wwwbugs login: l.le.ev.ve.el.lX.X
..
Password: ft_wandr...NDRel.L0L
```
"ft_wandr...NDRel.L0L" ne conctionne pas comme mot de passe pour flag02.
On comprend avec la chaine "l.le.ev.ve.el.lX.X" que les "." sont des backspace pour reconstituer la chaine "levelX"
Donc la chaine "ft_wandr...NDRel.L0L" correspond a "ft_waNDReL0L"


``` bash
level02@SnowCrash:~$ su flag02
Password: 
Don't forget to launch getflag !
flag02@SnowCrash:~$ getflag
Check flag.Here is your token : kooda2puivaav1idi4f57q8iq
```