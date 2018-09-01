// 将一个字符串转换成一个整数(实现Integer.valueOf(string)的功能，但是string不符合数字要求时返回0)，
// 要求不能使用字符串转换整数的库函数。 数值为0或者字符串不是一个合法的数值则返回0。


class Solution {
public:
    int StrToInt(string str) {
    	char sign;
    	int result = 0;
    	int i = 0;
        if (str[0] == '+' || str[0] == '-') {
         	sign = str[i];
         	++i;
        }
        for (; i < str.length(); ++i) {
            if((str[i]-'0') <= 9 && (str[i] - '0') >= 0) {
             	result = result*10 + (str[i]-'0');
            }
            else {
             	return 0;
            }
        }
        if(sign == '-'){
        	result = -result;
        }
        return result;
    }
};