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
    // x ^ y = ((~x) & y) | (x & (~y)) = ~((~((~x) & y)) & (~(x & (~y)))) 会多出一个
    // 不能同时为0 不能同时为1
    return (~(x & y)) & (~((~x) & (~y)));
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
    感觉这个也是交换，一位一位进行交换，依旧考虑怎么取出来，和上一个题目类似

    右移 31 位 <-> 右移 0 位 取交1 交换 异或写回去
    但感觉这样的运算符用得太多了 有没有简化的办法：for循环
    */
    
    for(int i = 0; (!(i >> 4)); i++){
        int move_r = 31 - i;
        int move_l = i;

        int r = (v >> move_r) & 1;
        int l = (v >> move_l) & 1;

        //要不要手搓一个异或？
        unsigned diff = ((~r) & l) | (r & (~l));
        v = ((~v) & (diff << move_r)) | (v & (~(diff << move_r)));
        v = ((~v) & (diff << move_l)) | (v & (~(diff << move_l)));
        
    }
    
    return v;
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
    给它再补一个1 
    问题在于怎么构造一个第n位为1其余位为0的数 
    想用左移但是没有减法
    用异或吗？但是感觉也没什么思路
    可以构造高位mask再取反 因为0xFFFFFFFF和0x80000000都默认unsigned了，所以用1左移右移就可以
    取and
    */
    int mask = ~(1 << 31 >> n << 1);
    return (x >> n) & mask;
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
    /*
    找最高位的0，即取反找最高位的1
    然后用32减一下 没有减法手搓补码

    */
    int v = ~x;
    int IsV0 = !v;

    int cmp_16 = !(!(v >> 16));
    int move_16 = cmp_16 << 4;
    v = v >> move_16;

    int cmp_8 = !(!(v >> 8));
    int move_8 = cmp_8 << 3;
    v = v >> move_8;

    int cmp_4 = !(!(v >> 4));
    int move_4 = cmp_4 << 2;
    v = v >> move_4;

    int cmp_2 = !(!(v >> 2));
    int move_2 = cmp_2 << 1;
    v = v >> move_2;

    int cmp_1 = !(!(v >> 1));
    int move_1 = cmp_1;
    
    int highest_0 = move_16 | move_8 | move_4 | move_2 | move_1;

    return (31 + ((~highest_0) + 1) + IsV0);
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
    /*
    符号位：保留最高位 右移31 左移31
    阶码：127 + e(最高1所在位置)
    尾数：从最高1往右：
     23位能放下：去掉首位全部保留
     23位放不下：舍入
    */

    //边界：x = 0
    if(x == 0){
        return 0;
    }

    int S = x & 0x80000000;
    // 如果是负数，取一下绝对值
    if (S){
        x = (~x) + 1;
    }

    // 同时确定尾数和阶码
    int i = 0; // i为前导0的个数
    int E;
    int M;
    while(!(x >> (31 - i))){
        i = i + 1;
    }
    E = (158 - i) << 23;


    if(i > 7){ 
        // 如果有效数字长度<=24 就把i那位作为阶码，i后面的放进尾数的前面，剩下的用0补齐(自动)，无需考虑舍入
        // 边界 i == 31, 防止出现移动32位的非法情况
        // 可以和后面的压缩在一起
        M = (x << (i - 8)) & 0x7FFFFF; //mask一下，防止算术右移导致前面全是1
    }
    else
    { 
        int drop = 8 - i;
        // 如果有效数字长度>24，就把i那位作为阶码，i后面的舍入到第23位，放进位数
        M = (x >> drop) & 0x7FFFFF; //防止左面全是1，同时直接去掉最高位
        // 舍入的时候全1的进位什么时候考虑呢？是进完位取前23位，还是用一个if把这个边界判断了呢
        // 感觉用if会好一点吧。。。不知道
        
        /*
        GRS的写法需要太多if了，因为移动32位是非法的，所以改用remainder

        // 判断要不要进位 : 存g、r、s
        int G = (x << (i + 24) >> 31) & 1; // 第24位
        int R = (x << (i + 25) >> 31) & 1; // 第25位
        int S = x << (i + 26); // 第26位及以后（左移版本，但是不影响判断是否为0）

        if (G == 0){ // 向下舍，不动
        }
        else{
            if(R | S |(M & 1)){ // 入 R=1;R=0,S=1;R=0,S=0,M为奇数
                M = M + 1;
                if(M >> 23){
                    M = 0;
                    E = E + (1<<23);
                }
            }
            else{ //舍
                
            }
        }
        */

        // 用remainder写，先把后drop位提出来，作为remainder，然后和首1后面全为0的比大小
        // 不可以左移32-drop位 和0x80000000比大小 因为int有正负
        // 直接提取末drop位
        // 大于入，小于舍，等于看M末位的奇偶

        int remainder = x & ((1 << drop) - 1);
        int cmp = 1 << (drop - 1);
        if (remainder + (M & 1) > cmp){ 
            // 这里省运算符完全没想到 它的含义是 remainder > cmp 或者 M 末位 1
            // 利用了 M & 1 只可能是 0 和 1 的性质
            M = M + 1;
            if(M >> 23){
                M = 0;
                E = E + 0x800000;
            }
        }
    }
    return S | E | M;
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
 /*
 题意: 把unsigned按照float理解，返回该float*2的结果，装在32位unsigned里面
 */
unsigned floatScale2(unsigned uf) {
    int E = (uf & 0x7F800000) >> 23;
    int M = uf & 0x7FFFFF;
    int S = uf & 0x80000000;

    if(E == 255){
        return uf;
    }
    if(E == 254){ 
        // 这里不够32位 取反会出问题
        // 只要乘二一定是无穷大
        return S | 0x7F800000;
    }
    if(E == 0){
        if(!M){
            return uf;
        }
        else
        {
            M = M << 1;
            return S | M;
        }
    }
    else{
        E = (E + 1) << 23;
        return S | E | M;
    }
    
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
    int S = uf2 & 0x80000000;
    int E = (uf2 & 0x7FF00000) >> 20;
    int e = E - 1023;
    int abs; // 先计算绝对值，最后根据S看要不要返回负值

    if (e < 0){
        return 0;
    }
    if (e > 30){ 
        // 这里边界有一个合并简化
        // e >= 32 的时候一定溢出
        // e == 31 时：32位int的范围 (-2^31) ~ (2^31 - 1)
        // 也就是说如果 s 表示正数，小数点右移 31 位一定溢出
        // 如果 s 表示负数，只有尾数为 0 的时候，小数点右移 31 位刚好 -2^31，不溢出
        // 但它就写作 0x80000000, 所以可以一同返回
        return 0x80000000;
    }
    else{
        if(e < 21){
            abs = (1 << e) | ((uf2 & 0xFFFFF) >> (20 - e));
        }
        else{
            abs = (1 << e) | ((uf2 & 0xFFFFF) << (e - 20)) | (uf1 >> (32 - (e - 20)));
        }
    }
    if(!S){
        return abs;
    }
    else{
        return -abs;
    }
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
    if(x > 127){
        // 超过单精度浮点数上限
        return 0x7F800000;
    }
    if(x < -149){
        // 低于非规格化数下限
        return 0;
    }
    if(x > -127){
        // -126 ~ 127 是规格化数
        return (x + 127) << 23;
    }
    else{
        // -149 ~ -127 是非规格化数
        return 1 << (149 + x); // 非规格化数最小是 2^-149 在末位写一个1
    }

}
