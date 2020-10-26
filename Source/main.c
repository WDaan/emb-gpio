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
    //connect mysql
    MYSQL *con = mysql_init(NULL);

    if (mysql_real_connect(con, "localhost", "root", "root",
                           "emb", 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    // init gpiochip
    struct gpiod_chip *gpiochip = gpiod_chip_open("/dev/gpiochip0");
    struct gpiod_line *gpioline;

    // led
    int ledPin = 4;
    struct gpiod_line *ledLine = gpiod_chip_get_line(gpiochip, ledPin);

    int status = 0;
    char operation[50];

    while (1)
    {
        printf("Enter pin to read: ");
        int pin;
        scanf("%d", &pin);

        // initialize the input
        gpioline = gpiod_chip_get_line(gpiochip, pin);

        int req = gpiod_line_request_input(gpioline, "gpio_state");

        status = gpiod_line_get_value(gpioline);
        status = status ? 1 : 0;

        req = gpiod_line_request_output(ledLine, "gpio", ledPin);

        gpiod_line_set_value(ledLine, status);

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
