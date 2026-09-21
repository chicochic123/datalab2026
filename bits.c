#include <stdio.h>
/* WARNING: Do not include any other libraries here,
 * otherwise you will get an error while running test.py
 * You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 *
 * Using printf will interfere with our script capturing the execution results.
 * At this point, you can only test correctness with ./btest.
 * After confirming everything is correct in ./btest, remove the printf
 * and run the complete tests with test.py.
 */

 /*
 * bitAnd - x & y using only ~ and |
 * Example: bitAnd(4, 5) = 4
 * Legal ops: ~ |
 * Max ops: 7
 * Difficulty: 1
 */
int bitAnd(int x, int y) {
    // x & y = ~((~x) | (~y))
    return ~( (~x) | (~y) );
}

/*
 * bitXor - x ^ y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 7
 *   Difficulty: 1
 */
int bitXor(int x, int y) {
    // x ^ y = ((~x) & y) | (x & (~y))
    return ((~x) & y) | (x & (~y));
}

/*
 * samesign - Determines if two integers have the same sign.
 *   0 is not positive, nor negative
 *   Example: samesign(0, 1) = 0, samesign(0, 0) = 1
 *            samesign(-4, -5) = 1, samesign(-4, 5) = 0
 *   Legal ops: >> << ! ^ && if else &
 *   Max ops: 12
 *   Difficulty: 2
 *
 * Parameters:
 *   x - The first integer.
 *   y - The second integer.
 *
 * Returns:
 *   1 if x and y have the same sign , 0 otherwise.
 */
int samesign(int x, int y) {
    /*
    先判断 x 是否为 0 :
     x == 0, 判断 y 是否为 0 :
      y == 0, return 1
      y != 0, return 0 (用 and 实现)
     x != 0, 判断 y 是否为 0：
       y == 0, return 0
       y != 0, 判断 x 和 y 是否同号(右移31位，异或):
        同号, return 1
        不同号, return 0
    */ 
    int IsX0 = (!x) && 1;
    int IsY0 = (!y) && 1;
    int Both0 = IsX0 && IsY0;
    if (IsX0){
        return Both0;
    }

    if (IsY0){
        return Both0;
    }

    return (!((x >> 31) ^ (y >> 31)));
}

/*
 * logtwo - Calculate the base-2 logarithm of a positive integer using bit
 *   shifting. (Think about bitCount)
 *   Note: You may assume that v > 0
 *   Example: logtwo(32) = 5
 *   Legal ops: > < >> << |
 *   Max ops: 25
 *   Difficulty: 4
 */
int logtwo(int v) {
    /*
    判断二进制写法下的最高位数

    肯定不够一位一位判断
    考虑几位几位并行，但怎么实现呢
    没有控制流，也没法二分查找吧

    可以用比较实现二分查找
    32bit 可以把最高位数表示成一个五位二进制数
    之后通过比较的方式做二分查找 相当于从高到低写这个二进制数 可以用 | 代替加法
    最后转回十进制就好了

    每次做完比较，记下结果之后把 v 压回合适的位数
    */
    int cmp_16 = (v >> 16) > 0;
    int move_16 = cmp_16 << 4;
    v = v >> move_16;

    int cmp_8 = (v >> 8) > 0;
    int move_8 = cmp_8 << 3;
    v = v >> move_8;

    int cmp_4 = (v >> 4) > 0;
    int move_4 = cmp_4 << 2;
    v = v >> move_4;

    int cmp_2 = (v >> 2) > 0;
    int move_2 = cmp_2 << 1;
    v = v >> move_2;

    int cmp_1 = (v >> 1) > 0;
    int move_1 = cmp_1;
    
    return (move_16 | move_8 | move_4 | move_2 | move_1);

}

/*
 *  byteSwap - swaps the nth byte and the mth byte
 *    Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *              byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *    Note: You may assume that 0 <= n <= 3, 0 <= m <= 3
 *    Legal ops: ! ~ & ^ | + << >>
 *    Max ops: 17
 *    Difficulty: 2
 */
