#include "PJ_RPI.h"
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main()
{
    // init gpio's
    if (map_peripheral(&gpio) == -1)
    {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }

    // connect mysql
    MYSQL *con = mysql_init(NULL);

    if (mysql_real_connect(con, "localhost", "root", "root",
                           "emb", 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    // // Define gpio 17 as output
    // INP_GPIO(17);
    // OUT_GPIO(17);

    // while (1)
    // {
    //     // Toggle 17 (blink a led!)
    //     GPIO_SET = 1 << 17;
    //     printf("after GPIO_SET= 1 << 17\n");
    //     sleep(1);

    //     GPIO_CLR = 1 << 17;
    //     printf("after GPIO_CLR= 1 << 17\n");
    //     sleep(1);
    // }

    int pin = 17;
    int status = 0;
    INP_GPIO(17);

    //led
    INP_GPIO(4);
    OUT_GPIO(4);

    char operation[50];

    printf("a: read input\n");
    printf("z: view last status\n");
    printf("s: save input to mysql\n");
    printf("e: exit\n");

    while (1)
    {
        printf("Status is %d \n", status);
        printf("Enter a character: ");
        char chr;
        scanf("%c", &chr);

        switch (chr)
        {
        case 'a':
            status = GPIO_READ(pin);
            GPIO_SET = 1 << 4;
            break;
        case 'z':
            printf("Status of pin%d is: %d \n", pin, status);
            GPIO_CLR = 1 << 4;
            break;
        case 's':
            sprintf(operation, "INSERT INTO gpio VALUES(%d, %d, NULL)", pin, status);
            printf("Executing operation....\n");
            printf("%s\n", operation);

            if (mysql_query(con, operation))
            {
                finish_with_error(con);
            }
            break;
        case 'e':
            exit(0);
        default:
            break;
        }

        //clear buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        printf("\n\n\n");
    }

    return 0;
}
