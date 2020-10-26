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

    int status = 0;
    char operation[50];

    //led
    int ledPin = 4;
    INP_GPIO(ledPin);
    OUT_GPIO(ledPin);

    while (1)
    {
        printf("Enter pin to read: ");
        int pin;
        scanf("%d", &pin);

        // initialize the input
        INP_GPIO(pin);

        status = GPIO_READ(pin);
        status = status ? 1 : 0;

        //set status as output
        if (status)
            GPIO_SET = 1 << ledPin;
        else
            GPIO_CLR = 1 << ledPin;

        sprintf(operation, "INSERT INTO gpio VALUES(%d, %d, NULL)", pin, status);
        printf("Executing the folowing query:\n");
        printf("\t%s\n", operation);

        if (mysql_query(con, operation))
        {
            finish_with_error(con);
        }

        //clear buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        printf("\n\n\n");
    }

    return 0;
}
