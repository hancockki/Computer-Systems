
farm.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <start_farm>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	b8 01 00 00 00       	mov    $0x1,%eax
   9:	5d                   	pop    %rbp
   a:	c3                   	retq   

000000000000000b <addval_202>:
   b:	55                   	push   %rbp
   c:	48 89 e5             	mov    %rsp,%rbp
   f:	89 7d fc             	mov    %edi,-0x4(%rbp)
  12:	8b 45 fc             	mov    -0x4(%rbp),%eax
  15:	2d b4 b7 76 38       	sub    $0x3876b7b4,%eax
  1a:	5d                   	pop    %rbp
  1b:	c3                   	retq   

000000000000001c <setval_177>:
  1c:	55                   	push   %rbp
  1d:	48 89 e5             	mov    %rsp,%rbp
  20:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  24:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  28:	c7 00 d8 90 90 90    	movl   $0x909090d8,(%rax)
  2e:	5d                   	pop    %rbp
  2f:	c3                   	retq   

0000000000000030 <setval_321>:
  30:	55                   	push   %rbp
  31:	48 89 e5             	mov    %rsp,%rbp
  34:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  38:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  3c:	c7 00 68 89 c7 90    	movl   $0x90c78968,(%rax)
  42:	5d                   	pop    %rbp
  43:	c3                   	retq   

0000000000000044 <getval_198>:
  44:	55                   	push   %rbp
  45:	48 89 e5             	mov    %rsp,%rbp
  48:	b8 5b 14 78 90       	mov    $0x9078145b,%eax
  4d:	5d                   	pop    %rbp
  4e:	c3                   	retq   

000000000000004f <setval_336>:
  4f:	55                   	push   %rbp
  50:	48 89 e5             	mov    %rsp,%rbp
  53:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  57:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  5b:	c7 00 a1 58 c3 0a    	movl   $0xac358a1,(%rax)
  61:	5d                   	pop    %rbp
  62:	c3                   	retq   

0000000000000063 <setval_266>:
  63:	55                   	push   %rbp
  64:	48 89 e5             	mov    %rsp,%rbp
  67:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  6b:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  6f:	c7 00 48 89 c7 91    	movl   $0x91c78948,(%rax)
  75:	5d                   	pop    %rbp
  76:	c3                   	retq   

0000000000000077 <getval_275>:
  77:	55                   	push   %rbp
  78:	48 89 e5             	mov    %rsp,%rbp
  7b:	b8 75 48 89 c7       	mov    $0xc7894875,%eax
  80:	5d                   	pop    %rbp
  81:	c3                   	retq   

0000000000000082 <getval_209>:
  82:	55                   	push   %rbp
  83:	48 89 e5             	mov    %rsp,%rbp
  86:	b8 58 90 90 c3       	mov    $0xc3909058,%eax
  8b:	5d                   	pop    %rbp
  8c:	c3                   	retq   

000000000000008d <mid_farm>:
  8d:	55                   	push   %rbp
  8e:	48 89 e5             	mov    %rsp,%rbp
  91:	b8 01 00 00 00       	mov    $0x1,%eax
  96:	5d                   	pop    %rbp
  97:	c3                   	retq   

0000000000000098 <add_xy>:
  98:	55                   	push   %rbp
  99:	48 89 e5             	mov    %rsp,%rbp
  9c:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  a0:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  a4:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  a8:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
  ac:	48 01 d0             	add    %rdx,%rax
  af:	5d                   	pop    %rbp
  b0:	c3                   	retq   

00000000000000b1 <addval_496>:
  b1:	55                   	push   %rbp
  b2:	48 89 e5             	mov    %rsp,%rbp
  b5:	89 7d fc             	mov    %edi,-0x4(%rbp)
  b8:	8b 45 fc             	mov    -0x4(%rbp),%eax
  bb:	2d b8 76 1f 6f       	sub    $0x6f1f76b8,%eax
  c0:	5d                   	pop    %rbp
  c1:	c3                   	retq   

