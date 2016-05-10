/*
 * $Copyright Open Broadcom Corporation$
 */

#include <common.h>
#include <post.h>

#if CONFIG_POST & CONFIG_SYS_POST_UART
#include <ns16550.h>

#define CONFIG_SYS_NS16550_COM0                 0x18020000
#define CONFIG_SYS_NS16550_COM1                 0x18021000
#define CONFIG_SYS_NS16550_COM2                 0x18022000
#define CONFIG_SYS_NS16550_COM4                 0x18024000

#define UART_MCR_LOOPBACK   0x10
#define UART_FCRVAL (UART_FCR_FIFO_EN | \
                     UART_FCR_RXSR |    \
                     UART_FCR_TXSR)             /* Clear & enable FIFOs */

#define serial_out(x, y)        writeb(x, y)
#define serial_in(y)            readb(y)

extern void iproc_uart_iomux(int op);

/* Note: The port number specified in the functions is 1 based.
 *       the array is 0 based.
 */
static NS16550_t serial_ports[5] = {
#ifdef CONFIG_SYS_NS16550_COM0
        (NS16550_t)CONFIG_SYS_NS16550_COM0,
#else
        NULL
#endif
#ifdef CONFIG_SYS_NS16550_COM1
        (NS16550_t)CONFIG_SYS_NS16550_COM1,
#else
        NULL,
#endif
#ifdef CONFIG_SYS_NS16550_COM2
        (NS16550_t)CONFIG_SYS_NS16550_COM2,
#else
        NULL,
#endif
#ifdef CONFIG_SYS_NS16550_COM3
        (NS16550_t)CONFIG_SYS_NS16550_COM3,
#else
        NULL,
#endif
#ifdef CONFIG_SYS_NS16550_COM4
        (NS16550_t)CONFIG_SYS_NS16550_COM4
#else
        NULL,
#endif
};

void _serial_enter_loopback( const int port )
{
	NS16550_t com_port = serial_ports[port];

	serial_out((serial_in(&com_port->mcr) |UART_MCR_LOOPBACK ), &com_port->mcr);
	serial_out(UART_FCRVAL, &com_port->fcr);
}

void _serial_exit_loopback( const int port )
{
	NS16550_t com_port = serial_ports[port];

	serial_out((serial_in(&com_port->mcr) & (~UART_MCR_LOOPBACK)) ,&com_port->mcr);
	serial_out(UART_FCRVAL, &com_port->fcr);
}

static int UART_post_testLoopback(int uartIndex )
{
    char testChar[]= "Hello";
    volatile char *p;
    char t;
    int ret =0;

    p = testChar;
    _serial_enter_loopback(uartIndex);
    
    while(*p)
    {
	NS16550_putc(serial_ports[uartIndex], *p);
#if defined(CONFIG_CYGNUS)
        udelay(10000);
	if (!NS16550_tstc(serial_ports[uartIndex]))
        { 
            ret = -1;
            break;
        }
#endif
        t = NS16550_getc(serial_ports[uartIndex]);
        if ( t!=*p)
        {
            post_log("\nExpected: %c(0x%2x), Actual:  %c(0x%2x) ", *p,*p, t, t);
            ret = -1;
            break;
        }
        p++;
    }
    _serial_exit_loopback(uartIndex);
    return ret;

}

