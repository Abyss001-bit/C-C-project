#include "first_index.h"

void first_index()
{
	show_bmp(0,0,"./black/welcome.bmp");

	int ret=0;
	while(1)
	{
		ret = get_two_xy();
		if((ret == 1) || (ret == 2) || (ret == 3))
		{
			sleep(1);
			break;
		}
	}
}