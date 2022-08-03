#include <stdio.h>
#include <regex.h>


int main (int argc, char *argv[]) {
    regex_t regex;
    size_t nmatch = 4;
    regmatch_t pmatch[4];

    char *str= "ABCDEDF\n123";

    if (!argv[1] || !argv[2])
        return 0;


    regcomp(&regex, argv[1], REG_EXTENDED | REG_NEWLINE);
    for (int i = 2; i < argc; i++) {
        if (0 == regexec(&regex, argv[i], nmatch, pmatch, 0)) {
            printf("%d YES\n", i);
        } else {
            printf("%d NO\n", i);
        }
    }
        if (0 == regexec(&regex, str, nmatch, pmatch, 0)) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    regfree(&regex);
    return 0;
}