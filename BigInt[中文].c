/*
    BigInt: ������
    update: 2014.06.09 by xkuga

    ����ʵ�����⼸�ִ������㣺+, -, *, /, ^, %, ^%
    ���õ��Ƕ����Ƶķ�ʽʵ��, BigInt�Ĵ洢����������ò���
    �˷������õ���Boothһλ��
    �������ö��ַ�ȥ������
    ��������ģ�����㣺���ǲ��ö����Ƶķ�ʽʵ�ֵģ�����һ��һ����
    ����ʵ��ԭ��ɿ�΢��@C����������ԣ�http://www.weibo.com/cpl86

    ��Ȼ���ܲ��Ǻܿ죬������������><
    ���ҳ���Ƚ��׶�����������������������һ���İ���
    ������κδ����Ľ�����΢����@C����������ԣ�лл!
    �����Ĵ���ɵ�Github�����أ�https://github.com/xkuga/bigint
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BIG_INT_BIT_LEN 1024           // ����BigInt��λ��
#define SIGN_BIT BIG_INT_BIT_LEN - 1   // ����λ��λ��
#define BUFFER_SIZE BIG_INT_BIT_LEN    // ��������С
#define POSITIVE 0                     // 0��ʾ����
#define NEGATIVE 1                     // 1��ʾ����

typedef struct    // ���������ͣ����ò����ʾ
{
    char bit[BIG_INT_BIT_LEN];
}BigInt;

typedef struct    // Number������ֵ��ʾ
{
    char value[BIG_INT_BIT_LEN];  // ��������
    int len;                      // ���鳤��
    int sign;                     // ���ű��
}Number;

// ��ӡBigInt
void PrintBigInt(BigInt* a)
{
    int i;
    for (i = SIGN_BIT; i >= 0; i--)
        printf("%d", a->bit[i]);
    printf("\n");
}

// ��ӡNumber
void PrintNumber(Number* n)
{
    int i;

    if (n->sign == NEGATIVE)
        printf("-");

    for (i = n->len - 1; i >= 0; i--)
    {
        if (n->value[i] > 9)  // ����10���Ƶ����
            printf("%c", n->value[i] - 10 + 'a');
        else
            printf("%d", n->value[i]);
    }
    printf("\n");
}

// ��strתΪNumber��������, ����Number*
Number* StrToNumber(char* str, Number* n)
{
    int i, j;

    if (str[0] == '-' || str[0] == '+')  // 0�ŵ�Ԫ��ŷ���
    {
        n->len = strlen(str) - 1;
        n->sign = str[0] == '+' ? POSITIVE : NEGATIVE;

        for (i = 0, j = n->len; j > 0; j--, i++)
        {
            if (str[j] > '9')  // ����10���Ƶ����
                n->value[i] = str[j] - 'a' + 10;
            else
                n->value[i] = str[j] - '0';
        }
    }
    else
    {
        n->len = strlen(str);
        n->sign = POSITIVE;

        for (i = 0, j = n->len - 1; j >= 0; j--, i++)
        {
            if (str[j] > '9')  // ����10���Ƶ����
                n->value[i] = str[j] - 'a' + 10;
            else
                n->value[i] = str[j] - '0';
        }
    }

    return n;
}

// Number����ת�ַ�������
char* NumberToStr(Number* n, char* str)
{
    int i = 0, j;

    if (n->sign == NEGATIVE)
        str[i++] = '-';

    for (j = n->len - 1; j >= 0; j--)
    {
        if (n->value[j] > 9)  // ����10���Ƶ����
            str[i++] = n->value[j] - 10 + 'a';
        else
            str[i++] = n->value[j] + '0';
    }

    str[i] = '\0';

    return str;
}

// 2�����ַ���ת16�����ַ���
char* BinStrToHexStr(char* binStr, char* hexStr)
{
    int i, j, t;
    Number binNum;
    Number hexNum;

    StrToNumber(binStr, &binNum);

    hexNum.sign = binNum.sign;
    hexNum.len = (int)ceil(binNum.len / 4.0);

    for (i = 0; i < hexNum.len; i++)
    {
        j = 4 * i;

        t = binNum.value[j];

        if (j + 1 < binNum.len)
            t += 2 * binNum.value[j + 1];

        if (j + 2 < binNum.len)
            t += 4 * binNum.value[j + 2];

        if (j + 3 < binNum.len)
            t += 8 * binNum.value[j + 3];

        hexNum.value[i] = t;
    }

    return NumberToStr(&hexNum, hexStr);
}

// �ַ�������ת��
char* ChangeStringRadix(char* str, int srcBase, int dstBase, char* resultStr)
{
    if (srcBase < dstBase)
    {
        char hexStr[BUFFER_SIZE];

        ChangeStringRadix(str, srcBase, 2, resultStr);
        BinStrToHexStr(resultStr, hexStr);

        return ChangeStringRadix(hexStr, 16, dstBase, resultStr);
    }

    if (srcBase == dstBase)
    {
        return strcpy(resultStr, str);
    }

    else
    {
        int i, t;
        Number dividend;   // ������
        Number quotient;   // ��
        Number resultNum;  // ���

        // ��strת��ΪNumber��������
        StrToNumber(str, &dividend);

        resultNum.len = 0;
        resultNum.sign = dividend.sign;

        while (dividend.len > 0)
        {
            quotient.len = dividend.len;

            // ģ�����������ķ�ʽ, ��һ��(��һλ����)�Ĺ���
            for (t = 0, i = dividend.len - 1; i >= 0; i--)
            {
                t = t * srcBase + dividend.value[i];
                quotient.value[i] = t / dstBase;
                t = t % dstBase;      // ѭ������t��Ϊһ�ֵĽ��
            }

            // ����һ�ֵĽ��, ��һλ����
            resultNum.value[resultNum.len++] = t;

            // �������ж����0
            for (i = quotient.len - 1; i >= 0 && quotient.value[i] == 0; i--);

            dividend.len = i + 1;

            // ������Ϊ��һ�ֵı�����
            for (i = 0; i < dividend.len; i++)
            {
                dividend.value[i] = quotient.value[i];
            }
        }

        return NumberToStr(&resultNum, resultStr);
    }
}

// ԭ��<=>����
BigInt* ToComplement(BigInt* src, BigInt* dst)
{
    int i;

    if (src->bit[SIGN_BIT] == NEGATIVE)  // ������
    {
        dst->bit[SIGN_BIT] = 1;

        for (i = 0; i < SIGN_BIT && src->bit[i] == 0; i++)
            dst->bit[i] = src->bit[i];

        if (i == SIGN_BIT)    // -0�Ĳ���
            dst->bit[i] = 0;
        else                  // ��0����
        {
            dst->bit[i] = src->bit[i];
            for (i++; i < SIGN_BIT; i++)
                dst->bit[i] = !src->bit[i];
        }
    }
    else  // �����󲹲���
    {
        for (i = 0; i < BIG_INT_BIT_LEN; i++)
            dst->bit[i] = src->bit[i];
    }

    return dst;
}

// תΪԭ��
BigInt* ToTrueForm(BigInt* src, BigInt* dst)
{
    return ToComplement(src, dst);
}

// תΪ�෴���Ĳ��� [x]�� => [-x]��, 
// ע�⣺���������8λ������������-128�෴���Ĳ���
// �㷨��˼������ͬ����λһ���󲹣�������λҲҪȡ������֤������ȷ��
BigInt* ToOppositeNumberComplement(BigInt* src, BigInt* dst)
{
    int i;

    for (i = 0; i < BIG_INT_BIT_LEN && src->bit[i] == 0; i++)
        dst->bit[i] = src->bit[i];

    // ���0�෴���Ĳ���
    if (i != BIG_INT_BIT_LEN)
    {
        dst->bit[i] = src->bit[i];

        // ������λҲҪȡ��
        for (i++; i < BIG_INT_BIT_LEN; i++)
            dst->bit[i] = !src->bit[i];
    }

    return dst;
}

/* ����ʵ��
// תΪ�෴���Ĳ��� [x]�� => [-x]��
BigInt* ToOppositeNumberComplement(BigInt* src, BigInt* dst)
{
    BigInt t;

    ToTrueForm(src, &t);
    t.bit[SIGN_BIT] = !t.bit[SIGN_BIT];
    ToComplement(&t, dst);

    return dst;
}
*/