00000000000000c2 <addval_358>:
  c2:	55                   	push   %rbp
  c3:	48 89 e5             	mov    %rsp,%rbp
  c6:	89 7d fc             	mov    %edi,-0x4(%rbp)
  c9:	8b 45 fc             	mov    -0x4(%rbp),%eax
  cc:	2d 77 29 ff 2d       	sub    $0x2dff2977,%eax
  d1:	5d                   	pop    %rbp
  d2:	c3                   	retq   

00000000000000d3 <setval_425>:
  d3:	55                   	push   %rbp
  d4:	48 89 e5             	mov    %rsp,%rbp
  d7:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  db:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  df:	c7 00 8b c1 90 c3    	movl   $0xc390c18b,(%rax)
  e5:	5d                   	pop    %rbp
  e6:	c3                   	retq   

00000000000000e7 <getval_271>:
  e7:	55                   	push   %rbp
  e8:	48 89 e5             	mov    %rsp,%rbp
  eb:	b8 99 ca 90 c3       	mov    $0xc390ca99,%eax
  f0:	5d                   	pop    %rbp
  f1:	c3                   	retq   

00000000000000f2 <setval_189>:
  f2:	55                   	push   %rbp
  f3:	48 89 e5             	mov    %rsp,%rbp
  f6:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  fa:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  fe:	c7 00 89 ca 20 d2    	movl   $0xd220ca89,(%rax)
 104:	5d                   	pop    %rbp
 105:	c3                   	retq   

0000000000000106 <setval_298>:
 106:	55                   	push   %rbp
 107:	48 89 e5             	mov    %rsp,%rbp
 10a:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 10e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 112:	c7 00 89 d6 a4 c0    	movl   $0xc0a4d689,(%rax)
 118:	5d                   	pop    %rbp
 119:	c3                   	retq   

000000000000011a <setval_315>:
 11a:	55                   	push   %rbp
 11b:	48 89 e5             	mov    %rsp,%rbp
 11e:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 122:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 126:	c7 00 89 c1 30 c9    	movl   $0xc930c189,(%rax)
 12c:	5d                   	pop    %rbp
 12d:	c3                   	retq   

000000000000012e <setval_378>:
 12e:	55                   	push   %rbp
 12f:	48 89 e5             	mov    %rsp,%rbp
 132:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 136:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 13a:	c7 00 81 c1 08 c0    	movl   $0xc008c181,(%rax)
 140:	5d                   	pop    %rbp
 141:	c3                   	retq   

0000000000000142 <addval_359>:
 142:	55                   	push   %rbp
 143:	48 89 e5             	mov    %rsp,%rbp
 146:	89 7d fc             	mov    %edi,-0x4(%rbp)
 149:	8b 45 fc             	mov    -0x4(%rbp),%eax
 14c:	2d 73 35 f7 2d       	sub    $0x2df73573,%eax
 151:	5d                   	pop    %rbp
 152:	c3                   	retq   

0000000000000153 <addval_488>:
 153:	55                   	push   %rbp
 154:	48 89 e5             	mov    %rsp,%rbp
 157:	89 7d fc             	mov    %edi,-0x4(%rbp)
 15a:	8b 45 fc             	mov    -0x4(%rbp),%eax
 15d:	2d 77 3e c7 36       	sub    $0x36c73e77,%eax
 162:	5d                   	pop    %rbp
 163:	c3                   	retq   

0000000000000164 <setval_142>:
 164:	55                   	push   %rbp
 165:	48 89 e5             	mov    %rsp,%rbp
 168:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 16c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 170:	c7 00 8b c1 38 c0    	movl   $0xc038c18b,(%rax)
 176:	5d                   	pop    %rbp
 177:	c3                   	retq   

0000000000000178 <setval_107>:
 178:	55                   	push   %rbp
 179:	48 89 e5             	mov    %rsp,%rbp
 17c:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 180:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 184:	c7 00 09 ca 90 c3    	movl   $0xc390ca09,(%rax)
 18a:	5d                   	pop    %rbp
 18b:	c3                   	retq   

