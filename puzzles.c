/* 
 * Lab 1 - A Bit Puzzling 
 * 
 * Kim Hancock
 * 
 * puzzles.c - Source file with your puzzle solutions.
 *
 * NOTE: Do not include the <stdio.h> header; it confuses the blc
 * compiler. You can still use printf for debugging, although you
 * might get a compiler warning (this is an exception to the
 * 'never accept compiler warnings' rule!).
 */

/* 
 * maxVal - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 *
 * Basic idea is that we are returning 0x7FFFFFFF, so we use the xOR
 * operator on 100..0000 and ~0
 */
int maxVal() {
  int x = 1; // initialize x, which will become our mask
  int y;//initialize y
  int z; //initialize z
  x <<= 31;//shift x by 31, gives a 1 in the most significant bit
  y = ~0; //becomes all 1s
  z = x^y; //XOR gives a 0 in most sig bit, 1s elsewhere
  return z;

}

/* 
 * negCheck - return 1 if x < 0, return 0 otherwise 
 *   Example: negCheck(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 1
 *
 *   We simply right shift by 31, which will give -1 if x is negative 
 *   and 0 if it is positive. We then AND with 1 so that -1 becomes 1
 *   and 0 stays 0 
 */
int negCheck(int x) {
	return (x>>31)&1;
}

/* 
 * lsbCopy - set all bits of result to least significant bit of x
 *   Example: lsbCopy(5) = 0xFFFFFFFF, lsbCopy(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 1
 *
 * The idea here is that left shift fills in with 0s to the right and throws left bits away.
 * So left shift 31 will bring the LSB to the MSB, then right shifting back fills it all in
 */
int lsbCopy(int x) {
  return(x << 31)>>31; //simply left shift then right shift back
}

/* 
 * andBits - x&y using only ~ and | 
 *   Example: andBits(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 *
 *
 * This one uses DeMorgan's laws which state that NOT (a AND b) = NOT a OR NOT b.
 *  So, it must be that NOT( NOT( a AND B)) = a AND B = NOT (NOT a OR NOT b)
 *  Hence, the following simple function 
 */
int andBits(int x, int y) {
  return ~(~x | ~y);
}

/* 
 * xorBits - x^y using only ~ and & 
 *   Example: xorBits(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 *
 * Again, uses deMorgan's laws. First, we know from logic (I took math reasoning) that
 *  the definition of XOR is (~x&y)|(x&~y).
 * We can then use DeMorgan's law  to get rid of the | operator, which states that ~(a OR b) = ~a AND ~b. 
 * To combine the definition of XOR with DeMorgan's laws, we let a = (~x&y)
 *  and b = (x&~y), therefore getting the desired result. We then get that ~(a OR b) =
 *  ~(~x+y) & ~(x+~y). Then, since we want to get a OR b, we negate the entire statement.
 */
int xorBits(int x, int y) {
	int notAorB = (~(~x&y)) & (~(x&~y)); //uses definition of XOR and deMorgan's
       	return ~(notAorB); //negate it to get a OR b
}

/* 
 * setThirdBits - return value with every third bit (starting with the LSB) set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 *  
 * We start with a 'base' of 1 byte where every third bit is 1, so 01001001, or 0x49. 
 * We then left shift that base left by 9 and OR with the base so that the first 17 
 * bits are set to 1 every third bit. We then let this be the new base.
 * Note that we shift by 9 not 8, so that we maintain every third bit being 1
 * (since the 2 most sig bits of 0x49 are 01, we need another 0 before the LSB of the base).
 *  Then, we do that one more time, shifting the new base by 18 and OR it with the base
 *  so that all 32 bits are set so that every third bit is 1. The 'extra' bits are dropped
 *  in the left shift.
 *
 */
int setThirdBits() {
  int x = 0x49; //starting sequence of 8 bits so every third is 1
  int y =  x | (x<<9); //shift x by 9 so the first 9 bits are 0, then OR so first 17 bits are correct
  int z = y | (y<<18); //shift y by 18 so the first 18 bits are 0, then OR so all 32 bits are correct
  return z; //returns final number
}