// 2����NumberתBigInt
BigInt* BinNumToBigInt(Number* binNum, BigInt* a)
{
    int i;

    memset(a->bit, 0, BIG_INT_BIT_LEN);  // ��ʼ��Ϊ0

    for (i = 0; i < binNum->len; i++)
    {
        a->bit[i] = binNum->value[i];
    }

    // ����ȡ�½���������4λ����[1000]
    if (binNum->len == BIG_INT_BIT_LEN)
    {
        return a;
    }
    else
    {
        a->bit[SIGN_BIT] = binNum->sign;  // ����λ
        return ToComplement(a, a);
    }
}

// BigIntת2����Number
Number* BigIntToBinNum(BigInt* a, Number* binNum)
{
    int i;
    BigInt t;

    binNum->sign = a->bit[SIGN_BIT];

    for (i = SIGN_BIT - 1; i >= 0 && a->bit[i] == 0; i--);

    // BigIntΪ�������½�ʱ
    if (binNum->sign == NEGATIVE && i == -1)
    {
        binNum->len = BIG_INT_BIT_LEN;
        for (i = 0; i < binNum->len; i++)
            binNum->value[i] = a->bit[i];
    }
    else
    {
        ToTrueForm(a, &t);
        for (i = SIGN_BIT - 1; i >= 0 && t.bit[i] == 0; i--);
        binNum->len = i == -1 ? 1 : i + 1;
        for (i = 0; i < binNum->len; i++)
            binNum->value[i] = t.bit[i];
    }

    return binNum;
}

