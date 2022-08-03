#include <stdio.h>
#include <regex.h>


int main (int argc, char *argv[]) {
    char *str, *pattern;
    regex_t regex;
    size_t nmatch = 3;
    regmatch_t pmatch[3];

    if (!argv[1] && !argv[2])
        return 0;

    pattern = argv[1];
    str = argv[2];

    regcomp(&regex, pattern, REG_EXTENDED);
    if (0 == regexec(&regex, str, nmatch, pmatch, 0)) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
    regfree(&regex);

    return 0;
}

/* 2 * (1 + 8 + 13 + 26) = 96 /(32 ~ 127)
SP + DEL
[]\;',./
{}|:"<>?
~!@#$%^&*()_+
`1234567890-=
abcdefghijklmnopqrstuvwxyz
ABCDEFGHIJKLMNOPQRSTUVWXYZ

*/

/*

 - .
 - *
 - +
 - ?
 - ^
 - $
 - |
 - ()
 - []
 - [^]
 - {m,n}

 - \A: 匹配字符串的开头
 - \b: 匹配单词的结尾
 - \d: 匹配数字字符
 - \D: 匹配非数字字符
 - \w: 匹配{字母/数字/-}字符
 - \W: 匹配非{字母/数字/-}字符
 - \s: 匹配空白字符
 - \S: 匹配非空白字符
 - \z: 匹配字符串的结尾
 */



char *ascii[]