/* 
 * byteExtract - Extract byte n from x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: byteExtract(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 *
 *   Basic idea here is that we need to shift x by 8 times n so that the nth byte is the LSByte.
 *   We then have to AND with 0xff so that the first 7 bytes are 0. This is now the value
 *   of the bit we want to extract.
 *
 *   Shifting n left 3 will multiply n by 2^3, or 8. We then shift x right by this value (note, if n is 0,
 *   we are shifting 0). Now, the nth byte will be in the 0 byte position.
 *   We have to AND with a mask of 0xff so that if the original number was negative, we omit the 1's 
 *   that resulted from the right shift.
 */
int byteExtract(int x, int n) {
	int nthbit = x >> (n<<3);
	return 0xFF & nthbit; 
}

/* 
 * bitFit - return 1 if x can be represented as an 
 *  n-bit, two's complement integer or 0 otherwise.
 *   1 <= n <= 32
 *   Examples: bitFit(5,3) = 0, bitFit(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 *
 *
 * 
 *   We have two cases to consider here:
 *   	1) x is positive
 *   		in this case, we just shift x by (n-1).
 *   		If the result is 0, we know we can
 *   		represent x in n bits, since it means that
 *   		all of the bits of value 1  have been dropped 
 *   		during the shift. If the result
 *   		is not 0, we know we can't since it means there
 *   		are 1 bits beyond the nth bit.
 *   	2) x is negative
 *   		This case is trickier since we have to worry
 *   		about the sign bit, which takes up an  extra bit.
 *   		We consider this by shifting x right by 31, so that
 *   		the resulting number is -1 if x is negative, and 0
 *   		if x is positive. We then AND this value with
 *   		~x. The intuition behind this is that smaller negative
 *   		numbers have 1s in the MSB and 0s in less sig. bits,
 *   		so when we negate x we get 0s in the more sig. bits.
 *   		This is basially undoing the operation y = ~x + 1, which
 *   		negates a number in two's complement, so that when we do the
 *   		operation ~x & mask, we are changing all the leading 1s to 0.
 *
 *   	We then consider each of these cases separately by using the AND operation
 *   	with the mask. When x is negative, ~x & mask will be one less than its
 *   	positive value. Then, when we shift by n-1, it will be 0 if we can represent 
 *   	it as a n-bit number.
 *   	When x is positive, x & ~mask will be the same as x, since ~mask is -1.
 *
 *   	We then negate the shifted number to get the desired return value
 *   	
 */
int bitFit(int x, int n) {
  int mask = x >> 31; //use this to get sign of x
  int neg = (~x & mask); //0 if x is positive
  int pos = (x & ~mask); //0 if x is negative
  int y = neg + pos; //add them to cover both cases
  int z = !(y >> (n + (~0))); //shift by n-1
  return z;
}

/* 
 * byteSwitch - swaps the nth byte and the mth byte
 *  Examples: byteSwitch(0x12345678, 1, 3) = 0x56341278
 *            byteSwitch(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 *
 *  We are switching bytes, so any shift we do will have to 
 *  be multiplied by 8. To encode this, we just left shift m and n
 *  by 3. Now, we want to create variables which are the value of the 
 *  n and mth bits of x, so we do this by shifting x by n<<3. We then
 *  AND with 0xff to make  every byte except the first 0.
 *
 *  We then create two masks, one at byte n, and one at byte m.
 *  We then OR these to get all 1s at bytes n and m, and 0 everywhere
 *  else. We XOR with -1 to get the opposite -- zeroes at byte 
 *  n and m, and 1 everywhere else. We do this so that (x AND mask)
 *  gives 0s at byte n and m, and the origianl bits of x
 *  in their correct location  everywhere else.
 *
 *  We then want to take the value of byte n and shift it to position
 *  m. Likewise, we take the value of byte m and shift it to position
 *  n. We then OR these together, and finally OR it with (x & mask) to
 *  get the desired result.
 */