// �ַ���תBigInt���Բ���洢
BigInt* StrToBigInt(char* s, BigInt* a)
{
    char buf[BUFFER_SIZE];
    Number binNum;

    ChangeStringRadix(s, 10, 2, buf);              // ʮ����ת������
    StrToNumber(buf, &binNum);           // stringתNumber
    return BinNumToBigInt(&binNum, a);   // NumberתBigInt
}

// BigIntת�ַ�������10���Ʊ�ʾ
char* BigIntToStr(BigInt* a, char* s)
{
    char buf[BUFFER_SIZE];
    Number binNum;

    BigIntToBinNum(a, &binNum);     // BigIntתNumber
    NumberToStr(&binNum, buf);      // Numberתstring

    return ChangeStringRadix(buf, 2, 10, s);  // ������תʮ����
}

// ����BigInt
BigInt* CopyBigInt(BigInt* src, BigInt* dst)
{
    int i;
    for (i = 0; i < BIG_INT_BIT_LEN; i++)
        dst->bit[i] = src->bit[i];
    return dst;
}

// ��������
BigInt* ShiftArithmeticLeft(BigInt* src, int indent, BigInt* dst)
{
    int i, j;

    dst->bit[SIGN_BIT] = src->bit[SIGN_BIT];

    for (i = SIGN_BIT - 1, j = i - indent; j >= 0; i--, j--)
    {
        dst->bit[i] = src->bit[j];
    }

    while (i >= 0)
    {
        dst->bit[i--] = 0;
    }

    return dst;
}

// �ӷ�ʵ��
BigInt* DoAdd(BigInt* a, BigInt* b, BigInt* result)
{
    int i, t, carryFlag;           // ��λ��־
    int aSign = a->bit[SIGN_BIT];  // a�ķ���
    int bSign = b->bit[SIGN_BIT];  // b�ķ���

    for (carryFlag = i = 0; i < BIG_INT_BIT_LEN; i++)
    {
        t = a->bit[i] + b->bit[i] + carryFlag;
        result->bit[i] = t % 2;
        carryFlag = t > 1 ? 1 : 0;
    }

    if (aSign == bSign && aSign != result->bit[SIGN_BIT])
    {
        printf("Overflow XD\n");
        exit(1);
    }

    return result;
}

// ����ʵ��
BigInt* DoSub(BigInt* a, BigInt* b, BigInt* result)
{
    BigInt t;

    ToOppositeNumberComplement(b, &t);
    DoAdd(a, &t, result);

    return result;
}