static void UART_post_init(void)
{
#if defined(CONFIG_CYGNUS)
    int i, clock_divisor;

    iproc_uart_iomux(1);

    for (i = CONFIG_UART0_INDEX; i <= CONFIG_UART4_INDEX; i++) {
        if (i == CONFIG_UART3_INDEX)
             continue;

        clock_divisor = ns16550_calc_divisor(serial_ports[i], CONFIG_SYS_NS16550_CLK, 115200);
        NS16550_reinit(serial_ports[i], clock_divisor);
    } 
#endif
}
static void UART_post_end(void)
{
#if defined(CONFIG_CYGNUS)
    iproc_uart_iomux(0);
#endif
}
static int UART_post_TR_test(int uartIndex )
{
    
    unsigned long counter = 0;
    unsigned long timeout = 0;
    char ch;
    char *s = "\ncan you see me now?press 'Y' to continue";

    post_log("\nPlease connect terminals to console %d !", uartIndex);
    post_log("\nPlease make sure the console is configured as :");
    post_log("\nBaud 115200\n8 bit data 1 bit stop\nno parity, no HW flow control");
    post_log("\nReady to test (Y): ");

    //Get 'Y' from the console to continue the test

    do
    {
        ch = (char)serial_getc();
    }while ( (ch != 'y') && ( ch!='Y'));


    post_log("\nSend test string <can you see me now? press 'Y' to continue> to console %d", uartIndex);
    //Send test string to the console under test
    while (*s) {
        if (*s == '\n')
                NS16550_putc(serial_ports[uartIndex], '\r');

        NS16550_putc(serial_ports[uartIndex], *s++);
    }
    counter = 0;

    do
    {
        counter++;
        if ( counter == 0xFFFFF )
        {
            post_log("\nWaiting on response from console %d",uartIndex);
            counter =0 ;
            timeout++;
            if ( timeout > 0xFFFF )
            {
                break;
            }
        }
        if (NS16550_tstc(serial_ports[uartIndex]) != 0 )
        {
            ch = (char)NS16550_getc(serial_ports[uartIndex]);
            if ( ('y' == ch) || ('Y'==ch ))
            {
                break;
            }
        }

    }while (1 );

    if ( timeout > 0xFFFF )
    {
        post_log("\nReceived no response from console %d",uartIndex );
        return -1;
    }
    post_log("\nReceived response from console %d",uartIndex );
    return 1;

}


#if defined(CONFIG_CYGNUS)

static void drain_out(void)
{
    int idx;
    for (idx = CONFIG_UART0_INDEX; idx <= CONFIG_UART4_INDEX; idx++)
    {
        while (NS16550_tstc(serial_ports[idx]))
        {
            NS16550_getc(serial_ports[idx]);
        }
    }   
}

int UART_post_outer_lp_test(int uartIndex, int *rxIdx)
{
    char testChar[]= "Hello";
    volatile char *p;
    char t;    
    int idx;
    int find_dest = 0xff;
	int num_ports = CONFIG_UART4_INDEX;

    p = testChar;
    post_log("\n");

	if (post_get_board_diags_type() == BCM958305K)
		num_ports = 2; /*UART 0,1 in WA*/

    while(*p)
    {
	NS16550_putc(serial_ports[uartIndex], *p);        
	udelay(1000);
        if (find_dest != 0xff)
        {
           if (!NS16550_tstc(serial_ports[find_dest])) {
               post_log("can't get char\n");
               return -1;
           }                     
           t = NS16550_getc(serial_ports[find_dest]); 
           //post_log("idx:%d,get char :%x\n", find_dest, t);
           if ( t != (*p)) 
           { 
              return -1;
           }
        } else {
			for (idx = CONFIG_UART0_INDEX; idx < num_ports; idx++)
            {
                if ((idx == CONFIG_UART3_INDEX) || (idx == uartIndex))
                      continue;
                if (!NS16550_tstc(serial_ports[idx]))
                      continue; 
                t = NS16550_getc(serial_ports[idx]);
                //post_log("idx:%d,get char :%x\n", idx, t);
                if ( t!=*p) {
                    continue;
                } else {
                    find_dest = idx;
                    * rxIdx = idx;
                    break;
                }
            }
            if (find_dest == 0xff) 
            { 
                return -1;
            }
        }
        p++;
    }
    return 1;
}

