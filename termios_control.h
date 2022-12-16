#pragma once
#include <termios.h>

class BufferToggle
{
    private:
        static struct termios term;

    public:

        /*
         * Disables buffered input
         */

        static void off(void)
        {
            tcgetattr(0, &term); //get the current terminal I/O structure
            BufferToggle::term.c_lflag &= ~ICANON; //Manipulate the flag bits to do what you want it to do
            tcsetattr(0, 0, &term); //Apply the new settings
        }


        /*
         * Enables buffered input
         */

        static void on(void)
        {
            tcgetattr(0, &term); //get the current terminal I/O structure
            BufferToggle::term.c_lflag |= ICANON; //Manipulate the flag bits to do what you want it to do
            tcsetattr(0, 0, &term); //Apply the new settings
        }
};