int byteSwitch(int x, int n, int m) {
    int x_n = 0xFF & (x>>(n<<3)); //put nth byte in position 0
    int x_m = 0xFF & (x>>(m<<3)); //put mth byte in position 0
    int n_mask = 0xFF <<(n << 3); //we will use this to put the mth byte in nth position
    int m_mask = 0xFF <<(m<<3); //we will use this to put the nth bit in mth position
    int mask = (n_mask | m_mask) ^ ~0; //XOR will flip all the bits so that there are 1s everywhere but byte n and m
    int xMask = x & mask; //original number BUT with zeroes in byte n and m
    int nmShift = (x_m << (n<<3)) | (x_n << (m<<3)); //puts n and m in their correct byte location
    return xMask | nmShift; //combines with xMask to get desired result
}

/* 
 * addOverflow - Determine if can compute x+y without overflow
 *   Example: addOverflow(0x80000000,0x80000000) = 0,
 *            addOverflow(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 *
 *   The idea of this one comes from the fact that if you start
 *   with two negative numers, the MSB will be 0 when you add them and 
 *   there's overflow, so when you XOR with the original, you get a 1 in the 
 *   MSB. If you start with two positive numbers, the MSB will be 1 if there's overflow,
 *   so when you XOR with the original you get a 1 in the MSB.
 *   Then, if sum ^ y and sum ^ x has a 1 in the MSB, it will return 1 if we right shift
 *   by 31
 *   We then negate this to get 0 if there is overflow and 1 if not
 *
 */
int addOverflow(int x, int y) {
  int sum = x + y; //changes the MSB if overflow
  return !(((sum ^ x) & (sum ^ y))>> 31); //XOR with original so that MSB is 1 if overflow
}

/* 
 * shiftLogical - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: shiftLogical(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 *
 *   We want to cover the case that the MSB is 1 (since if its 0, arithmetic and logical
 *   right shifts both fill with 0), which for an arithmetic right
 *   shift would fill the left with 1s. But we want it to fill with 0s
 *   To do this, we want to make a mask of 1s in the n MSBs and 0's everywhere else. We then negate
 *   the mask and AND it with x arithmetically shifted by n. This way, the n MSB bits will ALWAYS be 0 (since 0
 *   AND 1 is 0), and the rest of the digits will stay the same
 */
int shiftLogical(int x, int n) {
	int mask = ((1 << 31) >> n)<<1;//to create the mask, we shift 1 left to get 0x80000000, and then shift it right by n-1
	return ((x>>n)&(~mask)); //negate mask and AND it with x arithmetically shifted by n
}

/* 
 * not - Compute !x without using !
 *   Examples: not(3) = 0, not(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 *
 *   Returns 0 if x is not 0 and 1 if x is 0
 *   We want to change x so that it is 0 if the number is 0
 *   and -1 if the number is not 0, then add 1 to it so that 
 *   the final number is 0 if it's not 0 and 1 if its 0
 *
 *   To do this, we want to manipulate x so that the MSB is 1 if 
 *   the number is not 0, whether it's positive or negative. Then, we can shift by 31 right
 *   and add 1 to get the desired result.  This is done by negating x 
 *   and adding 1, essentially reversing the process of creating a negative number
 *   in two's complement.  We then OR it with x, so whether or not the original is negative, the MSB is 1. 
 *   This works since if x is positive, ~x + 1 | x will have a 1 in the MSB. If x
 *   is 0, this still works since (~x+1)=0 if x is 0. Then, a right shift will just be 0.
 *   Then, when we OR it with x, we just get 0. Then, we shift this right by 31
 *   which is -1 if x is NOT zero and 0 if x is 0. Simply add 1 to get the desired result.
 *
 */