// �˷�ʵ�� Booth�㷨[����1λ��] ת��Ϊ��λ�ͼӷ�
BigInt* DoMul(BigInt* a, BigInt* b, BigInt* result)
{
    int i;
    BigInt c, t;

    ToOppositeNumberComplement(a, &c);  // c=[-a]�Ĳ�

    memset(t.bit, 0, BIG_INT_BIT_LEN);  // ��ʼ��Ϊ0

    // �Ӹ�λ����ʼ��������ͬ��λ����Ϊ���Ϊ0
    for (i = SIGN_BIT; i > 0 && b->bit[i] == b->bit[i - 1]; i--);

    while (i > 0)
    {
        ShiftArithmeticLeft(&t, 1, &t);

        if (b->bit[i] != b->bit[i - 1])
        {
            DoAdd(&t, b->bit[i - 1] > b->bit[i] ? a : &c, &t);
        }

        i--;
    }

    // ���һ������λ
    ShiftArithmeticLeft(&t, 1, &t);
    if (b->bit[0] != 0)
    {
        DoAdd(&t, &c, &t);
    }

    return CopyBigInt(&t, result);
}

// �ڲ����������£���ȡ����������Ƶĳ���
int GetMaxLeftShiftLen(BigInt* a)
{
    int i, k;
    BigInt t;

    ToTrueForm(a, &t);

    for (i = SIGN_BIT - 1, k = 0; i >= 0 && t.bit[i] == 0; i--, k++);

    return k;
}

// �ж�Bigint�Ƿ�Ϊ0
int IsZero(BigInt* a)
{
    int i;
    for (i = 0; i < BIG_INT_BIT_LEN; i++)
    {
        if (a->bit[i] != 0)
            return 0;
    }
    return 1;
}

// ����ʵ�� ��2�ַ�ȥ���̵ĸ���Ϊ1��λ д�ò������><
BigInt* DoDiv(BigInt* a, BigInt* b, BigInt* result, BigInt* remainder)
{
    int low, high, mid;
    BigInt c, d, e, t;

    low = 0;                       // ��ʼ����������ֵ
    high = GetMaxLeftShiftLen(b);  // ��ȡ����������Ƶĳ���

    memset(t.bit, 0, BIG_INT_BIT_LEN);  // ��ʼ����Ϊ0
    CopyBigInt(a, &c);                  // ��ʼ��cΪ������a

    // ͬ���������
    if (a->bit[SIGN_BIT] == b->bit[SIGN_BIT])
    {
        t.bit[SIGN_BIT] = POSITIVE;

        while (1)
        {
            while (low <= high)
            {
                mid = (low + high) / 2;
                ShiftArithmeticLeft(b, mid, &d);
                DoSub(&c, &d, &e);  // e = c - d

                // e >= 0����ʾ����
                if (d.bit[SIGN_BIT] == e.bit[SIGN_BIT] || IsZero(&e))
                    low = mid + 1;
                else
                    high = mid - 1;
            }

            // high����󹻼�����λ��
            // high == -1 ��ʾ�Ѿ���1���ĳ�������������
            if (high != -1)
            {
                t.bit[high] = 1;

                // ����ͳһ�����ˣ��ɸĽ�
                ShiftArithmeticLeft(b, high, &d);
                DoSub(&c, &d, &c);  // c = c - d
                
                low = 0;
                high--;
            }
            else
            {
                // ��ʱc����ʾ�ı�������Ϊ��������
                CopyBigInt(&c, remainder);
                break;
            }
        }
    }

    // ����������
    else
    {
        t.bit[SIGN_BIT] = NEGATIVE;

        while (1)
        {
            while (low <= high)
            {
                mid = (low + high) / 2;
                ShiftArithmeticLeft(b, mid, &d);
                DoAdd(&c, &d, &e);  // e = c + d

                // e >= 0
                if (d.bit[SIGN_BIT] != e.bit[SIGN_BIT] || IsZero(&e))
                    low = mid + 1;
                else
                    high = mid - 1;
            }

            // high����󹻼�����λ��
            // high == -1 ��ʾ�Ѿ���1���ĳ�������������
            if (high != -1)
            {
                t.bit[high] = 1;

                // ����ͳһ�����ˣ��ɸĽ�
                ShiftArithmeticLeft(b, high, &d);
                DoAdd(&c, &d, &c);  // c = c + d

                low = 0;
                high--;
            }
            else
            {
                // ��ʱc����ʾ�ı�������Ϊ��������
                CopyBigInt(&c, remainder);
                break;
            }
        }
    }

    return ToComplement(&t, result);
}

