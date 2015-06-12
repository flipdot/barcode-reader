#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>

#define nullptr NULL
#define false 0
#define true (!false)
#define KEY_RELEASE 0
#define KEY_PRESS 1
#define KEY_AUTOREPEAT 2

typedef unsigned char byte;
typedef unsigned char bool;
typedef struct input_event input_event;

// This script prints a bar code followed by a \n.
// just read the stdout.

char get_char_from_scancode(byte code)
{
	// TODO: Add support for more scan codes and stuff
	// See: http://www.cs.fsu.edu/~baker/devices/lxr/http/source/linux/include/linux/input.h?v=2.6.11.8
	if(KEY_1 <= code && code <= KEY_9)
		return '1' + (code - KEY_1);
	if(code == KEY_0)
		return '0';
	printf("missing: %d\n", code);
	return 'E';
}

int main(int argc, char** argv)
{
	FILE *input = fopen("/dev/input/by-id/usb-GASIA_PS2toUSB_Adapter-event-kbd", "rb");
	if(input == nullptr)
		return 1;

	bool isUpperCase = false;

	while(!feof(input))
	{
		input_event ev;
		char current_char = 0;
		size_t read_bytes = fread(&ev, 1, sizeof(input_event), input);
		if(read_bytes != sizeof(input_event))
		{
			fputs("lolzuklein", stderr);
			fclose(input);
			input = nullptr;
			return 2;
		}
		switch(ev.type)
		{
			case EV_SYN: continue; // skip sync codes
			case EV_KEY:
				if(ev.code == KEY_LEFTSHIFT)
				{
					// toggle shift keys
					// may be used in later versions
					isUpperCase = (ev.value == KEY_PRESS);
					continue;
				}
				if(ev.code == KEY_TAB)
				{
					if(ev.value == KEY_RELEASE)
					{
						putchar('\n');
						fflush(stdout);
					}
					continue;
				}

				if(ev.value == KEY_RELEASE) continue;

				current_char = get_char_from_scancode(ev.code);
				putchar(current_char);
				break;
			case EV_MSC:
				//printf("EV_MSC\tcode: %x value: %x", ev.code, ev.value);
				break;
			default:
				printf("Unmatched type: %x", ev.type);
				break;
		}
	}
	fclose(input);
	input = nullptr;
	return 0;
}