int not(int x) {
  int y = ((~x + 1) | x)>> 31; //y is 
  return y + 1;
}
/* 
 * signMagnitude - Convert from two's complement to sign-magnitude 
 *   where the MSB is the sign bit
 *   You can assume that x > TMin
 *   Example: signMagnitude(-5) = 0x80000005.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 *
 * Basic idea is that if  the number is positive it stays the same, and if it's negative,
 * we want it to have a 1 out front followed by the positive representation of the number
 *
 *   The tricky part is having this work for positive and negative numbers. To do this, 
 *    we right shift the number by 31, calling this number mask, which has a value of 0
 *    if the number is positive and -1 if the number is negative. 
 *    We then left shift mask  by 31, calling it sign,  to get 0x80000000 if x
 *    is negative and 0 if the number is positive. 
 *
 *    The next part is to use mask to make x equal the absolute value of x in two's complement.
 *    Once we have this, we can simply OR it with sign to get the sign-magnitude version of the
 *    number. 
 *
 *    To use mask correctly (ie, not change x if it is positive), we want to perform ~(x-1)
 *    if x is negative (the inverse of taking a positive # and making it neg in 2's complement)
 *    in a way that does not change x if it is positive. What we do is we add mask to x (since if x
 *    is positive, mask is simply 0) and XOR the result with mask. When x is negative, mask is -1,
 *    so XOR flips all the bits (negating it). 
 */
int signMagnitude(int x) {
	int mask = x >> 31; //mask is -1 if x is negative, 0 if positive
	int sign = mask << 31; //sign is 0 if x is positive, 0x80000000 if negative
	int posNumber = ((x + mask)^mask); //posNumber is the absolute value of x
	return posNumber | sign; //gets the sign bit if x is negative
}

/* 
 * fp_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 *
 *   We know that the minimum NaN is all 1's in expU and at least 1 in the frac bit.
 *   We can then store this value as a constant.
 *   We also create a mask to get rid of the sign bit by shifting 1 left by 31
 *   and XOR with -1, to get 0x7FFFFFFFF
 *   The absolute value of the f.p number is thus the number AND mask
 *
 *   We now break it up into cases: 
 *   	if absVal is greater than the unsigned version of the minimum NaN,
 *   	we return the value.
 *   	Else, we return the absVal.
 */
unsigned fp_abs(unsigned uf) {
  unsigned mask = (~0)^(1<<31);
  unsigned minimum = 0x7F800001;
  unsigned absVal = uf & mask;

  // check if it is Not a Number, which happens when expU = 11111111 and frac != 0
  if (absVal >= minimum){
	  return uf;
  }
  	else {
		return absVal;
	}
}

/* 
 * fp_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 3
 *
 *   Case by case: we start with a mask to check what the value of expU is.
 *   We get this mask by shifting uf right by 23 (since there are 23 frac bits)
 *   and then AND with 0xFF (so that if the number is negative we mask the leading
 *   1's from the shift. The resulting number is simply the value of expU.
 *
 *   If uf is 0, simply return uf.
 *
 *   If expU is not 0, we want to simply add 1 to expU, since this will raise the 
 *   power of 2 by 1.
 *
 *   If expU is 0xFF, then we know uf is either infinity (in which case 2*uf = uf)
 *   or NaN. In either case, we return uf.
 *
 *   If expU is 0, we want to shift the frac bit left by 1 so that we add 1 to the negative power.
 *   Then, we have to ensure we keep the sign bit 1 if uf is negative, so we OR uf<<1 with (uf AND 
 *   10000000). Then, if uf is negative, when the MSB gets cut off from the right shift, we still
 *   have a 1 in the MSB.
 */
unsigned fp_twice(unsigned uf) {
	int expMask = (uf >> 23) & 0xFF; //expMask is the value of expU
	if(!uf){ //value is TRUE if uf == 0
		return uf;
	}
	if(!expMask) { //value is TRUE is expMask==0, denormalized case
		return (uf & (1<<31)) | (uf<<1); //shift frac bit and keep sign bit
	}
	if(expMask == 0xFF) { //NaN or infinity
		return uf;
	}
	return uf + (1<<23); //normalized case, just add 1 to expU
}

