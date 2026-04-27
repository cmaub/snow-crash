# LEVEL13


L'inspection du système de fichier avec `ls -la` révèle un exécutable `level13`.

``` bash
level13@SnowCrash:~$ ls -la
total 20
dr-x------ 1 level13 level13  120 Mar  5  2016 .
d--x--x--x 1 root    users    340 Aug 30  2015 ..
-r-x------ 1 level13 level13  220 Apr  3  2012 .bash_logout
-r-x------ 1 level13 level13 3518 Aug 30  2015 .bashrc
-rwsr-sr-x 1 flag13  level13 7303 Aug 30  2015 level13
-r-x------ 1 level13 level13  675 Apr  3  2012 .profile
```
L'exécution du fichier puis l'utilisation de `ltrace` indique que nous alons devoir modifier l'uid de l'utilisateur, la fonction `getuid()` renvoyant actuellement `2013`:

```bash
level13@SnowCrash:~$ ./level13
UID 2013 started us but we we expect 4242
```

```Diff
level13@SnowCrash:~$ ltrace ./level13
__libc_start_main(0x804858c, 1, 0xbffff7f4, 0x80485f0, 0x8048660 <unfinished ...>
+getuid()                                                               = 2013
+getuid()                                                               = 2013
+printf("UID %d started us but we we expe"..., 2013UID 2013 started us but we we expect 4242
)                    = 42
exit(1 <unfinished ...>
+++ exited (status 1) +++
```


Pour modifier l'uid nous utilisons gdb et desassemblons le main pour comprendre les différents appels de fonction.

```Diff
level13@SnowCrash:~$ ./level13
UID 2013 started us but we we expect 4242
...
level13@SnowCrash:~$ gdb ./level13
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
...
(gdb) disas main
Dump of assembler code for function main:
   0x0804858c <+0>:	push   %ebp
   0x0804858d <+1>:	mov    %esp,%ebp
   0x0804858f <+3>:	and    $0xfffffff0,%esp
   0x08048592 <+6>:	sub    $0x10,%esp
+  0x08048595 <+9>:	call   0x8048380 <getuid@plt>
+  0x0804859a <+14>:	cmp    $0x1092,%eax
+  0x0804859f <+19>:	je     0x80485cb <main+63>
   0x080485a1 <+21>:	call   0x8048380 <getuid@plt>
   0x080485a6 <+26>:	mov    $0x80486c8,%edx
   0x080485ab <+31>:	movl   $0x1092,0x8(%esp)
   0x080485b3 <+39>:	mov    %eax,0x4(%esp)
   0x080485b7 <+43>:	mov    %edx,(%esp)
   0x080485ba <+46>:	call   0x8048360 <printf@plt>
   0x080485bf <+51>:	movl   $0x1,(%esp)
   0x080485c6 <+58>:	call   0x80483a0 <exit@plt>
   0x080485cb <+63>:	movl   $0x80486ef,(%esp)
   0x080485d2 <+70>:	call   0x8048474 <ft_des>
   0x080485d7 <+75>:	mov    $0x8048709,%edx
   0x080485dc <+80>:	mov    %eax,0x4(%esp)
   0x080485e0 <+84>:	mov    %edx,(%esp)
   0x080485e3 <+87>:	call   0x8048360 <printf@plt>
   0x080485e8 <+92>:	leave  
   0x080485e9 <+93>:	ret    
End of assembler dump.
```
Le désassemblage de la fonction `main()` révèle un appel de fonction `getuid()` puis une comparaison (avec le branchement conditionnel `je`) à `main+14` entre le registre `eax` (contenant l'uid de l'utilisateur retourné par `getuid()`) et la représentation hexadécimale de `4242` (`0x1092`). 

Après cette comparaison le programme jump à `main+63` si la valeur de `eax` est identique à `4242`. La fonction `ft_des` est appellée à `main+70`, on suppose qu'elle correspond à une fonction de chiffrement qui permet de récupèrer le token. Si la comparaison révèle deux valeurs différentes le programme est quitté avec `exit` à `main+63`.

Par une manipulation des registres via GDB, nous écrasons la valeur de retour de `getuid()` dans `%eax` juste avant l'instruction de comparaison `cmp`.
On aura alors détourné le flux d'exécution (Control Flow Hijacking).

``` bash
(gdb) b *main+14
Breakpoint 1 at 0x804859a
(gdb) run
Starting program: /home/user/level13/level13 
Breakpoint 1, 0x0804859a in main ()
(gdb) print $eax
$2 = 2013
(gdb) set $eax=4242
(gdb) print $eax
$3 = 4242
(gdb) n
Single stepping until exit from function main,
which has no line number information.
your token is 2A31L79asukciNyi8uppkEuSx

```

ou

```bash
b getuid       #breakpoint sur getuid

run            # le programme se lance et se stop a la fonction getuid()

finish         # permet de se positionner juste à la sortie de la fonction getuid()

set $eax=4242  # on set le registre eax à eax=4242

continue       # on relance le programme
```

Commandes GDB utiles:
   - `stepi` (`si`) : avance d'une instruction, si c'est un call, entre dans la fonction
   - `nexti` (`ni`) : avance d'une instruction, si c'est un call, il l'execute entierement et s'arrete juste apres
   - `display/i $pc` : mojtrer a l'avance chacune des instructions qui va etre executee
   - `info registers`
   - `objdump -D`