000000000000018c <getval_461>:
 18c:	55                   	push   %rbp
 18d:	48 89 e5             	mov    %rsp,%rbp
 190:	b8 8d d6 38 c0       	mov    $0xc038d68d,%eax
 195:	5d                   	pop    %rbp
 196:	c3                   	retq   

0000000000000197 <addval_392>:
 197:	55                   	push   %rbp
 198:	48 89 e5             	mov    %rsp,%rbp
 19b:	89 7d fc             	mov    %edi,-0x4(%rbp)
 19e:	8b 45 fc             	mov    -0x4(%rbp),%eax
 1a1:	2d 77 29 f7 3f       	sub    $0x3ff72977,%eax
 1a6:	5d                   	pop    %rbp
 1a7:	c3                   	retq   

00000000000001a8 <getval_241>:
 1a8:	55                   	push   %rbp
 1a9:	48 89 e5             	mov    %rsp,%rbp
 1ac:	b8 ff 8b c1 c3       	mov    $0xc3c18bff,%eax
 1b1:	5d                   	pop    %rbp
 1b2:	c3                   	retq   

00000000000001b3 <setval_253>:
 1b3:	55                   	push   %rbp
 1b4:	48 89 e5             	mov    %rsp,%rbp
 1b7:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 1bb:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 1bf:	c7 00 a9 d6 38 c0    	movl   $0xc038d6a9,(%rax)
 1c5:	5d                   	pop    %rbp
 1c6:	c3                   	retq   

00000000000001c7 <getval_348>:
 1c7:	55                   	push   %rbp
 1c8:	48 89 e5             	mov    %rsp,%rbp
 1cb:	b8 89 d6 84 d2       	mov    $0xd284d689,%eax
 1d0:	5d                   	pop    %rbp
 1d1:	c3                   	retq   

00000000000001d2 <setval_230>:
 1d2:	55                   	push   %rbp
 1d3:	48 89 e5             	mov    %rsp,%rbp
 1d6:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 1da:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 1de:	c7 00 89 ca 28 d2    	movl   $0xd228ca89,(%rax)
 1e4:	5d                   	pop    %rbp
 1e5:	c3                   	retq   

00000000000001e6 <getval_401>:
 1e6:	55                   	push   %rbp
 1e7:	48 89 e5             	mov    %rsp,%rbp
 1ea:	b8 58 89 e0 90       	mov    $0x90e08958,%eax
 1ef:	5d                   	pop    %rbp
 1f0:	c3                   	retq   

00000000000001f1 <getval_438>:
 1f1:	55                   	push   %rbp
 1f2:	48 89 e5             	mov    %rsp,%rbp
 1f5:	b8 89 ca 78 db       	mov    $0xdb78ca89,%eax
 1fa:	5d                   	pop    %rbp
 1fb:	c3                   	retq   

00000000000001fc <setval_251>:
 1fc:	55                   	push   %rbp
 1fd:	48 89 e5             	mov    %rsp,%rbp
 200:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 204:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 208:	c7 00 48 89 e0 c7    	movl   $0xc7e08948,(%rax)
 20e:	5d                   	pop    %rbp
 20f:	c3                   	retq   

0000000000000210 <addval_111>:
 210:	55                   	push   %rbp
 211:	48 89 e5             	mov    %rsp,%rbp
 214:	89 7d fc             	mov    %edi,-0x4(%rbp)
 217:	8b 45 fc             	mov    -0x4(%rbp),%eax
 21a:	2d 38 76 1f 6f       	sub    $0x6f1f7638,%eax
 21f:	5d                   	pop    %rbp
 220:	c3                   	retq   