int byteSwap(int x, int n, int m) {
    /*
    这里我知道交换是用三次异或做，但是怎么构造指定位数的mask呢
    我发现这里不是说的交换第几位到第几位，而是哪两个字节
    
    先把两个字节取出来，右移+0xFF取交
    右移八倍，乘法可以用左移3位完成
    异或交换
    放回去（？用非制作两个mask，把原来的抠出来，再把交换完的右移回到原位置，取并）
    */

    int ns = n << 3;
    int ms = m << 3;
    int byte_n = (x >> ns) & 0xFF;
    int byte_m = (x >> ms) & 0xFF;

    /*
    // 交换
    byte_n = byte_n ^ byte_m;
    byte_m = byte_n ^ byte_m;
    byte_n = byte_n ^ byte_m;
    // mask
    int mask_n = 0xFF << ns;
    int mask_m = 0xFF << ms;
    return ((x & (~mask_n) & (~mask_m)) | (byte_n << ns) | (byte_m << ms));

    这个是对的，但是运算符用超了，可以简化整个异或的过程
    */

    int diff = byte_n ^ byte_m;

    return x ^ (diff << ns) ^ (diff << ms); //自己和自己异或相当于什么都没做，这样可以把两块的计算合并成一块
}

/*
 * reverse - Reverse the bit order of a 32-bit unsigned integer.
 *   Example: reverse(0xFFFF0000) = 0x0000FFFF reverse(0x80000000)=0x1 reverse(0xA0000000)=0x5
 *   Note: You may assume that an unsigned integer is 32 bits long.
 *   Legal ops: << | & - + >> for while ! ~ (You can define unsigned in this function)
 *   Max ops: 30
 *   Difficulty: 3
 */
unsigned reverse(unsigned v) {
    /*
    感觉这个也是交换，一位一位进行交换，依旧考虑怎么取出来，mask怎么做
    */
    return 2;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Note: You can assume that 0 <= n <= 31
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Difficulty: 3
 */
int logicalShift(int x, int n) {
    /*
    先右移
    新造一个数，全1，把首位改成0，右移 不行 最后取交一定要是一个全F的数 怎么办
    取and
    */
    int result = (x >> n) & (0x7FFFFFFF >> n);
    printf("%x\n",(0x7FFFFFFF >> n));
    printf("%x\n",(x >> n));
    printf("%x %x %x\n", x, n, result);
    return result;
}

/*
 * leftBitCount - returns count of number of consective 1's in left-hand (most) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12,
 *             leftBitCount(0xFE00FF0F) = 7
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Difficulty: 4
 */
int leftBitCount(int x) {
    return 2;
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but it is to be interpreted as
 *   the bit-level representation of a single-precision floating point values.
 *   Legal ops: if else while for & | ~ + - >> << < > ! ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned float_i2f(int x) {
    return 2;
}

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: & >> << | if > < >= <= ! ~ else + ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatScale2(unsigned uf) {
    return 2;
}

/*
 * float64_f2i - Convert a 64-bit IEEE 754 floating-point number to a 32-bit signed integer.
 *   The conversion rounds towards zero.
 *   Note: Assumes IEEE 754 representation and standard two's complement integer format.
 *   Parameters:
 *     uf1 - The lower 32 bits of the 64-bit floating-point number.
 *     uf2 - The higher 32 bits of the 64-bit floating-point number.
 *   Returns:
 *     The converted integer value, or 0x80000000 on overflow, or 0 on underflow.
 *   Legal ops: >> << | & ~ ! + - > < >= <= if else
 *   Max ops: 60
 *   Difficulty: 3
 */
int float64_f2i(unsigned uf1, unsigned uf2) {
    return 2;
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: < > <= >= << >> + - & | ~ ! if else &&
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatPower2(int x) {
    return 2;
}
