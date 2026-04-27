# level14

Dans ce niveau nous allons exploiter le binaire `getflag`.
Nous avons les permissions d'exécution et d'écriture dessus:

``` bash
level14@SnowCrash:~$ ls -la /bin/getflag
-rwxr-xr-x 1 root root 11833 Aug 30  2015 /bin/getflag
```

Nous allons devoir trouver un moyen d'envoyer au programme le RUID de l'utilisateur flag14 pour récupèrer le token.
L'UID de flag14 se trouve dans `/etc/passwd`.

``` bash
level14@SnowCrash:~$ cat /etc/passwd | grep flag14
flag14:x:3014:3014::/home/flag/flag14:/bin/bash
```

Un examen du binaire avec ltrace nous révèle une protection contre l'inspection avec un débugueur.
L'appel à `ptrace()` est une technique d'Anti-Reverse Engineering. Un processus ne peut être tracé que par un seul parent. En essayant de se tracer lui-même, le programme vérifie si un débugueur est déjà présent. Si `ptrace` renvoie -1, cela signifie qu'un débugueur (ltrace ou GDB) est detecté.

``` bash
level14@SnowCrash:~$ ltrace /bin/getflag
__libc_start_main(0x8048946, 1, 0xbffff7e4, 0x8048ed0, 0x8048f40 <unfinished ...>
ptrace(0, 0, 1, 0, 0)                            = -1
puts("You should not reverse this"You should not reverse this
)              = 28
+++ exited (status 1) +++
```

L'objectif est de contourner la protection Anti-Reverse Engineering pour ensuite falsifier la valeur de retour de `getuid()`.
L'analyse statique du binaire révèle une structure de contrôle de type Switch-Case, où chaque UID déclenche un algorithme de déchiffrement (`ft_des`) spécifique pour obtenir le token.

``` diff
(gdb) disas main
Dump of assembler code for function main:
   0x08048946 <+0>:	push   %ebp
   0x08048947 <+1>:	mov    %esp,%ebp
   ...
   ...
   0x08048982 <+60>:	movl   $0x0,(%esp)
+  0x08048989 <+67>:	call   0x8048540 <ptrace@plt>
   0x0804898e <+72>:	test   %eax,%eax
   0x08048990 <+74>:	jns    0x80489a8 <main+98>
   0x08048992 <+76>:	movl   $0x8048fa8,(%esp)
   ...
   ...
   0x08048af8 <+434>:	call   0x80484c0 <fwrite@plt>
+  0x08048afd <+439>:	call   0x80484b0 <getuid@plt>
   0x08048b02 <+444>:	mov    %eax,0x18(%esp)
   0x08048b06 <+448>:	mov    0x18(%esp),%eax
+  0x08048b0a <+452>:	cmp    $0xbbe,%eax
   0x08048b0f <+457>:	je     0x8048ccb <main+901>
   0x08048b15 <+463>:	cmp    $0xbbe,%eax
   ...
   ...
   0x08048ecb <+1413>:	ret    
End of assembler dump.
```

Nous procédons à une altération du flux d'exécution en manipulant les registres d'état au runtime.
On modifie donc la valeur de `$eax` à 0 après l'appel de `ptrace()` et avant la comparaison et le jump conditionnel en placant un breakpoint à `main+72`

``` bash
level14@SnowCrash:~$ gdb /bin/getflag
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
...
...
(gdb) b *main
Breakpoint 1 at 0x8048946
(gdb) run
Starting program: /bin/getflag 

Breakpoint 1, 0x08048946 in main ()
(gdb) b *main+72
(gdb) c
Continuing.
Breakpoint 2, 0x0804898e in main ()
1: x/i $pc
=> 0x804898e <main+72>:	test   %eax,%eax
(gdb) print $eax
$1 = -1
(gdb) set $eax=0
```

Puis l'on va mettre un breakpoint juste après l'appel de `getuid()` pour modifier la valeur retournée dans `$eax` pour y inscrire l'UID de flag14.
On reprend ensuite le flux d'exécution et on récupère le token.

``` bash
(gdb) b *main+439
Breakpoint 3 at 0x8048afd
(gdb) c
Continuing.

Breakpoint 3, 0x08048afd in main ()
(gdb) print $eax
$2 = 32
(gdb) b *main+452
Breakpoint 4 at 0x8048b0a
(gdb) print $eax
$3 = 32
(gdb) print 0x18
$4 = 24
(gdb) print $0x18
$5 = void
(gdb) print $esp
$6 = (void *) 0xbffff620
(gdb) set $eax=3014
(gdb) c
Continuing.

Breakpoint 4, 0x08048b0a in main ()
(gdb) print $eax
$7 = 2014
(gdb) set $eax=3014
(gdb) c
Continuing.
Check flag.Here is your token : 7QiHafiNa3HVozsaXkawuYrTstxbpABHD8CPnHJ
[Inferior 1 (process 3686) exited normally]

```

Et voilà, on a fini snowcrash !

``` bash
evel14@SnowCrash:~$ su flag14
Password: 
Congratulation. Type getflag to get the key and send it to me the owner of this livecd :)
```

Commandes GDB utiles:
   - `stepi` (`si`) : avance d'une instruction en assembleur, si c'est un call, entre dans la fonction
   - `nexti` (`ni`) : avance d'une instruction en assembleur, si c'est un call, il l'execute entierement et s'arrete juste apres
   - `display/i $pc` : mojtrer a l'avance chacune des instructions qui va etre executee
   - `info registers`
   - `objdump -D`
   - `p/x [arg]` 

Sources:
   - https://www.cs.ucr.edu/~csong/cs153/refs/gdb.html
   - https://reverse.zip/posts/introduction_au_reverse_partie_21/
   - https://reverse.zip/posts/introduction_au_reverse_partie_1/
