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

typedef struct {
	byte code;
	char c;
} key_entry;

key_entry alphabet[] = {{KEY_A, 'A'}, {KEY_B, 'B'}, {KEY_C, 'C'}, {KEY_D, 'D'}, {KEY_E, 'E'}, {KEY_F, 'F'}, {KEY_G, 'G'}, {KEY_H, 'H'}, {KEY_I, 'I'}, {KEY_J, 'J'}, {KEY_K, 'K'}, {KEY_L, 'L'}, {KEY_M, 'M'}, {KEY_N, 'N'}, {KEY_O, 'O'}, {KEY_P, 'P'}, {KEY_Q, 'Q'}, {KEY_R, 'R'}, {KEY_S, 'S'}, {KEY_T, 'T'}, {KEY_U, 'U'}, {KEY_V, 'V'}, {KEY_W, 'W'}, {KEY_X, 'X'}, {KEY_Y, 'Y'}, {KEY_Z, 'Z'},
{KEY_SLASH, '_'}};

// This script prints a bar code followed by a \n.
// just read the stdout.

void usage(const char* prog)
{
	fprintf(stderr, "usage: %s", prog);
	exit(1);
}

char get_char_from_scancode(byte code)
{
	// See: http://www.cs.fsu.edu/~baker/devices/lxr/http/source/linux/include/linux/input.h?v=2.6.11.8
	if(KEY_1 <= code && code <= KEY_9)
		return '1' + (code - KEY_1);
	if(code == KEY_0)
		return '0';
	
	for (size_t i=0; i<sizeof(alphabet)/sizeof(key_entry); i++) {
		if (code == alphabet[i].code)
			return alphabet[i].c;
	}
	fprintf(stderr, "missing: %d\n", code);
	return '\0';
}

int main(int argc, char** argv)
{
	if (argc != 2)
		usage(argv[0]);

	// argv[1] is usually "/dev/input/by-id/usb-GASIA_PS2toUSB_Adapter-event-kbd"
	FILE* input = fopen(argv[1], "rb");
	if (input == nullptr)
	{
		fprintf(stderr, "Couldn't open %s", argv[0]);
		return 1;
	}
	
	int char_num = 0;
	while (!feof(input))
	{
		input_event ev;
		char current_char = 0;
		size_t read_bytes = fread(&ev, 1, sizeof(input_event), input);
		if (read_bytes != sizeof(input_event))
		{
			fprintf(stderr, "fread returned early");
			fclose(input);
			input = nullptr;
			return 1;
		}

		switch (ev.type)
		{
			case EV_SYN: break; // skip sync codes
			case EV_KEY:
				if (ev.code == KEY_TAB)
				{
					if (ev.value == KEY_RELEASE)
					{
						char_num = 0;
						putchar('\n');
						fflush(stdout);
					}
					continue;
				}

				if (ev.value == KEY_RELEASE)
					continue;

				if (char_num++ == 0) {
					putchar('E');
				}
				current_char = get_char_from_scancode(ev.code);
				putchar(current_char);
				break;
			case EV_MSC:
				//printf("EV_MSC\tcode: %x value: %x", ev.code, ev.value);
				break;
			default:
				fprintf(stderr, "Unmatched type: %x", ev.type);
				break;
		}
	}

	fclose(input);
	input = nullptr;
	return 0;
}
