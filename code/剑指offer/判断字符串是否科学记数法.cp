// 请实现一个函数用来判断字符串是否表示数值（包括整数和小数）。
// 例如，字符串"+100","5e2","-123","3.1416"和"-1E-16"都表示数值。 但是"12e","1a3.14","1.2.3","+-5"和"12e+4.3"都不是。


class Solution {
public:
    bool isNumeric(char* string) {
        if(string==NULL)
            return false;
        if(*string=='+'||*string=='-')
            string++;
        if(*string=='\0')
            return false;
        int dot=0,num=0,nume=0;//分别用来标记小数点、整数部分和e指数是否存在
        while(*string!='\0'){
            if(*string>='0'&&*string<='9')
            {  
                string++;
                num=1;
            }
            else if(*string=='.'){
                if(dot>0||nume>0)
                    return false;
                string++;
                dot=1;
            }
            else if(*string=='e'||*string=='E')
                {
                  if(num==0||nume>0)
                      return false;
                  string++;
                  nume++;
                  if(*string=='+'||*string=='-')
                      string++;
                 if(*string=='\0')
                     return false;
                }
            else
                return false;
        }
        return true;
    }
};