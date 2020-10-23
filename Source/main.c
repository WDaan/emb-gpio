#include <linux/gpio.h>
#include <gpiod.h>
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
    // connect mysql
    MYSQL *con = mysql_init(NULL);

    if (mysql_real_connect(con, "localhost", "root", "root",
                           "emb", 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    int pin = 17;
    int status = 0;

    int output_chip = gpiod_chip_open_by_number(0);
    int output_line = gpiod_chip_get_line(output_chip, 27);
    gpiod_line_set_value(output_line, 1);

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

            break;
        case 'z':
            printf("Status of pin%d is: %d \n", pin, status);

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