char* Add(char* s1, char* s2, char* result)
{
    BigInt a, b, c;
    
    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoAdd(&a, &b, &c);

    return BigIntToStr(&c, result);
}

char* Sub(char* s1, char* s2, char* result)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoSub(&a, &b, &c);
    
    return BigIntToStr(&c, result);
}

char* Mul(char* s1, char* s2, char* result)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoMul(&a, &b, &c);

    return BigIntToStr(&c, result);
}

char* Div(char* s1, char* s2, char* result, char* remainder)
{
    BigInt a, b, c, d;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoDiv(&a, &b, &c, &d);
    BigIntToStr(&d, remainder);

    return BigIntToStr(&c, result);
}

// ��ģʵ��
BigInt* DoMod(BigInt* a, BigInt* b, BigInt* remainder)
{
    BigInt c;

    DoDiv(a, b, &c, remainder);

    return remainder;
}

char* Mod(char* s1, char* s2, char* remainder)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoMod(&a, &b, &c);

    return BigIntToStr(&c, remainder);
}

// ��ȡBigInt��ֵ��λ����
int GetTrueValueLen(BigInt* a)
{
    int i;
    BigInt t;

    ToTrueForm(a, &t);

    for (i = SIGN_BIT - 1; i >= 0 && t.bit[i] == 0; i--);

    return i + 1;
}

// ������(������ʵ��) ��������
BigInt* DoPow(BigInt* a, BigInt* b, BigInt* result)
{
    int i, len;
    BigInt t, buf;

    CopyBigInt(a, &buf);
    StrToBigInt("1", &t);
    len = GetTrueValueLen(b);  // ��ȡBigInt��ֵ��λ����

    for (i = 0; i < len; i++)
    {
        if (b->bit[i] == 1)
            DoMul(&t, &buf, &t);  // t = t * buf

        // ������������һ��
        DoMul(&buf, &buf, &buf);  // buf = buf * buf
    }
    
    return CopyBigInt(&t, result);
}

char* Pow(char* s1, char* s2, char* result)
{
    BigInt a, b, c;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    DoPow(&a, &b, &c);

    return BigIntToStr(&c, result);
}

// ģ������(������ʵ��)
BigInt* DoPowMod(BigInt* a, BigInt* b, BigInt* c, BigInt* result)
{
    int i, len;
    BigInt t, buf;

    CopyBigInt(a, &buf);
    StrToBigInt("1", &t);
    len = GetTrueValueLen(b);  // ��ȡBigInt��ֵ��λ����

    for (i = 0; i < len; i++)
    {
        if (b->bit[i] == 1)
        {
            DoMul(&t, &buf, &t);  // t = t * buf
            DoMod(&t, c, &t);     // t = t % c;
        }

        // ������������һ��
        DoMul(&buf, &buf, &buf);  // buf = buf * buf
        DoMod(&buf, c, &buf);     // buf = buf % c
    }

    return CopyBigInt(&t, result);
}

char* PowMod(char* s1, char* s2, char* s3, char* result)
{
    BigInt a, b, c, d;

    StrToBigInt(s1, &a);
    StrToBigInt(s2, &b);
    StrToBigInt(s3, &c);
    DoPowMod(&a, &b, &c, &d);

    return BigIntToStr(&d, result);
}

int main()
{
    // printf("hello, world");

    char result[BUFFER_SIZE];
    char remainder[BUFFER_SIZE];

    // ����ʱ����ȷ��BigInt��λ���㹻��������������
    // ��ͨ���ı����ʼ����ĺ�: BIG_INT_BIT_LEN��Ĭ����1024λ

    // �������
    puts(Add("2010", "4", result));
    puts(Sub("0", "2014", result));
    puts(Mul("2", "43", result));
    puts(Div("86", "10", result, remainder));
    puts(remainder);
    puts(Mod("-86", "10", result));
    puts(PowMod("7", "80", "86", result));

    // ��������
    puts(Sub("233333333333333333333333333333333333333333333333", "33", result));
    puts(Mul("2333333333333333333333333333333", "2333333333333333333", result));
    puts(Div("2333333333333333333333333333333", "2333333333333333332", result, remainder));
    puts(remainder);
    puts(Pow("8", "86", result));

    return 0;
}
