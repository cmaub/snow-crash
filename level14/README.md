# level14

``` bash
level14@SnowCrash:~$ objdump -D getflag
```

protection
```
level14@SnowCrash:~$ ltrace /bin/getflag
__libc_start_main(0x8048946, 1, 0xbffff7e4, 0x8048ed0, 0x8048f40 <unfinished ...>
ptrace(0, 0, 1, 0, 0)                                                                               = -1
puts("You should not reverse this"You should not reverse this
)                                                                 = 28
+++ exited (status 1) +++

```
``` bash
08048946 <main>:
 8048946:	55                   	push   %ebp
 8048947:	89 e5                	mov    %esp,%ebp
 8048949:	53                   	push   %ebx
 804894a:	83 e4 f0             	and    $0xfffffff0,%esp
 804894d:	81 ec 20 01 00 00    	sub    $0x120,%esp
 8048953:	65 a1 14 00 00 00    	mov    %gs:0x14,%eax
 8048959:	89 84 24 1c 01 00 00 	mov    %eax,0x11c(%esp)
 8048960:	31 c0                	xor    %eax,%eax
 8048962:	c7 44 24 10 00 00 00 	movl   $0x0,0x10(%esp)
 8048969:	00 
 804896a:	c7 44 24 0c 00 00 00 	movl   $0x0,0xc(%esp)
 8048971:	00 
 8048972:	c7 44 24 08 01 00 00 	movl   $0x1,0x8(%esp)
 8048979:	00 
 804897a:	c7 44 24 04 00 00 00 	movl   $0x0,0x4(%esp)
 8048981:	00 
 8048982:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
 8048989:	e8 b2 fb ff ff       	call   8048540 <ptrace@plt> ##### 
 804898e:	85 c0                	test   %eax,%eax ################ +72
 8048990:	79 16                	jns    80489a8 <main+0x62> ###### +74
 8048992:	c7 04 24 a8 8f 04 08 	movl   $0x8048fa8,(%esp)
 8048999:	e8 42 fb ff ff       	call   80484e0 <puts@plt>
 804899e:	b8 01 00 00 00       	mov    $0x1,%eax
 80489a3:	e9 0a 05 00 00       	jmp    8048eb2 <main+0x56c>
 80489a8:	c7 04 24 c4 8f 04 08 	movl   $0x8048fc4,(%esp)
 80489af:	e8 1c fb ff ff       	call   80484d0 <getenv@plt>
 80489b4:	85 c0                	test   %eax,%eax
 80489b6:	74 32                	je     80489ea <main+0xa4>
 80489b8:	a1 40 b0 04 08       	mov    0x804b040,%eax
 80489bd:	89 c2                	mov    %eax,%edx
 80489bf:	b8 d0 8f 04 08       	mov    $0x8048fd0,%eax
 80489c4:	89 54 24 0c          	mov    %edx,0xc(%esp)
 80489c8:	c7 44 24 08 25 00 00 	movl   $0x25,0x8(%esp)
 80489cf:	00 
 80489d0:	c7 44 24 04 01 00 00 	movl   $0x1,0x4(%esp)
 80489d7:	00 
 80489d8:	89 04 24             	mov    %eax,(%esp)
 80489db:	e8 e0 fa ff ff       	call   80484c0 <fwrite@plt>
 80489e0:	b8 01 00 00 00       	mov    $0x1,%eax
 80489e5:	e9 c8 04 00 00       	jmp    8048eb2 <main+0x56c>
 80489ea:	c7 44 24 04 00 00 00 	movl   $0x0,0x4(%esp)
 80489f1:	00 
 80489f2:	c7 04 24 f6 8f 04 08 	movl   $0x8048ff6,(%esp)
 80489f9:	e8 02 fb ff ff       	call   8048500 <open@plt>
 80489fe:	85 c0                	test   %eax,%eax
 8048a00:	7e 32                	jle    8048a34 <main+0xee>
 8048a02:	a1 40 b0 04 08       	mov    0x804b040,%eax
 8048a07:	89 c2                	mov    %eax,%edx
 8048a09:	b8 d0 8f 04 08       	mov    $0x8048fd0,%eax
 8048a0e:	89 54 24 0c          	mov    %edx,0xc(%esp)
 8048a12:	c7 44 24 08 25 00 00 	movl   $0x25,0x8(%esp)
 8048a19:	00 
 8048a1a:	c7 44 24 04 01 00 00 	movl   $0x1,0x4(%esp)
 8048a21:	00 
 8048a22:	89 04 24             	mov    %eax,(%esp)
 8048a25:	e8 96 fa ff ff       	call   80484c0 <fwrite@plt>
 8048a2a:	b8 01 00 00 00       	mov    $0x1,%eax
 8048a2f:	e9 7e 04 00 00       	jmp    8048eb2 <main+0x56c>
 8048a34:	c7 44 24 04 00 00 00 	movl   $0x0,0x4(%esp)
 8048a3b:	00 
 8048a3c:	c7 04 24 09 90 04 08 	movl   $0x8049009,(%esp)
 8048a43:	e8 d4 fc ff ff       	call   804871c <syscall_open>
 8048a48:	89 44 24 14          	mov    %eax,0x14(%esp)
 8048a4c:	83 7c 24 14 ff       	cmpl   $0xffffffff,0x14(%esp)
 8048a51:	0f 85 31 04 00 00    	jne    8048e88 <main+0x542>
 8048a57:	a1 40 b0 04 08       	mov    0x804b040,%eax
 8048a5c:	89 c2                	mov    %eax,%edx
 8048a5e:	b8 1c 90 04 08       	mov    $0x804901c,%eax
 8048a63:	89 54 24 0c          	mov    %edx,0xc(%esp)
 8048a67:	c7 44 24 08 46 00 00 	movl   $0x46,0x8(%esp)
 8048a6e:	00 
 8048a6f:	c7 44 24 04 01 00 00 	movl   $0x1,0x4(%esp)
 8048a76:	00 
 8048a77:	89 04 24             	mov    %eax,(%esp)
 8048a7a:	e8 41 fa ff ff       	call   80484c0 <fwrite@plt>
 8048a7f:	b8 01 00 00 00       	mov    $0x1,%eax
 8048a84:	e9 29 04 00 00       	jmp    8048eb2 <main+0x56c>
 8048a89:	c7 44 24 04 63 90 04 	movl   $0x8049063,0x4(%esp)
 8048a90:	08 
 8048a91:	8d 44 24 1c          	lea    0x1c(%esp),%eax
 8048a95:	89 04 24             	mov    %eax,(%esp)
 8048a98:	e8 a6 fd ff ff       	call   8048843 <isLib>
 8048a9d:	85 c0                	test   %eax,%eax
 8048a9f:	74 0d                	je     8048aae <main+0x168>
 8048aa1:	c7 44 24 10 01 00 00 	movl   $0x1,0x10(%esp)
 8048aa8:	00 
 8048aa9:	e9 db 03 00 00       	jmp    8048e89 <main+0x543>
 8048aae:	83 7c 24 10 00       	cmpl   $0x0,0x10(%esp)
 8048ab3:	0f 84 d0 03 00 00    	je     8048e89 <main+0x543>
 8048ab9:	c7 44 24 04 68 90 04 	movl   $0x8049068,0x4(%esp)
 8048ac0:	08 
 8048ac1:	8d 44 24 1c          	lea    0x1c(%esp),%eax
 8048ac5:	89 04 24             	mov    %eax,(%esp)
 8048ac8:	e8 76 fd ff ff       	call   8048843 <isLib>
 8048acd:	85 c0                	test   %eax,%eax
 8048acf:	0f 84 71 03 00 00    	je     8048e46 <main+0x500>
 8048ad5:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048ada:	89 c2                	mov    %eax,%edx
 8048adc:	b8 6c 90 04 08       	mov    $0x804906c,%eax
 8048ae1:	89 54 24 0c          	mov    %edx,0xc(%esp)
 8048ae5:	c7 44 24 08 20 00 00 	movl   $0x20,0x8(%esp)
 8048aec:	00 
 8048aed:	c7 44 24 04 01 00 00 	movl   $0x1,0x4(%esp)
 8048af4:	00 
 8048af5:	89 04 24             	mov    %eax,(%esp)
 8048af8:	e8 c3 f9 ff ff       	call   80484c0 <fwrite@plt>
 0x08048afd:	e8 ae f9 ff ff       	call   80484b0 <getuid@plt>  #+439
 8048b02:	89 44 24 18          	mov    %eax,0x18(%esp)
 8048b06:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048b0a:	3d be 0b 00 00       	cmp    $0xbbe,%eax ###########+452
 8048b0f:	0f 84 b6 01 00 00    	je     8048ccb <main+0x385>
 8048b15:	3d be 0b 00 00       	cmp    $0xbbe,%eax
 8048b1a:	77 4c                	ja     8048b68 <main+0x222>
 8048b1c:	3d ba 0b 00 00       	cmp    $0xbba,%eax
 8048b21:	0f 84 14 01 00 00    	je     8048c3b <main+0x2f5>
 8048b27:	3d ba 0b 00 00       	cmp    $0xbba,%eax
 8048b2c:	77 1f                	ja     8048b4d <main+0x207>
 8048b2e:	3d b8 0b 00 00       	cmp    $0xbb8,%eax
 8048b33:	0f 84 ba 00 00 00    	je     8048bf3 <main+0x2ad>
 8048b39:	3d b8 0b 00 00       	cmp    $0xbb8,%eax
 8048b3e:	0f 87 d3 00 00 00    	ja     8048c17 <main+0x2d1>
 8048b44:	85 c0                	test   %eax,%eax
 8048b46:	74 7e                	je     8048bc6 <main+0x280>
 8048b48:	e9 b9 02 00 00       	jmp    8048e06 <main+0x4c0>
 8048b4d:	3d bc 0b 00 00       	cmp    $0xbbc,%eax
 8048b52:	0f 84 2b 01 00 00    	je     8048c83 <main+0x33d>
 8048b58:	3d bc 0b 00 00       	cmp    $0xbbc,%eax
 8048b5d:	0f 87 44 01 00 00    	ja     8048ca7 <main+0x361>
 8048b63:	e9 f7 00 00 00       	jmp    8048c5f <main+0x319>
 8048b68:	3d c2 0b 00 00       	cmp    $0xbc2,%eax
 8048b6d:	0f 84 e8 01 00 00    	je     8048d5b <main+0x415>
 8048b73:	3d c2 0b 00 00       	cmp    $0xbc2,%eax
 8048b78:	77 1b                	ja     8048b95 <main+0x24f>
 8048b7a:	3d c0 0b 00 00       	cmp    $0xbc0,%eax
 8048b7f:	0f 84 8e 01 00 00    	je     8048d13 <main+0x3cd>
 8048b85:	3d c0 0b 00 00       	cmp    $0xbc0,%eax
 8048b8a:	0f 87 a7 01 00 00    	ja     8048d37 <main+0x3f1>
 8048b90:	e9 5a 01 00 00       	jmp    8048cef <main+0x3a9>
 8048b95:	3d c4 0b 00 00       	cmp    $0xbc4,%eax
 8048b9a:	0f 84 03 02 00 00    	je     8048da3 <main+0x45d>
 8048ba0:	3d c4 0b 00 00       	cmp    $0xbc4,%eax
 8048ba5:	0f 82 d4 01 00 00    	jb     8048d7f <main+0x439>
 8048bab:	3d c5 0b 00 00       	cmp    $0xbc5,%eax
 8048bb0:	0f 84 0e 02 00 00    	je     8048dc4 <main+0x47e>
 8048bb6:	3d c6 0b 00 00       	cmp    $0xbc6,%eax
 8048bbb:	0f 84 24 02 00 00    	je     8048de5 <main+0x49f>
 8048bc1:	e9 40 02 00 00       	jmp    8048e06 <main+0x4c0>
 8048bc6:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048bcb:	89 c2                	mov    %eax,%edx
 8048bcd:	b8 90 90 04 08       	mov    $0x8049090,%eax
 8048bd2:	89 54 24 0c          	mov    %edx,0xc(%esp)
 8048bd6:	c7 44 24 08 21 00 00 	movl   $0x21,0x8(%esp)
 8048bdd:	00 
 8048bde:	c7 44 24 04 01 00 00 	movl   $0x1,0x4(%esp)
 8048be5:	00 
 8048be6:	89 04 24             	mov    %eax,(%esp)
 8048be9:	e8 d2 f8 ff ff       	call   80484c0 <fwrite@plt>
 8048bee:	e9 3c 02 00 00       	jmp    8048e2f <main+0x4e9>
 8048bf3:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048bf8:	89 c3                	mov    %eax,%ebx
 8048bfa:	c7 04 24 b2 90 04 08 	movl   $0x80490b2,(%esp)
 8048c01:	e8 fe f9 ff ff       	call   8048604 <ft_des>
 8048c06:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048c0a:	89 04 24             	mov    %eax,(%esp)
 8048c0d:	e8 1e f9 ff ff       	call   8048530 <fputs@plt>
 8048c12:	e9 18 02 00 00       	jmp    8048e2f <main+0x4e9>
 8048c17:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048c1c:	89 c3                	mov    %eax,%ebx
 8048c1e:	c7 04 24 cc 90 04 08 	movl   $0x80490cc,(%esp)
 8048c25:	e8 da f9 ff ff       	call   8048604 <ft_des>
 8048c2a:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048c2e:	89 04 24             	mov    %eax,(%esp)
 8048c31:	e8 fa f8 ff ff       	call   8048530 <fputs@plt>
 8048c36:	e9 f4 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048c3b:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048c40:	89 c3                	mov    %eax,%ebx
 8048c42:	c7 04 24 e6 90 04 08 	movl   $0x80490e6,(%esp)
 8048c49:	e8 b6 f9 ff ff       	call   8048604 <ft_des>
 8048c4e:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048c52:	89 04 24             	mov    %eax,(%esp)
 8048c55:	e8 d6 f8 ff ff       	call   8048530 <fputs@plt>
 8048c5a:	e9 d0 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048c5f:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048c64:	89 c3                	mov    %eax,%ebx
 8048c66:	c7 04 24 00 91 04 08 	movl   $0x8049100,(%esp)
 8048c6d:	e8 92 f9 ff ff       	call   8048604 <ft_des>
 8048c72:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048c76:	89 04 24             	mov    %eax,(%esp)
 8048c79:	e8 b2 f8 ff ff       	call   8048530 <fputs@plt>
 8048c7e:	e9 ac 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048c83:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048c88:	89 c3                	mov    %eax,%ebx
 8048c8a:	c7 04 24 1a 91 04 08 	movl   $0x804911a,(%esp)
 8048c91:	e8 6e f9 ff ff       	call   8048604 <ft_des>
 8048c96:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048c9a:	89 04 24             	mov    %eax,(%esp)
 8048c9d:	e8 8e f8 ff ff       	call   8048530 <fputs@plt>
 8048ca2:	e9 88 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048ca7:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048cac:	89 c3                	mov    %eax,%ebx
 8048cae:	c7 04 24 34 91 04 08 	movl   $0x8049134,(%esp)
 8048cb5:	e8 4a f9 ff ff       	call   8048604 <ft_des>
 8048cba:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048cbe:	89 04 24             	mov    %eax,(%esp)
 8048cc1:	e8 6a f8 ff ff       	call   8048530 <fputs@plt>
 8048cc6:	e9 64 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048ccb:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048cd0:	89 c3                	mov    %eax,%ebx
 8048cd2:	c7 04 24 4e 91 04 08 	movl   $0x804914e,(%esp)
 8048cd9:	e8 26 f9 ff ff       	call   8048604 <ft_des>
 8048cde:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048ce2:	89 04 24             	mov    %eax,(%esp)
 8048ce5:	e8 46 f8 ff ff       	call   8048530 <fputs@plt>
 8048cea:	e9 40 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048cef:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048cf4:	89 c3                	mov    %eax,%ebx
 8048cf6:	c7 04 24 68 91 04 08 	movl   $0x8049168,(%esp)
 8048cfd:	e8 02 f9 ff ff       	call   8048604 <ft_des>
 8048d02:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048d06:	89 04 24             	mov    %eax,(%esp)
 8048d09:	e8 22 f8 ff ff       	call   8048530 <fputs@plt>
 8048d0e:	e9 1c 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048d13:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048d18:	89 c3                	mov    %eax,%ebx
 8048d1a:	c7 04 24 82 91 04 08 	movl   $0x8049182,(%esp)
 8048d21:	e8 de f8 ff ff       	call   8048604 <ft_des>
 8048d26:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048d2a:	89 04 24             	mov    %eax,(%esp)
 8048d2d:	e8 fe f7 ff ff       	call   8048530 <fputs@plt>
 8048d32:	e9 f8 00 00 00       	jmp    8048e2f <main+0x4e9>
 8048d37:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048d3c:	89 c3                	mov    %eax,%ebx
 8048d3e:	c7 04 24 9c 91 04 08 	movl   $0x804919c,(%esp)
 8048d45:	e8 ba f8 ff ff       	call   8048604 <ft_des>
 8048d4a:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048d4e:	89 04 24             	mov    %eax,(%esp)
 8048d51:	e8 da f7 ff ff       	call   8048530 <fputs@plt>
 8048d56:	e9 d4 00 00 00       	jmp    8048e2f <main+0x4e9>
 8048d5b:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048d60:	89 c3                	mov    %eax,%ebx
 8048d62:	c7 04 24 b6 91 04 08 	movl   $0x80491b6,(%esp)
 8048d69:	e8 96 f8 ff ff       	call   8048604 <ft_des>
 8048d6e:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048d72:	89 04 24             	mov    %eax,(%esp)
 8048d75:	e8 b6 f7 ff ff       	call   8048530 <fputs@plt>
 8048d7a:	e9 b0 00 00 00       	jmp    8048e2f <main+0x4e9>
 8048d7f:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048d84:	89 c3                	mov    %eax,%ebx
 8048d86:	c7 04 24 d0 91 04 08 	movl   $0x80491d0,(%esp)
 8048d8d:	e8 72 f8 ff ff       	call   8048604 <ft_des>
 8048d92:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048d96:	89 04 24             	mov    %eax,(%esp)
 8048d99:	e8 92 f7 ff ff       	call   8048530 <fputs@plt>
 8048d9e:	e9 8c 00 00 00       	jmp    8048e2f <main+0x4e9>
 8048da3:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048da8:	89 c3                	mov    %eax,%ebx
 8048daa:	c7 04 24 ea 91 04 08 	movl   $0x80491ea,(%esp)
 8048db1:	e8 4e f8 ff ff       	call   8048604 <ft_des>
 8048db6:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048dba:	89 04 24             	mov    %eax,(%esp)
 8048dbd:	e8 6e f7 ff ff       	call   8048530 <fputs@plt>
 8048dc2:	eb 6b                	jmp    8048e2f <main+0x4e9>
 8048dc4:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048dc9:	89 c3                	mov    %eax,%ebx
 8048dcb:	c7 04 24 04 92 04 08 	movl   $0x8049204,(%esp)
 8048dd2:	e8 2d f8 ff ff       	call   8048604 <ft_des>
 8048dd7:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048ddb:	89 04 24             	mov    %eax,(%esp)
 8048dde:	e8 4d f7 ff ff       	call   8048530 <fputs@plt>
 8048de3:	eb 4a                	jmp    8048e2f <main+0x4e9>
 8048de5:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048dea:	89 c3                	mov    %eax,%ebx
 8048dec:	c7 04 24 20 92 04 08 	movl   $0x8049220,(%esp)
 8048df3:	e8 0c f8 ff ff       	call   8048604 <ft_des>
 8048df8:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048dfc:	89 04 24             	mov    %eax,(%esp)
 8048dff:	e8 2c f7 ff ff       	call   8048530 <fputs@plt>
 8048e04:	eb 29                	jmp    8048e2f <main+0x4e9>
 8048e06:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048e0b:	89 c2                	mov    %eax,%edx
 8048e0d:	b8 48 92 04 08       	mov    $0x8049248,%eax
 8048e12:	89 54 24 0c          	mov    %edx,0xc(%esp)
 8048e16:	c7 44 24 08 38 00 00 	movl   $0x38,0x8(%esp)
 8048e1d:	00 
 8048e1e:	c7 44 24 04 01 00 00 	movl   $0x1,0x4(%esp)
 8048e25:	00 
 8048e26:	89 04 24             	mov    %eax,(%esp)
 8048e29:	e8 92 f6 ff ff       	call   80484c0 <fwrite@plt>
 8048e2e:	90                   	nop
 8048e2f:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048e34:	89 44 24 04          	mov    %eax,0x4(%esp)
 8048e38:	c7 04 24 0a 00 00 00 	movl   $0xa,(%esp)
 8048e3f:	e8 dc f6 ff ff       	call   8048520 <fputc@plt>
 8048e44:	eb 67                	jmp    8048ead <main+0x567>
 8048e46:	c7 44 24 04 81 92 04 	movl   $0x8049281,0x4(%esp)
 8048e4d:	08 
 8048e4e:	8d 44 24 1c          	lea    0x1c(%esp),%eax
 8048e52:	89 04 24             	mov    %eax,(%esp)
 8048e55:	e8 64 f9 ff ff       	call   80487be <afterSubstr>
 8048e5a:	85 c0                	test   %eax,%eax
 8048e5c:	75 2b                	jne    8048e89 <main+0x543>
 8048e5e:	a1 40 b0 04 08       	mov    0x804b040,%eax
 8048e63:	89 c2                	mov    %eax,%edx
 8048e65:	b8 94 92 04 08       	mov    $0x8049294,%eax
 8048e6a:	89 54 24 0c          	mov    %edx,0xc(%esp)
 8048e6e:	c7 44 24 08 30 00 00 	movl   $0x30,0x8(%esp)
 8048e75:	00 
 8048e76:	c7 44 24 04 01 00 00 	movl   $0x1,0x4(%esp)
 8048e7d:	00 
 8048e7e:	89 04 24             	mov    %eax,(%esp)
 8048e81:	e8 3a f6 ff ff       	call   80484c0 <fwrite@plt>
 8048e86:	eb 25                	jmp    8048ead <main+0x567>
 8048e88:	90                   	nop
 8048e89:	8b 44 24 14          	mov    0x14(%esp),%eax
 8048e8d:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048e91:	c7 44 24 04 00 01 00 	movl   $0x100,0x4(%esp)
 8048e98:	00 
 8048e99:	8d 44 24 1c          	lea    0x1c(%esp),%eax
 8048e9d:	89 04 24             	mov    %eax,(%esp)
 8048ea0:	e8 a7 f8 ff ff       	call   804874c <syscall_gets>
 8048ea5:	85 c0                	test   %eax,%eax
 8048ea7:	0f 85 dc fb ff ff    	jne    8048a89 <main+0x143>
 8048ead:	b8 00 00 00 00       	mov    $0x0,%eax
 8048eb2:	8b 94 24 1c 01 00 00 	mov    0x11c(%esp),%edx
 8048eb9:	65 33 15 14 00 00 00 	xor    %gs:0x14,%edx
 8048ec0:	74 05                	je     8048ec7 <main+0x581>
 8048ec2:	e8 d9 f5 ff ff       	call   80484a0 <__stack_chk_fail@plt>
 8048ec7:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 8048eca:	c9                   	leave  
 8048ecb:	c3                   	ret    
 8048ecc:	90                   	nop
 8048ecd:	90                   	nop
 8048ece:	90                   	nop
 8048ecf:	90                   	nop

```

On cherche l'UID de flag14 dans /etc/passwd
``` bash
level14@SnowCrash:~$ cat /etc/passwd | grep flag14
flag14:x:3014:3014::/home/flag/flag14:/bin/bash
```

call
getuid?
ptrace? 

strings -a -t x /bin/getflag

c continue

b *adrresse en hexa


``` bash
level14@SnowCrash:~$ gdb /bin/getflag
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /bin/getflag...(no debugging symbols found)...done.
(gdb) b *main
Breakpoint 1 at 0x8048946
(gdb) run
Starting program: /bin/getflag 

Breakpoint 1, 0x08048946 in main ()
(gdb) b *main+72
Breakpoint 2 at 0x804898e
(gdb) c
Continuing.

Breakpoint 2, 0x0804898e in main ()
(gdb) set $eax=0
(gdb) print $eax
$1 = 0
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

``` bash
evel14@SnowCrash:~$ su flag14
Password: 
Congratulation. Type getflag to get the key and send it to me the owner of this livecd :)
```