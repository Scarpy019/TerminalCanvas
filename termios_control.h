#include <termios.h>
class BufferToggle
{
    private:
        static struct termios t;

    public:

        /*
         * Disables buffered input
         */

        static void off(void)
        {
            tcgetattr(0, &t); //get the current terminal I/O structure
            BufferToggle::t.c_lflag &= ~ICANON; //Manipulate the flag bits to do what you want it to do
            tcsetattr(0, 0, &t); //Apply the new settings
        }


        /*
         * Enables buffered input
         */

        static void on(void)
        {
            tcgetattr(0, &t); //get the current terminal I/O structure
            BufferToggle::t.c_lflag |= ICANON; //Manipulate the flag bits to do what you want it to do
            tcsetattr(0, 0, &t); //Apply the new settings
        }
};
termios BufferToggle::t{};