0000000000000221 <addval_294>:
 221:	55                   	push   %rbp
 222:	48 89 e5             	mov    %rsp,%rbp
 225:	89 7d fc             	mov    %edi,-0x4(%rbp)
 228:	8b 45 fc             	mov    -0x4(%rbp),%eax
 22b:	2d 98 76 1f 6f       	sub    $0x6f1f7698,%eax
 230:	5d                   	pop    %rbp
 231:	c3                   	retq   

0000000000000232 <setval_465>:
 232:	55                   	push   %rbp
 233:	48 89 e5             	mov    %rsp,%rbp
 236:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 23a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 23e:	c7 00 8d d6 08 c0    	movl   $0xc008d68d,(%rax)
 244:	5d                   	pop    %rbp
 245:	c3                   	retq   

0000000000000246 <addval_180>:
 246:	55                   	push   %rbp
 247:	48 89 e5             	mov    %rsp,%rbp
 24a:	89 7d fc             	mov    %edi,-0x4(%rbp)
 24d:	8b 45 fc             	mov    -0x4(%rbp),%eax
 250:	2d b8 76 1f 6e       	sub    $0x6e1f76b8,%eax
 255:	5d                   	pop    %rbp
 256:	c3                   	retq   

0000000000000257 <getval_386>:
 257:	55                   	push   %rbp
 258:	48 89 e5             	mov    %rsp,%rbp
 25b:	b8 89 ca 90 c3       	mov    $0xc390ca89,%eax
 260:	5d                   	pop    %rbp
 261:	c3                   	retq   

0000000000000262 <setval_480>:
 262:	55                   	push   %rbp
 263:	48 89 e5             	mov    %rsp,%rbp
 266:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 26a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 26e:	c7 00 f4 48 89 e0    	movl   $0xe08948f4,(%rax)
 274:	5d                   	pop    %rbp
 275:	c3                   	retq   

0000000000000276 <setval_426>:
 276:	55                   	push   %rbp
 277:	48 89 e5             	mov    %rsp,%rbp
 27a:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 27e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 282:	c7 00 81 ca 20 d2    	movl   $0xd220ca81,(%rax)
 288:	5d                   	pop    %rbp
 289:	c3                   	retq   

000000000000028a <addval_134>:
 28a:	55                   	push   %rbp
 28b:	48 89 e5             	mov    %rsp,%rbp
 28e:	89 7d fc             	mov    %edi,-0x4(%rbp)
 291:	8b 45 fc             	mov    -0x4(%rbp),%eax
 294:	2d 78 3e c7 36       	sub    $0x36c73e78,%eax
 299:	5d                   	pop    %rbp
 29a:	c3                   	retq   

000000000000029b <setval_295>:
 29b:	55                   	push   %rbp
 29c:	48 89 e5             	mov    %rsp,%rbp
 29f:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 2a3:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 2a7:	c7 00 89 c1 90 c3    	movl   $0xc390c189,(%rax)
 2ad:	5d                   	pop    %rbp
 2ae:	c3                   	retq   

00000000000002af <setval_223>:
 2af:	55                   	push   %rbp
 2b0:	48 89 e5             	mov    %rsp,%rbp
 2b3:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
 2b7:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
 2bb:	c7 00 89 d6 18 d2    	movl   $0xd218d689,(%rax)
 2c1:	5d                   	pop    %rbp
 2c2:	c3                   	retq   

00000000000002c3 <addval_167>:
 2c3:	55                   	push   %rbp
 2c4:	48 89 e5             	mov    %rsp,%rbp
 2c7:	89 7d fc             	mov    %edi,-0x4(%rbp)
 2ca:	8b 45 fc             	mov    -0x4(%rbp),%eax
 2cd:	2d b8 76 1f 38       	sub    $0x381f76b8,%eax
 2d2:	5d                   	pop    %rbp
 2d3:	c3                   	retq   

00000000000002d4 <end_farm>:
 2d4:	55                   	push   %rbp
 2d5:	48 89 e5             	mov    %rsp,%rbp
 2d8:	b8 01 00 00 00       	mov    $0x1,%eax
 2dd:	5d                   	pop    %rbp
 2de:	c3                   	retq   
