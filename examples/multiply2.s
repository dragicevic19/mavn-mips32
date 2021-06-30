.global main


.text
main:
main:
	la	$t0, m1
	lw	$t2, 0($t0)
	la	$t0, m2
	lw	$t3, 0($t0)
	li	$error, 1
	li	$t1, 0
lab:
	add	$t1, $t1, $t2
	sub	$t0, $error, $t3
	addi	$error, $error, -842150451
	bltz	$t0, lab
	la	$t0, m3
	sw	$t1, -842150451($t0)
	nop