int UART_post_test (int flags)
{
    int ret,tot_test = 1;
    int i;
    char ch;
    int rxIdx;
	int num_ports = CONFIG_UART4_INDEX;
	int bitmask_ports = 0x1f;

#ifdef CONFIG_DIAGS_UART_BITS
	bitmask_ports = CONFIG_DIAGS_UART_BITS;
#endif

    post_log("\nUART diags starts  !");

    UART_post_init();

    post_log("\nUART loopback test..."); 
    
    for ( i = CONFIG_UART0_INDEX; i <= num_ports; i++)
    {
        /*use UART3 as the default console*/
        if (i == CONFIG_UART3_INDEX)
        {
            continue;
        }
#if 1
        if (i == CONFIG_UART4_INDEX) 
        {
            /*some issues for uart4 on svk*/
            _serial_exit_loopback(CONFIG_UART4_INDEX);  
            udelay(10000);  
        }
#endif
        drain_out();
        ret = UART_post_testLoopback(i);        
        if ( 0 == ret )
        {
            post_log("\nUART %d Loopback test successful!",i);
        }
        else
        {
            post_log("\nUART %d Loopback test failed!",i);
            tot_test = -1;
        }
    }

#if 1
     /*some issues for uart4 on svk*/
    _serial_exit_loopback(CONFIG_UART4_INDEX);  
    udelay(10000);  
#endif
    drain_out();

    post_log("\nwill you select outer loopback test (Y/N): ");
    do 
    {
        ch = (char)serial_getc();
    } while ((ch !='y') && (ch !='Y') && (ch !='N') && (ch !='n'));

    if ((ch == 'y') || ( ch =='Y')) {  
            
		for ( i = CONFIG_UART0_INDEX; i <= num_ports; i++)
        {
            /*use UART3 as the default console*/
            if (i == CONFIG_UART3_INDEX)
            {
                continue;
            }
            drain_out();
            ret = UART_post_outer_lp_test(i, &rxIdx);    
            if ( 1 == ret )
            {
                post_log("\nUART %d outer loopback test (%d->%d) successful!",i, i, rxIdx);
            }
            else
            {
                post_log("\nUART %d outer loopback test failed!",i);
                tot_test = -1;
            }
        }

    } else {
        for ( i = CONFIG_UART0_INDEX; i <= num_ports; i++)
		{
            /*use UART3 as the default console*/
			if (i == CONFIG_UART3_INDEX ||
				0 == ((bitmask_ports >> i) & 1))
				continue;

            ret = UART_post_TR_test(i);
    
            if ( 1 == ret )
            {
                post_log("\nUART %d TX/RX test successful!",i);
            }
            else
            {
                post_log("\nUART %d TX/RX test failed!",i);
                tot_test = -1;
            }
        }
    }
    UART_post_end();
    if (1 == tot_test)
        post_log("\nSuccessful, UART diags done  !\n");
    else {
        post_log("\nFailed, UART diags done  !\n");
        return -1;
    }
    return 0;
}
#else

int UART_post_test (int flags)
{
    int ret;
    int i;

#if (defined(CONFIG_NS_PLUS))

    if ( post_get_board_diags_type() & HR_ER_BOARDS )
    {
        printf("\nThis diag is not supported on this platform\n ");
        return 2;
    }
#endif

    post_log("\nPlease set strap pin A4 to 0x18, A5 to 0x18");
    post_getConfirmation("\nReady? (Y/N)");


    post_log("\nUART diags starts  !");

    UART_post_init();

    post_log("\nUART loopback test...");

#if defined(CONFIG_SABER2)
    for ( i = CONFIG_UART1_INDEX; i <= CONFIG_UART3_INDEX; i++)
#else
    for ( i = CONFIG_UART1_INDEX; i <= CONFIG_UART2_INDEX; i++)
#endif
    {
        ret = UART_post_testLoopback(i);

        if ( 0 == ret )
        {
            post_log("\nUART %d Loopback test successful!",i-1);
        }
        else
        {
            post_log("\nUART %d Loopback test failed!",i-1);
        }
    }
	
#if (defined(CONFIG_SABER2))
    for ( i = CONFIG_UART0_INDEX; i <= CONFIG_UART0_INDEX; i++)
#else
    for ( i = CONFIG_UART1_INDEX; i <= CONFIG_UART2_INDEX; i++)
#endif
    {
        ret = UART_post_TR_test(i);

        if ( 1 == ret )
        {
            post_log("\nUART %d TX/RX test successful!",i-1);
        }
        else
        {
            post_log("\nUART %d TX/RX test failed!",i-1);
        }
    }
    UART_post_end();
    post_log("\nSuccessful, UART diags done  !\n");
    return 0;
}
#endif

#endif /* CONFIG_POST & CONFIG_SYS_POST_QSPI */


